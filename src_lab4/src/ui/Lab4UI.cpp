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
        {"lab4_auto", "Авторасчёт"},
        {"lab4_result_title", "Вероятность осадков:"},
        {"lab4_rules_title", "Активные правила:"},
        {"lab4_status_title", "Состояние:"},
        {"lab4_graph_temperature", "Функции принадлежности: температура"},
        {"lab4_graph_humidity", "Функции принадлежности: влажность"},
        {"lab4_graph_pressure", "Функции принадлежности: давление"},
        {"lab4_graph_output", "Функции принадлежности: вероятность осадков"},
        {"lab4_diagnostic_group", "Активация правил Мамдани (F1: графики)"},
        {"lab4_diag_rule01", "R1: не рассчитано"},
        {"lab4_diag_rule02", "R2: не рассчитано"},
        {"lab4_diag_rule03", "R3: не рассчитано"},
        {"lab4_diag_rule04", "R4: не рассчитано"},
        {"lab4_diag_rule05", "R5: не рассчитано"},
        {"lab4_diag_rule06", "R6: не рассчитано"},
        {"lab4_diag_rule07", "R7: не рассчитано"},
        {"lab4_diag_rule08", "R8: не рассчитано"},
        {"lab4_diag_rule09", "R9: не рассчитано"},
        {"lab4_diag_summary", "Нет результатов расчёта"},
        {"lab4_value_temperature", "18.0 °C"},
        {"lab4_value_humidity", "65.0 %"},
        {"lab4_value_pressure", "1012.0 гПа"},
        {"lab4_value_result", "Не рассчитана"},
        {"lab4_value_rules", "Не рассчитаны"},
        {"lab4_value_status", "Нажмите «Рассчитать»"}
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
            result = m_fonts[i]->AddFontStyleData(GUI_FONT_REGULAR, fontData.Get(), ranges, 16);
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

    for (int i = 0; i < DIAGNOSTIC_RULE_COUNT; ++i)
    {
        if (m_stringTable->GetStringByID(g_textEntries[TEXT_DIAGNOSTIC_RULE_1 + i].id, m_diagnosticRuleValues[i].ReleaseAndGetAddressOf()) != GUI_ERR_OK)
            return false;
    }

    if (m_stringTable->GetStringByID(g_textEntries[TEXT_DIAGNOSTIC_SUMMARY].id, m_diagnosticSummary.ReleaseAndGetAddressOf()) != GUI_ERR_OK)
        return false;

    static const STextEntry graphTextEntries[] =
    {
        {"graph_term_low", "Низкая"},
        {"graph_term_medium", "Средняя"},
        {"graph_term_high", "Высокая"},
        {"graph_axis_zero", "0"},
        {"graph_axis_one", "1"},
        {"graph_t_min", "-30"},
        {"graph_t_max", "50"},
        {"graph_h_min", "0"},
        {"graph_h_max", "100"},
        {"graph_p_min", "950"},
        {"graph_p_max", "1050"},
        {"graph_o_min", "0%"},
        {"graph_o_max", "100%"}
    };
    static_assert(sizeof(graphTextEntries) / sizeof(graphTextEntries[0]) == GRAPH_TEXT_COUNT);

    for (int i = 0; i < GRAPH_TEXT_COUNT; ++i)
    {
        if (m_stringTable->AddPlainString(graphTextEntries[i].id, graphTextEntries[i].value, &m_textStyle, &m_graphTextRefs[i]) != GUI_ERR_OK)
            return false;
    }

    return m_context->SetActiveStringTable(m_stringTable.Get()) == GUI_ERR_OK;
}

bool CLab4UI::InitializeGraphViews(const CFuzzySystem* system)
{
    if (!system)
        return false;

    const EFuzzyVariable variables[GRAPH_COUNT] = {EFuzzyVariable::Temperature, EFuzzyVariable::Humidity, EFuzzyVariable::Pressure, EFuzzyVariable::Precipitation};
    const float minValues[GRAPH_COUNT] = {-30.0f, 0.0f, 950.0f, 0.0f};
    const float maxValues[GRAPH_COUNT] = {50.0f, 100.0f, 1050.0f, 100.0f};

    for (int i = 0; i < GRAPH_COUNT; ++i)
    {
        SMembershipGraphView& view = m_graphViews[i];
        view.model = system;
        view.variable = variables[i];
        view.minValue = minValues[i];
        view.maxValue = maxValues[i];
        view.minLabel = m_graphTextRefs[GRAPH_TEXT_T_MIN + i * 2];
        view.maxLabel = m_graphTextRefs[GRAPH_TEXT_T_MAX + i * 2];
        view.zeroLabel = m_graphTextRefs[GRAPH_TEXT_ZERO];
        view.oneLabel = m_graphTextRefs[GRAPH_TEXT_ONE];
        for (int term = 0; term < 3; ++term)
            view.termLabels[term] = m_graphTextRefs[GRAPH_TEXT_LOW + term];
    }
    SyncGraphInputs();
    return true;
}

