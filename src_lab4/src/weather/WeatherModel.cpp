#include "WeatherModel.h"

EWeatherStatus CWeatherModel::Evaluate(const SWeatherInput& input, SWeatherResult& result) const
{
    return m_fuzzySystem.Evaluate(input, result);
}

const CFuzzySystem& CWeatherModel::GetFuzzySystem() const
{
    return m_fuzzySystem;
}

const char* CWeatherModel::GetCategoryText(EFuzzyTerm term)
{
    switch (term)
    {
        case EFuzzyTerm::Low: return "низкая";
        case EFuzzyTerm::Medium: return "средняя";
        case EFuzzyTerm::High: return "высокая";
        default: return "не определена";
    }
}
