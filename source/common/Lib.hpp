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
#ifndef __LIB_HPP__
#define __LIB_HPP__

inline constexpr size_t k_MAX_ERR_STRING_LEN = 1024;

#if 0
#include <SDL3/SDL_assert.h>
#define assert SDL_assert
#else
#include <cassert>
#endif

#include "Exception.hpp"
#include "Heap.hpp"
#include "Pointers.hpp"
#include "String.hpp"
#include "Array.hpp"
#include "List.hpp"
#include "Vectors.hpp"
#include "Quaternion.hpp"
#include "Matrices.hpp"
#include "HalfFloat.hpp"

/// @brief Check if a integer is 2^
/// @tparam _t a integer type
/// @param x value to check 
/// @return true if value is power of two
template< typename _t >
constexpr bool _isPowerOfTwo( const _t x ) noexcept 
{
    return x > 0 && ( x & ( x - 1 ) ) == 0;
}

/// @brief align up a value 
/// @tparam _t integer type
/// @param size the size to be aligned
/// @param alignment the multiplyer alignment 
/// @return aligned value
template< typename _t>
inline _t _align( const _t size, const _t alignment ) noexcept 
{
    static_assert( std::is_integral<_t>::value, "Alignment requires integer types" );
    assert( ( alignment > 0) && ( ( alignment & ( alignment - 1 ) ) == 0 ) && "Alignment must be power of 2" );
    return ( size + alignment - 1 ) & ~( alignment - 1 );
}

///
template<typename _t >
inline _t* _align( _t* ptr, const uintptr_t alignment ) noexcept 
{
    assert( ( alignment > 0) && ( ( alignment & ( alignment - 1 ) ) == 0 ) && "Alignment must be power of 2" );
    uintptr_t addr = reinterpret_cast<uintptr_t>( ptr );
    uintptr_t aligned = ( addr + alignment - 1 ) & ~( alignment - 1 );
    return reinterpret_cast<_t*>( aligned );
}

#endif //!__LIB_HPP__