void CLab4UI::SyncGraphInputs()
{
    m_graphViews[0].currentValue = m_input.temperature;
    m_graphViews[1].currentValue = m_input.humidity;
    m_graphViews[2].currentValue = m_input.pressure;
}

bool CLab4UI::CreateMembershipGraph(const char* id, int parentIndex, int graphIndex)
{
    if (graphIndex < 0 || graphIndex >= GRAPH_COUNT)
        return false;

    GUI_MEMBERSHIP_GRAPH_DESC graph{};
    graph.view = &m_graphViews[graphIndex];

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", id);
    desc.m_type = CMembershipGraphSystem::CONTROL_TYPE;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_VISIBLE | GUI_STATE_ENABLED;
    desc.m_pos = CVector2(6.0f, 4.0f);
    desc.m_size = CVector2(460.0f, 124.0f);
    desc.m_subtypeControlDescPtr = &graph;
    return m_context->CreateControl(&desc) == GUI_ERR_OK;
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
    // Use a sufficiently wide title gap for the graph groupboxes.
    // Automatic header sizing is too narrow for their Cyrillic captions.
    if (title >= TEXT_GRAPH_TEMPERATURE && title <= TEXT_GRAPH_OUTPUT)
        group.m_headerWidth = title == TEXT_GRAPH_OUTPUT ? 442.0f : 370.0f;
    else if (title == TEXT_DIAGNOSTIC_GROUP)
        group.m_headerWidth = 365.0f;

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
    desc.m_size = CVector2(352.0f, 23.0f);
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
    desc.m_pos = CVector2(182.0f, 215.0f);
    desc.m_size = CVector2(184.0f, 28.0f);
    desc.m_subtypeControlDescPtr = &checkbox;
    return m_context->CreateControl(&desc) == GUI_ERR_OK;
}

bool CLab4UI::CreateDiagnosticProgressBar(const char* id, int parentIndex, const CVector2& pos, int ruleIndex)
{
    if (ruleIndex < 0 || ruleIndex >= DIAGNOSTIC_RULE_COUNT)
        return false;

    GUI_STD_PROGRESSBAR_DESC progress{};
    progress.m_min = 0.0f;
    progress.m_max = 1.0f;
    progress.m_value = 0.0f;

    GUI_CONTROL_DESC desc{};
    std::snprintf(desc.m_controlID, sizeof(desc.m_controlID), "%s", id);
    desc.m_type = GUI_STD_CONTROL_PROGRESSBAR;
    desc.m_parentIndex = parentIndex;
    desc.m_initialState = GUI_STATE_VISIBLE | GUI_STATE_ENABLED;
    desc.m_pos = pos;
    desc.m_size = CVector2(125.0f, 16.0f);
    desc.m_subtypeControlDescPtr = &progress;
    return m_context->CreateControl(&desc, &m_diagnosticBars[ruleIndex]) == GUI_ERR_OK;
}

bool CLab4UI::CreateDiagnosticsPanel(int parentIndex)
{
    int groupIndex = -1;
    if (!CreateGroupBox("diagnosticsPanel", parentIndex, CVector2(14.0f, 302.0f), CVector2(1008.0f, 348.0f), TEXT_DIAGNOSTIC_GROUP, &groupIndex))
        return false;

    for (int i = 0; i < DIAGNOSTIC_RULE_COUNT; ++i)
    {
        const int column = i >= 5 ? 1 : 0;
        const int row = column == 0 ? i : i - 5;
        const float x = column == 0 ? 16.0f : 510.0f;
        const float y = 16.0f + 52.0f * row;
        char labelID[64];
        char progressID[64];
        std::snprintf(labelID, sizeof(labelID), "diagnosticRule%u", static_cast<unsigned int>(i + 1));
        std::snprintf(progressID, sizeof(progressID), "diagnosticBar%u", static_cast<unsigned int>(i + 1));
        if (!CreateLabel(labelID, groupIndex, CVector2(x, y), CVector2(305.0f, 26.0f), static_cast<ETextID>(TEXT_DIAGNOSTIC_RULE_1 + i)) || !CreateDiagnosticProgressBar(progressID, groupIndex, CVector2(x + 330.0f, y + 5.0f), i))
            return false;
    }

    if (!CreateLabel("diagnosticSummary", groupIndex, CVector2(16.0f, 278.0f), CVector2(945.0f, 28.0f), TEXT_DIAGNOSTIC_SUMMARY))
        return false;

    if (m_context->GetControlProxy(groupIndex, m_diagnosticsPanel.ReleaseAndGetAddressOf()) != GUI_ERR_OK || !m_diagnosticsPanel.Get())
        return false;

    m_diagnosticsVisible = false;
    m_diagnosticsPanel->SetState(m_diagnosticsPanel->GetState() & ~GUI_STATE_VISIBLE);
    UpdateDiagnostics(nullptr);
    return true;
}

