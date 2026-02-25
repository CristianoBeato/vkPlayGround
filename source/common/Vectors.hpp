/*
===========================================================================================
    This file is part of vkPlayGround a Vulkan testing c++ engine.

    Copyright (c) 2026 Cristiano B. Santos <cristianobeato_dm@hotmail.com>
    Contributor(s): none yet.

-------------------------------------------------------------------------------------------

 This file is part of the vkPlayGround library and is licensed under the
 MIT License with Attribution Requirement.

 You are free to use, modify, and distribute this file (even commercially),
 as long as you give credit to the original author:

     “Based on vkPlayGround by Cristiano Beato – https://github.com/CristianoBeato”

 For full license terms, see the LICENSE file in the root of this repository.
===============================================================================================
*/

#ifndef __VECTORS_HPP__
#define __VECTORS_HPP__

/// deal trick, we can improve compiler otimization from 2 simd operations by calculation
/// to 1 operation calc 
//from:
// crVec3f operator + ( const crVec3f& in_other ):
//         movss   xmm0, dword ptr [rip + a+8]
//         addss   xmm0, dword ptr [rip + b+8]
//         movsd   xmm1, qword ptr [rip + a]
//         movsd   xmm2, qword ptr [rip + b]
//         addps   xmm2, xmm1
//         movlps  qword ptr [rip + r], xmm2
//         movss   dword ptr [rip + r+8], xmm0
//         ret
// to:
// crVec3f operator + ( const crVec3f& in_other ):
//         movaps  xmm0, xmmword ptr [rip + a]
//         addps   xmm0, xmmword ptr [rip + b]
//         movaps  xmmword ptr [rip + r], xmm0
//         ret
#define USE_VEC3_4PACK 1

class alignas( 16 ) crVec2f
{
public:
    float x;
    float y;

    inline crVec2f( void );
    inline crVec2f( const crVec2f &in_ref );
    inline crVec2f( const float in_v );
    inline crVec2f( const float in_x, const float in_y );
    inline ~crVec2f( void );

    inline crVec2f operator + ( const crVec2f& in_other ) const;
    inline crVec2f operator - ( const crVec2f& in_other ) const;
    inline crVec2f operator * ( const crVec2f& in_other ) const;
    inline crVec2f operator / ( const crVec2f& in_other ) const;
    
    inline crVec2f operator +=( const crVec2f& in_other );
    inline crVec2f operator -=( const crVec2f& in_other );
    inline crVec2f operator *=( const crVec2f& in_other );
    inline crVec2f operator /=( const crVec2f& in_other );
    
    inline float &operator[]( const uint32_t i );
    inline float operator[]( const uint32_t i ) const;

    inline const float*    ToFloatPtr( void ) const { return &x; }
};

crVec2f::crVec2f( void ) :
    x( 0.0f ),
    y( 0.0f )
{
}

crVec2f::crVec2f( const crVec2f &in_ref ) :
    x( in_ref.x ),
    y( in_ref.y )
{
}

crVec2f::crVec2f( const float in_v ) : 
    x( in_v ),
    y( in_v )
{
}

crVec2f::crVec2f(const float in_x, const float in_y) :
    x( in_x ),
    y( in_y )
{
}

crVec2f::~crVec2f( void )
{
    x = 0.0f;
    y = 0.0f;
}

crVec2f crVec2f::operator + (const crVec2f &in_other) const
{
    return { x + in_other.x, y + in_other.y };
}

crVec2f crVec2f::operator - (const crVec2f &in_other) const
{
    return { x - in_other.x, y - in_other.y };
}

crVec2f crVec2f::operator * (const crVec2f &in_other) const
{
    return { x * in_other.x, y * in_other.y };
}

crVec2f crVec2f::operator / (const crVec2f &in_other) const
{
    crVec2f r;
    r.x = ( in_other.x == 0.0f ) ? INFINITY : x / in_other.x;
    r.y = ( in_other.y == 0.0f ) ? INFINITY : y / in_other.y;
    return r;
}

crVec2f crVec2f::operator += (const crVec2f &in_other)
{
    x += in_other.x;
    y += in_other.y;
    return *this;
}

crVec2f crVec2f::operator -= (const crVec2f &in_other)
{
    x -= in_other.x;
    y -= in_other.y;
    return *this;
}

crVec2f crVec2f::operator *= (const crVec2f &in_other)
{
    x *= in_other.x;
    y *= in_other.y;
    return *this;
}

crVec2f crVec2f::operator /= (const crVec2f &in_other)
{
    x = ( in_other.x == 0.0f ) ? INFINITY : x / in_other.x;
    y = ( in_other.y == 0.0f ) ? INFINITY : y / in_other.y;
    return *this;
}

inline float &crVec2f::operator[](const uint32_t i)
{
    assert( i < 2 && "i is out of range" );
    return (&x)[i];
}

inline float crVec2f::operator[](const uint32_t i) const
{
    assert( i < 2 && "i is out of range" );
    return (&x)[i];
}

class alignas( 16 ) crVec3f
{
public:
    float x;
    float y;
    float z;

#if USE_VEC3_4PACK
private:
    float w = 1.0;
public:
#endif

    inline crVec3f( void );
    inline crVec3f( const crVec3f &in_ref );
    inline crVec3f( const float in_v );
    inline crVec3f( const crVec2f &in_other, const float in_z = 0.0f );
    inline crVec3f( const float in_x, const float in_y, const float in_z );
    inline ~crVec3f( void );

    inline crVec3f operator + ( const crVec3f& in_other ) const;
    inline crVec3f operator - ( const crVec3f& in_other ) const;
    inline crVec3f operator * ( const crVec3f& in_other ) const;
    inline crVec3f operator / ( const crVec3f& in_other ) const;
    
