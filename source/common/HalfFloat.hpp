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

#ifndef __HALF_FLOAT_HPP__
#define __HALF_FLOAT_HPP__

/*
================================================
halfFloat_t
================================================
*/
typedef uint16_t halfFloat_t;

// GPU half-float bit patterns
#define HF_MANTISSA(x)	(x&1023)
#define HF_EXP(x)		((x&32767)>>10)
#define HF_SIGN(x)		((x&32768)?-1:1)

/*
========================
F16toF32
========================
*/
inline float F16toF32( halfFloat_t x )
{
	int e = HF_EXP( x );
	int m = HF_MANTISSA( x );
	int s = HF_SIGN( x );
	
	if( 0 < e && e < 31 )
		return s * powf( 2.0f, ( e - 15.0f ) ) * ( 1 + m / 1024.0f );
	else if( m == 0 )
		return s * 0.0f;
	
	return s * powf( 2.0f, -14.0f ) * ( m / 1024.0f );
}

/*
========================
F32toF16
========================
*/
inline halfFloat_t F32toF16( float a )
{
	unsigned int f = *( unsigned* )( &a );
	unsigned int signbit  = ( f & 0x80000000 ) >> 16;
	int exponent = ( ( f & 0x7F800000 ) >> 23 ) - 112;
	unsigned int mantissa = ( f & 0x007FFFFF );
	
	if( exponent <= 0 )
		return 0;
	
	if( exponent > 30 )
		return ( halfFloat_t )( signbit | 0x7BFF );
	
	return ( halfFloat_t )( signbit | ( exponent << 10 ) | ( mantissa >> 13 ) );
}

struct float16_t
{
    halfFloat_t f;

    float16_t( void ) {}
    float16_t( const float16_t &r ) : f( r.f ) {}
    float16_t( const float &v ) { f = F32toF16( v ); }

    inline float16_t operator =( const float &v )
    {
        f = F32toF16( v );
        return *this;
    }

    inline operator float( void ) const 
    {
        return F16toF32( f );
    }
};


#endif //!__HALF_FLOAT_HPP__