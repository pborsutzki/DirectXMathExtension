#pragma once

/*------------------------------------------------------------------------------
// INFO

  These are my extensions for DirectXMath. They mainly help working with memory
  types vs. simd-types and automatically do conversions between them. There is
  absolutely no cpu or memory overhead in a fully optimized (release) compile
  when using these extensions correctly. Nevertheless there is a notable amount
  of overhead in non-optimized (debug) builds through non-inlining and
  additional safety checks.
  
  The extensions replace the memory-side-types like XMFLOAT2, XMFLOAT3,
  XMFLOAT4, etc. and XMFLOAT3X3, XMFLOAT4X4, etc. with MXMFLOAT2, MXMFLOAT3,
  MXMFLOAT3X3 etc.

  With these extensions you are able to simply assign simd-types (XMVECTOR/
  XMMATRIX) to memory-types (e.g. MXMFLOAT3/MXMFLOAT3X3) or assign from them to
  simd-types.

  If want to switch to my types, you can either rename your memory-types or
  define _MXM_USE_OVERWRITE_DEFINES in your preprocessor which effectively 
  overwrites the DirectXMath-memory-types using defines. You then don't have to
  change your code at all: every DirectXMath function is fully compatible to my
  types because they inherit from the original types and just add some code.

  BUT DON'T GET LAZY:
  Assigning from a memory-type to a simd-type when you use a variable multiple
  times in a row is crucial for the performance of your code (see examples).

//------------------------------------------------------------------------------
// Example

  //++++++++++++++++++++++++
  // usual DirectXMath code:

    XMFLOAT4X4 memmat;
    XMStoreFloat4x4(&memmat, XMMatrixTranslation(1,2,3));
    // ... //
    XMStoreFloat4x4(&memmat, XMMatrixScaling(4,5,6) * XMLoadFloat4x4(&memmat));

  //++++++++++++++++++++++++++
  // code using my extensions:

    MXMFLOAT4X4 memmat = XMMatrixTranslation(1,2,3);
    // ... //
    memmat = XMMatrixScaling(4,5,6) * memmat;

  //#################################################
  // DON'T GET LAZY and do things like the following:

    MXMFLOAT4X4 memmat = XMMatrixTranslation(1,2,3);
    // ... //
    memmat = XMMatrixScaling(4,5,6) * memmat;     // 1 XMLoad, 1 XMSave
    memmat = XMMatrixTranslation(7,8,9) * memmat; // 1 XMLoad, 1 XMSave

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Instead load the matrix into a simd-variable-register:

    MXMFLOAT4X4 memmat = XMMatrixTranslation(1,2,3);
    // ... //
    XMMATRIX simdmat = memmat;                     // 1 XMLoad
    simdmat = XMMatrixScaling(4,5,6) * simdmat;
    memmat = XMMatrixTranslation(7,8,9) * simdmat; // 1 XMSave

//------------------------------------------------------------------------------
// LICENSE

Copyright (c) 2013, Philipp Borsutzki
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//----------------------------------------------------------------------------*/

#include <DirectXMath.h>

namespace DirectX
{

#if (DIRECTXMATH_VERSION < 305) && (!defined(XM_CALLCONV))
# define XM_CALLCONV __fastcall
  typedef const XMVECTOR& HXMVECTOR;
  typedef const XMMATRIX& FXMMATRIX;
#endif

#define __MXM_INLINE __forceinline

__MXM_INLINE XMMATRIX XM_CALLCONV MXMMatrixAbs(const FXMMATRIX mat)
{
  XMMATRIX res;
  res.r[0] = XMVectorAbs(mat.r[0]);
  res.r[1] = XMVectorAbs(mat.r[1]);
  res.r[2] = XMVectorAbs(mat.r[2]);
  res.r[3] = XMVectorAbs(mat.r[3]);
  return res;
}

//------------------------------------------------------------------------------
// 2D Vectors

struct MXMFLOAT2 : public XMFLOAT2
{
  __MXM_INLINE MXMFLOAT2() : XMFLOAT2() {}
  __MXM_INLINE MXMFLOAT2(float _x, float _y) : XMFLOAT2(_x, _y) {}
  __MXM_INLINE explicit MXMFLOAT2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}

  __MXM_INLINE MXMFLOAT2(FXMVECTOR v) {
    XMStoreFloat2(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadFloat2(this);
  }

  __MXM_INLINE MXMFLOAT2& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreFloat2(this, v);
    return *this; 
  }
};