    inline crVec3f operator +=( const crVec3f& in_other );
    inline crVec3f operator -=( const crVec3f& in_other );
    inline crVec3f operator *=( const crVec3f& in_other );
    inline crVec3f operator /=( const crVec3f& in_other );
    
    inline float &operator[]( const uint32_t i );
    inline float operator[]( const uint32_t i ) const;

    inline const float*     ToFloatPtr( void ) const { return &x; }
    inline const crVec2f    ToVec2( void ) const { return { x, y }; }
};

crVec3f::crVec3f( void ) :
    x( 0.0f ),
    y( 0.0f ),
    z( 0.0f )
{
}

crVec3f::crVec3f( const crVec3f &in_ref ) :
    x( in_ref.x ),
    y( in_ref.y ),
    z( in_ref.z )
{
}

crVec3f::crVec3f( const float in_v ) :
    x( in_v ),
    y( in_v ),
    z( in_v )
{
}

crVec3f::crVec3f( const crVec2f &in_other, const float in_z ) : 
    x( in_other.x ),
    y( in_other.y ),
    z( in_z )
{
}

crVec3f::crVec3f( const float in_x, const float in_y, const float in_z ) :
    x( in_x ),
    y( in_y ),
    z( in_z )
{    
}

crVec3f::~crVec3f( void )
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

crVec3f crVec3f::operator + ( const crVec3f& in_other ) const
{
#if USE_VEC3_4PACK
    crVec3f r;
    r.x = x + in_other.x;
    r.y = y + in_other.y;
    r.z = z + in_other.z;
    r.w = w + in_other.w;
    return r;
#else
    return { x + in_other.x, y + in_other.y, z + in_other.z };
#endif
}

crVec3f crVec3f::operator - ( const crVec3f& in_other ) const
{
#if USE_VEC3_4PACK
    crVec3f r;
    r.x = x - in_other.x;
    r.y = y - in_other.y;
    r.z = z - in_other.z;
    r.w = w - in_other.w;
    return r;
#else
    return { x - in_other.x, y - in_other.y, z - in_other.z };
#endif
}

crVec3f crVec3f::operator * ( const crVec3f& in_other ) const
{
#if USE_VEC3_4PACK
    crVec3f r;
    r.x = x * in_other.x;
    r.y = y * in_other.y;
    r.z = z * in_other.z;
    r.w = w * in_other.w;
    return r;
#else
    return { x * in_other.x, y * in_other.y, z * in_other.z };
#endif
}

crVec3f crVec3f::operator / ( const crVec3f& in_other ) const
{
    crVec3f r;
    r.x = ( in_other.x == 0.0f ) ? INFINITY : x / in_other.x;
    r.y = ( in_other.y == 0.0f ) ? INFINITY : y / in_other.y;
    r.z = ( in_other.z == 0.0f ) ? INFINITY : z / in_other.z;
    return r;
}

crVec3f crVec3f::operator +=( const crVec3f& in_other )
{
    x += in_other.x;
    y += in_other.y;
    z += in_other.z;
#if USE_VEC3_4PACK
    w += in_other.w;
#endif
    return *this;
}

crVec3f crVec3f::operator -=( const crVec3f& in_other )
{
    x -= in_other.x;
    y -= in_other.y;
    z -= in_other.z;
#if USE_VEC3_4PACK
    w -= in_other.w;
#endif
    return *this;
}

crVec3f crVec3f::operator *=( const crVec3f& in_other )
{
    x *= in_other.x;
    y *= in_other.y;
    z *= in_other.z;
#if USE_VEC3_4PACK
    w *= in_other.w;
#endif
    return *this;
}

crVec3f crVec3f::operator /=( const crVec3f& in_other )
{
    x = ( in_other.x == 0.0f ) ? INFINITY : x / in_other.x;
    y = ( in_other.y == 0.0f ) ? INFINITY : y / in_other.y;
    z = ( in_other.z == 0.0f ) ? INFINITY : z / in_other.z;
    return *this;
}
    
float &crVec3f::operator[]( const uint32_t i )
{
    assert( i < 3 && "i is out of range" );
    return (&x)[i];
}

float crVec3f::operator[]( const uint32_t i ) const
{
    assert( i < 3 && "i is out of range" );
    return (&x)[i];
}

class alignas( 16 ) crVec4f
{
public:
    float x;
    float y;
    float z;
    float w;

    inline crVec4f( void );
    inline crVec4f( const crVec4f &in_ref );
    inline crVec4f( const float in_v );
    inline crVec4f( const crVec2f &in_other, const float in_z = 0.0f, const float in_w = 0.0f );
    inline crVec4f( const crVec3f &in_other, const float in_w = 0.0f );
    inline crVec4f( const float in_x, const float in_y, const float in_z, const float in_w );
    inline ~crVec4f( void );

    inline crVec4f operator + ( const crVec3f& in_other ) const;
    inline crVec4f operator - ( const crVec3f& in_other ) const;
    inline crVec4f operator * ( const crVec3f& in_other ) const;
    inline crVec4f operator / ( const crVec3f& in_other ) const;
    
    inline crVec4f operator +=( const crVec4f& in_other );
    inline crVec4f operator -=( const crVec4f& in_other );
    inline crVec4f operator *=( const crVec4f& in_other );
    inline crVec4f operator /=( const crVec4f& in_other );
    
    inline float &operator[]( const uint32_t i );
    inline float operator[]( const uint32_t i ) const;

    inline const float*     ToFloatPtr( void ) const { return &x; }
    inline const crVec3f    ToVec3( void ) const { return { x, y, z }; }
    inline const crVec2f    ToVec2( void ) const { return { x, y }; }
};

#endif //!__VECTORS_HPP__