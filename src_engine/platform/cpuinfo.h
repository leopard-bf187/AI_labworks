#pragma once

#include "platform_helpers.h"

using namespace krystallic;
using namespace krystallic::Platform;

extern dword GetMissingCpuFeatureMask(dword supportedMask, dword requiredMask);
extern bool ValidateCpuFeatureMask(dword supportedMask, dword requiredMask);
extern bool QuerySystemCPUInfo(SSystemCPUInfo* outInfo);


