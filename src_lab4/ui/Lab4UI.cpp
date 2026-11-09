#include "Lab4UI.h"

#include <cstdio>
#include <cstring>

using namespace krystallic;
using namespace krystallic::GUI;
using namespace krystallic::GUI::STDUI;
using namespace krystallic::SIMDMath;

namespace
{
    CLab4UI* g_activeUI = nullptr;
    static const char* g_buildID = KRYSTALLIC_BUILD_OUTPUT_DIRECTORY "." KRYSTALLIC_PLATFORM_SHORT "." KRYSTALLIC_BUILD_CONFIG_SHORT;
    static constexpr dword kLeftLabel = GUI_STD_LABEL_ALIGN_LEFT | GUI_STD_LABEL_ALIGN_MIDDLE;
    static constexpr float kMainFrameWidth = 2.0f;

    struct STextEntry
    {
        const char* id;
        const char* value;
    };

    static const STextEntry g_textEntries[] =
    {
        {"lab4_build", g_buildID},
        {"lab4_input_group", "Входные параметры"},
        {"lab4_result_group", "Результаты нечёткого вывода"},
        {"lab4_temperature", "Температура"},
        {"lab4_humidity", "Влажность"},
        {"lab4_pressure", "Атмосферное давление"},
        {"lab4_calculate", "Рассчитать"},
        {"lab4_auto", "Автоматический расчёт"},
        {"lab4_result_title", "Вероятность осадков:"},
        {"lab4_rules_title", "Активные правила:"},
        {"lab4_status_title", "Состояние:"},
        {"lab4_graph_temperature", "Функции принадлежности: температура"},
        {"lab4_graph_humidity", "Функции принадлежности: влажность"},
        {"lab4_graph_pressure", "Функции принадлежности: давление"},
        {"lab4_graph_output", "Функции принадлежности: вероятность осадков"},
        {"lab4_graph_waiting", "График будет добавлен на этапе реализации Мамдани"},
        {"lab4_value_temperature", "18.0 °C"},
        {"lab4_value_humidity", "65.0 %"},
        {"lab4_value_pressure", "1012.0 гПа"},
        {"lab4_value_result", "—"},
        {"lab4_value_rules", "—"},
        {"lab4_value_status", "Алгоритм Мамдани пока не подключён"}
    };
}

bool CLab4UI::LoadFonts()
{
    struct SFontInfo { const char* id; const char* path; };
    static const SFontInfo fonts[2] =
    {
        {"lab4_roboto", "SBoxData/UI/Fonts/Roboto-Regular.ttf"},
        {"lab4_roboto_condensed", "SBoxData/UI/Fonts/Roboto_Condensed-Regular.ttf"}
    };

    const dword ranges = GUI_GLYPH_RANGE_ASCII | GUI_GLYPH_RANGE_LATIN_EXT | GUI_GLYPH_RANGE_CYRILLIC;
    for (uint i = 0; i < 2; ++i)
    {
        RefCounted<Common::IBuffer> fontData;
        ERRCODE result = m_context->LoadFontFaceFromFileTTF(fonts[i].path, fontData.ReleaseAndGetAddressOf());
        if (result == GUI_ERR_OK)
            result = m_context->CreateFontFamilyObject(fonts[i].id, m_fonts[i].ReleaseAndGetAddressOf());
        if (result == GUI_ERR_OK)
            result = m_fonts[i]->AddFontStyleData(GUI_FONT_REGULAR, fontData.Get(), ranges, 18);
        if (result == GUI_ERR_OK)
            result = m_fonts[i]->Build();
        if (result != GUI_ERR_OK)
        {
            std::fprintf(stderr, "AI_Lab4: font initialization failed: %s (error %u)\n", fonts[i].path, unsigned(result));
            return false;
        }
        m_context->SetActiveFont(int(i), m_fonts[i].Get());
    }
    return true;
}

