#include "FuzzySystem.h"

#include <algorithm>
#include <cmath>

namespace
{
    constexpr float kMinTemperature = -30.0f;
    constexpr float kMaxTemperature = 50.0f;
    constexpr float kMinHumidity = 0.0f;
    constexpr float kMaxHumidity = 100.0f;
    constexpr float kMinPressure = 950.0f;
    constexpr float kMaxPressure = 1050.0f;
    constexpr float kIntegrationStep = 0.25f;
}

CFuzzySystem::CFuzzySystem()
{
    // Shoulder sets cover the limits of the input/output universes.
    m_sets[0][0] = {-30.0f, -30.0f, 0.0f, 12.0f};
    m_sets[0][1] = {0.0f, 15.0f, 20.0f, 30.0f};
    m_sets[0][2] = {20.0f, 35.0f, 50.0f, 50.0f};

    m_sets[1][0] = {0.0f, 0.0f, 25.0f, 45.0f};
    m_sets[1][1] = {30.0f, 45.0f, 60.0f, 75.0f};
    m_sets[1][2] = {60.0f, 80.0f, 100.0f, 100.0f};

    m_sets[2][0] = {950.0f, 950.0f, 975.0f, 995.0f};
    m_sets[2][1] = {980.0f, 1000.0f, 1015.0f, 1030.0f};
    m_sets[2][2] = {1015.0f, 1035.0f, 1050.0f, 1050.0f};

    m_sets[3][0] = {0.0f, 0.0f, 20.0f, 40.0f};
    m_sets[3][1] = {25.0f, 45.0f, 55.0f, 75.0f};
    m_sets[3][2] = {60.0f, 80.0f, 100.0f, 100.0f};

    using V = EFuzzyVariable;
    using T = EFuzzyTerm;
    using O = EFuzzyOperator;

    // These nine rules follow the order and conclusions in AILab4.md.
    m_rules[0] = {{V::Temperature, T::Low}, O::And, {V::Humidity, T::High}, T::High};
    m_rules[1] = {{V::Temperature, T::Medium}, O::And, {V::Humidity, T::High}, T::High};
    m_rules[2] = {{V::Temperature, T::High}, O::And, {V::Pressure, T::High}, T::Low};
    m_rules[3] = {{V::Temperature, T::High}, O::And, {V::Humidity, T::Low}, T::Low};
    m_rules[4] = {{V::Pressure, T::Low}, O::And, {V::Humidity, T::High}, T::High};
    m_rules[5] = {{V::Pressure, T::Medium}, O::And, {V::Humidity, T::Medium}, T::Medium};
    m_rules[6] = {{V::Temperature, T::Low}, O::And, {V::Pressure, T::Low}, T::High};
    m_rules[7] = {{V::Temperature, T::Medium}, O::And, {V::Pressure, T::High}, T::Low};
    m_rules[8] = {{V::Temperature, T::High}, O::And, {V::Humidity, T::Medium}, T::Medium};
}

float CFuzzySystem::EvaluateMembership(const SFuzzyMembership& membership, float value)
{
    if (value < membership.a || value > membership.d)
        return 0.0f;
    if (value >= membership.b && value <= membership.c)
        return 1.0f;
    if (value < membership.b)
        return (value - membership.a) / (membership.b - membership.a);
    return (membership.d - value) / (membership.d - membership.c);
}

float CFuzzySystem::EvaluateOperator(EFuzzyOperator operation, float left, float right)
{
    return operation == EFuzzyOperator::And ? std::min(left, right) : std::max(left, right);
}

float CFuzzySystem::GetMembership(EFuzzyVariable variable, EFuzzyTerm term, float value) const
{
    const unsigned int variableIndex = static_cast<unsigned int>(variable);
    const unsigned int termIndex = static_cast<unsigned int>(term);
    if (variableIndex >= 4 || termIndex >= kTermCount || !std::isfinite(value))
        return 0.0f;
    return EvaluateMembership(m_sets[variableIndex][termIndex], value);
}

