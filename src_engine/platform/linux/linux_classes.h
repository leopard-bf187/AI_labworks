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
#pragma once


#include "platform.h"
#include "inherit.h"

#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */ 
#include <unistd.h>


#include <wayland-client.h>
#include <wayland-server.h>
#include <wayland-egl.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

#include "xdg-shell-client-protocol.h"
#include "xdg-decoration-unstable-v1-client-protocol.h"

#include "wayland_common_types.h"

using namespace krystallic;
using namespace krystallic::Common;
using namespace krystallic::Platform;


#include "../platform_helpers.h"


struct CPlatformManager final : Inherit<CPlatformManager, IPlatformManager>
{
    RefCounted<IPlatformMemoryManager>  m_memoryManager;
    RefCounted<IPlatformThreadManager>  m_threadManager;
    RefCounted<IPlatformProcessManager> m_processManager;
    RefCounted<IPlatformTimeManager>    m_timeManager;
    RefCounted<IPlatformInputManager>   m_inputManager;
    RefCounted<IPlatformSystemInfo>     m_systemInfo;
    SNativeHandle m_hInstHandle;
    wl_display* m_display;
    wl_registry* m_registry;
    wl_compositor* m_compositor;
    wl_shm* m_shm;
    zxdg_decoration_manager_v1* m_decorationManager;
    xdg_wm_base* m_xdgWmBase;
    int m_exitCode;
    bool m_ownsDisplay;
    bool m_running;

    CPlatformManager(IAllocator* allocator, const SNativeHandle* sysHandle);
    virtual ~CPlatformManager();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual EPlatformType GetPlatformType() const
    {
        return PLATFORM_LINUX;
    };

    virtual ERRCODE QueryMemoryManager(IPlatformMemoryManager** outManager);
    virtual ERRCODE QueryThreadManager(IPlatformThreadManager** outMgr);
    virtual ERRCODE QueryProcessManager(IPlatformProcessManager** outMgr);
    virtual ERRCODE QueryTimeManager(IPlatformTimeManager** outTimer);
    virtual ERRCODE QueryInputManager(IPlatformInputManager** outMgr);
    virtual ERRCODE QuerySystemInfo(IPlatformSystemInfo** outSysInfo);

    virtual ERRCODE CreateNativeWindow(const Stdlib::String& title, const SPlatformWindowDesc* desc, IPlatformWindowCallback* optionalWndCallback, IPlatformWindow* parentWindow, IPlatformWindow** outWindow);

    virtual bool PollEvents();
    virtual bool IsRunning() const;
    virtual void RequestQuit(int exitCode);

    virtual ERRCODE GetNativeHandle(SNativeHandle* appInstanceHandle) const;


    bool InitializeWayland();
    void ShutdownWayland();

    static void RegistryGlobal(void* data, wl_registry* registry, uint32 name, const char* interface, uint32 version);
    static void RegistryGlobalRemove(void* data, wl_registry* registry, uint32 name);
    static void XdgWmBasePing(void* data, xdg_wm_base* wmBase, uint32 serial);
};


//MEMORY
struct CPlatformMemoryManager final : Inherit<CPlatformMemoryManager, IPlatformMemoryManager>
{
    CPlatformMemoryManager(IAllocator* allocator);
    virtual ~CPlatformMemoryManager();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual ERRCODE ReserveVirtualMemory(uint64 size, IVirtualMemory** outMemory);
    virtual ERRCODE CreateSharedMemoryObject(const char* name, uint64 size, ISharedMemory** outShared);
    virtual ERRCODE OpenSharedMemoryObject(const char* name, ISharedMemory** outShared);
    virtual ERRCODE DestroySharedMemoryObject(const char* name);

    virtual ERRCODE GetMemoryStatus(SSystemMemoryStatus* memStatus);
};


struct CVirtualMemory final : Inherit<CVirtualMemory, IVirtualMemory>
{
    void*  m_baseAddress;
    uint64 m_size;

    CVirtualMemory(IAllocator* allocator);
    virtual ~CVirtualMemory();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual void* GetBaseAddress();
    virtual const void* GetBaseAddress() const;
    virtual uint64       GetReservedBytes() const;
    virtual ERRCODE Commit(void* baseAddress, uint64 size);
    virtual ERRCODE Decommit(void* baseAddress, uint64 size);
    virtual ERRCODE LockPages(void* baseAddress, uint64 sizeInBytes);
    virtual ERRCODE UnlockPages(void* baseAddress, uint64 sizeInBytes);
    virtual uint64 GetPageSize();
    virtual uint64 GetAllocationGranularity();
    virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const;

