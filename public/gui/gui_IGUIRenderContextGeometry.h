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
#include "gui_IGUIRenderContext.h"
#include "stdlib_dll.h"


namespace krystallic
{
    namespace GUI
    {
        namespace GUIGeometry
        {
            template <int rows = 1, int cols = 1> class PatchGrid
            {
            };

            template <int rows, int cols> class SplitPatchGrid
            {
            };
        } // namespace GUIGeometry

        struct IGUIRenderContext::SolidRect
        {
            Render::XYUV1C32DW2 m_v[4];
            uint                m_i[6];

            inline uint                 GetVertexCount() const { return sizeof(m_v) / sizeof(m_v[0]); }
            inline uint                 GetIndexCount() const { return sizeof(m_i) / sizeof(m_i[0]); }
            inline Render::XYUV1C32DW2* GetVB() { return m_v; }
            inline uint*                GetIB() { return m_i; }

            explicit SolidRect(
                GUI_RECT world, GUI_UV uv, Math::CColorRGBA color, uint textureSlot, uint textureArrayIndex)
            {
                float l = world.m_left;
                float t = world.m_top;
                float r = world.m_right;
                float b = world.m_bottom;

                float uvl = uv.m_left;
                float uvt = uv.m_top;
                float uvr = uv.m_right;
                float uvb = uv.m_bottom;

                m_v[0] = {{l, t}, {uvl, uvt}, color, textureSlot, textureArrayIndex};
                m_v[1] = {{r, t}, {uvr, uvt}, color, textureSlot, textureArrayIndex};
                m_v[2] = {{l, b}, {uvl, uvb}, color, textureSlot, textureArrayIndex};
                m_v[3] = {{r, b}, {uvr, uvb}, color, textureSlot, textureArrayIndex};

                m_i[0] = 0;
                m_i[1] = 1;
                m_i[2] = 2;
                m_i[3] = 2;
                m_i[4] = 1;
                m_i[5] = 3;
            }

            void SetUV(GUI_UV uv)
            {
                float l = uv.m_left;
                float t = uv.m_top;
                float r = uv.m_right;
                float b = uv.m_bottom;

                m_v[0].uv = {l, t};
                m_v[1].uv = {r, t};
                m_v[2].uv = {l, b};
                m_v[3].uv = {r, b};
            }

            void SetColor(Math::CColorRGBA color)
            {
                m_v[0].color = color;
                m_v[1].color = color;
                m_v[2].color = color;
                m_v[3].color = color;
            }

            void SetTexture(uint textureSlot, uint textureArrayIndex)
            {
                m_v[0].textureSlot = textureSlot;
                m_v[1].textureSlot = textureSlot;
                m_v[2].textureSlot = textureSlot;
                m_v[3].textureSlot = textureSlot;
                m_v[0].textureArrayIndex = textureArrayIndex;
                m_v[1].textureArrayIndex = textureArrayIndex;
                m_v[2].textureArrayIndex = textureArrayIndex;
                m_v[3].textureArrayIndex = textureArrayIndex;
            }
        };


        struct IGUIRenderContext::Rect
        {
            Render::XYUV1C32DW2 m_v[8];
            uint                m_i[24];

            inline uint                 GetVertexCount() const { return sizeof(m_v) / sizeof(m_v[0]); }
            inline uint                 GetIndexCount() const { return sizeof(m_i) / sizeof(m_i[0]); }
            inline Render::XYUV1C32DW2* GetVB() { return m_v; }
            inline uint*                GetIB() { return m_i; }