void CLab4UI::UpdateDiagnostics(const SWeatherResult* result)
{
    const CFuzzySystem* model = m_graphViews[0].model;
    if (!model || !m_context.Get())
        return;

    static const char* variableNames[] = {"T", "H", "P", "O"};
    static const char* termNames[] = {"низ.", "сред.", "выс."};

    char buffer[192];
    for (int i = 0; i < DIAGNOSTIC_RULE_COUNT; ++i)
    {
        const SFuzzyRule* rule = model->GetRule(static_cast<unsigned int>(i));
        if (!rule)
            continue;

        const char* leftVar = variableNames[static_cast<unsigned int>(rule->left.variable)];
        const char* rightVar = variableNames[static_cast<unsigned int>(rule->right.variable)];
        const char* leftTerm = termNames[static_cast<unsigned int>(rule->left.term)];
        const char* rightTerm = termNames[static_cast<unsigned int>(rule->right.term)];
        const char* conclusion = termNames[static_cast<unsigned int>(rule->conclusion)];
        const char* operation = rule->operation == EFuzzyOperator::And ? "И" : "ИЛИ";

        if (result && result->status != EWeatherStatus::InvalidInput)
        {
            std::snprintf(buffer, sizeof(buffer), "R%u: %s %s %s %s %s => %s | %.3f", static_cast<unsigned int>(i + 1), leftVar, leftTerm, operation, rightVar, rightTerm, conclusion, result->ruleActivation[i]);
        }
        else
        {
            std::snprintf(buffer, sizeof(buffer), "R%u: %s %s %s %s %s => %s | н/д", static_cast<unsigned int>(i + 1), leftVar, leftTerm, operation, rightVar, rightTerm, conclusion);
        }

        SetText(m_diagnosticRuleValues[i].Get(), buffer);
        if (m_diagnosticBars[i] >= 0)
            SetProgressBarValue(m_context.Get(), m_diagnosticBars[i], result && result->status != EWeatherStatus::InvalidInput ? result->ruleActivation[i] : 0.0f);
    }

    if (!result)
        SetText(m_diagnosticSummary.Get(), "Нет актуального расчёта. Измените параметры и нажмите «Рассчитать».");
    else if (result->status == EWeatherStatus::Success)
    {
        std::snprintf(buffer, sizeof(buffer), "Активных правил: %u из %u. Центроид: %.2f %%.", result->activeRuleCount, static_cast<unsigned int>(DIAGNOSTIC_RULE_COUNT), result->probability);
        SetText(m_diagnosticSummary.Get(), buffer);
    }
    else if (result->status == EWeatherStatus::NoActiveRules)
        SetText(m_diagnosticSummary.Get(), "Активных правил: 0. Ни одно выходное множество не активировано.");
    else
        SetText(m_diagnosticSummary.Get(), "Ошибка: недопустимые входные параметры.");
}

void CLab4UI::ToggleDiagnostics()
{
    if (!m_diagnosticsPanel.Get())
        return;

    m_diagnosticsVisible = !m_diagnosticsVisible;

    // The chart groupboxes and the diagnostics occupy the same screen area.
    // Make their visibility mutually exclusive rather than relying on
    // sibling draw order and background transparency.
    for (int i = 0; i < GRAPH_COUNT; ++i)
    {
        if (!m_graphGroupControls[i].Get())
            continue;

        dword graphState = m_graphGroupControls[i]->GetState();
        graphState = m_diagnosticsVisible ? (graphState & ~GUI_STATE_VISIBLE) : (graphState | GUI_STATE_VISIBLE);
        m_graphGroupControls[i]->SetState(graphState);
    }

    dword state = m_diagnosticsPanel->GetState();
    state = m_diagnosticsVisible ? (state | GUI_STATE_VISIBLE) : (state & ~GUI_STATE_VISIBLE);
    m_diagnosticsPanel->SetState(state);
}

