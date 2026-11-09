/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 05.01.2026
*/


#pragma once


#include "simd_library.h"
#include "simd_registers_SSE42.h"
#include "constants.h"
#include <cmath>


namespace krystallic
{
    namespace SIMDMath
    {
      inline R128x4F  __vectorcall Mat44Identity()
      {
          return R128x4F_Set(1.f, 0.f, 0.f, 0.f,
                             0.f, 1.f, 0.f, 0.f,
                             0.f, 0.f, 1.f, 0.f,
                             0.f, 0.f, 0.f, 1.f);
      }

      inline R128x1F __vectorcall Mat33Determinant(R128x4F mat44)
      {
          R128x1F row1, row2, row3, row4, temp1, temp2;
          row1.v1 = mat44.v1;
          row2.v1 = mat44.v2;
          row3.v1 = mat44.v3;
          row4.v1 = mat44.v4;

          //11 12 13 14
          //22 23 24 21
          //33 34 31 32
          //44 41 42 43

          row2 = VecPermute<1, 2, 3, 0>(row2);
          row3 = VecPermute<2, 3, 0, 1>(row3);
          row4 = VecPermute<3, 0, 1, 2>(row4);

          temp1 = VecMul(row1, row2);
          temp1 = VecMul(temp1, row3);
          temp1 = VecDP<0xFF>(temp1, row4);

          //reset
          row1.v1 = mat44.v1;
          row2.v1 = mat44.v2;
          row3.v1 = mat44.v3;
          row4.v1 = mat44.v4;

          // 11 12 13 14
          // 24 21 22 23
          // 33 34 31 32
          // 42 43 44 41

          row2 = VecPermute<3, 0, 1, 2>(row2);
          row3 = VecPermute<2, 3, 0, 1>(row3);
          row4 = VecPermute<1, 2, 3, 0>(row4);

          temp2 = VecMul(row1, row2);
          temp2 = VecMul(temp2, row3);
          temp2 = VecDP<0xFF>(temp2, row4);

          return VecSub(temp1, temp2);
      }