            explicit Rect(GUI_RECT         world,
                          GUI_UV           uvOuter,
                          GUI_UV           uvInner,
                          Math::CColorRGBA color,
                          uint             textureSlot,
                          uint             textureArrayIndex,
                          float            borderWidth = 1.0f)
            {
                float w = borderWidth;
                float l = world.m_left;
                float t = world.m_top;
                float r = world.m_right;
                float b = world.m_bottom;

                float uvol = uvOuter.m_left;
                float uvot = uvOuter.m_top;
                float uvor = uvOuter.m_right;
                float uvob = uvOuter.m_bottom;

                float uvil = uvInner.m_left;
                float uvit = uvInner.m_top;
                float uvir = uvInner.m_right;
                float uvib = uvInner.m_bottom;

                m_v[0] = {{l, t}, {uvol, uvot}, color, textureSlot, textureArrayIndex};         // 0
                m_v[1] = {{r, t}, {uvor, uvot}, color, textureSlot, textureArrayIndex};         // 1
                m_v[2] = {{l, b}, {uvol, uvob}, color, textureSlot, textureArrayIndex};         // 2
                m_v[3] = {{r, b}, {uvor, uvob}, color, textureSlot, textureArrayIndex};         // 3
                m_v[4] = {{l + w, t + w}, {uvil, uvit}, color, textureSlot, textureArrayIndex}; // 4
                m_v[5] = {{r - w, t + w}, {uvir, uvit}, color, textureSlot, textureArrayIndex}; // 5
                m_v[6] = {{l + w, b - w}, {uvil, uvib}, color, textureSlot, textureArrayIndex}; // 6
                m_v[7] = {{r - w, b - w}, {uvir, uvib}, color, textureSlot, textureArrayIndex}; // 7

                m_i[0] = 0;
                m_i[1] = 4;
                m_i[2] = 2;
                m_i[3] = 2;
                m_i[4] = 4;
                m_i[5] = 6;

                m_i[0] = 0;
                m_i[1] = 1;
                m_i[2] = 4;
                m_i[3] = 4;
                m_i[4] = 1;
                m_i[5] = 5;

                m_i[0] = 5;
                m_i[1] = 1;
                m_i[2] = 7;
                m_i[3] = 7;
                m_i[4] = 1;
                m_i[5] = 3;

                m_i[0] = 6;
                m_i[1] = 7;
                m_i[2] = 2;
                m_i[3] = 2;
                m_i[4] = 7;
                m_i[5] = 3;
            }

            void SetUV(GUI_UV uvOuter, GUI_UV uvInner, float width)
            {
                float uvol = uvOuter.m_left;
                float uvot = uvOuter.m_top;
                float uvor = uvOuter.m_right;
                float uvob = uvOuter.m_bottom;

                float uvil = uvInner.m_left;
                float uvit = uvInner.m_top;
                float uvir = uvInner.m_right;
                float uvib = uvInner.m_bottom;

                m_v[0].uv = {uvol, uvot};
                m_v[1].uv = {uvor, uvot};
                m_v[2].uv = {uvol, uvob};
                m_v[3].uv = {uvor, uvob};
                m_v[4].uv = {uvil, uvit};
                m_v[5].uv = {uvir, uvit};
                m_v[6].uv = {uvil, uvib};
                m_v[7].uv = {uvir, uvib};
            }

            void SetColor(Math::CColorRGBA color)
            {
                for (int i = 0; i < 8; i++)
                    m_v[i].color = color;
            }

            void SetTexture(uint textureSlot, uint textureArrayIndex)
            {
                for (int i = 0; i < 8; i++)
                {
                    m_v[i].textureSlot = textureSlot;
                    m_v[i].textureArrayIndex = textureArrayIndex;
                }
            }
        };


        struct IGUIRenderContext::InnerBorder
        {
            Render::XYUV1C32DW2 m_v[16];
            uint                m_i[24];

            inline uint                 GetVertexCount() const { return sizeof(m_v) / sizeof(m_v[0]); }
            inline uint                 GetIndexCount() const { return sizeof(m_i) / sizeof(m_i[0]); }
            inline Render::XYUV1C32DW2* GetVB() { return m_v; }
            inline uint*                GetIB() { return m_i; }

            explicit InnerBorder() {}

            void SetUV(int side) {}

            void SetColor(int side) {}

            void SetTexture(int side, uint textureSlot, uint textureArrayIndex)
            {
                for (int i = 0; i < 4; i++)
                {
                    m_v[side + i].textureSlot = textureSlot;
                    m_v[side + i].textureArrayIndex = textureArrayIndex;
                }
            }
        };