    ERRCODE Initialize(uint64 size);

    inline bool IsRangeValid(void* baseAddress, uint64 size) const
    {
        qword start = reinterpret_cast<qword>(baseAddress);
        qword end = start + size;

        qword reserveStart = reinterpret_cast<qword>(m_baseAddress);
        qword reserveEnd = reserveStart + m_size;

        return !(end < start || start < reserveStart || end > reserveEnd);
    }
};


struct CSharedMemory final : Inherit<CSharedMemory, ISharedMemory>
{
    int    m_shmFd;
    void*  m_mappedPtr;
    uint64 m_size;

    CSharedMemory(IAllocator* allocator);
    virtual ~CSharedMemory();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual void* Map();
    virtual void  Unmap();
    virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const;
};


// THREADS
struct CPlatformThreadManager final : Inherit<CPlatformThreadManager, IPlatformThreadManager>
{
    CPlatformThreadManager(IAllocator* allocator) : Inherit(allocator) {}
    virtual ~CPlatformThreadManager() {}

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);
};


// PROCESSES
struct CPlatformProcessManager final : Inherit<CPlatformProcessManager, IPlatformProcessManager>
{
    CPlatformProcessManager(IAllocator* allocator) : Inherit(allocator) {}
    virtual ~CPlatformProcessManager() {}

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);
};


// INPUT
struct CPlatformInputManager final : Inherit<CPlatformInputManager, IPlatformInputManager>
{
    CPlatformInputManager(IAllocator* allocator) : Inherit(allocator) {}
    virtual ~CPlatformInputManager() {}

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);
};


// TIME
struct CPlatformTimeManager final : Inherit<CPlatformTimeManager, IPlatformTimeManager>
{
    CPlatformTimeManager(IAllocator* allocator);
    virtual ~CPlatformTimeManager();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual Tick GetSystemTime();
    virtual Tick GetUTCTime();

    virtual Tick GetMonotonicTime();

    virtual Tick GetTicks();

    virtual Tick GetPerformanceCounter();
    virtual Tick GetPerformanceFrequency();

    virtual ERRCODE CreateTimer(ISystemTimer** outTimer);
};


struct CSystemTimer : Inherit<CSystemTimer, ISystemTimer>
{
    Tick m_frequency;
    Tick m_startCounter;
    Tick m_elapsedCounter;
    bool m_running;

    CSystemTimer(IAllocator* allocator);
    virtual ~CSystemTimer();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual void Start();
    virtual void Stop();
    virtual void Reset();

    virtual double GetSeconds();
    virtual double GetMilliseconds();
    virtual double GetMicroseconds();

    Tick GetElapsedCounter() const;
};


// SYSINFO
struct CPlatformSystemInfo final : Inherit<CPlatformSystemInfo, IPlatformSystemInfo>
{
    SSystemOSInfo m_os;
    SSystemCPUInfo m_cpu;
    SSystemMemoryInfo m_ram;

    CPlatformSystemInfo(IAllocator* allocator);
    virtual ~CPlatformSystemInfo();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual const char* GetOSProductName() const;
    virtual const char* GetOSEdition() const;
    virtual const char* GetOSVersion() const;

    virtual const char* GetOSUserName() const;
    virtual const char* GetOSDesktopName() const;
    virtual const char* GetOSInstallDate() const;

    virtual const char* GetCPUVendor() const;
    virtual const char* GetCPUBrand() const;

    virtual uint32 GetCPUPhysicalCoreCount() const;
    virtual uint32 GetCPULogicalCoreCount() const;

    virtual uint32 GetCPUSIMDFlags() const;

    virtual uint64 GetPhysicalMemorySize() const;
    virtual uint64 GetAvailableMemory() const;

    virtual ERRCODE GetFullOSInfo(SSystemOSInfo* outOSInfo) const;
    virtual ERRCODE GetFullCPUInfo(SSystemCPUInfo* outCpuInfo) const;
    virtual ERRCODE GetFullRAMInfo(SSystemMemoryInfo* outCpuInfo) const;

