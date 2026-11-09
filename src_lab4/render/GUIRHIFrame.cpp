/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Description: RHI swap-chain and framebuffer lifetime for GUI.
*/

#include "GUIRHI.h"
#include <cstdio>

#if defined(KRYSTALLIC_OS_WINNT)
#include <Windows.h>
#elif defined(KRYSTALLIC_OS_LINUX)
#include <dlfcn.h>
#endif

using namespace krystallic::Platform;

bool CGUIRHIFrame::CreateFrameBuffer()
{
    if (m_swapChain->GetRenderTargetView(m_backBuffer.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return false;

    SRHIFrameBufferDesc desc{};
    desc.m_usageMask = RHI_FRAME_BUFFER_USAGE_RTV;
    desc.m_rtvCapacity = 1;

    if (m_device->CreateFrameBuffer(&desc, m_frameBuffer.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return false;

    if (m_frameBuffer->SetRenderTarget(0, m_backBuffer.Get()) != RHI_ERR_OK)
        return false;

    return m_frameBuffer->CommitFrameBufferState() == RHI_ERR_OK;
}


CGUIRHIFrame::~CGUIRHIFrame()
{
    Shutdown();
}


bool CGUIRHIFrame::Initialize(IPlatformManager* manager, IPlatformWindow* window, ERHIBackend backend, uint width, uint height)
{
    if (!manager || !window || !width || !height)
        return false;

    if (backend != RHI_BACKEND_OPENGL)
    {
        std::fprintf(stderr, "AI_Lab4 RHI: only the implemented GL4 backend is currently supported\n");
        return false;
    }

#if defined(KRYSTALLIC_OS_WINNT)
    m_module = LoadLibraryA("render_gl4.dll");
    if (!m_module)
        return false;
    CreateRHIFactoryFn createFactory = reinterpret_cast<CreateRHIFactoryFn>(GetProcAddress(static_cast<HMODULE>(m_module), "CreateRHIFactory"));
#elif defined(KRYSTALLIC_OS_LINUX)
    m_module = dlopen("./librender_gl4.so", RTLD_NOW | RTLD_LOCAL);
    if (!m_module)
        return false;
    CreateRHIFactoryFn createFactory = reinterpret_cast<CreateRHIFactoryFn>(dlsym(m_module, "CreateRHIFactory"));
#else
    return false;
#endif

    if (!createFactory || createFactory(g_stdlibDefaultAllocator, m_factory.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return false;

    SNativeHandle platformHandle{};
    if (manager->GetNativeHandle(&platformHandle) != PLATFORM_ERR_OK)
        return false;

    SRHIDeviceInitialDesc deviceDesc{};
    deviceDesc.platformNativeHandle = &platformHandle;

    if (m_factory->CreateDevice(nullptr, &deviceDesc, m_device.ReleaseAndGetAddressOf(), m_context.ReleaseAndGetAddressOf(), m_compiler.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return false;

    SRHISwapChainDesc swapDesc{};
    swapDesc.platformWindow = window;
    swapDesc.width = width;
    swapDesc.height = height;
    swapDesc.bufferCount = 2;
    swapDesc.colorFormat = RHI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.sampleDesc.count = 1;
    swapDesc.swapEffect = RHI_SWAP_EFFECT_DISCARD;
    swapDesc.alphaMode = RHI_SWAP_ALPHA_MODE_OPAQUE;
    swapDesc.vsyncMode = RHI_VSYNC_FULL;

    const ERRCODE swapResult = m_factory->CreateSwapChain(m_device.Get(), &swapDesc, m_swapChain.ReleaseAndGetAddressOf());
    if (swapResult != RHI_ERR_OK)
    {
        std::fprintf(stderr, "CreateSwapChain failed: %d\n", static_cast<int>(swapResult));

        RefCounted<Common::IError> error;
        if (m_factory->GetLastError(error.ReleaseAndGetAddressOf()) == RHI_ERR_OK && error.Get())
            std::fprintf(stderr, "  %s\n", error->GetErrorMessage() ? error->GetErrorMessage() : "");

        return false;
    }

    m_width = width;
    m_height = height;
    return CreateFrameBuffer();
}


bool CGUIRHIFrame::Resize(uint width, uint height)
{
    if (!width || !height)
        return true;

    if (width == m_width && height == m_height)
        return true;

    m_context->OM_SetFrameBuffer(nullptr);
    m_frameBuffer.Reset();
    m_backBuffer.Reset();

    if (m_swapChain->ResizeBuffers(0, width, height, RHI_FORMAT_UNKNOWN, 0) != RHI_ERR_OK)
        return false;

    m_width = width;
    m_height = height;
    return CreateFrameBuffer();
}


bool CGUIRHIFrame::Present(IGUIContext* gui)
{
    if (!gui || !m_context.Get() || !m_frameBuffer.Get())
        return false;

    m_context->OM_SetFrameBuffer(m_frameBuffer.Get());

    SRHIViewport vp{};
    vp.Width = static_cast<float>(m_width);
    vp.Height = static_cast<float>(m_height);
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;
    m_context->RS_SetViewports(1, &vp);

    const float clear[4] = { 0.25f, 0.25f, 0.5f, 1.0f };
    m_context->ClearRenderTarget(0, clear);

    if (gui->Render() != GUI_ERR_OK)
        return false;

    return m_swapChain->Present(RHI_VSYNC_FULL, 0) == RHI_ERR_OK;
}


void CGUIRHIFrame::Shutdown()
{
    if (m_context.Get())
    {
        m_context->OM_SetFrameBuffer(nullptr);
        m_context->ClearState();
    }

    m_frameBuffer.Reset();
    m_backBuffer.Reset();
    m_swapChain.Reset();
    m_compiler.Reset();
    m_context.Reset();
    m_device.Reset();
    m_factory.Reset();

#if defined(KRYSTALLIC_OS_WINNT)
    if (m_module)
        FreeLibrary(static_cast<HMODULE>(m_module));
#elif defined(KRYSTALLIC_OS_LINUX)
    if (m_module)
        dlclose(m_module);
#endif
    m_module = nullptr;
}
