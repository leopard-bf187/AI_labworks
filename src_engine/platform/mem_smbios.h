#pragma once

#include <cstdint>
#include <cstring>
#include "data_types.h"
#include "platform.h"

using namespace krystallic;
using namespace krystallic::Platform;


#pragma pack(push, 1)

struct SSMBIOSHeader
{
	byte type;
	byte length;
	word handle;
};

#pragma pack(pop)


extern const char* GetSMBIOSString(const SSMBIOSHeader* header, byte stringIndex, const byte* tableEnd);
extern const byte* GetNextSMBIOSStructure(const SSMBIOSHeader* header, const byte* tableEnd);
extern qword DecodeSMBIOSMemorySize(const byte* data, byte length);
extern dword DecodeSMBIOSMemorySpeed(const byte* data, byte length, size_t legacyOffset, size_t extendedOffset);
extern bool ParseFirstSMBIOSMemoryDevice(const byte* table, size_t tableSize, SPhysicalMemoryDeviceInfo& outInfo, dword* outModuleCount, bool* outHasECC);
bool ParseSMBIOSMemoryDevice(const byte* table, size_t tableSize, dword requestedIndex, SPhysicalMemoryDeviceInfo* outInfo, dword* outDeviceCount);
dword CountSMBIOSMemoryDevices(const byte* table, size_t tableSize);

