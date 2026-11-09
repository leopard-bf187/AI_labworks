#include "MembershipGraph.h"

#include <algorithm>
#include <cmath>
#include <new>

using namespace krystallic;
using namespace krystallic::GUI;
using namespace krystallic::SIMDMath;

namespace
{
    struct SGraphBinding
    {
        SMembershipGraphView* view = nullptr;
    };

    static const CColorRGBA kTermColors[3] =
    {
        CColorRGBA(0.30f, 0.70f, 1.0f, 1.0f),
        CColorRGBA(0.42f, 0.91f, 0.53f, 1.0f),
        CColorRGBA(1.0f, 0.65f, 0.32f, 1.0f)
    };
    static const CColorRGBA kAxisColor(0.73f, 0.75f, 0.79f, 0.80f);
    static const CColorRGBA kGridColor(0.55f, 0.57f, 0.60f, 0.30f);
    static const CColorRGBA kMarkerColor(1.0f, 1.0f, 1.0f, 0.90f);
    static const CColorRGBA kAggregateColor(0.96f, 0.83f, 0.32f, 0.28f);
    static const CColorRGBA kAggregateLineColor(0.98f, 0.85f, 0.36f, 0.98f);

    static void DrawSegment(IGUIRenderContext* context, float x0, float y0, float x1, float y1, float thickness, const CColorRGBA& color)
    {
        const float dx = x1 - x0;
        const float dy = y1 - y0;
        const float length = std::sqrt(dx * dx + dy * dy);
        if (length <= 0.0001f)
            return;

        const float nx = -dy * thickness * 0.5f / length;
        const float ny = dx * thickness * 0.5f / length;
        Render::XYUV1C32DW2 vertices[4] =
        {
            {{x0 + nx, y0 + ny}, 0.0f, color, 0, 0},
            {{x1 + nx, y1 + ny}, 0.0f, color, 0, 0},
            {{x0 - nx, y0 - ny}, 0.0f, color, 0, 0},
            {{x1 - nx, y1 - ny}, 0.0f, color, 0, 0}
        };
        uint indices[6] = {0, 1, 2, 2, 1, 3};
        context->DrawGeometry(4, vertices, 6, indices);
    }

    static void DrawCaption(IGUIRenderContext* context, const GUI_TEXT_REF& text, const GUI_RECT& rect, const CColorRGBA& color, dword horizontalAlignment)
    {
        const dword drawFlags = GUI_STRING_DRAW_USE_CUSTOM_SIZE | GUI_STRING_DRAW_VERTICAL_ALIGN_CENTER | GUI_STRING_DRAW_OVERRIDE_STYLE_COLOR | horizontalAlignment;
        context->DrawString(text, rect, drawFlags, 11.0f, color);
    }

    static float MapX(float value, const SMembershipGraphView& view, float left, float right)
    {
        const float normalized = std::clamp((value - view.minValue) / (view.maxValue - view.minValue), 0.0f, 1.0f);
        return left + normalized * (right - left);
    }

    static float MapY(float membership, float top, float bottom)
    {
        return bottom - std::clamp(membership, 0.0f, 1.0f) * (bottom - top);
    }
}

void CMembershipGraphSystem::ConstructData(uint startPos, uint count, void* controlArray)
{
    SGraphBinding* bindings = static_cast<SGraphBinding*>(controlArray);
    for (uint i = startPos; i < startPos + count; ++i)
        new (&bindings[i]) SGraphBinding{};
}

void CMembershipGraphSystem::DestructData(uint startPos, uint count, void* controlArray)
{
    SGraphBinding* bindings = static_cast<SGraphBinding*>(controlArray);
    for (uint i = startPos; i < startPos + count; ++i)
        bindings[i].~SGraphBinding();
}

ERRCODE CMembershipGraphSystem::Register(IGUIContext* context)
{
    if (!context)
        return GUI_ERR_INVALID_ARG;

    GUI_CONTROL_REGISTER_DATA registration{};
    registration.m_controlTypeID = CONTROL_TYPE;
    registration.m_controlSize = sizeof(SGraphBinding);
    registration.m_ctor = &ConstructData;
    registration.m_dtor = &DestructData;
    registration.m_system = this;
    return context->RegisterControl(&registration);
}

ERRCODE CMembershipGraphSystem::OnCreate(GUI_CONTROL_INITIAL_DATA* data, void* subtypeDescStructPtr, void* destStruct)
{
    if (!data || !subtypeDescStructPtr || !destStruct)
        return GUI_ERR_INVALID_ARG;

    const GUI_MEMBERSHIP_GRAPH_DESC* desc = static_cast<const GUI_MEMBERSHIP_GRAPH_DESC*>(subtypeDescStructPtr);
    if (!desc->view || !desc->view->model || desc->view->minValue >= desc->view->maxValue)
        return GUI_ERR_INVALID_ARG;

    static_cast<SGraphBinding*>(destStruct)->view = desc->view;
    return GUI_ERR_OK;
}

ERRCODE CMembershipGraphSystem::OnDestroy(void* subtypeControlData)
{
    if (subtypeControlData)
        static_cast<SGraphBinding*>(subtypeControlData)->view = nullptr;
    return GUI_ERR_OK;
}

ERRCODE CMembershipGraphSystem::OnEvent(const GUI_EVENT_DATA* evt, void* subtypeControlData)
{
    (void)evt;
    (void)subtypeControlData;
    return GUI_ERR_OK;
}