bool CLab4UI::LoadStrings()
{
    static_assert(sizeof(g_textEntries) / sizeof(g_textEntries[0]) == TEXT_COUNT);

    if (m_context->CreateStringTable("lab4_strings", m_stringTable.ReleaseAndGetAddressOf()) != GUI_ERR_OK)
        return false;

    m_textStyle.m_fontTextureSlot = 0;
    m_textStyle.m_fontChannel = 0;
    m_textStyle.m_color = CColorRGBA(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i < TEXT_COUNT; ++i)
    {
        if (m_stringTable->AddPlainString(g_textEntries[i].id, g_textEntries[i].value, &m_textStyle) != GUI_ERR_OK)
            return false;
    }

    for (int i = 0; i < TEXT_COUNT; ++i)
    {
        m_textRefs[i] = m_stringTable->GetStringRefByID(g_textEntries[i].id);
        if (!m_textRefs[i].IsValid())
            return false;
    }

    for (int i = 0; i < SLOT_COUNT; ++i)
    {
        if (m_stringTable->GetStringByID(g_textEntries[TEXT_VALUE_TEMPERATURE + i].id, m_values[i].ReleaseAndGetAddressOf()) != GUI_ERR_OK)
            return false;
    }

    if (m_stringTable->GetStringByID(g_textEntries[TEXT_VALUE_RESULT].id, m_resultValue.ReleaseAndGetAddressOf()) != GUI_ERR_OK || m_stringTable->GetStringByID(g_textEntries[TEXT_VALUE_RULES].id, m_ruleValue.ReleaseAndGetAddressOf()) != GUI_ERR_OK || m_stringTable->GetStringByID(g_textEntries[TEXT_VALUE_STATUS].id, m_statusValue.ReleaseAndGetAddressOf()) != GUI_ERR_OK)
        return false;

    return m_context->SetActiveStringTable(m_stringTable.Get()) == GUI_ERR_OK;
}

bool CLab4UI::CreateFrame(const char* id, int parentIndex, const CVector2& pos, const CVector2& size, int* outIndex)
{
    GUI_STD_FRAME_DESC frame{};
    frame.m_frameWidth = kMainFrameWidth;

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", id);
    desc.m_type = GUI_STD_CONTROL_FRAME;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_VISIBLE | GUI_STATE_ENABLED;
    desc.m_pos = pos;
    desc.m_size = size;
    desc.m_subtypeControlDescPtr = &frame;
    return m_context->CreateControl(&desc, outIndex) == GUI_ERR_OK;
}

bool CLab4UI::CreateGroupBox(const char* id, int parentIndex, const CVector2& pos, const CVector2& size, ETextID title, int* outIndex)
{
    GUI_STD_GROUPBOX_DESC group{};
    group.m_text = m_textRefs[title];
    group.m_borderWidth = 1.5f;

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", id);
    desc.m_type = GUI_STD_CONTROL_GROUPBOX;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_VISIBLE | GUI_STATE_ENABLED;
    desc.m_pos = pos;
    desc.m_size = size;
    desc.m_subtypeControlDescPtr = &group;
    return m_context->CreateControl(&desc, outIndex) == GUI_ERR_OK;
}

bool CLab4UI::CreateLabel(const char* id, int parentIndex, const CVector2& pos, const CVector2& size, ETextID text, int* outIndex)
{
    GUI_STD_LABEL_DESC label{};
    label.m_text = m_textRefs[text];
    label.m_pixelSize = 0.0f;
    label.m_flags = kLeftLabel;

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", id);
    desc.m_type = GUI_STD_CONTROL_LABEL;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_VISIBLE | GUI_STATE_ENABLED;
    desc.m_pos = pos;
    desc.m_size = size;
    desc.m_subtypeControlDescPtr = &label;
    return m_context->CreateControl(&desc, outIndex) == GUI_ERR_OK;
}

bool CLab4UI::CreateSlider(const char* id, int parentIndex, const CVector2& pos, float minValue, float maxValue, float value, EInputSlot slot)
{
    GUI_STD_SLIDER_DESC slider{};
    slider.m_min = minValue;
    slider.m_max = maxValue;
    slider.m_value = value;
    slider.m_step = slot == SLOT_PRESSURE ? 1.0f : 0.5f;
    slider.m_onValueChanged = OnSliderChanged;

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", id);
    desc.m_type = GUI_STD_CONTROL_SLIDER;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_ENABLED | GUI_STATE_VISIBLE;
    desc.m_controlFlags = GUI_FLAG_SUPPORTS_INPUT;
    desc.m_pos = pos;
    desc.m_size = CVector2(365.0f, 27.0f);
    desc.m_userData = &m_sliderSlotIds[slot];
    desc.m_subtypeControlDescPtr = &slider;
    return m_context->CreateControl(&desc, &m_sliders[slot]) == GUI_ERR_OK;
}

