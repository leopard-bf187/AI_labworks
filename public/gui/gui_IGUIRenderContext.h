/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @Ra192192
*
*  Description:
*
*  Date: 15.10.2025
*/


#pragma once


#include "gui_dll_types.h"
#include "stdlib_dll.h"


namespace krystallic
{
    namespace GUI
    {
        struct IGUIRenderContext
        {
        public:
            struct SolidRect;
            struct Rect;
            struct InnerBorder;
            struct OuterBorder;
            struct TriPatch;
            struct SixPatch;
            struct NinePatch;

        protected:
            Stdlib::Vector<Render::XYUV1C32DW2>* m_vb;
            Stdlib::Vector<uint>*                m_ib;
            uint                                 m_vertexOffset;

            GUI_COLOR_THEME* m_theme;

        protected:
            IGUIRenderContext() : m_vb(0), m_ib(0), m_vertexOffset(0), m_theme(0) {};

            virtual ~IGUIRenderContext() = default;

        public:
            //virtual void SetColorTheme(int theme) = 0;
            //virtual void SetCustomColorTheme(GUI_COLOR_THEME* theme) = 0;

            virtual void DrawString(GUI_TEXT_REF ref,
                                    GUI_RECT     clip,
                                    dword        stringDrawFlags = GUI_STRING_DRAW_VERTICAL_ALIGN_CENTER |
                                                            GUI_STRING_DRAW_HORIZONTAL_ALIGN_MIDDLE,
                                    Math::CVector2   pos = Math::CVector2(0.0f, 0.0f),
                                    Math::CColorRGBA color = Math::CColorRGBA(0.0f, 0.0f, 0.0f, 1.0f)) {};

            void DrawGeometry(SolidRect& r);
            void DrawGeometry(Rect& r);
            void DrawGeometry(InnerBorder& b);
            void DrawGeometry(OuterBorder& b);
            void DrawGeometry(TriPatch& np);
            void DrawGeometry(SixPatch& np);
            void DrawGeometry(NinePatch& np);


            void
            DrawGeometry(uint numVertices, krystallic::Render::XYUV1C32DW2* vertices, uint numIndices, uint* indices)
            {
                for (uint i = 0; i < numVertices; i++)
                    m_vb->PushBack(vertices[i]);

                for (uint i = 0; i < numIndices; i++)
                    m_ib->PushBack(m_vertexOffset + indices[i]);

                //for (uint i = 0; i < numIndices; i++)
                //	indices[i] += m_vertexOffset;

                m_vertexOffset += numVertices;
            }


            void DrawRect(GUI_RECT world, Math::CColorRGBA col, dword flags, dword style, float width = 1.0f)
            {
                float w = width;
                float l = world.m_left;
                float t = world.m_top;
                float r = world.m_right;
                float b = world.m_bottom;

                Render::XYUV1C32DW2 v[8] = {
                    {{l, t}, 0.0f, col, 0, 0},         // 0
                    {{r, t}, 0.0f, col, 0, 0},         // 1
                    {{l, b}, 0.0f, col, 0, 0},         // 2
                    {{r, b}, 0.0f, col, 0, 0},         // 3
                    {{l + w, t + w}, 0.0f, col, 0, 0}, // 4
                    {{r - w, t + w}, 0.0f, col, 0, 0}, // 5
                    {{l + w, b - w}, 0.0f, col, 0, 0}, // 6
                    {{r - w, b - w}, 0.0f, col, 0, 0}, // 7
                };

                uint idx[] = {
                    0, 4, 2, 2, 4, 6,

                    0, 1, 4, 4, 1, 5,

                    5, 1, 7, 7, 1, 3,

                    6, 7, 2, 2, 7, 3,
                };

                DrawGeometry(sizeof(v) / sizeof(Render::XYUV1C32DW2), v, sizeof(idx) / sizeof(uint), idx);
            }

