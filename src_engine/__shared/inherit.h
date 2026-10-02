/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 14.07.2026
*/
#pragma once


#include <cstddef>
#include <cstdlib>
#include <utility>
#include <new>


#include "common.h"


namespace krystallic
{
	template <typename Clazz, typename... IFace>
	struct Inherit : IFace...
	{
		RefCounted<Common::IAllocator> m_allocator;

		explicit Inherit(Common::IAllocator* allocator) : m_allocator(allocator)
		{}

		template<typename... Args>
		inline static Clazz* _Create(Common::IAllocator* allocator, Args&&... args)
		{
			void* mem = allocator->Alloc(sizeof(Clazz), alignof(Clazz));

			if (!mem)
				return nullptr;

			return new (mem) Clazz(allocator, std::forward<Args>(args)...);
		}

		inline static void _Destroy(Clazz* ptr)
		{
			if (!ptr)
				return;

			Common::IAllocator* allocator = ptr->m_allocator.Get();
			allocator->IncRef();

			ptr->~Clazz();

			allocator->Free(ptr);
			allocator->Delete();
		}

		template<typename... Args>
		inline static Clazz* _CreateN(uint n, Common::IAllocator* allocator, Args&&... args)
		{
			void* mem = allocator->Alloc(sizeof(Clazz) * n, alignof(Clazz));

			if (!mem)
				return nullptr;

			Clazz* arr = static_cast<Clazz*>(mem);

			for (uint i = 0; i < n; i++)
				new (&arr[i]) Clazz(allocator, std::forward<Args>(args)...);

			return arr;
		}

		inline static void _DestroyN(uint n, Clazz* ptr)
		{
			if (!ptr)
				return;

			Common::IAllocator* allocator = ptr->m_allocator.Get();
			allocator->IncRef();

			for (uint i = n; i > 0; i--)
				ptr[i - 1].~Clazz();

			allocator->Free(ptr);
			allocator->Delete();
		}
	};



	template <typename Clazz, typename... IFace>
	struct VirtualInherit : virtual IFace...
	{
		RefCounted<Common::IAllocator> m_allocator;

		explicit VirtualInherit(Common::IAllocator* allocator) : m_allocator(allocator)
		{}

		template<typename... Args>
		inline static Clazz* _Create(Common::IAllocator* allocator, Args&&... args)
		{
			void* mem = allocator->Alloc(sizeof(Clazz), alignof(Clazz));

			if (!mem)
				return nullptr;

			return new (mem) Clazz(allocator, std::forward<Args>(args)...);
		}

		inline static void _Destroy(Clazz* ptr)
		{
			if (!ptr)
				return;

			Common::IAllocator* allocator = ptr->m_allocator.Get();
			allocator->IncRef();

			ptr->~Clazz();

			allocator->Free(ptr);
			allocator->Delete();
		}

		template<typename... Args>
		inline static Clazz* _CreateN(uint n, Common::IAllocator* allocator, Args&&... args)
		{
			void* mem = allocator->Alloc(sizeof(Clazz) * n, alignof(Clazz));

			if (!mem)
				return nullptr;

			Clazz* arr = static_cast<Clazz*>(mem);

			for (uint i = 0; i < n; i++)
				new (&arr[i]) Clazz(allocator, std::forward<Args>(args)...);

			return arr;
		}

		inline static void _DestroyN(uint n, Clazz* ptr)
		{
			if (!ptr)
				return;

			Common::IAllocator* allocator = ptr->m_allocator.Get();
			allocator->IncRef();

			for (uint i = n; i > 0; i--)
				ptr[i - 1].~Clazz();

			allocator->Free(ptr);
			allocator->Delete();
		}
	};



	template <typename Clazz, typename... IFace>
	struct InheritSingleton : IFace...
	{
		static Clazz* m_instance;

		inline Clazz* GetInstance()
		{
			return m_instance;
		}

		inline const Clazz* GetInstance() const
		{
			return m_instance;
		}
	};
}