float CFuzzySystem::GetAggregatedMembership(const SWeatherResult& result, float value) const
{
    float aggregated = 0.0f;
    for (unsigned int term = 0; term < kTermCount; ++term)
    {
        const float membership = GetMembership(EFuzzyVariable::Precipitation, static_cast<EFuzzyTerm>(term), value);
        aggregated = std::max(aggregated, std::min(result.outputActivation[term], membership));
    }
    return aggregated;
}

const SFuzzyRule* CFuzzySystem::GetRule(unsigned int index) const
{
    return index < kRuleCount ? &m_rules[index] : nullptr;
}

EWeatherStatus CFuzzySystem::Evaluate(const SWeatherInput& input, SWeatherResult& result) const
{
    result = {};

    if (!std::isfinite(input.temperature) || !std::isfinite(input.humidity) || !std::isfinite(input.pressure) ||
        input.temperature < kMinTemperature || input.temperature > kMaxTemperature ||
        input.humidity < kMinHumidity || input.humidity > kMaxHumidity ||
        input.pressure < kMinPressure || input.pressure > kMaxPressure)
    {
        result.status = EWeatherStatus::InvalidInput;
        return result.status;
    }

    const float inputValues[kInputCount] = {input.temperature, input.humidity, input.pressure};

    // Fuzzification: each input may belong to several linguistic terms.
    for (unsigned int variable = 0; variable < kInputCount; ++variable)
    {
        for (unsigned int term = 0; term < kTermCount; ++term)
        {
            result.inputMembership[variable][term] = GetMembership(static_cast<EFuzzyVariable>(variable), static_cast<EFuzzyTerm>(term), inputValues[variable]);
        }
    }

    // Mamdani inference: min for AND, max for OR, max for aggregation.
    for (unsigned int index = 0; index < kRuleCount; ++index)
    {
        const SFuzzyRule& rule = m_rules[index];
        const float left = result.inputMembership[static_cast<unsigned int>(rule.left.variable)][static_cast<unsigned int>(rule.left.term)];
        const float right = result.inputMembership[static_cast<unsigned int>(rule.right.variable)][static_cast<unsigned int>(rule.right.term)];
        const float strength = EvaluateOperator(rule.operation, left, right);

        result.ruleActivation[index] = strength;
        if (strength > 0.0f)
            ++result.activeRuleCount;
        const unsigned int outputTerm = static_cast<unsigned int>(rule.conclusion);
        result.outputActivation[outputTerm] = std::max(result.outputActivation[outputTerm], strength);
    }

    if (result.activeRuleCount == 0)
    {
        result.status = EWeatherStatus::NoActiveRules;
        return result.status;
    }

    // Centroid defuzzification on the output universe [0, 100].
    // The fixed step makes the result independent from the frame rate.
    double numerator = 0.0;
    double denominator = 0.0;
    for (unsigned int index = 0; index <= 400; ++index)
    {
        const float probability = static_cast<float>(index) * kIntegrationStep;
        const double membership = GetAggregatedMembership(result, probability);
        const double weight = (index == 0 || index == 400) ? 0.5 : 1.0;
        numerator += weight * membership * probability;
        denominator += weight * membership;
    }

    if (denominator <= 0.0)
    {
        result.status = EWeatherStatus::NoActiveRules;
        return result.status;
    }

    result.probability = static_cast<float>(numerator / denominator);

    // Category of the crisp centroid, not the largest output activation.
    float bestMembership = -1.0f;
    for (unsigned int term = 0; term < kTermCount; ++term)
    {
        const float membership = GetMembership(EFuzzyVariable::Precipitation, static_cast<EFuzzyTerm>(term), result.probability);
        if (membership > bestMembership)
        {
            bestMembership = membership;
            result.category = static_cast<EFuzzyTerm>(term);
        }
    }

    result.status = EWeatherStatus::Success;
    return result.status;
}