__declspec(align(16)) struct MXMFLOAT2A : public XMFLOAT2A
{
  __MXM_INLINE MXMFLOAT2A() : XMFLOAT2A() {}
  __MXM_INLINE MXMFLOAT2A(float _x, float _y) : XMFLOAT2A(_x, _y) {}
  __MXM_INLINE explicit MXMFLOAT2A(_In_reads_(2) const float *pArray) : XMFLOAT2A(pArray) {}

  __MXM_INLINE MXMFLOAT2A(FXMVECTOR v) {
    XMStoreFloat2A(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadFloat2A(this);
  }

  __MXM_INLINE MXMFLOAT2A& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreFloat2A(this, v);
    return *this; 
  }
};

struct MXMINT2 : public XMINT2
{
  __MXM_INLINE MXMINT2() : XMINT2() {}
  __MXM_INLINE MXMINT2(int32_t _x, int32_t _y) : XMINT2(_x, _y) {}
  __MXM_INLINE explicit MXMINT2(_In_reads_(2) const int32_t *pArray) : XMINT2(pArray) {}

  __MXM_INLINE MXMINT2(FXMVECTOR v) {
    XMStoreSInt2(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadSInt2(this);
  }

  __MXM_INLINE MXMINT2& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreSInt2(this, v);
    return *this; 
  }
};

struct MXMUINT2 : public XMUINT2
{
  __MXM_INLINE MXMUINT2() : XMUINT2() {}
  __MXM_INLINE MXMUINT2(uint32_t _x, uint32_t _y) : XMUINT2(_x, _y) {}
  __MXM_INLINE explicit MXMUINT2(_In_reads_(2) const uint32_t *pArray) : XMUINT2(pArray) {}

  __MXM_INLINE MXMUINT2(FXMVECTOR v) {
    XMStoreUInt2(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadUInt2(this);
  }

  __MXM_INLINE MXMUINT2& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreUInt2(this, v);
    return *this; 
  }
};

//------------------------------------------------------------------------------
// 3D Vectors

struct MXMFLOAT3 : public XMFLOAT3
{
  __MXM_INLINE MXMFLOAT3() : XMFLOAT3() {}
  __MXM_INLINE MXMFLOAT3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
  __MXM_INLINE explicit MXMFLOAT3(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}

  __MXM_INLINE MXMFLOAT3(FXMVECTOR v) {
    XMStoreFloat3(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadFloat3(this);
  }

  __MXM_INLINE MXMFLOAT3& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreFloat3(this, v);
    return *this; 
  }
};

__declspec(align(16)) struct MXMFLOAT3A : public XMFLOAT3A
{
  __MXM_INLINE MXMFLOAT3A() : XMFLOAT3A() {}
  __MXM_INLINE MXMFLOAT3A(float _x, float _y, float _z) : XMFLOAT3A(_x, _y, _z) {}
  __MXM_INLINE explicit MXMFLOAT3A(_In_reads_(3) const float *pArray) : XMFLOAT3A(pArray) {}

  __MXM_INLINE MXMFLOAT3A(FXMVECTOR v) {
    XMStoreFloat3A(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadFloat3A(this);
  }

  __MXM_INLINE MXMFLOAT3A& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreFloat3A(this, v);
    return *this; 
  }
};

struct MXMINT3 : public XMINT3
{
  __MXM_INLINE MXMINT3() : XMINT3() {}
  __MXM_INLINE MXMINT3(int32_t _x, int32_t _y, int32_t _z) : XMINT3(_x, _y, _z) {}
  __MXM_INLINE explicit MXMINT3(_In_reads_(3) const int32_t *pArray) : XMINT3(pArray) {}

  __MXM_INLINE MXMINT3(FXMVECTOR v) {
    XMStoreSInt3(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadSInt3(this);
  }

  __MXM_INLINE MXMINT3& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreSInt3(this, v);
    return *this; 
  }
};

struct MXMUINT3 : public XMUINT3
{
  __MXM_INLINE MXMUINT3() : XMUINT3() {}
  __MXM_INLINE MXMUINT3(uint32_t _x, uint32_t _y, uint32_t _z) : XMUINT3(_x, _y, _z) {}
  __MXM_INLINE explicit MXMUINT3(_In_reads_(3) const uint32_t *pArray) : XMUINT3(pArray) {}

  __MXM_INLINE MXMUINT3(FXMVECTOR v) {
    XMStoreUInt3(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadUInt3(this);
  }

  __MXM_INLINE MXMUINT3& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreUInt3(this, v);
    return *this; 
  }
};