bool CLab4UI::CreateButton(const char* id, int parentIndex, const CVector2& pos, const CVector2& size, ETextID text)
{
    GUI_STD_BUTTON_DESC button{};
    button.m_text = m_textRefs[text];
    button.m_pixelSize = 0.0f;
    button.m_onMouseClick = OnCalculatePressed;

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", id);
    desc.m_type = GUI_STD_CONTROL_BUTTON;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_ENABLED | GUI_STATE_VISIBLE;
    desc.m_controlFlags = GUI_FLAG_SUPPORTS_INPUT;
    desc.m_pos = pos;
    desc.m_size = size;
    desc.m_subtypeControlDescPtr = &button;
    return m_context->CreateControl(&desc) == GUI_ERR_OK;
}

bool CLab4UI::CreateAutoCheckbox(int parentIndex)
{
    GUI_STD_CHECKBOX_DESC checkbox{};
    checkbox.m_text = m_textRefs[TEXT_AUTO_CALCULATE];
    checkbox.m_onCheckChanged = OnAutoChanged;

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", "autoCalculate");
    desc.m_type = GUI_STD_CONTROL_CHECKBOX;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_ENABLED | GUI_STATE_VISIBLE;
    desc.m_controlFlags = GUI_FLAG_SUPPORTS_INPUT;
    desc.m_pos = CVector2(202.0f, 265.0f);
    desc.m_size = CVector2(194.0f, 30.0f);
    desc.m_subtypeControlDescPtr = &checkbox;
    return m_context->CreateControl(&desc) == GUI_ERR_OK;
}

