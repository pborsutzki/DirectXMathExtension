DirectXMathExtension
====================

This header file extends DirectXMath by implementing assignments from or to
memory from simd-types without the need to call annoying functions like
XMLoad\* and XMStore\*.

What is it?
-----------

These are my extensions for DirectXMath. They mainly help working with memory
types vs. simd-types and automatically do conversions between them. There is
absolutely **no cpu or memory overhead** in a fully optimized (release) compile
when using these extensions correctly. Nevertheless there is a notable overhead
in non-optimized (debug) builds through not inlined functions and
additional safety checks.

The extensions replace the memory-side-types like XMFLOAT2, XMFLOAT3,
XMFLOAT4, etc. and XMFLOAT3X3, XMFLOAT4X4, etc. with MXMFLOAT2, MXMFLOAT3,
MXMFLOAT3X3 etc.

With these extensions you are able to simply assign simd-types (XMVECTOR/
XMMATRIX) to memory-types (e.g. MXMFLOAT3/MXMFLOAT3X3) or assign from them to
simd-types.

If want to switch to my types, you can either rename your memory-types or
define \_MXM\_USE\_OVERWRITE\_DEFINES in your preprocessor which effectively 
overwrites the DirectXMath-memory-types using defines. You then don't have to
change your code at all: every DirectXMath function is fully compatible to my
types because they inherit from the original types and just add some code.

*BUT DON'T GET LAZY* while using these extensions:
Assigning from a memory-type to a simd-type when you use a variable multiple
times in a row is crucial for the performance of your code (see examples).


Why not SimpleMath (DirectXTK)?
-------------------------------

SimpleMath doesn't give you the choice when to use simd-types and when to store
them to memory-types. It always loads from memory-types to simd-types, does one
calculation and then stores it again to a memory-type - even if the same variable
is used again shortly after that. This might be more straight forward to use but
results in slower code. So I decided to write my own lightweight wrapper to
DirectXMath which just gets rid of the annoying XMLoad and XMStore functions.

Examples
--------

### Simple Example ###
* Usual DirectXMath code:

    ```C++
    XMFLOAT4X4 memmat;
    XMStoreFloat4x4(&memmat, XMMatrixTranslation(1,2,3));
    // ... //
    XMStoreFloat4x4(&memmat, XMMatrixScaling(4,5,6) * XMLoadFloat4x4(&memmat));
    ```
* The same code using DirectXMathExtension:

    ```C++
    MXMFLOAT4X4 memmat = XMMatrixTranslation(1,2,3);
    // ... //
    memmat = XMMatrixScaling(4,5,6) * memmat;
    ```

### Negative example ###
* **DON'T GET LAZY** and do things like the following:

    ```C++
    MXMFLOAT4X4 memmat = XMMatrixTranslation(1,2,3);
    // ... //
    memmat = XMMatrixScaling(4,5,6) * memmat;     // 1 XMLoad, 1 XMSave
    memmat = XMMatrixTranslation(7,8,9) * memmat; // 1 XMLoad, 1 XMSave
    ```
* Instead load the matrix into a simd-variable-register:

    ```C++
    MXMFLOAT4X4 memmat = XMMatrixTranslation(1,2,3);
    // ... //
    XMMATRIX simdmat = memmat;                     // 1 XMLoad
    simdmat = XMMatrixScaling(4,5,6) * simdmat;
    memmat = XMMatrixTranslation(7,8,9) * simdmat; // 1 XMSave
    ```

### Example from SimpleMath ###
* Using plain DirectXMath:

    ```C++
    __declspec(align(16)) class PlayerCat : public AlignedNew<PlayerCat>
    {
    public:
      void Update()
      {
        const float cFriction = 0.99f;
    
        XMVECTOR pos = XMLoadFloat3A(&mPosition);
        XMVECTOR vel = XMLoadFloat3A(&mVelocity);
    
        XMStoreFloat3A(&mPosition, pos + vel);
        XMStoreFloat3A(&mVelocity, vel * cFriction);
      }
    
    private:
      XMFLOAT3A mPosition;
      XMFLOAT3A mVelocity;
    };
    ```

* Using plain DirectXMathExtensions:

    ```C++
    __declspec(align(16)) class MPlayerCat : public AlignedNew<MPlayerCat>
    {
    public:
      void Update()
      {
        const float cFriction = 0.99f;
    
        XMVECTOR vel = mVelocity;    //used twice -> load into simd-register!
    
        mPosition = mPosition + vel;
        mVelocity = vel * cFriction;
      }
    
    private:
      MXMFLOAT3A mPosition;
      MXMFLOAT3A mVelocity;
    };
    ```
    
    **And the best is, both MPlayerCat and PlayerCat generate the exact same assembly-code on a release-compile!**

Requirements
------------
- Visual Studio 2010 or better
- A Windows Kit containing DirectXMath
- A need for performant mathematics
- A dislike for XMLoad\* and XMStore\* functions

Download
--------

Get the source via git:

    $ git clone https://github.com/pborsutzki/DirectXMathExtension.git

How does it work internally?
----------------------------

My types are inherited from the original DirectXMath memory-types. For example, for XMFLOAT2:

```C++
struct MXMFLOAT2 : public XMFLOAT2 // inheritance from original type
{
  // constructor forwardings (would be easier with c++11, but currently still unsupported)
  MXMFLOAT2() : XMFLOAT2() {}
  MXMFLOAT2(float _x, float _y) : XMFLOAT2(_x, _y) {}
  explicit MXMFLOAT2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray) {}

  // construction from a XMVECTOR (storage, simd-type -> memory-type)
  MXMFLOAT2(XMVECTOR v) {
    XMStoreFloat2(this, v);
  }

  // construction of a XMVECTOR from a XMFLOAT2 (loading, memory-type -> simd-type)
  operator const XMVECTOR() const {
    return XMLoadFloat2(this);
  }

  // assignment of a XMVECTOR to a XMFLOAT2 (storage, simd-type -> memory-type)
  // this is not necessary - there is already a overloaded = operator available, but this is faster
  MXMFLOAT2& operator= (const XMVECTOR v) {
    XMStoreFloat2(this, v);
    return *this; 
  }
};
```
*Please note that the above code is stripped of optimizations like inlining and calling
conventions for better understanding. Of course the extensions-header does contain those optimizations.*

License
-------
The extension is released under the standard bsd-license. You can find a copy in
[LICENSE.txt](LICENSE.txt).
Talk to me if you need a different license and I'll see if that is possible.
