#pragma once

#include "fuzzy/FuzzyTypes.h"

struct SWeatherInput
{
    float temperature = 18.0f; // degrees Celsius
    float humidity = 65.0f;    // percent
    float pressure = 1012.0f;  // hPa
};

enum class EWeatherStatus : unsigned int
{
    Success,
    InvalidInput,
    NoActiveRules
};

struct SWeatherResult
{
    EWeatherStatus status = EWeatherStatus::NoActiveRules;
    float probability = 0.0f;
    EFuzzyTerm category = EFuzzyTerm::Low;
    float inputMembership[3][3]{};
    float outputActivation[3]{};
    float ruleActivation[9]{};
    unsigned int activeRuleCount = 0;
};