bool CLab4UI::CreateControls()
{
    if (!CreateLabel("buildInfo", -1, CVector2(14.0f, 6.0f), CVector2(1210.0f, 24.0f), TEXT_BUILD))
        return false;

    int mainFrame = -1;
    if (!CreateFrame("mainFrame", -1, CVector2(12.0f, 34.0f), CVector2(1280.0f, 856.0f), &mainFrame))
        return false;

    int inputGroup = -1;
    int resultGroup = -1;
    if (!CreateGroupBox("inputGroup", mainFrame, CVector2(14.0f, 14.0f), CVector2(420.0f, 320.0f), TEXT_INPUT_GROUP, &inputGroup) || !CreateGroupBox("resultGroup", mainFrame, CVector2(448.0f, 14.0f), CVector2(814.0f, 320.0f), TEXT_RESULT_GROUP, &resultGroup))
        return false;

    if (!CreateLabel("temperatureTitle", inputGroup, CVector2(14.0f, 24.0f), CVector2(235.0f, 25.0f), TEXT_TEMPERATURE) || !CreateLabel("temperatureValue", inputGroup, CVector2(266.0f, 24.0f), CVector2(118.0f, 25.0f), TEXT_VALUE_TEMPERATURE) || !CreateSlider("temperatureSlider", inputGroup, CVector2(14.0f, 55.0f), -30.0f, 50.0f, m_input.temperature, SLOT_TEMPERATURE))
        return false;

    if (!CreateLabel("humidityTitle", inputGroup, CVector2(14.0f, 106.0f), CVector2(235.0f, 25.0f), TEXT_HUMIDITY) || !CreateLabel("humidityValue", inputGroup, CVector2(266.0f, 106.0f), CVector2(118.0f, 25.0f), TEXT_VALUE_HUMIDITY) || !CreateSlider("humiditySlider", inputGroup, CVector2(14.0f, 138.0f), 0.0f, 100.0f, m_input.humidity, SLOT_HUMIDITY))
        return false;

    if (!CreateLabel("pressureTitle", inputGroup, CVector2(14.0f, 187.0f), CVector2(235.0f, 25.0f), TEXT_PRESSURE) || !CreateLabel("pressureValue", inputGroup, CVector2(266.0f, 187.0f), CVector2(118.0f, 25.0f), TEXT_VALUE_PRESSURE) || !CreateSlider("pressureSlider", inputGroup, CVector2(14.0f, 219.0f), 950.0f, 1050.0f, m_input.pressure, SLOT_PRESSURE))
        return false;

    if (!CreateButton("calculateButton", inputGroup, CVector2(14.0f, 265.0f), CVector2(170.0f, 30.0f), TEXT_CALCULATE) || !CreateAutoCheckbox(inputGroup))
        return false;

    if (!CreateLabel("resultTitle", resultGroup, CVector2(20.0f, 28.0f), CVector2(350.0f, 27.0f), TEXT_RESULT_TITLE) || !CreateLabel("resultValue", resultGroup, CVector2(20.0f, 59.0f), CVector2(300.0f, 42.0f), TEXT_VALUE_RESULT) || !CreateLabel("rulesTitle", resultGroup, CVector2(20.0f, 134.0f), CVector2(350.0f, 27.0f), TEXT_RULES_TITLE) || !CreateLabel("ruleValue", resultGroup, CVector2(20.0f, 167.0f), CVector2(280.0f, 28.0f), TEXT_VALUE_RULES) || !CreateLabel("statusTitle", resultGroup, CVector2(20.0f, 214.0f), CVector2(350.0f, 27.0f), TEXT_STATUS_TITLE) || !CreateLabel("statusValue", resultGroup, CVector2(20.0f, 249.0f), CVector2(735.0f, 27.0f), TEXT_VALUE_STATUS))
        return false;

    struct SGraphPlaceholder { const char* id; ETextID title; CVector2 pos; };
    const SGraphPlaceholder graphs[] =
    {
        {"graphTemperature", TEXT_GRAPH_TEMPERATURE, CVector2(14.0f, 352.0f)},
        {"graphHumidity", TEXT_GRAPH_HUMIDITY, CVector2(648.0f, 352.0f)},
        {"graphPressure", TEXT_GRAPH_PRESSURE, CVector2(14.0f, 578.0f)},
        {"graphPrecipitation", TEXT_GRAPH_OUTPUT, CVector2(648.0f, 578.0f)}
    };

    for (const SGraphPlaceholder& graph : graphs)
    {
        int graphGroup = -1;
        if (!CreateGroupBox(graph.id, mainFrame, graph.pos, CVector2(614.0f, 212.0f), graph.title, &graphGroup) || !CreateLabel("graphPlaceholder", graphGroup, CVector2(20.0f, 80.0f), CVector2(570.0f, 32.0f), TEXT_GRAPH_PLACEHOLDER))
            return false;
    }
    return true;
}

void CLab4UI::SetText(IGUIString* destination, const char* text)
{
    if (destination)
        destination->SetText(text, &m_textStyle);
}

void CLab4UI::UpdateInputLabels()
{
    char text[96];
    std::snprintf(text, sizeof(text), "%.1f °C", m_input.temperature);
    SetText(m_values[SLOT_TEMPERATURE].Get(), text);
    std::snprintf(text, sizeof(text), "%.1f %%", m_input.humidity);
    SetText(m_values[SLOT_HUMIDITY].Get(), text);
    std::snprintf(text, sizeof(text), "%.1f гПа", m_input.pressure);
    SetText(m_values[SLOT_PRESSURE].Get(), text);
}

ERRCODE CLab4UI::OnSliderChanged(int controlIndex, float oldValue, float newValue, void* userData)
{
    (void)oldValue;
    if (!g_activeUI || !userData)
        return GUI_ERR_INVALID_ARG;

    const int slot = *static_cast<const int*>(userData);
    if (slot < 0 || slot >= SLOT_COUNT || controlIndex != g_activeUI->m_sliders[slot])
        return GUI_ERR_INVALID_ARG;

    switch (slot)
    {
        case SLOT_TEMPERATURE: g_activeUI->m_input.temperature = newValue; break;
        case SLOT_HUMIDITY: g_activeUI->m_input.humidity = newValue; break;
        case SLOT_PRESSURE: g_activeUI->m_input.pressure = newValue; break;
        default: return GUI_ERR_INVALID_ARG;
    }

    g_activeUI->UpdateInputLabels();
    if (g_activeUI->m_autoCalculation)
        g_activeUI->m_calculationRequested = true;
    return GUI_ERR_OK;
}

