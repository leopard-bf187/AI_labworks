#include "mem_smbios.h"


void ResetSystemMemoryInfo(SSystemMemoryInfo& info)
{
	memset(&info, 0, sizeof(info));
}

void ResetPhysicalMemoryDeviceInfo(SPhysicalMemoryDeviceInfo& info)
{
	memset(&info, 0, sizeof(info));
	info.type = EMemoryType::MEM_TYPE_UNKNOWN;
	info.formFactor = EMemoryFormFactor::MEM_FORM_FACTOR_UNKNOWN;
}

inline uint16 ReadU16(const void* ptr)
{
	uint16 value;
	memcpy(&value, ptr, sizeof(value));
	return value;
}

inline uint32 ReadU32(const void* ptr)
{
	uint32 value;
	memcpy(&value, ptr, sizeof(value));
	return value;
}



const char* GetSMBIOSString(const SSMBIOSHeader* header, byte stringIndex, const byte* tableEnd)
{
	if (!header || stringIndex == 0)
		return "";

	const char* string = reinterpret_cast<const char*>(header) + header->length;
	const char* end = reinterpret_cast<const char*>(tableEnd);

	for (byte index = 1; index < stringIndex && string < end && *string; ++index)
	{
		while (string < end && *string)
			++string;

		if (string < end)
			++string;
	}

	if (string >= end || !*string)
		return "";

	return string;
}

const byte* GetNextSMBIOSStructure(const SSMBIOSHeader* header, const byte* tableEnd)
{
	if (!header || header->length < sizeof(SSMBIOSHeader))
		return nullptr;

	const byte* ptr = reinterpret_cast<const byte*>(header) + header->length;

	while (ptr + 1 < tableEnd)
	{
		if (ptr[0] == 0 && ptr[1] == 0)
			return ptr + 2;

		++ptr;
	}

	return nullptr;
}

qword DecodeSMBIOSMemorySize(const byte* data, byte length)
{
	if (length <= 0x0D)
		return 0;

	const word size = ReadU16(data + 0x0C);

	if (size == 0 || size == 0xFFFF)
		return 0;

	if (size == 0x7FFF)
	{
		if (length <= 0x1F)
			return 0;

		const dword extendedSizeMB = ReadU32(data + 0x1C) & 0x7FFFFFFFu;
		return static_cast<qword>(extendedSizeMB) * 1024ull * 1024ull;
	}

	if (size & 0x8000)
		return static_cast<qword>(size & 0x7FFF) * 1024ull;

	return static_cast<qword>(size) * 1024ull * 1024ull;
}

dword DecodeSMBIOSMemorySpeed(const byte* data, byte length, size_t legacyOffset, size_t extendedOffset)
{
	if (length <= legacyOffset + sizeof(word) - 1)
		return 0;

	const word legacySpeed = ReadU16(data + legacyOffset);

	if (legacySpeed != 0 && legacySpeed != 0xFFFF)
		return legacySpeed;

	if (legacySpeed == 0xFFFF && length > extendedOffset + sizeof(dword) - 1)
		return ReadU32(data + extendedOffset);

	return 0;
}

bool ParseFirstSMBIOSMemoryDevice(const byte* table, size_t tableSize, SPhysicalMemoryDeviceInfo& outInfo, dword* outModuleCount, bool* outHasECC)
{
	if (!table || tableSize < sizeof(SSMBIOSHeader))
		return false;

	const byte* ptr = table;
	const byte* end = table + tableSize;

	bool found = false;
	bool systemHasECC = false;
	dword moduleCount = 0;

	while (ptr + sizeof(SSMBIOSHeader) <= end)
	{
		const SSMBIOSHeader* header = reinterpret_cast<const SSMBIOSHeader*>(ptr);

		if (header->length < sizeof(SSMBIOSHeader) || ptr + header->length > end)
			break;

		if (header->type == 17)
		{
			const qword capacity = DecodeSMBIOSMemorySize(ptr, header->length);

			if (capacity != 0)
			{
				++moduleCount;

				word totalWidth = 0;
				word dataWidth = 0;

				if (header->length > 0x09)
					totalWidth = ReadU16(ptr + 0x08);

				if (header->length > 0x0B)
					dataWidth = ReadU16(ptr + 0x0A);

				const bool hasECC = totalWidth != 0 && totalWidth != 0xFFFF &&
					dataWidth != 0 && dataWidth != 0xFFFF &&
					totalWidth > dataWidth;

				systemHasECC = systemHasECC || hasECC;

				if (!found)
				{
					ResetPhysicalMemoryDeviceInfo(outInfo);

					outInfo.capacity = capacity;
					outInfo.totalWidth = totalWidth == 0xFFFF ? 0 : totalWidth;
					outInfo.dataWidth = dataWidth == 0xFFFF ? 0 : dataWidth;
					outInfo.hasECC = hasECC;

					if (header->length > 0x0E)
						outInfo.formFactor = static_cast<EMemoryFormFactor>(ptr[0x0E]);

					if (header->length > 0x12)
						outInfo.type = static_cast<EMemoryType>(ptr[0x12]);

					outInfo.speedMHz = DecodeSMBIOSMemorySpeed(ptr, header->length, 0x15, 0x54);
					outInfo.configuredSpeedMHz = DecodeSMBIOSMemorySpeed(ptr, header->length, 0x20, 0x58);

					if (header->length > 0x1B)
						outInfo.rank = ptr[0x1B] & 0x0F;

					if (header->length > 0x10)
						Stdlib::StrCopyN(outInfo.deviceLocator, GetSMBIOSString(header, ptr[0x10], end), sizeof(outInfo.deviceLocator));

					if (header->length > 0x11)
						Stdlib::StrCopyN(outInfo.bankLocator, GetSMBIOSString(header, ptr[0x11], end), sizeof(outInfo.bankLocator));

					if (header->length > 0x17)
						Stdlib::StrCopyN(outInfo.manufacturer, GetSMBIOSString(header, ptr[0x17], end), sizeof(outInfo.manufacturer));

					if (header->length > 0x18)
						Stdlib::StrCopyN(outInfo.serialNumber, GetSMBIOSString(header, ptr[0x18], end), sizeof(outInfo.serialNumber));

					if (header->length > 0x1A)
						Stdlib::StrCopyN(outInfo.partNumber, GetSMBIOSString(header, ptr[0x1A], end), sizeof(outInfo.partNumber));

					found = true;
				}
			}
		}

		if (header->type == 127)
			break;

		ptr = GetNextSMBIOSStructure(header, end);

		if (!ptr)
			break;
	}

	if (outModuleCount)
		*outModuleCount = moduleCount;

	if (outHasECC)
		*outHasECC = systemHasECC;

	return found;
}


