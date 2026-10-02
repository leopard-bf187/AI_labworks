/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 11.07.2026
*/


#include "android_classes.h"


constexpr Tick NANOSECONDS_PER_SECOND = 1000000000ull;
constexpr Tick NANOSECONDS_PER_MILLISECOND = 1000000ull;


inline Tick TimespecToNanoseconds(const timespec& value)
{
    return static_cast<Tick>(value.tv_sec) * NANOSECONDS_PER_SECOND + static_cast<Tick>(value.tv_nsec);
}


inline Tick ReadClockNanoseconds(clockid_t clockId)
{
    timespec value{};

    if (clock_gettime(clockId, &value) != 0)
        return 0;

    return TimespecToNanoseconds(value);
}


inline Tick GetLocalUnixNanoseconds()
{
    timespec utcTime{};

    if (clock_gettime(CLOCK_REALTIME, &utcTime) != 0)
        return 0;

    time_t utcSeconds = utcTime.tv_sec;
    tm localTime{};

    if (!localtime_r(&utcSeconds, &localTime))
        return TimespecToNanoseconds(utcTime);

    const time_t localSeconds = timegm(&localTime);

    if (localSeconds < 0)
        return TimespecToNanoseconds(utcTime);

    return static_cast<Tick>(localSeconds) * NANOSECONDS_PER_SECOND +
        static_cast<Tick>(utcTime.tv_nsec);
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
    return ReadClockNanoseconds(CLOCK_REALTIME);
}


Tick CPlatformTimeManager::GetMonotonicTime()
{
    return ReadClockNanoseconds(CLOCK_MONOTONIC);
}


Tick CPlatformTimeManager::GetTicks()
{
    return ReadClockNanoseconds(CLOCK_MONOTONIC) / NANOSECONDS_PER_MILLISECOND;
}


Tick CPlatformTimeManager::GetPerformanceCounter()
{
    return ReadClockNanoseconds(CLOCK_MONOTONIC);
}


Tick CPlatformTimeManager::GetPerformanceFrequency()
{
    return NANOSECONDS_PER_SECOND;
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