ERRCODE CLab4UI::OnAutoChanged(int controlIndex, dword oldState, dword newState, void* userData)
{
    (void)controlIndex;
    (void)oldState;
    (void)userData;
    if (!g_activeUI)
        return GUI_ERR_INVALID_ARG;

    g_activeUI->m_autoCalculation = (newState & GUI_STATE_CHECKED) != 0;
    if (g_activeUI->m_autoCalculation)
        g_activeUI->m_calculationRequested = true;
    return GUI_ERR_OK;
}

ERRCODE CLab4UI::OnCalculatePressed()
{
    if (!g_activeUI)
        return GUI_ERR_INVALID_ARG;
    g_activeUI->m_calculationRequested = true;
    return GUI_ERR_OK;
}

bool CLab4UI::Initialize(IGUIRenderStrategy* renderer, float width, float height)
{
    if (!renderer || g_activeUI)
        return false;

    const qword mask = GUI_REG_STD_CONTROL_SYS_FRAME | GUI_REG_STD_CONTROL_SYS_BUTTON | GUI_REG_STD_CONTROL_SYS_LABEL | GUI_REG_STD_CONTROL_SYS_SLIDER | GUI_REG_STD_CONTROL_SYS_GROUPBOX | GUI_REG_STD_CONTROL_SYS_CHECKBOX;
    if (CreateGUIContext(mask, m_context.ReleaseAndGetAddressOf()) != GUI_ERR_OK || !m_context.Get())
        return false;

    g_activeUI = this;
    if (!LoadFonts() || !LoadStrings() || !CreateControls())
    {
        Shutdown();
        return false;
    }

    m_context->SetColorTheme(GUI_STD_COLOR_THEME_DARK);
    m_context->SetRenderStrategy(renderer);
    Resize(width, height);
    UpdateInputLabels();
    return true;
}

void CLab4UI::Resize(float width, float height)
{
    if (!m_context.Get() || width <= 0.0f || height <= 0.0f)
        return;
    m_context->SetScreenResolution(width, height);
    m_context->SetViewport(GUI_VIEWPORT(0.0f, 0.0f, width, height));
}

void CLab4UI::Update(float deltaTime)
{
    if (m_context.Get())
        m_context->Update(deltaTime);
}

IGUIContext* CLab4UI::GetContext()
{
    return m_context.Get();
}

bool CLab4UI::ConsumeCalculationRequest()
{
    const bool requested = m_calculationRequested;
    m_calculationRequested = false;
    return requested;
}

void CLab4UI::GetWeatherInput(SWeatherInput& outInput) const
{
    outInput = m_input;
}

void CLab4UI::ShowCalculationPending()
{
    SetText(m_resultValue.Get(), "—");
    SetText(m_ruleValue.Get(), "—");
    SetText(m_statusValue.Get(), "Алгоритм Мамдани пока не подключён");
}

void CLab4UI::SetWeatherResult(float probability, const char* category, uint32 activeRules)
{
    char text[128];
    std::snprintf(text, sizeof(text), "%.1f %% (%s)", probability, category ? category : "—");
    SetText(m_resultValue.Get(), text);
    std::snprintf(text, sizeof(text), "%u", unsigned(activeRules));
    SetText(m_ruleValue.Get(), text);
    SetText(m_statusValue.Get(), "Расчёт завершён");
}

void CLab4UI::Shutdown()
{
    if (g_activeUI == this)
        g_activeUI = nullptr;

    m_context.Reset();
    for (int i = 0; i < SLOT_COUNT; ++i)
    {
        m_values[i].Reset();
        m_sliders[i] = -1;
    }
    m_resultValue.Reset();
    m_ruleValue.Reset();
    m_statusValue.Reset();
    m_stringTable.Reset();
    m_fonts[0].Reset();
    m_fonts[1].Reset();
    m_calculationRequested = false;
    m_autoCalculation = false;
}