        struct IGUIRenderContext::OuterBorder
        {
            Render::XYUV1C32DW2 m_v[16];
            uint                m_i[24];

            inline uint                 GetVertexCount() const { return sizeof(m_v) / sizeof(m_v[0]); }
            inline uint                 GetIndexCount() const { return sizeof(m_i) / sizeof(m_i[0]); }
            inline Render::XYUV1C32DW2* GetVB() { return m_v; }
            inline uint*                GetIB() { return m_i; }

            explicit OuterBorder() {}

            void SetUV(int side) {}

            void SetColor(int side) {}

            void SetTexture(int side, uint textureSlot, uint textureArrayIndex)
            {
                for (int i = 0; i < 4; i++)
                {
                    m_v[side + i].textureSlot = textureSlot;
                    m_v[side + i].textureArrayIndex = textureArrayIndex;
                }
            }
        };


        struct IGUIRenderContext::TriPatch
        {
            Render::XYUV1C32DW2 m_v[12];
            uint                m_i[24];

            inline uint                 GetVertexCount() const { return sizeof(m_v) / sizeof(m_v[0]); }
            inline uint                 GetIndexCount() const { return sizeof(m_i) / sizeof(m_i[0]); }
            inline Render::XYUV1C32DW2* GetVB() { return m_v; }
            inline uint*                GetIB() { return m_i; }
        };


        struct IGUIRenderContext::SixPatch
        {
            Render::XYUV1C32DW2 m_v[24];
            uint                m_i[24];

            inline uint                 GetVertexCount() const { return sizeof(m_v) / sizeof(m_v[0]); }
            inline uint                 GetIndexCount() const { return sizeof(m_i) / sizeof(m_i[0]); }
            inline Render::XYUV1C32DW2* GetVB() { return m_v; }
            inline uint*                GetIB() { return m_i; }
        };


        struct IGUIRenderContext::NinePatch
        {
            Render::XYUV1C32DW2 m_v[36];
            uint                m_i[24];

            inline uint                 GetVertexCount() const { return sizeof(m_v) / sizeof(m_v[0]); }
            inline uint                 GetIndexCount() const { return sizeof(m_i) / sizeof(m_i[0]); }
            inline Render::XYUV1C32DW2* GetVB() { return m_v; }
            inline uint*                GetIB() { return m_i; }
        };


        inline void IGUIRenderContext::DrawGeometry(SolidRect& r)
        {
            return DrawGeometry(r.GetVertexCount(), r.GetVB(), r.GetIndexCount(), r.GetIB());
        }


        inline void IGUIRenderContext::DrawGeometry(Rect& sr)
        {
            return DrawGeometry(sr.GetVertexCount(), sr.GetVB(), sr.GetIndexCount(), sr.GetIB());
        }


        inline void IGUIRenderContext::DrawGeometry(InnerBorder& ib)
        {
            return DrawGeometry(ib.GetVertexCount(), ib.GetVB(), ib.GetIndexCount(), ib.GetIB());
        }


        inline void IGUIRenderContext::DrawGeometry(OuterBorder& ob)
        {
            return DrawGeometry(ob.GetVertexCount(), ob.GetVB(), ob.GetIndexCount(), ob.GetIB());
        }


        inline void IGUIRenderContext::DrawGeometry(TriPatch& tp)
        {
            return DrawGeometry(tp.GetVertexCount(), tp.GetVB(), tp.GetIndexCount(), tp.GetIB());
        }


        inline void IGUIRenderContext::DrawGeometry(SixPatch& sp)
        {
            return DrawGeometry(sp.GetVertexCount(), sp.GetVB(), sp.GetIndexCount(), sp.GetIB());
        }


        inline void IGUIRenderContext::DrawGeometry(NinePatch& np)
        {
            return DrawGeometry(np.GetVertexCount(), np.GetVB(), np.GetIndexCount(), np.GetIB());
        }
    } // namespace GUI
} // namespace krystallic
