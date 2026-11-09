/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
* 
*  Authors: 
*
*  Description:
*
*  Date: 
*/


#pragma once


#define GUI_VERSION_MAJOR 0x01
#define	GUI_VERSION_MINOR 0x00
#define GUI_VERSION_PATCH 0x00


#include "../pch.h"


#include "../simdmath.h"
#include "../common.h"
#include "../core.h"
#include "../vertexformats.h"


#include "gui_dll_enums.h"


namespace krystallic
{
	namespace GUI
	{
		// systems
		struct IGUIContext;
		struct IGUIInputContext;

		struct IGUIRenderContext;  
		struct IGUIRenderStrategy; 

		// controls
		struct IGUIControlProxy;
		struct IGUIControlSystem; 

		// fonts
		struct IGUIFont;
		struct IGUIString;
		struct IGUIStringTable;

		// cursors
		struct IGUICursor;
		struct IGUICursorController;


		typedef void (*GUIComponentCtorFn)(uint startPos, uint count, void* controlArray);
		typedef void (*GUIComponentDtorFn)(uint startPos, uint count, void* controlArray);


		struct GUI_TEXT_STYLE
		{
			dword m_fontTextureSlot : 8; // 0..1, èëè áîëüøå â áóäóùåì
			dword m_fontChannel : 8;     // 0=R, 1=G, 2=B, 3=A
			dword m_decorationMask : 16; // underline, strike, etc.
			SIMDMath::CColorRGBA m_color;
		};


		struct GUI_COLOR_SCHEME
		{
            SIMDMath::CColorRGBA m_frame;
			SIMDMath::CColorRGBA m_hoveredFrame;
			SIMDMath::CColorRGBA m_pushedFrame;
			SIMDMath::CColorRGBA m_innerFrame;

			SIMDMath::CColorRGBA m_border;
			SIMDMath::CColorRGBA m_borderEdgeLeft;
			SIMDMath::CColorRGBA m_borderEdgeTop;
			SIMDMath::CColorRGBA m_borderEdgeRight;
			SIMDMath::CColorRGBA m_borderEdgeBottom;

			SIMDMath::CColorRGBA m_activeWindowHeaderLeft;
            SIMDMath::CColorRGBA m_activeWindowHeaderRight;
            SIMDMath::CColorRGBA m_inactiveWindowHeaderLeft;
            SIMDMath::CColorRGBA m_inactiveWindowHeaderRight;

			SIMDMath::CColorRGBA m_activeFont;
            SIMDMath::CColorRGBA m_inactiveFont;
            SIMDMath::CColorRGBA m_selectedFont;
		};


		inline int GetFontStyleBit(GUI_FONT_STYLE style)
		{
			if (style == GUI_FONT_BOLD)
				return 1;

			if (style == GUI_FONT_ITALIC)
				return 2;

			if (style == GUI_FONT_BOLD_ITALIC)
				return 3;

			return 0;
		}


		inline bool operator == (const GUI_TEXT_STYLE& a, const GUI_TEXT_STYLE& b)
		{
			return	a.m_fontTextureSlot == b.m_fontTextureSlot &&
				a.m_fontChannel == b.m_fontChannel &&
				a.m_decorationMask == b.m_decorationMask &&
				a.m_color == b.m_color;
		}


		struct GUI_TEXT_REF
		{
			const char* m_stringID = nullptr;
			dword m_stringHashID = ~0u;
			uint32 m_index = ~0u;
			uint32 m_tableVersion = ~0u;

			inline bool IsValid() const
			{
				return !(!m_stringID || (m_stringHashID == ~0u));
			}
		};


		struct GUI_PLAIN_TEXT
		{
			const char* utf8Text;
			GUI_TEXT_STYLE m_style;
		};


		struct GUI_TEXT_RUN
		{
			uint32 m_start;
			uint32 m_length;
			GUI_TEXT_STYLE m_style;
		};


		struct GUI_FONT_FACE_KEY
		{
			dword m_fontHashID;
			dword m_faceStyle;      // regular/bold/italic
			uint  m_sizePx;
		};


		struct GUI_SCRIPT_REF
		{
			const char* scriptText;
			uint bytelen;
		};


		inline bool operator==(const GUI_FONT_FACE_KEY& a, const GUI_FONT_FACE_KEY& b)
		{
			return	a.m_fontHashID == b.m_fontHashID &&
					a.m_faceStyle == b.m_faceStyle &&
					a.m_sizePx == b.m_sizePx;
		}


		struct GUI_CONTROL_REGISTER_DATA
		{
			dword m_controlTypeID;
			dword m_controlSize;
			GUIComponentCtorFn m_ctor;
			GUIComponentDtorFn m_dtor;
			IGUIControlSystem* m_system;
		};


		struct GUI_CONTROL_DESC
		{
			char m_controlID[64];
			dword m_type;
			int m_parentIndex;
			SIMDMath::CVector2 m_pos;
			SIMDMath::CVector2 m_size;
			dword m_controlFlags;
			dword m_controlStyle;
			dword m_initialState;
			void* m_userData;
			void* m_subtypeControlDescPtr;
		};