bool CLab4UI::CreateControls()
{
    if (!CreateLabel("buildInfo", -1, CVector2(14.0f, 5.0f), CVector2(1030.0f, 24.0f), TEXT_BUILD))
        return false;

    int mainFrame = -1;
    if (!CreateFrame("mainFrame", -1, CVector2(12.0f, 34.0f), CVector2(1036.0f, 664.0f), &mainFrame))
        return false;

    int inputGroup = -1;
    int resultGroup = -1;
    if (!CreateGroupBox("inputGroup", mainFrame, CVector2(14.0f, 14.0f), CVector2(390.0f, 280.0f), TEXT_INPUT_GROUP, &inputGroup) || !CreateGroupBox("resultGroup", mainFrame, CVector2(418.0f, 14.0f), CVector2(604.0f, 280.0f), TEXT_RESULT_GROUP, &resultGroup))
        return false;

    if (!CreateLabel("temperatureTitle", inputGroup, CVector2(14.0f, 19.0f), CVector2(230.0f, 23.0f), TEXT_TEMPERATURE) || !CreateLabel("temperatureValue", inputGroup, CVector2(250.0f, 19.0f), CVector2(120.0f, 23.0f), TEXT_VALUE_TEMPERATURE) || !CreateSlider("temperatureSlider", inputGroup, CVector2(14.0f, 47.0f), -30.0f, 50.0f, m_input.temperature, SLOT_TEMPERATURE))
        return false;

    if (!CreateLabel("humidityTitle", inputGroup, CVector2(14.0f, 83.0f), CVector2(230.0f, 23.0f), TEXT_HUMIDITY) || !CreateLabel("humidityValue", inputGroup, CVector2(250.0f, 83.0f), CVector2(120.0f, 23.0f), TEXT_VALUE_HUMIDITY) || !CreateSlider("humiditySlider", inputGroup, CVector2(14.0f, 111.0f), 0.0f, 100.0f, m_input.humidity, SLOT_HUMIDITY))
        return false;

    if (!CreateLabel("pressureTitle", inputGroup, CVector2(14.0f, 147.0f), CVector2(230.0f, 23.0f), TEXT_PRESSURE) || !CreateLabel("pressureValue", inputGroup, CVector2(250.0f, 147.0f), CVector2(120.0f, 23.0f), TEXT_VALUE_PRESSURE) || !CreateSlider("pressureSlider", inputGroup, CVector2(14.0f, 175.0f), 950.0f, 1050.0f, m_input.pressure, SLOT_PRESSURE))
        return false;

    if (!CreateButton("calculateButton", inputGroup, CVector2(14.0f, 215.0f), CVector2(150.0f, 28.0f), TEXT_CALCULATE) || !CreateAutoCheckbox(inputGroup))
        return false;

    if (!CreateLabel("resultTitle", resultGroup, CVector2(18.0f, 20.0f), CVector2(560.0f, 24.0f), TEXT_RESULT_TITLE) || !CreateLabel("resultValue", resultGroup, CVector2(18.0f, 48.0f), CVector2(540.0f, 30.0f), TEXT_VALUE_RESULT) || !CreateLabel("rulesTitle", resultGroup, CVector2(18.0f, 93.0f), CVector2(560.0f, 24.0f), TEXT_RULES_TITLE) || !CreateLabel("ruleValue", resultGroup, CVector2(18.0f, 120.0f), CVector2(540.0f, 25.0f), TEXT_VALUE_RULES) || !CreateLabel("statusTitle", resultGroup, CVector2(18.0f, 160.0f), CVector2(560.0f, 24.0f), TEXT_STATUS_TITLE) || !CreateLabel("statusValue", resultGroup, CVector2(18.0f, 188.0f), CVector2(560.0f, 35.0f), TEXT_VALUE_STATUS))
        return false;

    struct SGraphDefinition { const char* id; ETextID title; CVector2 pos; };
    const SGraphDefinition graphs[] =
    {
        {"graphTemperature", TEXT_GRAPH_TEMPERATURE, CVector2(14.0f, 302.0f)},
        {"graphHumidity", TEXT_GRAPH_HUMIDITY, CVector2(526.0f, 302.0f)},
        {"graphPressure", TEXT_GRAPH_PRESSURE, CVector2(14.0f, 484.0f)},
        {"graphPrecipitation", TEXT_GRAPH_OUTPUT, CVector2(526.0f, 484.0f)}
    };

    for (int i = 0; i < GRAPH_COUNT; ++i)
    {
        int graphGroup = -1;
        if (!CreateGroupBox(graphs[i].id, mainFrame, graphs[i].pos, CVector2(496.0f, 166.0f), graphs[i].title, &graphGroup) || !CreateMembershipGraph("membershipPlot", graphGroup, i))
            return false;

        if (m_context->GetControlProxy(graphGroup, m_graphGroupControls[i].ReleaseAndGetAddressOf()) != GUI_ERR_OK || !m_graphGroupControls[i].Get())
            return false;
    }

    return CreateDiagnosticsPanel(mainFrame);
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
    g_activeUI->SyncGraphInputs();
    g_activeUI->ShowCalculationPending();
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

bool CLab4UI::Initialize(IGUIRenderStrategy* renderer, float width, float height, const CFuzzySystem* fuzzySystem)
{
    if (!renderer || !fuzzySystem || g_activeUI)
        return false;

    const qword mask = GUI_REG_STD_CONTROL_SYS_FRAME | GUI_REG_STD_CONTROL_SYS_BUTTON | GUI_REG_STD_CONTROL_SYS_LABEL | GUI_REG_STD_CONTROL_SYS_SLIDER | GUI_REG_STD_CONTROL_SYS_GROUPBOX | GUI_REG_STD_CONTROL_SYS_CHECKBOX | GUI_REG_STD_CONTROL_SYS_PROGRESSBAR;
    if (CreateGUIContext(mask, m_context.ReleaseAndGetAddressOf()) != GUI_ERR_OK || !m_context.Get())
        return false;

    g_activeUI = this;
    if (!LoadFonts() || !LoadStrings() || !InitializeGraphViews(fuzzySystem) || m_graphSystem.Register(m_context.Get()) != GUI_ERR_OK || !CreateControls())
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
    m_graphViews[3].hasResult = false;
    SetText(m_resultValue.Get(), "Не рассчитана");
    SetText(m_ruleValue.Get(), "Не рассчитаны");
    SetText(m_statusValue.Get(), "Параметры изменены. Выполните расчёт.");
    UpdateDiagnostics(nullptr);
}

void CLab4UI::SetWeatherResult(const SWeatherResult& result, const char* category)
{
    m_graphViews[3].result = result;
    m_graphViews[3].hasResult = result.status == EWeatherStatus::Success;

    if (result.status == EWeatherStatus::NoActiveRules)
    {
        SetText(m_resultValue.Get(), "Не определена");
        SetText(m_ruleValue.Get(), "0");
        SetText(m_statusValue.Get(), "Нет активных правил. Вероятность не определена.");
    }
    else if (result.status == EWeatherStatus::InvalidInput)
    {
        SetText(m_resultValue.Get(), "Не определена");
        SetText(m_ruleValue.Get(), "Не определены");
        SetText(m_statusValue.Get(), "Ошибка: недопустимые входные параметры.");
    }
    else
    {
        char text[128];
        std::snprintf(text, sizeof(text), "%.1f %% (%s)", result.probability, category ? category : "не определена");
        SetText(m_resultValue.Get(), text);
        std::snprintf(text, sizeof(text), "%u", result.activeRuleCount);
        SetText(m_ruleValue.Get(), text);
        SetText(m_statusValue.Get(), "Расчёт завершён");
    }

    UpdateDiagnostics(&result);
}

void CLab4UI::Shutdown()
{
    if (g_activeUI == this)
        g_activeUI = nullptr;

    m_diagnosticsPanel.Reset();
    for (int i = 0; i < GRAPH_COUNT; ++i)
        m_graphGroupControls[i].Reset();
    m_context.Reset();
    for (int i = 0; i < SLOT_COUNT; ++i)
    {
        m_values[i].Reset();
        m_sliders[i] = -1;
    }
    m_resultValue.Reset();
    m_ruleValue.Reset();
    m_statusValue.Reset();
    for (int i = 0; i < DIAGNOSTIC_RULE_COUNT; ++i)
    {
        m_diagnosticRuleValues[i].Reset();
        m_diagnosticBars[i] = -1;
    }
    m_diagnosticSummary.Reset();
    m_diagnosticsVisible = false;
    m_stringTable.Reset();
    m_fonts[0].Reset();
    m_fonts[1].Reset();
    m_calculationRequested = false;
    m_autoCalculation = false;
    for (int i = 0; i < GRAPH_COUNT; ++i)
        m_graphViews[i] = {};
}