//------------------------------------------------------------------------------
// 4D Vectors

struct MXMFLOAT4 : public XMFLOAT4
{
  __MXM_INLINE MXMFLOAT4() : XMFLOAT4() {}
  __MXM_INLINE MXMFLOAT4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
  __MXM_INLINE explicit MXMFLOAT4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}

  __MXM_INLINE MXMFLOAT4(FXMVECTOR v) {
    XMStoreFloat4(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadFloat4(this);
  }

  __MXM_INLINE MXMFLOAT4& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreFloat4(this, v);
    return *this; 
  }
};

__declspec(align(16)) struct MXMFLOAT4A : public XMFLOAT4A
{
  __MXM_INLINE MXMFLOAT4A() : XMFLOAT4A() {}
  __MXM_INLINE MXMFLOAT4A(float _x, float _y, float _z, float _w) : XMFLOAT4A(_x, _y, _z, _w) {}
  __MXM_INLINE explicit MXMFLOAT4A(_In_reads_(4) const float *pArray) : XMFLOAT4A(pArray) {}

  __MXM_INLINE MXMFLOAT4A(FXMVECTOR v) {
    XMStoreFloat4A(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadFloat4A(this);
  }

  __MXM_INLINE MXMFLOAT4A& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreFloat4A(this, v);
    return *this; 
  }
};

struct MXMINT4 : public XMINT4
{
  __MXM_INLINE MXMINT4() : XMINT4() {}
  __MXM_INLINE MXMINT4(int32_t _x, int32_t _y, int32_t _z, int32_t _w) : XMINT4(_x, _y, _z, _w) {}
  __MXM_INLINE explicit MXMINT4(_In_reads_(4) const int32_t *pArray) : XMINT4(pArray) {}

  __MXM_INLINE MXMINT4(FXMVECTOR v) {
    XMStoreSInt4(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadSInt4(this);
  }

  __MXM_INLINE MXMINT4& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreSInt4(this, v);
    return *this; 
  }
};

struct MXMUINT4 : public XMUINT4
{
  __MXM_INLINE MXMUINT4() : XMUINT4() {}
  __MXM_INLINE MXMUINT4(uint32_t _x, uint32_t _y, uint32_t _z, uint32_t _w) : XMUINT4(_x, _y, _z, _w) {}
  __MXM_INLINE explicit MXMUINT4(_In_reads_(4) const uint32_t *pArray) : XMUINT4(pArray) {}

  __MXM_INLINE MXMUINT4(FXMVECTOR v) {
    XMStoreUInt4(this, v);
  }

  __MXM_INLINE XM_CALLCONV operator const XMVECTOR() const {
    return XMLoadUInt4(this);
  }

  __MXM_INLINE MXMUINT4& XM_CALLCONV operator= (const FXMVECTOR v) {
    XMStoreUInt4(this, v);
    return *this; 
  }
};

//------------------------------------------------------------------------------
// 3x3 Matrices

struct MXMFLOAT3X3 : public XMFLOAT3X3
{
  __MXM_INLINE MXMFLOAT3X3() : XMFLOAT3X3() {}
  __MXM_INLINE MXMFLOAT3X3(float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22)
    : XMFLOAT3X3(m00, m01, m02, m10, m11, m12, m20, m21, m22) {}
  __MXM_INLINE explicit MXMFLOAT3X3(_In_reads_(9) const float *pArray)
    : XMFLOAT3X3(pArray) {}

  __MXM_INLINE MXMFLOAT3X3(CXMMATRIX m) {
    XMStoreFloat3x3(this, m);
  }

  __MXM_INLINE XM_CALLCONV operator const XMMATRIX() const {
    return XMLoadFloat3x3(this);
  }

  __MXM_INLINE MXMFLOAT3X3& XM_CALLCONV operator= (const FXMMATRIX m) {
    XMStoreFloat3x3(this, m);
    return *this; 
  }
};

//------------------------------------------------------------------------------
// 4x3 Matrices

struct MXMFLOAT4X3 : public XMFLOAT4X3
{
  __MXM_INLINE MXMFLOAT4X3() : XMFLOAT4X3() {}
  __MXM_INLINE MXMFLOAT4X3(float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32)
    : XMFLOAT4X3(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) {}
  __MXM_INLINE explicit MXMFLOAT4X3(_In_reads_(12) const float *pArray)
    : XMFLOAT4X3(pArray) {}