            void DrawInnerBorderRect(GUI_RECT world, dword flags, dword style, float width = 1.0f)
            {
                float w = width;
                float l = world.m_left;
                float t = world.m_top;
                float r = world.m_right;
                float b = world.m_bottom;

                Math::CColorRGBA cl = {0.700f, 0.700f, 0.700f, 1.0f};
                Math::CColorRGBA ct = {0.600f, 0.600f, 0.600f, 1.0f};
                Math::CColorRGBA cr = {0.900f, 0.900f, 0.900f, 1.0f};
                Math::CColorRGBA cb = {1.000f, 1.000f, 1.000f, 1.0f};

                Render::XYUV1C32DW2 v[] = {
                    // left
                    {{l, t}, 0.0f, cl, 0, 0},         // 0
                    {{l + w, t + w}, 0.0f, cl, 0, 0}, // 1
                    {{l, b}, 0.0f, cl, 0, 0},         // 2
                    {{l + w, b - w}, 0.0f, cl, 0, 0}, // 3

                    // top
                    {{l, t}, 0.0f, ct, 0, 0},         // 4
                    {{r, t}, 0.0f, ct, 0, 0},         // 5
                    {{l + w, t + w}, 0.0f, ct, 0, 0}, // 6
                    {{r - w, t + w}, 0.0f, ct, 0, 0}, // 7

                    // right
                    {{r - w, t + w}, 0.0f, cr, 0, 0}, // 8
                    {{r, t}, 0.0f, cr, 0, 0},         // 9
                    {{r - w, b - w}, 0.0f, cr, 0, 0}, // 10
                    {{r, b}, 0.0f, cr, 0, 0},         // 11

                    // bottom
                    {{l + w, b - w}, 0.0f, cb, 0, 0}, // 12
                    {{r - w, b - w}, 0.0f, cb, 0, 0}, // 13
                    {{l, b}, 0.0f, cb, 0, 0},         // 14
                    {{r, b}, 0.0f, cb, 0, 0},         // 15
                };

                uint idx[] = {0,  1,  2,  2,  1,  3,

                              4,  5,  6,  6,  5,  7,

                              8,  9,  10, 10, 9,  11,

                              12, 13, 14, 14, 13, 15};

                DrawGeometry(16, v, 24, idx);
            }

            void DrawOuterBorderRect(GUI_RECT world, dword flags, dword style, float width = 1.0f)
            {
                float w = width;
                float l = world.m_left;
                float t = world.m_top;
                float r = world.m_right;
                float b = world.m_bottom;

                Math::CColorRGBA cr = {0.700f, 0.700f, 0.700f, 1.0f};
                Math::CColorRGBA cb = {0.600f, 0.600f, 0.600f, 1.0f};
                Math::CColorRGBA cl = {0.900f, 0.900f, 0.900f, 1.0f};
                Math::CColorRGBA ct = {1.000f, 1.000f, 1.000f, 1.0f};

                Render::XYUV1C32DW2 v[] = {
                    // left
                    {{l, t}, 0.0f, cl, 0, 0},         // 0
                    {{l - w, t - w}, 0.0f, cl, 0, 0}, // 1
                    {{l, b}, 0.0f, cl, 0, 0},         // 2
                    {{l - w, b + w}, 0.0f, cl, 0, 0}, // 3

                    // top
                    {{l, t}, 0.0f, ct, 0, 0},         // 4
                    {{r, t}, 0.0f, ct, 0, 0},         // 5
                    {{l - w, t - w}, 0.0f, ct, 0, 0}, // 6
                    {{r + w, t - w}, 0.0f, ct, 0, 0}, // 7

                    // right
                    {{r + w, t - w}, 0.0f, cr, 0, 0}, // 8
                    {{r, t}, 0.0f, cr, 0, 0},         // 9
                    {{r + w, b + w}, 0.0f, cr, 0, 0}, // 10
                    {{r, b}, 0.0f, cr, 0, 0},         // 11

                    // bottom
                    {{l - w, b + w}, 0.0f, cb, 0, 0}, // 12
                    {{r + w, b + w}, 0.0f, cb, 0, 0}, // 13
                    {{l, b}, 0.0f, cb, 0, 0},         // 14
                    {{r, b}, 0.0f, cb, 0, 0},         // 15
                };

                uint idx[] = {0,  1,  2,  2,  1,  3,

                              4,  5,  6,  6,  5,  7,

                              8,  9,  10, 10, 9,  11,

                              12, 13, 14, 14, 13, 15};

                DrawGeometry(16, v, 24, idx);
            }

            void DrawSolidRect(GUI_RECT world, Math::CColorRGBA col, dword flags, dword style)
            {
                float l = world.m_left;
                float t = world.m_top;
                float r = world.m_right;
                float b = world.m_bottom;

                Render::XYUV1C32DW2 v[4] = {
                    {{l, t}, 0.0f, col, 0, 0},
                    {{r, t}, 0.0f, col, 0, 0},
                    {{l, b}, 0.0f, col, 0, 0},
                    {{r, b}, 0.0f, col, 0, 0},
                };
                uint idx[6] = {0, 1, 2, 2, 1, 3};

                DrawGeometry(sizeof(v) / sizeof(Render::XYUV1C32DW2), v, sizeof(idx) / sizeof(uint), idx);
            }

            void Draw2Patch() {}

            void Draw3Patch() {}

            void Draw6Patch() {}

            void Draw9Patch() {}

            void DrawCircle() {}
        };
    } // namespace GUI
} // namespace krystallic
