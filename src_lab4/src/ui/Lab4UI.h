#pragma once

#include "pch.h"
#include "gui.h"
#include "weather/WeatherTypes.h"
#include "MembershipGraph.h"

class CLab4UI
{
private:
    enum EInputSlot : int
    {
        SLOT_TEMPERATURE,
        SLOT_HUMIDITY,
        SLOT_PRESSURE,
        SLOT_COUNT
    };

    enum ETextID : int
    {
        TEXT_BUILD,
        TEXT_INPUT_GROUP,
        TEXT_RESULT_GROUP,
        TEXT_TEMPERATURE,
        TEXT_HUMIDITY,
        TEXT_PRESSURE,
        TEXT_CALCULATE,
        TEXT_AUTO_CALCULATE,
        TEXT_RESULT_TITLE,
        TEXT_RULES_TITLE,
        TEXT_STATUS_TITLE,
        TEXT_GRAPH_TEMPERATURE,
        TEXT_GRAPH_HUMIDITY,
        TEXT_GRAPH_PRESSURE,
        TEXT_GRAPH_OUTPUT,
        TEXT_DIAGNOSTIC_GROUP,
        TEXT_DIAGNOSTIC_RULE_1,
        TEXT_DIAGNOSTIC_RULE_2,
        TEXT_DIAGNOSTIC_RULE_3,
        TEXT_DIAGNOSTIC_RULE_4,
        TEXT_DIAGNOSTIC_RULE_5,
        TEXT_DIAGNOSTIC_RULE_6,
        TEXT_DIAGNOSTIC_RULE_7,
        TEXT_DIAGNOSTIC_RULE_8,
        TEXT_DIAGNOSTIC_RULE_9,
        TEXT_DIAGNOSTIC_SUMMARY,
        TEXT_VALUE_TEMPERATURE,
        TEXT_VALUE_HUMIDITY,
        TEXT_VALUE_PRESSURE,
        TEXT_VALUE_RESULT,
        TEXT_VALUE_RULES,
        TEXT_VALUE_STATUS,
        TEXT_COUNT
    };

    enum EGraphTextID : int
    {
        GRAPH_TEXT_LOW,
        GRAPH_TEXT_MEDIUM,
        GRAPH_TEXT_HIGH,
        GRAPH_TEXT_ZERO,
        GRAPH_TEXT_ONE,
        GRAPH_TEXT_T_MIN,
        GRAPH_TEXT_T_MAX,
        GRAPH_TEXT_H_MIN,
        GRAPH_TEXT_H_MAX,
        GRAPH_TEXT_P_MIN,
        GRAPH_TEXT_P_MAX,
        GRAPH_TEXT_O_MIN,
        GRAPH_TEXT_O_MAX,
        GRAPH_TEXT_COUNT
    };

    static constexpr int GRAPH_COUNT = 4;
    static constexpr int DIAGNOSTIC_RULE_COUNT = CFuzzySystem::kRuleCount;
    CMembershipGraphSystem m_graphSystem;
    SMembershipGraphView m_graphViews[GRAPH_COUNT]{};
    krystallic::GUI::GUI_TEXT_REF m_graphTextRefs[GRAPH_TEXT_COUNT]{};

    krystallic::RefCounted<krystallic::GUI::IGUIContext> m_context;
    krystallic::RefCounted<krystallic::GUI::IGUIControlProxy> m_diagnosticsPanel;
    krystallic::RefCounted<krystallic::GUI::IGUIControlProxy> m_graphGroupControls[GRAPH_COUNT];
    krystallic::RefCounted<krystallic::GUI::IGUIStringTable> m_stringTable;
    krystallic::RefCounted<krystallic::GUI::IGUIFont> m_fonts[2];
    krystallic::RefCounted<krystallic::GUI::IGUIString> m_values[SLOT_COUNT];
    krystallic::RefCounted<krystallic::GUI::IGUIString> m_resultValue;
    krystallic::RefCounted<krystallic::GUI::IGUIString> m_ruleValue;
    krystallic::RefCounted<krystallic::GUI::IGUIString> m_statusValue;
    krystallic::RefCounted<krystallic::GUI::IGUIString> m_diagnosticRuleValues[DIAGNOSTIC_RULE_COUNT];
    krystallic::RefCounted<krystallic::GUI::IGUIString> m_diagnosticSummary;
    int m_diagnosticBars[DIAGNOSTIC_RULE_COUNT]{};
    bool m_diagnosticsVisible = false;
    krystallic::GUI::GUI_TEXT_STYLE m_textStyle{};
    krystallic::GUI::GUI_TEXT_REF m_textRefs[TEXT_COUNT]{};
    SWeatherInput m_input{};

    int m_sliders[SLOT_COUNT] = {-1, -1, -1};
    int m_sliderSlotIds[SLOT_COUNT] = {0, 1, 2};
    bool m_autoCalculation = false;
    bool m_calculationRequested = false;

    bool LoadFonts();
    bool LoadStrings();
    bool CreateControls();
    bool CreateDiagnosticsPanel(int parentIndex);
    bool CreateDiagnosticProgressBar(const char* id, int parentIndex, const krystallic::SIMDMath::CVector2& pos, int ruleIndex);
    void UpdateDiagnostics(const SWeatherResult* result);
    bool InitializeGraphViews(const CFuzzySystem* system);
    void SyncGraphInputs();
    bool CreateMembershipGraph(const char* id, int parentIndex, int graphIndex);
    bool CreateFrame(const char* id, int parentIndex, const krystallic::SIMDMath::CVector2& pos, const krystallic::SIMDMath::CVector2& size, int* outIndex);
    bool CreateGroupBox(const char* id, int parentIndex, const krystallic::SIMDMath::CVector2& pos, const krystallic::SIMDMath::CVector2& size, ETextID title, int* outIndex);
    bool CreateLabel(const char* id, int parentIndex, const krystallic::SIMDMath::CVector2& pos, const krystallic::SIMDMath::CVector2& size, ETextID text, int* outIndex = nullptr);
    bool CreateSlider(const char* id, int parentIndex, const krystallic::SIMDMath::CVector2& pos, float minValue, float maxValue, float value, EInputSlot slot);
    bool CreateButton(const char* id, int parentIndex, const krystallic::SIMDMath::CVector2& pos, const krystallic::SIMDMath::CVector2& size, ETextID text);
    bool CreateAutoCheckbox(int parentIndex);
    void UpdateInputLabels();
    void SetText(krystallic::GUI::IGUIString* destination, const char* text);

    static ERRCODE OnSliderChanged(int controlIndex, float oldValue, float newValue, void* userData);
    static ERRCODE OnAutoChanged(int controlIndex, dword oldState, dword newState, void* userData);
    static ERRCODE OnCalculatePressed();

public:
    bool Initialize(krystallic::GUI::IGUIRenderStrategy* renderer, float width, float height, const CFuzzySystem* fuzzySystem);
    void Shutdown();
    void Update(float deltaTime);
    void Resize(float width, float height);
    void ToggleDiagnostics();

    krystallic::GUI::IGUIContext* GetContext();
    bool ConsumeCalculationRequest();
    void GetWeatherInput(SWeatherInput& outInput) const;
    void ShowCalculationPending();
    void SetWeatherResult(const SWeatherResult& result, const char* category);
};
