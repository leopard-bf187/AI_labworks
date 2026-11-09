/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD & @Ra192192
*
*  Description:
*
*  Date: 15.10.2025
*/


#pragma once


#include "gui_dll_types.h"


namespace krystallic
{
	namespace GUI
	{
		typedef void(*PrintFn)(int tabs, const char* str);


		struct IGUIInputContext : IBase
		{
			GUI_INPUT_STATE m_state;

			virtual ~IGUIInputContext() = default;

			virtual void CommitState() = 0;

			inline static SGuid GUID()
			{
				return { 0x480a22ba, 0xbf48, 0x4531, { 0xbf, 0xc2, 0xa2, 0x18, 0x58, 0x5a, 0x2a, 0x53 } };
			}
		};


		struct IGUIContext : IBase
		{
			virtual ~IGUIContext() = default;

			virtual void GetInputContext(IGUIInputContext** outInputContext) = 0;

			virtual ERRCODE RegisterControl(GUI_CONTROL_REGISTER_DATA* regData) = 0;

			virtual ERRCODE CreateControl(GUI_CONTROL_DESC* desc, int* outControlIndex = nullptr, IGUIControlProxy** outControlProxy = nullptr) = 0;

			virtual ERRCODE GetControlByID(const char* idPath, int* outIndex) = 0;
			virtual ERRCODE GetControlProxy(int index, IGUIControlProxy** outControl) = 0;
			virtual ERRCODE GetControlProxyByID(const char* idPath, IGUIControlProxy** outControl) = 0;

			virtual ERRCODE DestroyControl(int index) = 0;
			virtual ERRCODE DestroyControlByID(const char* idPath) = 0;
			virtual ERRCODE DestroyControlByProxy(IGUIControlProxy* control) = 0;

			// Generic bridge for inline STDUI typed accessors.
			virtual ERRCODE GetStdControlCheckState(int controlIndex, dword expectedType, dword* outState) = 0;
			virtual ERRCODE SetStdControlCheckState(int controlIndex, dword expectedType, dword checkState) = 0;
			virtual ERRCODE GetStdControlFloatValue(int controlIndex, dword expectedType, float* outValue) = 0;
			virtual ERRCODE SetStdControlFloatValue(int controlIndex, dword expectedType, float value) = 0;

			virtual ERRCODE LoadFontFaceFromFileTTF(const char* filePath, Common::IBuffer** outBuffer) = 0;
            virtual ERRCODE LoadFontFaceFromMemoryTTF(const void* loadedData, uint loadedDataSize, Common::IBuffer** outBuffer) = 0;

			virtual ERRCODE CreateFontFamilyObject(const char* fontID, IGUIFont** outFont) = 0;
			virtual ERRCODE GetFontByID(const char* fontID, IGUIFont** outFont) = 0;
			virtual ERRCODE GetFontByHashID(dword fontHashID, IGUIFont** outFont) = 0;
			virtual dword GetFontHashID(const char* fontID) const = 0;
			virtual ERRCODE DestroyFontByID(const char* fontID) = 0;
			virtual ERRCODE DestroyFontByHashID(dword fontHashID) = 0;
			virtual void SetActiveFont(int slot, IGUIFont* font) = 0;
			virtual void SetActiveFontByID(int slot, const char* id) = 0;

			virtual ERRCODE CreateString(IGUIString** outString) = 0;

			virtual ERRCODE CreateStringTable(const char* stringTableID, IGUIStringTable** outTable) = 0;
			virtual ERRCODE GetStringTableByID(const char* stringTableID, IGUIStringTable** outTable) = 0;
			virtual ERRCODE GetActiveStringTable(IGUIStringTable** outTable) = 0;
			virtual ERRCODE GetStringTableByHashID(dword stringTableHashID, IGUIStringTable** outTable) = 0;
			virtual ERRCODE DestroyStringTableByID(const char* stringTableID) = 0;
			virtual ERRCODE DestroyStringTableByHashID(dword stringTableHashID) = 0;

			virtual ERRCODE GetDefaultStringTable(IGUIStringTable** outTable) = 0;
			virtual ERRCODE SetActiveStringTable(IGUIStringTable* table) = 0;
			virtual ERRCODE SetActiveStringTableByID(const char* id) = 0;

			virtual void SetStringGeometryCacheSize(uint cacheSizeInBytes) = 0;
			virtual uint GetStringGeometryCacheSize() const = 0;

			virtual void SetScreenResolution(float width, float height) = 0;
			virtual void GetScreenResoultion(float* width, float* height) = 0;
			virtual void SetViewport(GUI_VIEWPORT vp) = 0;
			virtual void GetViewport(GUI_VIEWPORT* vp) = 0;
			virtual SIMDMath::CVector2 GetScale() const = 0;

			virtual void SetRenderStrategy(IGUIRenderStrategy* strategy) = 0;
			virtual void GetRenderStrategy(IGUIRenderStrategy** outStrategy) = 0;

			virtual ERRCODE CreateAllRenderableResources() = 0;
			virtual void DestroyAllRenderableResources() = 0;

			virtual void SetColorTheme(int theme) = 0;
			virtual void SetCustomColorTheme(GUI_COLOR_SCHEME* theme) = 0;

			virtual ERRCODE Update(float deltaTime) = 0;
			virtual ERRCODE Render() = 0;

			virtual void Dump(PrintFn fn) = 0;

			inline static SGuid GUID()
			{
				return { 0x76fc9ffa, 0x45b4, 0x4bb3, { 0x83, 0x28, 0x12, 0xe6, 0x96, 0xd0, 0xc5, 0xf8 } };
			}
		};
	}
}