      inline R128x4F __vectorcall Mat44Transpose(R128x4F mat44)
      {
          R128x1F row1, row2, row3, row4, temp1, temp2, temp3, temp4;
          row1.v1 = mat44.v1;
          row2.v1 = mat44.v2;
          row3.v1 = mat44.v3;
          row4.v1 = mat44.v4;

          temp1 = VecMergeLow(row1, row2);//1.x, 2.x, 1.y, 2.y
          temp2 = VecMergeLow(row3, row4);//3.x, 4.x, 3.y, 4.y
          temp3 = VecMergeHigh(row1, row2); // 1.z, 2.z, 1.w, 2.w
          temp4 = VecMergeHigh(row3, row4); //3.z, 4.z, 3.w, 4.w

          R128x1F resMatRow1,resMatRow2,resMatRow3,resMatRow4;

          resMatRow1 = VecShuffle<0, 1, 0, 1>(temp1, temp2);
          resMatRow2 = VecShuffle<2, 3, 2, 3>(temp1, temp2);
          resMatRow3 = VecShuffle<0, 1, 0, 1>(temp3, temp4);
          resMatRow4 = VecShuffle<2, 3, 2, 3>(temp3, temp4);

          R128x4F result;
          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline R128x4F __vectorcall Mat44Inverse(R128x4F mat44)
      {
          R128x1F row1, row2, row3, row4;
          row1.v1 = mat44.v1;
          row2.v1 = mat44.v2;
          row3.v1 = mat44.v3;
          row4.v1 = mat44.v4;

          R128x1F c0, c1, c2, c3, det;

          c0 = Vec4Cross(row2, row3, row4);
          c1 = Vec4Cross(row3, row4, row1);
          c2 = Vec4Cross(row4, row1, row2);
          c3 = Vec4Cross(row1, row2, row3);

          det = VecDP<0xFF>(row1,c0);
          if(VecMask(VecInBounds(det, R128x1F_One(1.5e-5f))) != 0xf)
          {
              return R128x4F_Zero();
          }

          R128x4F result;

          result.v1 = VecDiv(c0, det).v1;
          result.v2 = VecDiv(c1, det).v1;
          result.v3 = VecDiv(c2, det).v1;
          result.v4 = VecDiv(c3, det).v1;

          return Mat44Transpose(result);
      }

      inline R128x4F __vectorcall Mat44Multiply(R128x4F a, R128x4F b)
      {
          R128x1F ARow1, ARow2, ARow3, ARow4, BRow1, BRow2, BRow3, BRow4, rowElem1, rowElem2, rowElem3, rowElem4;

          ARow1.v1 = a.v1;
          ARow2.v1 = a.v2;
          ARow3.v1 = a.v3;
          ARow4.v1 = a.v4;

          BRow1.v1 = b.v1;
          BRow2.v1 = b.v2;
          BRow3.v1 = b.v3;
          BRow4.v1 = b.v4;

          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;

          rowElem1 = VecPermute<0,0,0,0>(ARow1);
          rowElem2 = VecPermute<1,1,1,1>(ARow1);
          rowElem3 = VecPermute<2,2,2,2>(ARow1);
          rowElem4 = VecPermute<3,3,3,3>(ARow1);

          resMatRow1 = VecMul(rowElem1, BRow1);
          resMatRow1 = VecFMAdd(rowElem2, BRow2, resMatRow1);
          resMatRow1 = VecFMAdd(rowElem3, BRow3, resMatRow1);
          resMatRow1 = VecFMAdd(rowElem4, BRow4, resMatRow1);

          rowElem1 = VecPermute<0,0,0,0>(ARow2);
          rowElem2 = VecPermute<1,1,1,1>(ARow2);
          rowElem3 = VecPermute<2,2,2,2>(ARow2);
          rowElem4 = VecPermute<3,3,3,3>(ARow2);

          resMatRow2 = VecMul(rowElem1, BRow1);
          resMatRow2 = VecFMAdd(rowElem2, BRow2, resMatRow2);
          resMatRow2 = VecFMAdd(rowElem3, BRow3, resMatRow2);
          resMatRow2 = VecFMAdd(rowElem4, BRow4, resMatRow2);

          rowElem1 = VecPermute<0,0,0,0>(ARow3);
          rowElem2 = VecPermute<1,1,1,1>(ARow3);
          rowElem3 = VecPermute<2,2,2,2>(ARow3);
          rowElem4 = VecPermute<3,3,3,3>(ARow3);

          resMatRow3 = VecMul(rowElem1, BRow1);
          resMatRow3 = VecFMAdd(rowElem2, BRow2, resMatRow3);
          resMatRow3 = VecFMAdd(rowElem3, BRow3, resMatRow3);
          resMatRow3 = VecFMAdd(rowElem4, BRow4, resMatRow3);

          rowElem1 = VecPermute<0,0,0,0>(ARow4);
          rowElem2 = VecPermute<1,1,1,1>(ARow4);
          rowElem3 = VecPermute<2,2,2,2>(ARow4);
          rowElem4 = VecPermute<3,3,3,3>(ARow4);

          resMatRow4 = VecMul(rowElem1, BRow1);
          resMatRow4 = VecFMAdd(rowElem2, BRow2, resMatRow4);
          resMatRow4 = VecFMAdd(rowElem3, BRow3, resMatRow4);
          resMatRow4 = VecFMAdd(rowElem4, BRow4, resMatRow4);

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline R128x4F __vectorcall Mat44MultiplyTrasnpose(R128x4F a, R128x4F b)
      {
          return Mat44Transpose(Mat44Multiply(a, b));
      }

      inline R128x4F __vectorcall Mat44Set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
      {
          return R128x4F_Set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
      }

      inline  R128x4F  __vectorcall Mat44LookAtLH(R128x1F eyePos, R128x1F focusPos, R128x1F upDir)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = Vec3Normalize(VecSub(focusPos, eyePos));
          temp2 = Vec3Normalize(Vec3Cross(upDir, temp1));
          temp3 = Vec3Cross(temp1, temp2);

          R128x3F matrix;

          matrix.v1 = temp1.v1;
          matrix.v2 = temp2.v1;
          matrix.v3 = temp3.v1;

          matrix = Mat33Transpose(matrix);

          temp1 = Vec3Dot(temp2, eyePos);
          temp2 = Vec3Dot(temp3, eyePos);
          temp3 = Vec3Dot(temp1, eyePos);

          temp3 = R128x1F_Set(VecGetX(temp1), VecGetX(temp2), VecGetX(temp3), -1.f);

          temp3 = VecNeg(temp3);

          R128x4F resMatrix;

          resMatrix.v1 = matrix.v1;
          resMatrix.v2 = matrix.v2;
          resMatrix.v3 = matrix.v3;
          resMatrix.v4 = temp3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44LookAtRH(R128x1F eyePos, R128x1F focusPos, R128x1F upDir)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = Vec3Normalize(VecSub(eyePos, focusPos));
          temp2 = Vec3Normalize(Vec3Cross(upDir, temp1));
          temp3 = Vec3Cross(temp1, temp2);

          R128x3F matrix;

          matrix.v1 = temp1.v1;
          matrix.v2 = temp2.v1;
          matrix.v3 = temp3.v1;

          matrix = Mat33Transpose(matrix);

          temp1 = Vec3Dot(temp2, eyePos);
          temp2 = Vec3Dot(temp3, eyePos);
          temp3 = Vec3Dot(temp1, eyePos);

          temp3 = R128x1F_Set(VecGetX(temp1), VecGetX(temp2), VecGetX(temp3), -1.f);

          temp3 = VecNeg(temp3);

          R128x4F resMatrix;

          resMatrix.v1 = matrix.v1;
          resMatrix.v2 = matrix.v2;
          resMatrix.v3 = matrix.v3;
          resMatrix.v4 = temp3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44LookToLH(R128x1F eyePos, R128x1F eyeDir, R128x1F upDir)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = Vec3Normalize(eyeDir);
          temp2 = Vec3Normalize(Vec3Cross(upDir, temp1));
          temp3 = Vec3Cross(temp1, temp2);

          R128x3F matrix;

          matrix.v1 = temp1.v1;
          matrix.v2 = temp2.v1;
          matrix.v3 = temp3.v1;

          matrix = Mat33Transpose(matrix);

          temp1 = Vec3Dot(temp2, eyePos);
          temp2 = Vec3Dot(temp3, eyePos);
          temp3 = Vec3Dot(temp1, eyePos);

          temp3 = R128x1F_Set(VecGetX(temp1), VecGetX(temp2), VecGetX(temp3), -1.f);

          temp3 = VecNeg(temp3);

          R128x4F resMatrix;

          resMatrix.v1 = matrix.v1;
          resMatrix.v2 = matrix.v2;
          resMatrix.v3 = matrix.v3;
          resMatrix.v4 = temp3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44LookToRH(R128x1F eyePos, R128x1F eyeDir, R128x1F upDir)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = Vec3Normalize(VecNeg(eyeDir));
          temp2 = Vec3Normalize(Vec3Cross(upDir, temp1));
          temp3 = Vec3Cross(temp1, temp2);

          R128x3F matrix;

          matrix.v1 = temp1.v1;
          matrix.v2 = temp2.v1;
          matrix.v3 = temp3.v1;

          matrix = Mat33Transpose(matrix);

          temp1 = Vec3Dot(temp2, eyePos);
          temp2 = Vec3Dot(temp3, eyePos);
          temp3 = Vec3Dot(temp1, eyePos);

          temp3 = R128x1F_Set(VecGetX(temp1), VecGetX(temp2), VecGetX(temp3), -1.f);

          temp3 = VecNeg(temp3);

          R128x4F resMatrix;

          resMatrix.v1 = matrix.v1;
          resMatrix.v2 = matrix.v2;
          resMatrix.v3 = matrix.v3;
          resMatrix.v4 = temp3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = R128x1F_Set(2.f, 2.f, 1.f, nearZ);
          temp2 = R128x1F_Set(viewWidth, viewHeight, farZ, nearZ);
          temp3 = R128x1F_Set(0.f, 0.f, nearZ, farZ);
          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);

          R128x4F resMatrix;

          resMatrix = Mat44ScaleFromVector(temp1);
          temp1 = VecMergeHigh(temp1, temp1);// z, z, w, w
          temp2 = VecBlend<0b1011>(temp1, g_IdentityR3);
          resMatrix.v4 = temp2.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44OrthographicRH(float viewWidth, float viewHeight, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = R128x1F_Set(2.f, 2.f, 1.f, nearZ);
          temp2 = R128x1F_Set(viewWidth, viewHeight, nearZ, nearZ);
          temp3 = R128x1F_Set(0.f, 0.f, farZ, farZ);
          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);

          R128x4F resMatrix;

          resMatrix = Mat44ScaleFromVector(temp1);
          temp1 = VecMergeHigh(temp1, temp1);// z, z, w, w
          temp2 = VecBlend<0b1011>(temp1, g_IdentityR3);
          resMatrix.v4 = temp2.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44OrthographicOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;
          R128x4F resMatrix;

          temp1 = R128x1F_Set(2.f, 2.f, 1.f, 0.f);
          temp2 = R128x1F_Set(viewRight, viewTop, farZ, 2.f); // last element = 2.f is needed so as not to divide by 0 after sub
          temp3 = R128x1F_Set(viewLeft, viewBottom, nearZ, 1.f);

          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);

