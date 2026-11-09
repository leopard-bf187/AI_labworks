#pragma once

#include "gui.h"
#include "fuzzy/FuzzySystem.h"

// Stable application-owned state. The GUI registry only stores a pointer to it.
struct SMembershipGraphView
{
    const CFuzzySystem* model = nullptr;
    EFuzzyVariable variable = EFuzzyVariable::Temperature;
    float minValue = 0.0f;
    float maxValue = 1.0f;
    float currentValue = 0.0f;
    bool hasResult = false;
    SWeatherResult result{};
    krystallic::GUI::GUI_TEXT_REF termLabels[3]{};
    krystallic::GUI::GUI_TEXT_REF zeroLabel{};
    krystallic::GUI::GUI_TEXT_REF oneLabel{};
    krystallic::GUI::GUI_TEXT_REF minLabel{};
    krystallic::GUI::GUI_TEXT_REF maxLabel{};
};

struct GUI_MEMBERSHIP_GRAPH_DESC
{
    SMembershipGraphView* view = nullptr;
};

class CMembershipGraphSystem final : public krystallic::GUI::IGUIControlSystem
{
public:
    static constexpr dword CONTROL_TYPE = 32;

    ERRCODE Register(krystallic::GUI::IGUIContext* context);
    ERRCODE OnCreate(krystallic::GUI::GUI_CONTROL_INITIAL_DATA* data, void* subtypeDescStructPtr, void* destStruct) override;
    ERRCODE OnDestroy(void* subtypeControlData) override;
    ERRCODE OnEvent(const krystallic::GUI::GUI_EVENT_DATA* evt, void* subtypeControlData) override;
    ERRCODE OnDraw(krystallic::GUI::IGUIRenderContext* rndContext, void* subtypeControlData, krystallic::GUI::GUI_RECT world, dword state, dword style, dword flags) override;

private:
    static void ConstructData(uint startPos, uint count, void* controlArray);
    static void DestructData(uint startPos, uint count, void* controlArray);
};