    virtual dword GetPhysicalMemoryDeviceCount() const;
    virtual ERRCODE GetPhysicalMemoryDeviceInfo(dword index, SPhysicalMemoryDeviceInfo* outInfo) const;

    bool InitializeCPU();
    bool InitializeOS();
    bool InitializeRAM();
};


// WINDOW
struct CWindowCallback : IPlatformWindowCallback
{
    virtual void OnCreate(const SNativeHandle* window);

    virtual void OnCreateEx(const SNativeHandle* window);

    virtual void OnHittest(const SNativeHandle* window);

    virtual void OnDestroy(const SNativeHandle* window);

    virtual void OnShow(const SNativeHandle* window);
    virtual void OnHide(const SNativeHandle* window);

    virtual void OnActivate(const SNativeHandle* window, bool active);
    virtual void OnSetFocus(const SNativeHandle* window);
    virtual void OnKillFocus(const SNativeHandle* window);

    virtual void OnSize(const SNativeHandle* window, uint32 width, uint32 height);
    virtual void OnSizing(const SNativeHandle* window, uint32 width, uint32 height);
    virtual void OnMinimize(const SNativeHandle* window);
    virtual void OnMaximize(const SNativeHandle* window);

    virtual void OnPaint(const SNativeHandle* window);

    virtual void OnInput(const SNativeHandle* window);
    virtual void OnChar(const SNativeHandle* window);
};


extern CWindowCallback* g_DefaultWindowCallback;


struct CPlatformWindow : Inherit<CPlatformWindow, IPlatformWindow>
{
    RefCounted<CPlatformManager> m_manager;
    wl_surface* m_surface;
    xdg_surface* m_xdgSurface;
    xdg_toplevel* m_xdgToplevel;
    zxdg_toplevel_decoration_v1* m_toplevelDecoration;

    wl_buffer* m_placeholderBuffer;
    void* m_placeholderMemory;
    uint64 m_placeholderMemorySize;
    int m_placeholderFd;
    uint32 m_placeholderWidth;
    uint32 m_placeholderHeight;

    SWaylandWindowNativeHandle m_waylandHandle;

    SPlatformWindowDesc m_desc;
    SNativeHandle m_window;
    IPlatformWindowCallback* m_callback;

    bool m_visible;
    bool m_active;
    bool m_maximized;
    bool m_fullscreen;
    bool m_created;
    bool m_configured;

    CPlatformWindow(IAllocator* allocator);
    virtual ~CPlatformWindow();

    virtual uint32 Delete();
    virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

    virtual ERRCODE Show();
    virtual ERRCODE Hide();

    virtual ERRCODE Minimize();
    virtual ERRCODE Maximize();
    virtual ERRCODE Restore();

    virtual ERRCODE SetTitle(const Stdlib::String& title);
    virtual ERRCODE SetSize(uint32 width, uint32 height);
    virtual ERRCODE SetPosition(int32 x, int32 y);

    virtual void SetCallback(IPlatformWindowCallback* callback);
    virtual IPlatformWindowCallback* GetCallback();

    virtual ERRCODE GetDesc(SPlatformWindowDesc* outDesc) const;

    virtual uint32 GetWidth() const;
    virtual uint32 GetHeight() const;

    /*
    virtual ERRCODE GetClientRect(Rect* outRect);
    virtual ERRCODE GetWindowRect(Rect* outRect);
    */

    virtual bool IsVisible() const;
    virtual bool IsActive() const;

    virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const;


    ERRCODE Initialize(CPlatformManager* manager, const Stdlib::String& title, const SPlatformWindowDesc* desc, IPlatformWindowCallback* callback, CPlatformWindow* parentWindow);
    void DestroyNativeWindow(bool nitifyCallback);
    ERRCODE CreatePlaceholderBuffer(uint32 width, uint32 height);
    void DestroyPlaceholderBuffer();
    ERRCODE AttachPlaceholderBuffer();

    static void XdgSurfaceConfigure(void* data, xdg_surface* surface, uint32 serial);
    static void XdgToplevelConfigure(void* data, xdg_toplevel* toplevel, int32 width, int32 height, wl_array* states);
    static void XdgToplevelClose(void* data, xdg_toplevel* toplevel);
    static void XdgDecorationConfigure(void* data, zxdg_toplevel_decoration_v1* decoration, uint32 mode);
};


