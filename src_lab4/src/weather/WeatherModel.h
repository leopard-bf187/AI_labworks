#pragma once

#include "fuzzy/FuzzySystem.h"

class CWeatherModel
{
public:
    EWeatherStatus Evaluate(const SWeatherInput& input, SWeatherResult& result) const;

    const CFuzzySystem& GetFuzzySystem() const;
    static const char* GetCategoryText(EFuzzyTerm term);

private:
    CFuzzySystem m_fuzzySystem;
};
