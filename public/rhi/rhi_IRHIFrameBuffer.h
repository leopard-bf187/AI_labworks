#pragma once

#include "rhi_dll_types.h"

namespace krystallic
{
    namespace RenderHardwareInterface
    {
        struct IRHIFrameBuffer : IBase
        {
        protected:
            dword m_bindMask = 0;
            bool m_isCommitted = false;

        public:
            virtual ~IRHIFrameBuffer() = default;
            virtual ERRCODE CommitFrameBufferState() = 0;
            virtual ERRCODE GetDesc(SRHIFrameBufferDesc* outDesc) = 0;
            virtual ERRCODE SetRenderTarget(uint rtvSlot, IRHIRenderTargetView* rtv) = 0;
            virtual ERRCODE SetDepthStencil(IRHIDepthStencilView* dsv) = 0;
            virtual ERRCODE SetUnorderedAccess(uint uavSlot, IRHIUnorderedAccessView* uav) = 0;
            virtual ERRCODE GetRenderTarget(uint rtvSlot, IRHIRenderTargetView** rtv) = 0;
            virtual ERRCODE GetDepthStencil(IRHIDepthStencilView** outDSV) = 0;
            virtual ERRCODE GetUnorderedAccess(uint uavSlot, IRHIUnorderedAccessView** outUAV) = 0;
            virtual ERRCODE GetLastError(Common::IError** outError) = 0;

            dword GetBindMask() const { return m_bindMask; }
            bool IsCommitted() const { return m_isCommitted; }
            inline static SGuid GUID() { return {0xdc64d835, 0x3ea2, 0x4135, {0x98, 0x97, 0x7a, 0x18, 0xdc, 0xee, 0x8a, 0x47}}; }
        };
    }
}
