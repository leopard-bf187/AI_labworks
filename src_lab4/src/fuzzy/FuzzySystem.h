#pragma once

#include "FuzzyTypes.h"
#include "weather/WeatherTypes.h"

class CFuzzySystem
{
public:
    static constexpr unsigned int kTermCount = 3;
    static constexpr unsigned int kInputCount = 3;
    static constexpr unsigned int kRuleCount = 9;

    CFuzzySystem();

    EWeatherStatus Evaluate(const SWeatherInput& input, SWeatherResult& result) const;

    float GetMembership(EFuzzyVariable variable, EFuzzyTerm term, float value) const;
    float GetAggregatedMembership(const SWeatherResult& result, float value) const;
    const SFuzzyRule* GetRule(unsigned int index) const;

private:
    SFuzzyMembership m_sets[4][kTermCount]{};
    SFuzzyRule m_rules[kRuleCount]{};

    static float EvaluateMembership(const SFuzzyMembership& membership, float value);
    static float EvaluateOperator(EFuzzyOperator operation, float left, float right);
};
