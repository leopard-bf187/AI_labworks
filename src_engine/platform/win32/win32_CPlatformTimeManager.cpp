/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 19.07.2026
*/


#include "win32_classes.h"


constexpr Tick WINDOWS_TO_UNIX_EPOCH_100NS = 116444736000000000ull;
constexpr Tick NANOSECONDS_PER_100NS = 100ull;
constexpr Tick NANOSECONDS_PER_MILLISECOND = 1000000ull;
constexpr Tick NANOSECONDS_PER_SECOND = 1000000000ull;


Tick FileTimeTo100Nanoseconds(const FILETIME& fileTime)
{
    ULARGE_INTEGER value{};
    value.LowPart = fileTime.dwLowDateTime;
    value.HighPart = fileTime.dwHighDateTime;
    return static_cast<Tick>(value.QuadPart);
}


inline Tick FileTimeToUnixNanoseconds(const FILETIME& fileTime)
{
    const Tick windowsTicks = FileTimeTo100Nanoseconds(fileTime);

    if (windowsTicks < WINDOWS_TO_UNIX_EPOCH_100NS)
        return 0;

    return (windowsTicks - WINDOWS_TO_UNIX_EPOCH_100NS) * NANOSECONDS_PER_100NS;
}


inline Tick GetUTCUnixNanoseconds()
{
    FILETIME fileTime{};
    GetSystemTimeAsFileTime(&fileTime);
    return FileTimeToUnixNanoseconds(fileTime);
}


inline Tick GetLocalUnixNanoseconds()
{
    FILETIME utcFileTime{};
    FILETIME localFileTime{};

    GetSystemTimeAsFileTime(&utcFileTime);

    if (!FileTimeToLocalFileTime(&utcFileTime, &localFileTime))
        return FileTimeToUnixNanoseconds(utcFileTime);

    return FileTimeToUnixNanoseconds(localFileTime);
}


inline Tick QPCToNanoseconds(Tick counter, Tick frequency)
{
    if (frequency == 0)
        return 0;

    const Tick seconds = counter / frequency;
    const Tick remainder = counter % frequency;

    return seconds * NANOSECONDS_PER_SECOND +
        remainder * NANOSECONDS_PER_SECOND / frequency;
}



CPlatformTimeManager::CPlatformTimeManager(IAllocator* allocator) : Inherit(allocator) 
{
}


CPlatformTimeManager::~CPlatformTimeManager()
{

}


uint32 CPlatformTimeManager::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformTimeManager::_Destroy(this);
        return 0;
    }

    return ref;
}


uint32 CPlatformTimeManager::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformTimeManager::GUID())
    {
        *IFace = static_cast<IPlatformTimeManager*>(this);
        return this->IncRef();
    }

    return 0;
}


Tick CPlatformTimeManager::GetSystemTime()
{
    return GetLocalUnixNanoseconds();
}


Tick CPlatformTimeManager::GetUTCTime()
{
    return GetUTCUnixNanoseconds();
}


Tick CPlatformTimeManager::GetMonotonicTime()
{
    const Tick frequency = GetPerformanceFrequency();

    if (frequency == 0)
        return 0;

    return QPCToNanoseconds(GetPerformanceCounter(), frequency);
}


Tick CPlatformTimeManager::GetTicks()
{
    return GetMonotonicTime() / NANOSECONDS_PER_MILLISECOND;
}


Tick CPlatformTimeManager::GetPerformanceCounter()
{
    LARGE_INTEGER counter{};

    if (!QueryPerformanceCounter(&counter))
        return 0;

    return static_cast<Tick>(counter.QuadPart);
}


Tick CPlatformTimeManager::GetPerformanceFrequency()
{
    LARGE_INTEGER frequency{};

    if (!QueryPerformanceFrequency(&frequency))
        return 0;

    return static_cast<Tick>(frequency.QuadPart);
}


ERRCODE CPlatformTimeManager::CreateTimer(ISystemTimer** outTimer)
{
    if (!outTimer)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    RefCounted<CSystemTimer> timer;
    timer.Attach(CSystemTimer::_Create(m_allocator.Get()));

    if (!timer.Get())
        return PLATFORM_ERR_OUT_OF_MEMORY;

    if (*outTimer) 
        (*outTimer)->Delete();

    *outTimer = timer.Cast<ISystemTimer*>();
    timer->IncRef();

    return PLATFORM_ERR_OK;
}



