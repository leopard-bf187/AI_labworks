/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: IBase ref-counted interface definition
*
*  Date: 19.12.2024, 19.04.2026
*/


#pragma once


#define BASE_VERSION 0x23


#include "pch.h"


namespace krystallic
{
    struct SGuid
    {
        uint32 Data1;
        uint16 Data2;
        uint16 Data3;
        uint8  Data4[8];

        inline bool IsEqualGUID(const SGuid& other) const
        {
            const uint32* guid1 = &this->Data1;
            const uint32* guid2 = &other.Data1;

            return ((guid1[0] == guid2[0]) && (guid1[1] == guid2[1]) && (guid1[2] == guid2[2]) &&
                    (guid1[3] == guid2[3]));
        }
    };

    inline bool operator==(const SGuid& a, const SGuid& b)
    {
        return a.IsEqualGUID(b);
    }

    inline bool operator!=(const SGuid& a, const SGuid& b)
    {
        return !(a == b);
    }


    enum EBaseError : ERRCODE
    {
        ERR_OK,
        ERR_INVALID_ARGUMENT,
        ERR_NOT_SUPPORTED,
        ERR_NOT_IMPLEMENTED,
        ERR_NOT_FOUND,
    };


    struct IBase
    {
    protected:
        uint32 m_refCount = 1;

    protected:
        uint32 DecRef() { return --m_refCount; };
        virtual ~IBase() = default;

    public:
        uint32 QueryBase(IBase** base)
        {
            if (!base)
                return 0;
            *base = static_cast<IBase*>(this);
            return IncRef();
        }

        uint32 GetRefCount() const { return m_refCount; };

        virtual uint32 IncRef() { return ++m_refCount; };

        inline static SGuid GUID()
        {
			return { 0xaeee0532, 0xfe52, 0x4527, { 0x87, 0x37, 0x6d, 0x14, 0xd1, 0x3d, 0x45, 0x12 } };
        }

        virtual uint32 Delete() = 0;
        virtual uint32 QueryIFace(const SGuid& guid, void** IFace) = 0;
    };



    template <typename T> class RefCounted
    {
    private:
        T* m_ptr;

    public:
        RefCounted() { m_ptr = nullptr; }

        explicit RefCounted(T* t)
        {
            m_ptr = t;
            if (m_ptr)
                m_ptr->IncRef();
        };

        RefCounted(const RefCounted<T>& t) : RefCounted<T>(t.m_ptr) {};

        RefCounted(RefCounted<T>&& mv) noexcept
        {
            m_ptr = mv.m_ptr;
            mv.m_ptr = nullptr;
        }

        ~RefCounted()
        {
            if (m_ptr)
            {
                m_ptr->Delete();
                m_ptr = nullptr;
            };
        }

        RefCounted& operator=(T* t)
        {
            if (this->m_ptr != t)
                RefCounted(t).Swap(*this);
            return *this;
        }

        RefCounted& operator=(const RefCounted<T>& t)
        {
            if (this->m_ptr != t.m_ptr)
                RefCounted(t).Swap(*this);
            return *this;
        }

        RefCounted& operator=(RefCounted<T>&& mv)
        {
            if (*this != mv)
            {
                Reset();
                m_ptr = mv.m_ptr;
                mv.m_ptr = nullptr;
            }
            return *this;
        }

        explicit operator bool() const { return m_ptr != nullptr; }

        T*       operator->() { return m_ptr; }
        const T* operator->() const { return m_ptr; }

        bool IsNull() const { return (m_ptr == nullptr); }
        bool operator==(const T* t) const { return m_ptr == t; }
        bool operator!=(const T* t) const { return m_ptr != t; }
        bool operator==(const RefCounted<T>& t) const { return m_ptr == t.m_ptr; }
        bool operator!=(const RefCounted<T>& t) const { return m_ptr != t.m_ptr; }

        T*        Get() { return m_ptr; }
        const T*  Get() const { return m_ptr; }
        T**       GetAddressOf() { return &m_ptr; }
        const T** GetAddressOf() const { return &m_ptr; }

        T** ReleaseAndGetAddressOf()
        {
            Reset();
            return &m_ptr;
        }

        T*       ToType() { return m_ptr; };
        const T* ToConstType() { return m_ptr; };

        void*       ToVoidPtr() { return m_ptr; }
        const void* ToConstVoidPtr() { return m_ptr; }

        template <typename Q> Q As(void) { return dynamic_cast<Q>(m_ptr); }

        template <typename Q> Q To(void) { return reinterpret_cast<Q>(m_ptr); }

        template <typename Q> Q Cast(void) { return static_cast<Q>(m_ptr); }

        bool IsEqualObj(T* other)
        {
            if (m_ptr == 0 && other == 0)
                return true;
            if (m_ptr == 0 || other == 0)
                return false;

            RefCounted<IBase> p1;
            RefCounted<IBase> p2;
            m_ptr->QueryBase(p1.GetAddressOf());
            other->QueryBase(p2.GetAddressOf());

            return (p1 == p2);
        }

        void Swap(RefCounted& other)
        {
            T* p = m_ptr;
            m_ptr = other.m_ptr;
            other.m_ptr = p;
        }

        void Reset()
        {
            if (m_ptr)
            {
                m_ptr->Delete(); // или Release()
                m_ptr = nullptr;
            }
        }

        void Attach(T* ptr)
        {
            if (m_ptr)
            {
                uint ref = m_ptr->Delete();
                (void)(ref);
                assert(ref != 0 || m_ptr != ptr);
            }

            m_ptr = ptr;
        }

        T* Detach()
        {
            T* ptr = m_ptr;
            m_ptr = 0;
            return ptr;
        }

        uint32 InternalIncRef()
        {
            uint32 ref = 0;
            if (m_ptr)
                ref = m_ptr->IncRef();
            return ref;
        }

        uint32 Release()
        {
            uint32 ref = 0;
            T*     t = m_ptr;
            if (t)
            {
                m_ptr = 0;
                ref = t->Delete();
            }
            return ref;
        }

        ERRCODE QueryIFace(const SGuid& guid, void** IFace)
        {
            if (m_ptr)
                return m_ptr->QueryIFace(guid, IFace);
            return 0;
        }
    };
} // namespace krystallic
