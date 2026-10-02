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


#include "linux_classes.h"


constexpr Tick NANOSECONDS_PER_SECOND = 1000000000ull;

Tick ReadPerformanceCounter()
{
    timespec value{};

    if (clock_gettime(CLOCK_MONOTONIC, &value) != 0)
        return 0;

    return static_cast<Tick>(value.tv_sec) * NANOSECONDS_PER_SECOND +
        static_cast<Tick>(value.tv_nsec);
}


CSystemTimer::CSystemTimer(IAllocator* allocator) : Inherit(allocator)
{
}


CSystemTimer::~CSystemTimer()
{

}


uint32 CSystemTimer::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CSystemTimer::_Destroy(this);
        return 0;
    }

    return ref;
}


uint32 CSystemTimer::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == ISystemTimer::GUID())
    {
        *IFace = static_cast<ISystemTimer*>(this);
        return this->IncRef();
    }

    return 0;
}


void CSystemTimer::Start()
{
    if (m_running)
        return;

    m_startCounter = ReadPerformanceCounter();
    m_running = true;
}


void CSystemTimer::Stop()
{
    if (!m_running)
        return;

    const Tick currentCounter = ReadPerformanceCounter();

    if (currentCounter >= m_startCounter)
        m_elapsedCounter += currentCounter - m_startCounter;

    m_startCounter = 0;
    m_running = false;
}


void CSystemTimer::Reset()
{
    m_elapsedCounter = 0;
    m_startCounter = m_running ? ReadPerformanceCounter() : 0;
}


double CSystemTimer::GetSeconds()
{
    return static_cast<double>(GetElapsedCounter()) / static_cast<double>(m_frequency);
}


double CSystemTimer::GetMilliseconds()
{
    return static_cast<double>(GetElapsedCounter()) / 1000000.0;
}


double CSystemTimer::GetMicroseconds()
{
    return static_cast<double>(GetElapsedCounter()) / 1000.0;
}


Tick CSystemTimer::GetElapsedCounter() const
{
    Tick elapsed = m_elapsedCounter;

    if (!m_running)
        return elapsed;

    const Tick currentCounter = ReadPerformanceCounter();

    if (currentCounter >= m_startCounter)
        elapsed += currentCounter - m_startCounter;

    return elapsed;
}