bool ParseSMBIOSMemoryDevice(const byte* table, size_t tableSize, dword requestedIndex, SPhysicalMemoryDeviceInfo* outInfo, dword* outDeviceCount)
{
	if (!table || tableSize < sizeof(SSMBIOSHeader))
		return false;

	const byte* ptr = table;
	const byte* end = table + tableSize;

	dword deviceCount = 0;
	bool found = false;

	while (ptr + sizeof(SSMBIOSHeader) <= end)
	{
		const SSMBIOSHeader* header = reinterpret_cast<const SSMBIOSHeader*>(ptr);

		if (header->length < sizeof(SSMBIOSHeader) || ptr + header->length > end)
			break;

		if (header->type == 17)
		{
			const qword capacity = DecodeSMBIOSMemorySize(ptr, header->length);

			// Нулевой размер означает пустой слот.
			if (capacity != 0)
			{
				if (outInfo && deviceCount == requestedIndex)
				{
					ResetPhysicalMemoryDeviceInfo(*outInfo);

					word totalWidth = 0;
					word dataWidth = 0;

					if (header->length > 0x09)
						totalWidth = ReadU16(ptr + 0x08);

					if (header->length > 0x0B)
						dataWidth = ReadU16(ptr + 0x0A);

					outInfo->capacity = capacity;
					outInfo->totalWidth = totalWidth == 0xFFFF ? 0 : totalWidth;
					outInfo->dataWidth = dataWidth == 0xFFFF ? 0 : dataWidth;

					outInfo->hasECC = totalWidth != 0 && totalWidth != 0xFFFF &&
						dataWidth != 0 && dataWidth != 0xFFFF &&
						totalWidth > dataWidth;

					if (header->length > 0x0E)
						outInfo->formFactor = static_cast<EMemoryFormFactor>(ptr[0x0E]);

					if (header->length > 0x12)
						outInfo->type = static_cast<EMemoryType>(ptr[0x12]);

					outInfo->speedMHz = DecodeSMBIOSMemorySpeed(ptr, header->length, 0x15, 0x54);
					outInfo->configuredSpeedMHz = DecodeSMBIOSMemorySpeed(ptr, header->length, 0x20, 0x58);

					if (header->length > 0x1B)
						outInfo->rank = ptr[0x1B] & 0x0F;

					if (header->length > 0x10)
						Stdlib::StrCopyN(outInfo->deviceLocator, GetSMBIOSString(header, ptr[0x10], end), sizeof(outInfo->deviceLocator));

					if (header->length > 0x11)
						Stdlib::StrCopyN(outInfo->bankLocator, GetSMBIOSString(header, ptr[0x11], end), sizeof(outInfo->bankLocator));

					if (header->length > 0x17)
						Stdlib::StrCopyN(outInfo->manufacturer, GetSMBIOSString(header, ptr[0x17], end), sizeof(outInfo->manufacturer));

					if (header->length > 0x18)
						Stdlib::StrCopyN(outInfo->serialNumber, GetSMBIOSString(header, ptr[0x18], end), sizeof(outInfo->serialNumber));

					if (header->length > 0x1A)
						Stdlib::StrCopyN(outInfo->partNumber, GetSMBIOSString(header, ptr[0x1A], end), sizeof(outInfo->partNumber));

					found = true;
				}

				++deviceCount;
			}
		}

		if (header->type == 127)
			break;

		ptr = GetNextSMBIOSStructure(header, end);

		if (!ptr)
			break;
	}

	if (outDeviceCount)
		*outDeviceCount = deviceCount;

	return found;
}


dword CountSMBIOSMemoryDevices(const byte* table, size_t tableSize)
{
	dword count = 0;
	ParseSMBIOSMemoryDevice(table, tableSize, 0, nullptr, &count);
	return count;
}

