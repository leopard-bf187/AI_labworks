/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4 & leopard-bf187
*
*  Description: stdlib default allocator implementation
*
*  Date: 03.06.2026
*/

#include <cstddef>
#include <cstdlib>

#define STDLIB_API_EXPORT
#include "stdlib_dll.h"

#ifdef _WIN32
#include <malloc.h>
#endif

namespace krystallic
{
    namespace Stdlib
    {
        struct CStdAllocator : Common::IAllocator
        {
            void*  Alloc(uint size, uint alignment) override;
            void* Realloc(void* ptr, uint size, uint alignment) override;
            void   Free(void* ptr) override;
            uint32 IncRef() override;
            uint32 Delete() override;
            uint32 QueryIFace(const SGuid& guid, void** iface) override;
        };

        void* CStdAllocator::Alloc(uint size, uint alignment)
        {
            if (size == 0)
            {
                return nullptr;
            }

            // TODO: Temporarily disabled alignment to test cross-platform consistency
            // Use simple malloc instead of aligned allocation
            (void) alignment;
            return std::malloc(size);
        }

        void* CStdAllocator::Realloc(void* ptr, uint size, uint alignment)
        {
            if (size == 0)
            {
                return nullptr;
            }

            (void)alignment;
            return std::realloc(ptr, size);
        }

        void CStdAllocator::Free(void* ptr)
        {
            std::free(ptr);
        }

        uint32 CStdAllocator::IncRef()
        {
            return 1;
        }

        uint32 CStdAllocator::Delete()
        {
            return 1;
        }

        uint32 CStdAllocator::QueryIFace(const SGuid& guid, void** iface)
        {
            if (!iface)
            {
                return 0;
            }

            if (guid == IBase::GUID())
            {
                *iface = static_cast<IBase*>(this);
                return IncRef();
            }

            if (guid == Common::IAllocator::GUID())
            {
                *iface = static_cast<Common::IAllocator*>(this);
                return IncRef();
            }

            *iface = nullptr;
            return 0;
        }


        static CStdAllocator s_stdlibDefaultAllocator;
        STDLIB_API Common::IAllocator* g_stdlibDefaultAllocator = &s_stdlibDefaultAllocator;
    } // namespace Stdlib
} // namespace krystallic
