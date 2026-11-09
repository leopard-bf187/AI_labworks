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
		struct IScriptContext : public IBase
		{
			virtual ~IScriptContext() = default;

			virtual void* GetContextState() = 0;
			virtual void ResetContext(int stackStart = 2, int numInputs = 0, int numOutputs = 0) = 0;

			virtual void PushBool(bool v) = 0;
			virtual void PushInt(int v) = 0;
			virtual void PushFloat(float v) = 0;
			virtual void PushString(char* v) = 0;
			virtual void PushVector2(const SIMDMath::CVector2& v) = 0;
			virtual void PushVector3(const SIMDMath::CVector3& v) = 0;
			virtual void PushVector4(const SIMDMath::CVector4& v) = 0;
			virtual void PushQuaternion(const SIMDMath::CQuaternion& v) = 0;
			virtual void PushColorRGBA(const SIMDMath::CColorRGBA& v) = 0;
			virtual void PushPlane(const SIMDMath::CPlane& v) = 0;

			virtual bool					AsBool(int i) = 0;
			virtual int						AsInt(int i) = 0;
			virtual float					AsFloat(int i) = 0;
			virtual char*					AsString(int i) = 0;
			virtual SIMDMath::CVector2*		AsVector2(int i) = 0;
			virtual SIMDMath::CVector3*		AsVector3(int i) = 0;
			virtual SIMDMath::CVector4*		AsVector4(int i) = 0;
			virtual SIMDMath::CQuaternion*	AsQuaternion(int i) = 0;
			virtual SIMDMath::CColorRGBA*	AsColorRGBA(int i) = 0;
			virtual SIMDMath::CPlane*		AsPlane(int i) = 0;

			virtual int GetArgCount() = 0;
			virtual int GetTotalArgCount() = 0;
			virtual int ValidateArg(int i) = 0;
		};
	}
}


