#if defined(_MSC_VER)
    #include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
    #include <cpuid.h>
#endif


void CpuId(dword function, dword subFunction, dword data[4])
{
#if defined(_MSC_VER)
	__cpuidex(reinterpret_cast<int*>(data), static_cast<int>(function), static_cast<int>(subFunction));
#else
	__cpuid_count(function, subFunction, data[0], data[1], data[2], data[3]);
#endif
}

dword GetMaxBasicCpuId()
{
	dword data[4]{};
	CpuId(0, 0, data);
	return data[0];
}

dword GetMaxExtendedCpuId()
{
	dword data[4]{};
	CpuId(0x80000000u, 0, data);
	return data[0];
}

qword ReadXcr0()
{
#if defined(_MSC_VER)
	return static_cast<qword>(_xgetbv(0));
#else
	dword eax = 0;
	dword edx = 0;
	__asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
	return (static_cast<qword>(edx) << 32) | eax;
#endif
}

void QueryX86Vendor(char vendor[20])
{
	dword data[4]{};
	CpuId(0, 0, data);
	memcpy(vendor + 0, &data[1], 4);
	memcpy(vendor + 4, &data[3], 4);
	memcpy(vendor + 8, &data[2], 4);
	vendor[12] = '\0';
}

void QueryX86Brand(char brand[64])
{
	brand[0] = '\0';

	if (GetMaxExtendedCpuId() < 0x80000004u)
		return;

	dword data[4]{};

	for (dword index = 0; index < 3; ++index)
	{
		CpuId(0x80000002u + index, 0, data);
		memcpy(brand + index * 16, data, 16);
	}

	brand[48] = '\0';
	char* begin = brand;

	while (*begin == ' ')
		++begin;

	if (begin != brand)
		memmove(brand, begin, Stdlib::StrLen(begin) + 1);
}

dword QueryX86FeatureMask()
{
	dword mask = CPU_FEATURE_NONE;
	const dword maxBasicLeaf = GetMaxBasicCpuId();
	const dword maxExtendedLeaf = GetMaxExtendedCpuId();

	if (maxBasicLeaf < 1)
		return mask;

	dword data[4]{};
	CpuId(1, 0, data);

	const dword ecx1 = data[2];
	const dword edx1 = data[3];
	const bool hasSse41 = (ecx1 & (1u << 19)) != 0;
	const bool hasSse42 = (ecx1 & (1u << 20)) != 0;
	const bool hasXsave = (ecx1 & (1u << 26)) != 0;
	const bool hasOsXsave = (ecx1 & (1u << 27)) != 0;
	const bool hasHardwareAvx = (ecx1 & (1u << 28)) != 0;

	if (edx1 & (1u << 23)) mask |= CPU_FEATURE_MMX;
	if (edx1 & (1u << 25)) mask |= CPU_FEATURE_SSE;
	if (edx1 & (1u << 26)) mask |= CPU_FEATURE_SSE2;
	if (ecx1 & (1u << 0)) mask |= CPU_FEATURE_SSE3;
	if (ecx1 & (1u << 9)) mask |= CPU_FEATURE_SSSE3;
	if (hasSse41) mask |= CPU_FEATURE_SSE41;

	if (hasSse42)
	{
		mask |= CPU_FEATURE_SSE42;
		mask |= CPU_FEATURE_CRC32;
	}

	//if (hasSse41 && hasSse42) mask |= CPU_FEATURE_SSE4;
	if (ecx1 & (1u << 23)) mask |= CPU_FEATURE_POPCNT;
	if (ecx1 & (1u << 25)) mask |= CPU_FEATURE_AES;

	bool osSupportsAvx = false;
	bool osSupportsAvx512 = false;

	if (hasXsave && hasOsXsave)
	{
		const qword xcr0 = ReadXcr0();
		osSupportsAvx = (xcr0 & 0x6u) == 0x6u;
		osSupportsAvx512 = (xcr0 & 0xE6u) == 0xE6u;
	}

	if (hasHardwareAvx && osSupportsAvx) mask |= CPU_FEATURE_AVX;

	if ((ecx1 & (1u << 12)) && osSupportsAvx)
	{
		mask |= CPU_FEATURE_FMA;
		mask |= CPU_FEATURE_FMA3;
	}

	if ((ecx1 & (1u << 29)) && osSupportsAvx) mask |= CPU_FEATURE_F16C;

	if (maxBasicLeaf >= 7)
	{
		CpuId(7, 0, data);
		const dword ebx7 = data[1];

		if ((ebx7 & (1u << 5)) && osSupportsAvx) mask |= CPU_FEATURE_AVX2;
		if ((ebx7 & (1u << 16)) && osSupportsAvx512) mask |= CPU_FEATURE_AVX512F;
		if (ebx7 & (1u << 3)) mask |= CPU_FEATURE_BMI1;
		if (ebx7 & (1u << 8)) mask |= CPU_FEATURE_BMI2;
		if (ebx7 & (1u << 29)) mask |= CPU_FEATURE_SHA;
	}

	if (maxExtendedLeaf >= 0x80000001u)
	{
		CpuId(0x80000001u, 0, data);

		if (data[2] & (1u << 5))
			mask |= CPU_FEATURE_LZCNT;
	}

	return mask;
}


bool QuerySystemCPUInfo(SSystemCPUInfo* outInfo)
{
    if (!outInfo)
        return false;

    memset(outInfo, 0, sizeof(SSystemCPUInfo));
    QueryX86Vendor(outInfo->vendor);
    QueryX86Brand(outInfo->brand);

    if (outInfo->brand[0] == '\0')
        Stdlib::StrCopyN(outInfo->brand, "Unknown x86 CPU", sizeof(outInfo->brand));

    QuerySystemCoreCount(&outInfo->numPhysCores, &outInfo->numLogicCores);
    outInfo->cpuFeatures.mask = QueryX86FeatureMask();
    return true;
}
