/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD
*
*  Description:
*
*  Date: 10.07.2025
*/


#pragma once


#include "scr_lib_types.h"
#include "scr_CArg.inl"


namespace krystallic
{
	namespace Runtime
	{
		class CArgStack
		{
		private:
			CArg m_InArg[16];
			CArg m_OutArg[16];
			int m_NumInArgs;
			int m_NumOutArgs;
			int m_CurrInArg;
			int m_CurrOutArg;

		public:
			inline CArgStack();
			inline ~CArgStack();

			inline int GetNumInputArgs() const;
			inline int GetNumOutputArgs() const;
			inline CArg* GetInputArg(unsigned int num);
			inline CArg* PushInputArg();
			inline CArg* PopInputArg();
			inline CArg* GetOutputArg(unsigned int num);
			inline CArg* PushOutputArg();
			inline CArg* PopOutputArg();
			inline void Clear();
			inline void Rewind();

			inline int& GetCurrInArg();
			inline int& GetCurrOutArg();
		};


		CArgStack::CArgStack()
		{
			m_NumInArgs = 0;
			m_NumOutArgs = 0;
			m_CurrInArg = 0;
			m_CurrOutArg = 0;

			for (int i = 0; i < 16; i++)
			{
				m_InArg[i].m_argType = SCR_ARG_VOID;
				m_InArg[i].m_int = 0;
				m_OutArg[i].m_argType = SCR_ARG_VOID;
				m_InArg[i].m_int = 0;
			}
		}


		CArgStack::~CArgStack()
		{

		}


		int CArgStack::GetNumInputArgs() const
		{
			return m_NumInArgs;
		}


		int CArgStack::GetNumOutputArgs() const
		{
			return m_NumOutArgs;
		}


		CArg* CArgStack::GetInputArg(unsigned int num)
		{
			return &m_InArg[num];
		}


		CArg* CArgStack::PushInputArg()
		{
			m_CurrInArg = m_NumInArgs++;
			return &m_InArg[m_CurrInArg];
		}


		CArg* CArgStack::PopInputArg()
		{
			return &m_InArg[m_CurrInArg++];
		}


		CArg* CArgStack::GetOutputArg(unsigned int num)
		{
			return &m_OutArg[num];
		}


		CArg* CArgStack::PushOutputArg()
		{
			m_CurrOutArg = m_NumOutArgs++;
			return &m_OutArg[m_CurrOutArg];
		}


		CArg* CArgStack::PopOutputArg()
		{
			return &m_OutArg[m_CurrOutArg++];
		}


		void CArgStack::Clear()
		{
			m_NumInArgs = 0;
			m_NumOutArgs = 0;
			m_CurrInArg = 0;
			m_CurrOutArg = 0;
		}


		void CArgStack::Rewind()
		{

		}


		int& CArgStack::GetCurrInArg()
		{
			return m_CurrInArg;
		}


		int& CArgStack::GetCurrOutArg()
		{
			return m_CurrOutArg;
		}
	}
}