		typedef ERRCODE(*OnCallbackFn)();
		typedef ERRCODE(*OnCheckStateChangeFn)(int controlIndex, dword oldState, dword newState, void* userData);
		typedef ERRCODE(*OnCheckValueChangeFn)(int controlIndex, float oldValue, float newValue, void* userData);


		//typedef ERRCODE(*OnEnableFn)(IGUIControl* control);
		//typedef ERRCODE(*OnFocusFn)(IGUIControl* control);
		//typedef ERRCODE(*OnHoverFn)(IGUIControl* control);
		//typedef ERRCODE(*OnMouseClickFn)(IGUIControl* control);
		//typedef ERRCODE(*OnMouseDblClickFn)(IGUIControl* control);
		//typedef ERRCODE(*OnMouseHoldFn)(IGUIControl* control);
		//typedef ERRCODE(*OnValueChangeFn)(IGUIControl* control);
		//typedef ERRCODE(*OnSelectFn)(IGUIControl* control);
		//typedef ERRCODE(*OnThumbMoveFn)(IGUIControl* control);
		//typedef ERRCODE(*OnSetCursorFn)(IGUIControl* control);
		//typedef ERRCODE(*OnCharFn)(IGUIControl* control);
		//typedef ERRCODE(*OnInputFn)(IGUIControl* control);
		//typedef ERRCODE(*OnKeyComboFn)(IGUIControl* control);


		typedef struct GUI_RECT
		{
			union
			{
				struct
				{
					float m_left;
					float m_top;
					float m_right;
					float m_bottom;
				};
				struct
				{
					float x;
					float y;
					float w;
					float h;
				};
				SIMDMath::CVector4 __simd;
			};

			GUI_RECT()
			{
				m_left = m_top = m_right = m_bottom = 0.0f;
			}
			
			GUI_RECT(float f)
			{
				m_left = m_top = m_right = m_bottom = f;
			}
			
			GUI_RECT(float leftRight, float topBottom)
			{
				m_left = m_right = leftRight;
				m_top = m_bottom = topBottom;
			}
			
			GUI_RECT(float l, float t, float r, float b)
			{
				m_left = l;
				m_right = r;
				m_top = t;
				m_bottom = b;
			}

			GUI_RECT(const GUI_RECT& other)
			{
				__simd = other.__simd;
			}

			GUI_RECT& operator= (const float& f)
			{
				m_left = m_top = m_right = m_bottom = f;
				return *this;
			}

			GUI_RECT& operator= (const GUI_RECT& other)
			{
				__simd = other.__simd;
				return *this;
			}

			GUI_RECT Translate(SIMDMath::CVector2 pos)
			{
				return GUI_RECT(pos.x + m_left, pos.y + m_top, pos.x + m_right, pos.y + m_bottom);
			}

			inline GUI_RECT Intersect(GUI_RECT a)
			{
				return GUI_RECT
				(
					(a.m_left > m_left) ? a.m_left : m_left,
					(a.m_top > m_top) ? a.m_top : m_top,
					(a.m_right < m_right) ? a.m_right : m_right,
					(a.m_bottom < m_bottom) ? a.m_bottom : m_bottom
				);
			}

			bool Collide(GUI_RECT a) const
			{
				return !(m_left >= a.m_right || m_right <= a.m_left || m_top >= a.m_bottom || m_bottom <= a.m_top);
			}

			SIMDMath::CVector2 GetPos()
			{
				return SIMDMath::CVector2(m_left, m_top);
			}

			SIMDMath::CVector2 GetSize()
			{
				return SIMDMath::CVector2(m_right - m_left, m_bottom - m_top);
			}

			float GetWidth()
			{
				return m_right - m_left;
			}

			float GetHeight()
			{
				return m_bottom - m_top;
			}

			bool IsValid()
			{
				return (m_left < m_right) && (m_top < m_bottom);
			}

			bool operator ==(GUI_RECT& o)
			{
				return (x == o.x) && (y == o.y) && (w == o.w) && (h == o.h);
			}

			bool operator !=(GUI_RECT& o)
			{
				return !(*this == o);
			}

		} GUI_UV, GUI_MARGIN, GUI_PADDING;

		struct GUI_EVENT_DATA
		{
			GUI_EVENT_TYPE m_type;
			int m_mouseButton = -1;
			SIMDMath::CVector2 m_mousePos;
			int m_controlIndex = -1;
			GUI_RECT m_worldRect;
			IGUIContext* m_context = nullptr;
		};



		struct GUI_INPUT_STATE
		{
			SIMDMath::CVector2 m_mousePos;
			SIMDMath::CVector2 m_mouseDelta;
			int m_Z;
			float m_wheel;
			union
			{
				long long m_mouse;
				byte m_mouseButton[8];
			};
			union
			{
				long long m_keyboard[64];
				byte m_keys[512];
			};
			union
			{
				struct
				{
					byte m_ctrl;
					byte m_shift;
					byte m_alt;
					byte m_super;
				};
				dword m_superKeys;
			};

			explicit GUI_INPUT_STATE()
			{
				MakeNull();
			}