ERRCODE CMembershipGraphSystem::OnDraw(IGUIRenderContext* rndContext, void* subtypeControlData, GUI_RECT world, dword state, dword style, dword flags)
{
    (void)state;
    if (!rndContext || !subtypeControlData)
        return GUI_ERR_INVALID_ARG;

    const SGraphBinding* binding = static_cast<const SGraphBinding*>(subtypeControlData);
    const SMembershipGraphView* view = binding->view;
    if (!view || !view->model || view->maxValue <= view->minValue)
        return GUI_ERR_INVALID_ARG;

    const float left = world.m_left + 27.0f;
    const float right = world.m_right - 10.0f;
    const float top = world.m_top + 7.0f;
    const float bottom = world.m_bottom - 35.0f;
    if (right - left <= 20.0f || bottom - top <= 20.0f)
        return GUI_ERR_OK;

    // Each plot has its own axes, legend and input/output marker.
    DrawSegment(rndContext, left, top, left, bottom, 1.0f, kAxisColor);
    DrawSegment(rndContext, left, bottom, right, bottom, 1.0f, kAxisColor);
    DrawSegment(rndContext, left, MapY(0.5f, top, bottom), right, MapY(0.5f, top, bottom), 0.75f, kGridColor);
    DrawSegment(rndContext, left, top, right, top, 0.75f, kGridColor);

    constexpr unsigned int kSegments = 96;
    const float segmentWidth = (right - left) / static_cast<float>(kSegments);
    const bool hasOutput = view->variable == EFuzzyVariable::Precipitation && view->hasResult && view->result.status == EWeatherStatus::Success;

    // Mamdani output: clipped, aggregated membership plus its centroid.
    if (hasOutput)
    {
        for (unsigned int i = 0; i < kSegments; ++i)
        {
            const float x0 = left + i * segmentWidth;
            const float x1 = left + (i + 1) * segmentWidth;
            const float value0 = view->minValue + (view->maxValue - view->minValue) * static_cast<float>(i) / kSegments;
            const float value1 = view->minValue + (view->maxValue - view->minValue) * static_cast<float>(i + 1) / kSegments;
            const float y0 = MapY(view->model->GetAggregatedMembership(view->result, value0), top, bottom);
            const float y1 = MapY(view->model->GetAggregatedMembership(view->result, value1), top, bottom);
            const float fillTop = std::min(y0, y1);
            if (fillTop < bottom)
                rndContext->DrawSolidRect(GUI_RECT(x0, fillTop, x1, bottom), kAggregateColor, flags, style);
            DrawSegment(rndContext, x0, y0, x1, y1, 1.7f, kAggregateLineColor);
        }
    }

    for (unsigned int term = 0; term < 3; ++term)
    {
        float previousX = left;
        float previousY = MapY(view->model->GetMembership(view->variable, static_cast<EFuzzyTerm>(term), view->minValue), top, bottom);
        for (unsigned int i = 1; i <= kSegments; ++i)
        {
            const float value = view->minValue + (view->maxValue - view->minValue) * static_cast<float>(i) / kSegments;
            const float x = left + static_cast<float>(i) * segmentWidth;
            const float y = MapY(view->model->GetMembership(view->variable, static_cast<EFuzzyTerm>(term), value), top, bottom);
            DrawSegment(rndContext, previousX, previousY, x, y, 1.8f, kTermColors[term]);
            previousX = x;
            previousY = y;
        }
    }

    if (view->variable != EFuzzyVariable::Precipitation || hasOutput)
    {
        const float value = hasOutput ? view->result.probability : view->currentValue;
        const float x = MapX(value, *view, left, right);
        DrawSegment(rndContext, x, top, x, bottom, 1.1f, kMarkerColor);
    }

    DrawCaption(rndContext, view->oneLabel, GUI_RECT(world.m_left + 3.0f, top - 6.0f, left - 4.0f, top + 9.0f), kAxisColor, GUI_STRING_DRAW_HORIZONTAL_ALIGN_RIGHT);
    DrawCaption(rndContext, view->zeroLabel, GUI_RECT(world.m_left + 3.0f, bottom - 8.0f, left - 4.0f, bottom + 7.0f), kAxisColor, GUI_STRING_DRAW_HORIZONTAL_ALIGN_RIGHT);
    DrawCaption(rndContext, view->minLabel, GUI_RECT(left, bottom + 3.0f, left + 80.0f, bottom + 19.0f), kAxisColor, GUI_STRING_DRAW_HORIZONTAL_ALIGN_LEFT);
    DrawCaption(rndContext, view->maxLabel, GUI_RECT(right - 80.0f, bottom + 3.0f, right, bottom + 19.0f), kAxisColor, GUI_STRING_DRAW_HORIZONTAL_ALIGN_RIGHT);

    const float legendPositions[3] = {left + 41.0f, left + 165.0f, left + 290.0f};
    for (unsigned int term = 0; term < 3; ++term)
    {
        const float x = legendPositions[term];
        const float y = world.m_bottom - 13.0f;
        rndContext->DrawSolidRect(GUI_RECT(x, y + 5.0f, x + 14.0f, y + 7.0f), kTermColors[term], flags, style);
        DrawCaption(rndContext, view->termLabels[term], GUI_RECT(x + 18.0f, y - 2.0f, x + 102.0f, y + 17.0f), kAxisColor, GUI_STRING_DRAW_HORIZONTAL_ALIGN_LEFT);
    }

    return GUI_ERR_OK;
}
