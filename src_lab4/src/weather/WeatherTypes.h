#pragma once

// Input ranges are initial UI defaults. The fuzzy membership sets and rules
// will be designed separately from the Krystallic rendering infrastructure.
struct SWeatherInput
{
    float temperature = 18.0f; // degrees Celsius
    float humidity = 65.0f;    // percent
    float pressure = 1012.0f;  // hPa
};