          resMatrix = Mat44ScaleFromVector(temp1);

          temp1 = R128x1F_Set(viewLeft, viewTop, nearZ, 1.f);
          temp2 = R128x1F_Set(viewRight, viewBottom, 0.f, 0.f);
          temp1 = VecAdd(temp1, temp2);

          temp2 = R128x1F_Set(viewLeft, viewBottom, nearZ, 1.f);
          temp3 = R128x1F_Set(viewRight, viewTop, farZ, 0.f);
          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);
          resMatrix.v4 = temp1.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44OrthographicOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;
          R128x4F resMatrix;

          temp1 = R128x1F_Set(2.f, 2.f, 1.f, 0.f);
          temp2 = R128x1F_Set(viewRight, viewTop, nearZ, 2.f); // last element = 2.f is needed so as not to divide by 0 after sub      }
          temp3 = R128x1F_Set(viewLeft, viewBottom, farZ, 1.f);

          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);

          resMatrix = Mat44ScaleFromVector(temp1);

          temp1 = R128x1F_Set(viewLeft, viewTop, nearZ, 1.f);
          temp2 = R128x1F_Set(viewRight, viewBottom, 0.f, 0.f);
          temp1 = VecAdd(temp1, temp2);

          temp2 = R128x1F_Set(viewLeft, viewBottom, nearZ, 1.f);
          temp3 = R128x1F_Set(viewRight, viewTop, farZ, 0.f);
          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);
          resMatrix.v4 = temp1.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveLH(float viewWidth, float viewHeight, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = R128x1F_Set(2.f, 2.f, 1.f, nearZ);
          temp2 = R128x1F_Set(nearZ, nearZ, farZ, farZ);
          temp1 = VecMul(temp1, temp2);

          temp2 = R128x1F_Set(viewWidth, viewHeight, farZ, nearZ);
          temp3 = R128x1F_Set(0.f, 0.f, nearZ, farZ);
          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);

          R128x4F resMatrix;

          resMatrix.v1 = VecBlend<0b1110>(temp1, g_Zero).v1;
          resMatrix.v2 = VecBlend<0b1101>(temp1, g_Zero).v1;
          resMatrix.v3 = VecBlend<0b1011>(temp1, g_IdentityR2).v1;
          temp1 = VecPermute<3, 3, 3, 3>(temp1);
          resMatrix.v4 = VecBlend<0b1011>(temp1, g_Zero).v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveRH(float viewWidth, float viewHeight, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = R128x1F_Set(2.f, 2.f, 1.f, nearZ);
          temp2 = R128x1F_Set(nearZ, nearZ, farZ, farZ);
          temp1 = VecMul(temp1, temp2);

          temp2 = R128x1F_Set(viewWidth, viewHeight, nearZ, nearZ);
          temp3 = R128x1F_Set(0.f, 0.f, farZ, farZ);
          temp2 = VecSub(temp2, temp3);
          temp1 = VecDiv(temp1, temp2);

          R128x4F resMatrix;

          resMatrix.v1 = VecBlend<0b1110>(temp1, g_Zero).v1;
          resMatrix.v2 = VecBlend<0b1101>(temp1, g_Zero).v1;
          resMatrix.v3 = VecBlend<0b1011>(temp1, g_IdentityR2).v1;
          temp1 = VecPermute<3, 3, 3, 3>(temp1);
          resMatrix.v4 = VecBlend<0b1011>(temp1, g_Zero).v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveFovXLH(float fovAngleX, float aspectRatio, float nearZ, float farZ)
      {
          R128x1F scaleX, scaleY, range, vecFovAngleX, tan, vecAspectRatio, vecNearZ, vecFarZ, temp1; vecAspectRatio = R128x1F_One(aspectRatio);
          vecNearZ = R128x1F_One(nearZ);
          vecFarZ = R128x1F_One(farZ);

          vecFovAngleX = VecDiv(R128x1F_One(fovAngleX), R128x1F_One(2.f));

          tan = VecTan(vecFovAngleX);

          scaleX = VecRcp(tan);

          scaleY = VecMul(scaleX, vecAspectRatio);

          range = VecDiv(vecFarZ, VecSub(vecFarZ, vecNearZ));

          temp1 = VecMul(VecNeg(vecNearZ), range);

          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          R128x4F resMatrix;

          resMatRow1 = VecBlend<0b0001>(g_Zero, scaleX);
          resMatRow2 = VecBlend<0b0010>(g_Zero, scaleY);
          resMatRow3 = VecBlend<0b0100>(g_IdentityR3, range);
          resMatRow4 = VecBlend<0b0100>(g_Zero, temp1);

          resMatrix.v1 = resMatRow1.v1;
          resMatrix.v2 = resMatRow2.v1;
          resMatrix.v3 = resMatRow3.v1;
          resMatrix.v4 = resMatRow4.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveFovXRH(float fovAngleX, float aspectRatio, float nearZ, float farZ)
      {
          R128x1F scaleX, scaleY, range, vecFovAngleX, tan, vecAspectRatio, vecNearZ, vecFarZ, temp1;
          vecAspectRatio = R128x1F_One(aspectRatio);
          vecNearZ = R128x1F_One(nearZ);
          vecFarZ = R128x1F_One(farZ);

          vecFovAngleX = VecDiv(R128x1F_One(fovAngleX), R128x1F_One(2.f));

          tan = VecTan(vecFovAngleX);

          scaleX = VecRcp(tan);

          scaleY = VecMul(scaleX, vecAspectRatio);

          range = VecDiv(vecFarZ, VecSub(vecNearZ, vecFarZ));

          temp1 = VecMul(vecNearZ, range);

          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          R128x4F resMatrix;

          resMatRow1 = VecBlend<0b0001>(g_Zero, scaleX);
          resMatRow2 = VecBlend<0b0010>(g_Zero, scaleY);
          resMatRow3 = VecBlend<0b0100>(g_NegIdentityR3, range);
          resMatRow4 = VecBlend<0b0100>(g_Zero, temp1);

          resMatrix.v1 = resMatRow1.v1;
          resMatrix.v2 = resMatRow2.v1;
          resMatrix.v3 = resMatRow3.v1;
          resMatrix.v4 = resMatRow4.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveFovYLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
      {
          R128x1F scaleX, scaleY, range, vecFovAngleY, tan, vecAspectRatio, vecNearZ, vecFarZ, temp1; vecAspectRatio = R128x1F_One(aspectRatio);
          vecNearZ = R128x1F_One(nearZ);
          vecFarZ = R128x1F_One(farZ);

          vecFovAngleY = VecDiv(R128x1F_One(fovAngleY), R128x1F_One(2.f));

          tan = VecTan(vecFovAngleY);

          scaleY = VecRcp(tan);

          scaleX = VecDiv(scaleY, vecAspectRatio);

          range = VecDiv(vecFarZ, VecSub(vecFarZ, vecNearZ));

          temp1 = VecMul(VecNeg(vecNearZ), range);

          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          R128x4F resMatrix;

          resMatRow1 = VecBlend<0b0001>(g_Zero, scaleX);
          resMatRow2 = VecBlend<0b0010>(g_Zero, scaleY);
          resMatRow3 = VecBlend<0b0100>(g_IdentityR3, range);
          resMatRow4 = VecBlend<0b0100>(g_Zero, temp1);

          resMatrix.v1 = resMatRow1.v1;
          resMatrix.v2 = resMatRow2.v1;
          resMatrix.v3 = resMatRow3.v1;
          resMatrix.v4 = resMatRow4.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveFovYRH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
      {
          R128x1F scaleX, scaleY, range, vecFovAngleY, tan, vecAspectRatio, vecNearZ, vecFarZ, temp1;
          vecAspectRatio = R128x1F_One(aspectRatio);
          vecNearZ = R128x1F_One(nearZ);
          vecFarZ = R128x1F_One(farZ);

          vecFovAngleY = VecDiv(R128x1F_One(fovAngleY), R128x1F_One(2.f));

          tan = VecTan(vecFovAngleY);

          scaleY = VecRcp(tan);

          scaleX = VecDiv(scaleY, vecAspectRatio);

          range = VecDiv(vecFarZ, VecSub(vecNearZ, vecFarZ));

          temp1 = VecMul(vecNearZ, range);

          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          R128x4F resMatrix;

          resMatRow1 = VecBlend<0b0001>(g_Zero, scaleX);
          resMatRow2 = VecBlend<0b0010>(g_Zero, scaleY);
          resMatRow3 = VecBlend<0b0100>(g_NegIdentityR3, range);
          resMatRow4 = VecBlend<0b0100>(g_Zero, temp1);

          resMatrix.v1 = resMatRow1.v1;
          resMatrix.v2 = resMatRow2.v1;
          resMatrix.v3 = resMatRow3.v1;
          resMatrix.v4 = resMatRow4.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = R128x1F_Set(2.f, 2.f, nearZ, 0.f);
          temp2 = R128x1F_Set(nearZ, nearZ, farZ, 0.f);
          temp1 = VecMul(temp1, temp2);


          temp2 = R128x1F_Set(viewRight, viewTop, nearZ, 2.f);// last element = 2.f is needed so as not to divide by 0 after sub
          temp3 = R128x1F_Set(viewLeft, viewBottom, farZ, 1.f);
          temp2 = VecSub(temp2, temp3);

          temp1 = VecDiv(temp1, temp2);

          R128x4F resMatrix;

          resMatrix.v1 = VecBlend<0b1110>(temp1, g_Zero).v1;
          resMatrix.v2 = VecBlend<0b1101>(temp1, g_Zero).v1;
          resMatrix.v4 = VecBlend<0b1011>(temp1, g_Zero).v1;

          temp1 =R128x1F_Set(viewLeft, viewTop, farZ, 1.f);
          temp2 = R128x1F_Set(viewRight, viewBottom, 0.f, 0.f);
          temp1 = VecAdd(temp1, temp2);

          temp2 = R128x1F_Set(viewLeft, viewBottom, farZ, 1.f);
          temp3 = R128x1F_Set(viewRight, viewTop, nearZ, 0.f);
          temp2 = VecSub(temp2, temp3);

          temp1 = VecDiv(temp1, temp2);

          resMatrix.v3 = temp1.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44PerspectiveOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
      {
          R128x1F temp1, temp2, temp3;

          temp1 = R128x1F_Set(2.f, 2.f, nearZ, 0.f);
          temp2 = R128x1F_Set(nearZ, nearZ, farZ, 0.f);
          temp1 = VecMul(temp1, temp2);


          temp2 = R128x1F_Set(viewRight, viewTop, nearZ, 2.f);// last element = 2.f is needed so as not to divide by 0 after sub
          temp3 = R128x1F_Set(viewLeft, viewBottom, farZ, 1.f);
          temp2 = VecSub(temp2, temp3);

          temp1 = VecDiv(temp1, temp2);

          R128x4F resMatrix;

          resMatrix.v1 = VecBlend<0b1110>(temp1, g_Zero).v1;
          resMatrix.v2 = VecBlend<0b1101>(temp1, g_Zero).v1;
          resMatrix.v4 = VecBlend<0b1011>(temp1, g_Zero).v1;

          temp1 = R128x1F_Set(viewLeft, viewTop, farZ, 1.f);
          temp2 = R128x1F_Set(viewRight, viewBottom, 0.f, 0.f);
          temp1 = VecAdd(temp1, temp2);

          temp2 = R128x1F_Set(viewLeft, viewBottom, farZ, 1.f);
          temp3 = R128x1F_Set(viewRight, viewTop, nearZ, 0.f);
          temp2 = VecSub(temp3, temp2);

          temp1 = VecDiv(temp1, temp2);

          resMatrix.v3 = temp1.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44RotationX(float angle)
      {
          R128x1F sin, cos, temp1;
          VecSinCos(R128x1F_One(angle), &sin, &cos);

          temp1 = VecShuffle<0, 0, 0, 0>(sin, cos);
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;

          resMatRow1 = g_IdentityR0;
          resMatRow3 = VecBlend<0b1001>(temp1, g_Zero);
          resMatRow2 = VecPermute<0, 2, 1, 3>(resMatRow3);
          resMatRow2 = VecXor(resMatRow2, Cast128x1IF(g_FlipZ));
          resMatRow4 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline  R128x4F  __vectorcall Mat44RotationY(float angle)
      {
          R128x1F sin, cos, temp1;
          VecSinCos(R128x1F_One(angle), &sin, &cos);

          temp1 = VecShuffle<0, 0, 0, 0>(cos, sin);
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;

          resMatRow2 = g_IdentityR1;
          resMatRow1 = VecBlend<0b0101>(temp1, g_Zero);
          resMatRow3 = VecPermute<2, 1, 0, 3>(resMatRow1);
          resMatRow3 = VecXor(resMatRow3, Cast128x1IF(g_FlipX));
          resMatRow4 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline  R128x4F  __vectorcall Mat44RotationZ(float angle)
      {
          R128x1F sin, cos, temp1;
          VecSinCos(R128x1F_One(angle), &sin, &cos);

          temp1 = VecMergeHigh(sin, cos);
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;

          resMatRow3 = g_IdentityR2;
          resMatRow2 = VecShuffle<0,1,0,0>(temp1, g_Zero);
          resMatRow1 = VecPermute<1, 0, 2, 3>(resMatRow2);
          resMatRow1 = VecXor(resMatRow1, Cast128x1IF(g_FlipY));
          resMatRow4 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline  R128x4F  __vectorcall Mat44RotationAxis(R128x1F axis, float angle)
      {
          R128x1F scaleVec, row1, row2, row3, sin, cos, oneMinusCos, temp1, temp2, temp3, temp4, temp5; axis = Vec3Normalize(axis);

          axis = VecBlend<0b1000>(axis, g_Zero);

          VecSinCos(R128x1F_One(angle), &sin, &cos);

          oneMinusCos = VecSub(g_One, cos);

          scaleVec = VecMul(axis, axis);
          scaleVec = VecFMAdd(scaleVec, oneMinusCos, cos);

          temp1 = VecPermute<0, 0, 1, 3>(axis);
          temp2 = VecPermute<1, 2, 2, 3>(axis);

          temp1 = VecMul(temp1, temp2);//xy, xz, yz, ?
          temp1 = VecMul(temp1, oneMinusCos);

          temp2 = VecMul(axis, sin);

          // вычисления готовы, нужно раставить по нужным местам

          temp3 = VecPermute<1, 0, 2, 3>(temp1);
          temp4 = VecPermute<1, 2, 0, 3>(temp2);
          temp3 = VecAdd(temp3, temp4);

          temp4 = VecPermute<0, 2, 1, 3>(temp1);
          temp5 = VecPermute<2, 1, 0, 3>(temp2);
          temp4 = VecSub(temp4, temp5);

          row1 = VecBlend<0b0001>(g_Zero, scaleVec);
          row1 = VecBlend<0b0010>(row1, temp3);
          row1 = VecBlend<0b0100>(row1, temp4);

          row2 = VecBlend<0b0001>(row2, temp4);
          row2 = VecBlend<0b0010>(g_Zero, scaleVec);
          row2 = VecBlend<0b0100>(row2, temp3);

          row3 = VecBlend<0b0001>(row3, temp3);
          row3 = VecBlend<0b0010>(row3, temp4);
          row3 = VecBlend<0b0100>(g_Zero, scaleVec);

          R128x4F resMatrix;

          resMatrix.v1 = row1.v1;
          resMatrix.v2 = row2.v1;
          resMatrix.v3 = row3.v1;
          resMatrix.v4 = g_IdentityR3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44RotationQuaternion(R128x1F quat)
      {//directx 55 latency, this code also 55
          /*
              m00 = 1 - 2(y² + z²)
              m01 = 2(xy + zw)
              m02 = 2(xz - yw)

              m10 = 2(xy - zw)
              m11 = 1 - 2(x² + z²)
              m12 = 2(yz + xw)

              m20 = 2(xz + yw)
              m21 = 2(yz - xw)
              m22 = 1 - 2(x² + y²)

              m03 = 0
              m13 = 0
              m23 = 0

              m30 = 0
              m31 = 0
              m32 = 0
              m33 = 1
           */
          R128x1F quatSquare, vec3quat, temp1, temp2, temp3, temp4, temp5, temp6;

          vec3quat = VecBlend<0b1000>(quat, g_Zero);
          quatSquare = VecMul(quat, vec3quat);
          temp1 = VecPermute<1, 0, 0, 3>(quatSquare);
          temp2 = VecPermute<2, 2, 1, 3>(quatSquare);
          temp1 = VecAdd(temp1, temp2);
          temp1 = VecMul(g_Two, temp1);
          temp1 = VecSub(g_One3, temp1);

          temp2 = VecPermute<0, 0, 1, 3>(vec3quat);
          temp3 = VecPermute<1, 2, 2, 3>(vec3quat);
          temp2 = VecMul(temp2, temp3); // xy, xz, yz, 0
          temp3 = VecPermute<2, 1, 0, 3>(vec3quat);
          temp4 = VecPermute<3, 3, 3, 3>(quat);
          temp3 = VecMul(temp3, temp4); // zw, yw, xw, 0

          temp4 = VecPermute<0, 0, 1, 3>(temp2);
          temp5 = VecPermute<0, 0, 1, 3>(temp3);
          temp5 = VecXor(temp5, Cast128x1IF(g_FlipXZ));
          temp4 = VecAdd(temp4, temp5);
          temp4 = VecMul(temp4, g_Two);

          temp5 = VecPermute<1, 2, 2, 3>(temp2);
          temp6 = VecPermute<1, 2, 2, 3>(temp3);
          temp6 = VecXor(temp6, Cast128x1IF(g_FlipY));
          temp5 = VecAdd(temp5, temp6);
          temp5 = VecMul(temp5, g_Two);

          R128x4F resMatrix;
          R128x1F resMatRow1,resMatRow2, resMatRow3, resMatRow4;

          resMatRow1 = VecBlend<0b0001>(temp4, temp1);
          resMatRow2 = VecBlend<0b1101>(temp1, temp4);
          resMatRow2 = VecBlend<0b1100>(resMatRow2, temp5);
          resMatRow3 = VecBlend<0b0100>(temp5, temp1);
          resMatRow4 = g_IdentityR3;

          resMatrix.v1 = resMatRow1.v1;
          resMatrix.v2 = resMatRow2.v1;
          resMatrix.v3 = resMatRow3.v1;
          resMatrix.v4 = resMatRow4.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44RotationPitchYawRoll(float pitch, float yaw, float roll)
      {
          return Mat44RotationPitchYawRollFromVector(R128x1F_Set(pitch, yaw, roll, 0.f));
      }

      inline  R128x4F  __vectorcall Mat44RotationPitchYawRollFromVector(R128x1F pitchYawRoll)
      {
          /*
           m00 = cr*cy + sr*sp*sy
           m01 = sr*cp
           m02 = sr*sp*cy - cr*sy

           m10 = cr*sp*sy - sr*cy
           m11 = cr*cp
           m12 = sr*sy + cr*sp*cy

           m20 = cp*sy
           m21 = -sp
           m22 = cp*cy
           */

          pitchYawRoll = VecBlend<0b1000>(pitchYawRoll, g_Zero);

          R128x1F sinPYR, cosPYR, mergeLow, mergeHigh, temp1, temp2, temp3, temp4, spSplat, cpSplat;

          VecSinCos(pitchYawRoll, &sinPYR, &cosPYR);

          mergeLow = VecMergeLow(sinPYR, cosPYR); //S_p, C_p, S_y, C_y
          mergeHigh = VecMergeHigh(sinPYR, cosPYR); // S_r, C_r, 0, 0

          // a = [C_r, C_r, S_r, S_r] *
          // [C_y, S_y, C_y, S_y]
          // b = permute(a, [3, 2, 1, 0])
          // a = Xor(a, [0, -0, -0, 0])
          // a = fmadd(b, set1(S_p), a)
          // c = [S_r, 0, C_r, 0]*
          // [C_p, 0, C_p, 0]
          // d = [C_p, -1, C_p, 0] *
          // [S_y, S_p, C_y, 0]
          //
          // row0 = MergeLow(a, c)
          // row1 = MergeHigh(a, c)
          // row2 = d
          // row3 = identityR3

          temp1 = VecPermute<1, 1, 0, 0>(mergeHigh);
          temp2 = VecPermute<3, 2, 3, 2>(mergeLow);
          temp1 = VecMul(temp1, temp2);

          temp2 = VecPermute<3, 2, 1, 0>(temp1); // b
          temp1 = VecXor(temp1, R128x1F_Set(0.f, g_negZero, g_negZero, 0.f));

          spSplat = VecPermute<0, 0, 0, 0>(mergeLow);
          temp1 = VecFMAdd(temp2, spSplat, temp1); // a

          temp3 = VecPermute<0, 3, 1, 3>(mergeHigh);
          cpSplat = VecPermute<1, 1, 1, 1>(mergeLow);
          temp4 = VecMergeLow(cpSplat, g_Zero);
          temp2 = VecMul(temp3, temp4); // c
          temp3 = VecBlend<0b0010>(temp4, g_NegativeOne);
          temp4 = VecPermute<2, 0, 3, 0>(mergeLow);
          temp4 = VecBlend<0b1000>(temp4, g_Zero);
          temp3 = VecMul(temp3, temp4); // d

          R128x4F resMatrix;

          resMatrix.v1 = VecMergeLow(temp1, temp2).v1;
          resMatrix.v2 = VecMergeHigh(temp1, temp2).v1;
          resMatrix.v3 = temp3.v1;
          resMatrix.v4 = g_IdentityR3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44Scale(float scaleX, float scaleY, float scaleZ)
      {
          return Mat44ScaleFromVector(R128x1F_Set(scaleX, scaleY, scaleZ, 0.f));
      }

      inline  R128x4F  __vectorcall Mat44ScaleFromVector(R128x1F scale)
      {
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;

          resMatRow1 = VecBlend<0b1110>(scale, g_Zero);
          resMatRow2 = VecBlend<0b1101>(scale, g_Zero);
          resMatRow3 = VecBlend<0b1011>(scale, g_Zero);
          resMatRow4 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline  R128x4F  __vectorcall Mat44Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY)
      {
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          resMatRow1 = R128x1F_Set(1.f, skewXY, skewXZ, 0.f);
          resMatRow2 = R128x1F_Set(skewYX, 1.f, skewYZ, 0.f);
          resMatRow3 = R128x1F_Set(skewZX, skewZY, 1.f, 0.f);
          resMatRow4 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline  R128x4F  __vectorcall Mat44SkewFromVector(R128x1F skewX, R128x1F skewY, R128x1F skewZ)
      {
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          resMatRow1 = VecBlend<0b1001>(skewX, g_IdentityR0);
          resMatRow2 = VecBlend<0b0101>(skewY, g_IdentityR1);
          resMatRow3 = VecShuffle<0, 1, 0, 1>(skewZ, g_IdentityR0);
          resMatRow4 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline  R128x4F  __vectorcall Mat44Translation(float translateX, float translateY, float translateZ)
      {
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          resMatRow1 = R128x1F_Set(1.f, 0.f, 0.f, translateX);
          resMatRow2 = R128x1F_Set(0.f, 1.f, 0.f, translateY);
          resMatRow3 = R128x1F_Set(0.f, 0.f, 1.f, translateZ);
          resMatRow4 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }

      inline  R128x4F  __vectorcall Mat44TranslationFromVector(R128x1F translationVec3)
      {
          R128x1F resMatRow1, resMatRow2, resMatRow3, resMatRow4;
          resMatRow1 = VecShuffle<0, 1, 3, 0>(g_IdentityR0, translationVec3);
          resMatRow2 = VecShuffle<0, 1, 3, 1>(g_IdentityR1, translationVec3);
          resMatRow3 = VecBlend<0b0001>(g_IdentityR2, translationVec3);
          resMatRow3 = g_IdentityR3;

          R128x4F result;

          result.v1 = resMatRow1.v1;
          result.v2 = resMatRow2.v1;
          result.v3 = resMatRow3.v1;
          result.v4 = resMatRow4.v1;

          return result;
      }


      inline  R128x4F  __vectorcall Mat44AffineTransformation(R128x1F scale, R128x1F rotationOrigin, R128x1F rotationQuaternion, R128x1F translation)
      {
          R128x4F matrixScaling, matrixRotation, resMatrix;

          matrixScaling = Mat44ScaleFromVector(scale);
          rotationOrigin = VecBlend<0b1000>(rotationOrigin, g_Zero);
          matrixRotation = Mat44RotationQuaternion(rotationQuaternion);
          translation = VecBlend<0b1000>(translation, g_Zero);

          resMatrix = matrixScaling;

          R128x1F resMatRow3;

          resMatRow3.v1 = resMatrix.v4;
          resMatRow3 = VecSub(resMatRow3, rotationOrigin);
          resMatrix.v4 = resMatRow3.v1;

          resMatrix = Mat44Multiply(resMatrix, matrixRotation);

          resMatRow3.v1 = resMatrix.v4;
          resMatRow3 = VecAdd(resMatRow3, rotationOrigin);
          resMatRow3 = VecAdd(resMatRow3, translation);
          resMatrix.v4 = resMatRow3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44Transformation(R128x1F scaleOrigin, R128x1F scaleOrientationQuaternion, R128x1F scale, R128x1F rotationOrigin, R128x1F rotationQuaternion, R128x1F translation)
      {
          R128x1F negScaleOrigin;
          R128x4F matScaleOriginI, matScaleOrientation, matScaleOrientationT, matScaling, matRotation;

          negScaleOrigin = VecNeg(scaleOrigin);
          scaleOrigin = VecBlend<0b1000>(scaleOrigin, g_Zero);
          rotationOrigin = VecBlend<0b1000>(rotationOrigin, g_Zero);
          translation = VecBlend<0b1000>(translation, g_Zero);

          matScaleOriginI = Mat44TranslationFromVector(negScaleOrigin);
          matScaleOrientation = Mat44RotationQuaternion(scaleOrientationQuaternion);
          matScaleOrientationT = Mat44Transpose(matScaleOrientation);
          matScaling = Mat44ScaleFromVector(scale);
          matRotation =  Mat44RotationQuaternion(rotationQuaternion);

          R128x4F resMatrix;
          R128x1F resMatRow3;

          resMatrix = Mat44Multiply(matScaleOriginI, matScaleOrientationT);
          resMatrix = Mat44Multiply(resMatrix, matScaling);
          resMatrix = Mat44Multiply(resMatrix, matScaleOrientation);

          resMatRow3.v1 = resMatrix.v4;
          resMatRow3 = VecAdd(resMatRow3, scaleOrigin);
          resMatRow3 = VecSub(resMatRow3, rotationOrigin);
          resMatrix.v4 = resMatRow3.v1;

          resMatrix = Mat44Multiply(resMatrix, matRotation);

          resMatRow3.v1 = resMatrix.v4;
          resMatRow3 = VecAdd(resMatRow3, rotationOrigin);
          resMatRow3 = VecAdd(resMatRow3, translation);
          resMatrix.v4 = resMatRow3.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44AffineTransformation2D(R128x1F scale, R128x1F rotationOrigin, float rotationAngle, R128x1F translation)
      {
          R128x1F temp1, temp2, temp3;
          R128x4F resMatrix, rotationMatrix;
          temp1 = VecBlend<0b1100>(scale, g_One);
          resMatrix = Mat44ScaleFromVector(temp1);
          temp2 = VecBlend<0b1100>(rotationOrigin, g_One);

          rotationMatrix = Mat44RotationZ(rotationAngle);
          temp3 = VecBlend<0b1100>(translation, g_One);
          R128x1F matRow4;

          matRow4.v1 = resMatrix.v3;

          matRow4 = VecSub(matRow4, temp2);
          resMatrix.v3 = matRow4.v1;

          resMatrix = Mat44Multiply(resMatrix, rotationMatrix);
          matRow4.v1 = resMatrix.v3;
          matRow4 = VecAdd(matRow4, temp2);
          matRow4 = VecAdd(matRow4, temp3);
          resMatrix.v3 = matRow4.v1;

          return resMatrix;
      }

      inline  R128x4F  __vectorcall Mat44Transformation2D(R128x1F scaleOrigin, float scaleOrientation, R128x1F scale, R128x1F rotationOrigin, float rotationAngle, R128x1F translation)
      {
          R128x1F temp1;
          R128x4F resMat;

          resMat = Mat44Multiply(Mat44TranslationFromVector(VecNeg(scaleOrigin)), Mat44RotationZ(-scaleOrientation));
          resMat = Mat44Multiply(resMat, Mat44ScaleFromVector(scale));
          resMat = Mat44Multiply(resMat, Mat44RotationZ(scaleOrientation));

          temp1.v1 = resMat.v3;
          temp1 = VecAdd(temp1, VecSub(scaleOrigin, rotationOrigin));
          resMat.v4 = temp1.v1;

          resMat = Mat44Multiply(resMat, Mat44RotationZ(rotationAngle));

          temp1.v1 = resMat.v4;
          temp1 = VecAdd(temp1, VecAdd(rotationOrigin, translation));
          resMat.v4 = temp1.v1;

          return resMat;

      }
      inline bool __vectorcall Mat44IsIdentity(R128x4F mat44)
      {
          return Mat44Equal(mat44, Mat44Identity());
      }
      inline bool __vectorcall Mat44IsNaN(R128x4F mat44)
      {
          R128x1F row1, row2, row3, row4;
          row1.v1 = mat44.v1;
          row2.v1 = mat44.v2;
          row3.v1 = mat44.v3;
          row4.v1 = mat44.v4;

         return Vec4IsNaN(row1) or Vec4IsNaN(row2) or Vec4IsNaN(row3) or Vec4IsNaN(row4);
      }
      inline bool __vectorcall Mat44IsInf(R128x4F mat44)
      {
          R128x1F row1, row2, row3, row4;
          row1.v1 = mat44.v1;
          row2.v1 = mat44.v2;
          row3.v1 = mat44.v3;
          row4.v1 = mat44.v4;

         return Vec4IsInf(row1) or Vec4IsInf(row2) or Vec4IsInf(row3) or Vec4IsInf(row4);
      }
      inline bool __vectorcall Mat44Equal(R128x4F a, R128x4F b)
      {
          R128x1F aRow1, aRow2, aRow3, aRow4, bRow1, bRow2, bRow3, bRow4;
          aRow1.v1 = a.v1;
          aRow2.v1 = a.v2;
          aRow3.v1 = a.v3;
          aRow4.v1 = a.v4;
          bRow1.v1 = b.v1;
          bRow2.v1 = b.v2;
          bRow3.v1 = b.v3;
          bRow4.v1 = b.v4;

          aRow1 = VecEqual(aRow1, bRow1);
          aRow2 = VecEqual(aRow2, bRow2);
          aRow3 = VecEqual(aRow3, bRow3);
          aRow4 = VecEqual(aRow4, bRow4);

          aRow1 = VecAnd(aRow1, aRow2);
          aRow1 = VecAnd(aRow1, aRow3);
          aRow1 = VecAnd(aRow1, aRow4);

          return VecMask(aRow1) == 0xF;
      }
      inline bool __vectorcall Mat44NotEqual(R128x4F a, R128x4F b)
      {
          R128x1F aRow1, aRow2, aRow3, aRow4, bRow1, bRow2, bRow3, bRow4;
          aRow1.v1 = a.v1;
          aRow2.v1 = a.v2;
          aRow3.v1 = a.v3;
          aRow4.v1 = a.v4;
          bRow1.v1 = b.v1;
          bRow2.v1 = b.v2;
          bRow3.v1 = b.v3;
          bRow4.v1 = b.v4;

          aRow1 = VecNotEqual(aRow1, bRow1);
          aRow2 = VecNotEqual(aRow2, bRow2);
          aRow3 = VecNotEqual(aRow3, bRow3);
          aRow4 = VecNotEqual(aRow4, bRow4);

          aRow1 = VecOr(aRow1, aRow2);
          aRow1 = VecOr(aRow1, aRow3);
          aRow1 = VecOr(aRow1, aRow4);

          return VecMask(aRow1) != 0.f;
      }

    }
} // namespace krystallic