			void MakeNull()
			{
				m_mousePos = 0.0f;
				m_mouseDelta = 0.0f;
				m_Z = 0;
				m_wheel = 0.0f;
				m_mouse = 0ull;

				for (int i = 0; i < 64; i++)
					m_keyboard[i] = 0ull;

				m_superKeys = 0ul;
			}

			GUI_INPUT_STATE& operator = (const GUI_INPUT_STATE& other)
			{
				m_mousePos = other.m_mousePos;
				m_mouseDelta = other.m_mouseDelta;
				m_Z = other.m_Z;
				m_wheel = other.m_wheel;
				m_mouse = other.m_mouse;

				for (int i = 0; i < 64; i++)
					m_keyboard[i] = other.m_keyboard[i];

				m_superKeys = other.m_superKeys;

				return *this;
			}
		};


		struct GUI_CONTROL_INITIAL_DATA
		{
			dword* m_state;
			dword* m_flags;
			dword* m_style;
			GUI_RECT* m_local;
			GUI_RECT* m_clipRect;
		};


		typedef void* InLayoutHandle;
		typedef void* VBuffHandle;
		typedef void* IBuffHandle;
		typedef void* Tex2DHandle;
		typedef void* ShaderBlobHandle;
		typedef void* VertexShaderHandle;
		typedef void* PixelShaderHandle;


		struct GUI_VIEWPORT
		{
			union
			{
				struct
				{
					float m_x;
					float m_y;
					float m_width;
					float m_height;
				};
				SIMDMath::CVector4 m_vec;
				GUI_RECT m_rect;
			};

			GUI_VIEWPORT() : GUI_VIEWPORT(0.0f, 0.0f, 0.0f, 0.0f) {}

			GUI_VIEWPORT(float x, float y, float w, float h) : m_x(x), m_y(y), m_width(w), m_height(h) {}

			GUI_VIEWPORT(SIMDMath::CVector4 v) : m_vec(v) {}

			GUI_VIEWPORT(const GUI_VIEWPORT& other)
			{
				m_rect = other.m_rect;
			}

			GUI_VIEWPORT& operator = (const GUI_VIEWPORT& other)
			{
				m_rect = other.m_rect;
				return *this;
			}
		};


		typedef struct _GUI_MATERIAL
		{
			union
			{
				struct
				{
					Tex2DHandle m_tex2D_0;
					Tex2DHandle m_tex2D_1;
					Tex2DHandle m_tex2D_2;
					Tex2DHandle m_tex2D_3;
					Tex2DHandle m_tex2D_4;
					Tex2DHandle m_tex2D_5;
					Tex2DHandle m_tex2D_6;
					Tex2DHandle m_tex2D_7;
				};
				Tex2DHandle m_textures2D[8];
				void* m_textures[8];
			};
			uint m_numTextures;
			VertexShaderHandle m_vertexShader;
			PixelShaderHandle m_pixelShader;
		} GUI_MATERIAL;


		typedef struct _GUI_DRAW_COMMAND
		{
			GUI_RECT m_clipRect;
			uint m_vertexOffset;
			uint m_vertexCount;
			uint m_indexOffset;
			uint m_indexCount;
		} GUI_DRAW_COMMAND;


		typedef struct _GUI_MAPPED_RESOURCE
		{
			void* m_mappedResourceHandle;
			void* m_data;
			uint m_rowPitch;
			uint m_depthPitch;
		} GUI_MAPPED_RESOURCE;


		typedef struct _GUI_CREATE_INPUT_LAYOUT
		{
			dword m_inputLayoutFlags;
			void* m_vsByteCode;
			uint m_vsByteCodeLength;
		} GUI_CREATE_INPUT_LAYOUT;


		typedef struct _GUI_CREATE_BUFF
		{
			uint m_bufferSize;
			GUI_RESOURCE_USAGE_TYPE m_usage;
		} GUI_CREATE_BUFF;


		typedef struct _GUI_CREATE_TEXTURE
		{
			uint m_width;
			uint m_height;
			uint m_mips;
			GUI_TEXTURE_FORMAT m_format;
			GUI_RESOURCE_USAGE_TYPE m_usage;
			void* m_initialData;
			uint m_rowPitch;
		} GUI_CREATE_TEXTURE;


		typedef struct _GUI_COMPILE_SHADER
		{
			void* m_data;
			uint m_size;
			const char* m_entry;
			int m_profile_0VS_1PS;
		} GUI_COMPILE_SHADER;

		
		const dword DEFAULT_CONTROL_STATE = GUI_STATE_ENABLED|GUI_STATE_VISIBLE;
		const dword DEFAULT_CONTROL_STYLE = 0;
		const dword DEFAULT_CONTROL_FLAGS = GUI_FLAG_HAS_CHILDREN|GUI_FLAG_SUPPORTS_INPUT;


		typedef ERRCODE(*__CreateGUIContextFn)(qword registerStdControlMask, IGUIContext** outManager);
		CORE_API ERRCODE CreateGUIContext(qword registerStdControlMask, IGUIContext** outContext);
	}
}