  __MXM_INLINE MXMFLOAT4X3(CXMMATRIX m) {
    XMStoreFloat4x3(this, m);
  }

  __MXM_INLINE XM_CALLCONV operator const XMMATRIX() const {
    return XMLoadFloat4x3(this);
  }

  __MXM_INLINE MXMFLOAT4X3& XM_CALLCONV operator= (const FXMMATRIX m) {
    XMStoreFloat4x3(this, m);
    return *this; 
  }
};

__declspec(align(16)) struct MXMFLOAT4X3A : public XMFLOAT4X3A
{
  __MXM_INLINE MXMFLOAT4X3A() : XMFLOAT4X3A() {}
  __MXM_INLINE MXMFLOAT4X3A(float m00, float m01, float m02,
                            float m10, float m11, float m12,
                            float m20, float m21, float m22,
                            float m30, float m31, float m32)
    : XMFLOAT4X3A(m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32) {}
  __MXM_INLINE explicit MXMFLOAT4X3A(_In_reads_(12) const float *pArray)
    : XMFLOAT4X3A(pArray) {}

  __MXM_INLINE MXMFLOAT4X3A(CXMMATRIX m) {
    XMStoreFloat4x3A(this, m);
  }

  __MXM_INLINE XM_CALLCONV operator const XMMATRIX() const {
    return XMLoadFloat4x3A(this);
  }

  __MXM_INLINE MXMFLOAT4X3A& XM_CALLCONV operator= (const FXMMATRIX m) {
    XMStoreFloat4x3A(this, m);
    return *this; 
  }
};

//------------------------------------------------------------------------------
// 4x4 Matrices

struct MXMFLOAT4X4 : public XMFLOAT4X4
{
  __MXM_INLINE MXMFLOAT4X4() : XMFLOAT4X4() {}
  __MXM_INLINE MXMFLOAT4X4(float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33)
    : XMFLOAT4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
  __MXM_INLINE explicit MXMFLOAT4X4(_In_reads_(16) const float *pArray)
    : XMFLOAT4X4(pArray) {}

  __MXM_INLINE MXMFLOAT4X4(CXMMATRIX m) {
    XMStoreFloat4x4(this, m);
  }

  __MXM_INLINE XM_CALLCONV operator const XMMATRIX() const {
    return XMLoadFloat4x4(this);
  }

  __MXM_INLINE MXMFLOAT4X4& XM_CALLCONV operator= (const FXMMATRIX m) {
    XMStoreFloat4x4(this, m);
    return *this; 
  }
};

__declspec(align(16)) struct MXMFLOAT4X4A : public XMFLOAT4X4A
{
  __MXM_INLINE MXMFLOAT4X4A() : XMFLOAT4X4A() {}
  __MXM_INLINE MXMFLOAT4X4A(float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33)
    : XMFLOAT4X4A(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
  __MXM_INLINE explicit MXMFLOAT4X4A(_In_reads_(16) const float *pArray)
    : XMFLOAT4X4A(pArray) {}

  __MXM_INLINE MXMFLOAT4X4A(CXMMATRIX m) {
    XMStoreFloat4x4A(this, m);
  }

  __MXM_INLINE XM_CALLCONV operator const XMMATRIX() const {
    return XMLoadFloat4x4A(this);
  }

  __MXM_INLINE MXMFLOAT4X4A& XM_CALLCONV operator= (const FXMMATRIX m) {
    XMStoreFloat4x4A(this, m);
    return *this; 
  }
};

#ifdef _MXM_USE_OVERWRITE_DEFINES

# define XMFLOAT2    MXMFLOAT2
# define XMINT2      MXMINT2
# define XMUINT2     MXMUINT2

# define XMFLOAT2A   MXMFLOAT2A
# define XMFLOAT3    MXMFLOAT3
# define XMFLOAT3A   MXMFLOAT3A
# define XMINT3      MXMINT3
# define XMUINT3     MXMUINT3

# define XMFLOAT4    MXMFLOAT4
# define XMFLOAT4A   MXMFLOAT4A
# define XMINT4      MXMINT4
# define XMUINT4     MXMUINT4

# define XMFLOAT3X3  MXMFLOAT3X3
# define XMFLOAT4X3  MXMFLOAT4X3
# define XMFLOAT4X3A MXMFLOAT4X3A
# define XMFLOAT4X4  MXMFLOAT4X4
# define XMFLOAT4X4A MXMFLOAT4X4A

#endif

} //namespace DirectX