#pragma once

#include "pch.h"
#include "platform.h"
#include "gui.h"
#include "rhi.h"
#include "stdlib_dll.h"

using namespace krystallic;
using namespace krystallic::GUI;
using namespace krystallic::SIMDMath;
using namespace krystallic::Render;
using namespace krystallic::RenderHardwareInterface;

struct SGUIRHITexture
{
    RefCounted<IRHIResource> resource;
    RefCounted<IRHIShaderResourceView> view;
};

class CGUIRHIStrategy final : public IGUIRenderStrategy
{
private:
    IRHIDevice* m_device = nullptr;
    IRHIDeviceContext* m_context = nullptr;
    IRHIShaderCompiler* m_compiler = nullptr;
    ERHIBackend m_backend = RHI_BACKEND_OPENGL;

    RefCounted<Common::IBuffer> m_vertexByteCode;
    RefCounted<IRHIShader> m_vertexShader;
    RefCounted<IRHIShader> m_pixelShader;
    RefCounted<IRHIShaderProgram> m_program;
    RefCounted<IRHIResource> m_projectionBuffer;
    RefCounted<IRHIConstantBufferView> m_projectionView;
    RefCounted<IRHISampler> m_pointSampler;
    RefCounted<IRHIRasterizerState> m_rasterizer;
    RefCounted<IRHIBlendState> m_blend;
    RefCounted<IRHIDepthStencilState> m_depth;
    GUI_VIEWPORT m_viewport{};

    bool CompileShader(ERHIShaderStage stage, const char* source, const char* entryPoint, RefCounted<IRHIShader>& shader, Common::IBuffer** byteCode = nullptr);

public:
    void Initialize(IRHIDevice* device, IRHIDeviceContext* context, IRHIShaderCompiler* compiler, ERHIBackend backend);

    ERRCODE OnResourceCreate(GUI_RENDERABLE_RESOURCE_TYPE type, void* creationStructurePtr, void** outHandle) override;
    ERRCODE OnResourceDestroy(GUI_RENDERABLE_RESOURCE_TYPE type, void* handle) override;
    ERRCODE OnResourceMap(GUI_RENDERABLE_RESOURCE_TYPE type, void* resourceHandle, uint subresource, GUI_MAPPED_RESOURCE* outMappedResource) override;
    ERRCODE OnResourceUnmap(GUI_RENDERABLE_RESOURCE_TYPE type, void* resourceHandle, uint subresource) override;
    ERRCODE OnResourceUpdateRegion(GUI_RENDERABLE_RESOURCE_TYPE type, void* resourceHandle, const GUI_RECT& dstRectPixels, const void* srcData, uint srcRowPitch) override;
    ERRCODE OnRenderCreate() override;
    ERRCODE OnRenderDestroy() override;
    ERRCODE OnRenderReset() override;
    ERRCODE OnRenderUpdate() override;
    ERRCODE OnRenderResize(GUI_VIEWPORT vp) override;
    ERRCODE OnRenderBegin(InLayoutHandle inputLayout, VBuffHandle vbuff, IBuffHandle ibuff, const GUI_MATERIAL& material) override;
    ERRCODE OnRenderEnd() override;
    ERRCODE PreRender(GUI_DRAW_COMMAND* command) override;
    ERRCODE OnRender(GUI_DRAW_COMMAND* command) override;
    ERRCODE PostRender() override;
};

class CGUIRHIFrame
{
private:
    void* m_module = nullptr;
    RefCounted<IRHIFactory> m_factory;
    RefCounted<IRHIDevice> m_device;
    RefCounted<IRHIDeviceContext> m_context;
    RefCounted<IRHIShaderCompiler> m_compiler;
    RefCounted<IRHISwapChain> m_swapChain;
    RefCounted<IRHIRenderTargetView> m_backBuffer;
    RefCounted<IRHIFrameBuffer> m_frameBuffer;
    uint m_width = 0;
    uint m_height = 0;

    bool CreateFrameBuffer();

public:
    ~CGUIRHIFrame();
    bool Initialize(Platform::IPlatformManager* manager, Platform::IPlatformWindow* window, ERHIBackend backend, uint width, uint height);
    bool Resize(uint width, uint height);
    bool Present(IGUIContext* gui);
    void Shutdown();

    IRHIDevice* Device() { return m_device.Get(); }
    IRHIDeviceContext* Context() { return m_context.Get(); }
    IRHIShaderCompiler* Compiler() { return m_compiler.Get(); }
};
