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


namespace krystallic
{
	namespace Runtime
	{
		class CArg
		{
			friend class CArgStack;

		private:
			EScriptArgumentType m_argType;
			union
			{
				bool m_bool;
				int m_int;
				float m_float;
				char* m_str;
				SIMDMath::CVector2 m_vec2;
				SIMDMath::CVector3 m_vec3;
				SIMDMath::CVector4 m_vec4;
				SIMDMath::CQuaternion m_quat;
				SIMDMath::CColorRGBA m_rgba;
				SIMDMath::CPlane m_plane;
				void* m_object;
			};

		public:
			inline CArg();
			inline CArg(EScriptArgumentType type, const void* data);
			inline ~CArg();

			inline void SetBool(bool v);
			inline void SetInt(int v);
			inline void SetFloat(float v);
			inline void SetString(char* v);
			inline void SetVector2(const SIMDMath::CVector2& v);
			inline void SetVector3(const SIMDMath::CVector3& v);
			inline void SetVector4(const SIMDMath::CVector4& v);
			inline void SetQuaternion(const SIMDMath::CQuaternion& v);
			inline void SetColorRGBA(const SIMDMath::CColorRGBA& v);
			inline void SetPlane(const SIMDMath::CPlane& v);

			inline bool					 AsBool() const;
			inline int					 AsInt() const;
			inline float				 AsFloat() const;
			inline char*				 AsString() const;
			inline SIMDMath::CVector2	 AsVector2() const;
			inline SIMDMath::CVector3	 AsVector3() const;
			inline SIMDMath::CVector4	 AsVector4() const;
			inline SIMDMath::CQuaternion AsQuaternion() const;
			inline SIMDMath::CColorRGBA	 AsColorRGBA() const;
			inline SIMDMath::CPlane		 AsPlane() const;

			inline EScriptArgumentType GetArgumentDataType() const;
		};


		CArg::CArg()
		{
			m_argType = SCR_ARG_VOID;
			m_int = 0;
		}

		CArg::CArg(EScriptArgumentType type, const void* data)
		{

		}

		CArg::~CArg()
		{
			if (m_argType == SCR_ARG_STR)
			{

			}

			if (m_argType == SCR_ARG_OBJECT)
			{

			}
		}

		void CArg::SetBool(bool v)
		{
			m_argType = SCR_ARG_BOOL;
			m_bool = v;
		}


		void CArg::SetInt(int v)
		{
			m_argType = SCR_ARG_INT;
			m_int = v;
		}


		void CArg::SetFloat(float v)
		{
			m_argType = SCR_ARG_FLOAT;
			m_float = v;
		}


		void CArg::SetString(char* v)
		{
			m_argType = SCR_ARG_STR;
			m_str = v;
		}


		void CArg::SetVector2(const SIMDMath::CVector2& v)
		{
			m_argType = SCR_ARG_VEC2;
			m_vec2 = v;
		}


		void CArg::SetVector3(const SIMDMath::CVector3& v)
		{
			m_argType = SCR_ARG_VEC3;
			m_vec3 = v;
		}


		void CArg::SetVector4(const SIMDMath::CVector4& v)
		{
			m_argType = SCR_ARG_VEC4;
			m_vec4 = v;
		}


		void CArg::SetQuaternion(const SIMDMath::CQuaternion& v)
		{
			m_argType = SCR_ARG_QUAT;
			m_quat = v;
		}


		void CArg::SetColorRGBA(const SIMDMath::CColorRGBA& v)
		{
			//m_argType = SCR_ARG_RGBA;
			//m_rgba = *v;
		}


		void CArg::SetPlane(const SIMDMath::CPlane& v)
		{
			m_argType = SCR_ARG_PLANE;
			m_plane = *v;
		}


		bool CArg::AsBool() const
		{
			return m_bool;
		}


		int CArg::AsInt() const
		{
			return m_int;
		}


		float CArg::AsFloat() const
		{
			return m_float;
		}


		char* CArg::AsString() const
		{
			return m_str;
		}


		SIMDMath::CVector2 CArg::AsVector2() const
		{
			return m_vec2;
		}


		SIMDMath::CVector3 CArg::AsVector3() const
		{
			return m_vec3;
		}


		SIMDMath::CVector4 CArg::AsVector4() const
		{
			return m_vec4;
		}


		SIMDMath::CQuaternion CArg::AsQuaternion() const
		{
			return m_quat;
		}


		SIMDMath::CColorRGBA CArg::AsColorRGBA() const
		{
			return m_rgba;
		}


		SIMDMath::CPlane CArg::AsPlane() const
		{
			return m_plane;
		}


		EScriptArgumentType CArg::GetArgumentDataType() const
		{
			return m_argType;
		}
	}
}


