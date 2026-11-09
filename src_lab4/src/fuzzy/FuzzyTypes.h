#pragma once

enum class EFuzzyVariable : unsigned int
{
    Temperature = 0,
    Humidity,
    Pressure,
    Precipitation,
    Count
};

enum class EFuzzyTerm : unsigned int
{
    Low = 0,
    Medium,
    High,
    Count
};

enum class EFuzzyOperator : unsigned int
{
    And,
    Or
};

struct SFuzzyMembership
{
    float a;
    float b;
    float c;
    float d;
};

struct SFuzzyCondition
{
    EFuzzyVariable variable;
    EFuzzyTerm term;
};

struct SFuzzyRule
{
    SFuzzyCondition left;
    EFuzzyOperator operation;
    SFuzzyCondition right;
    EFuzzyTerm conclusion;
};
