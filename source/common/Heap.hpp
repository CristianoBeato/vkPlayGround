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
#ifndef __HEAP_HPP__
#define __HEAP_HPP__

/// @brief Perform a common memory unalignied allocation from main heap
/// @param in_size the amount of bytes to be allocated 
/// @return nullptr on error
extern void*    MemAlloc( const size_t in_size );

extern void*    MemAllocAligned( const size_t in_size, const size_t in_align );

/// @brief Perform a common memory unalignied allocation from main heap, and clear content to zeroes
/// @param in_size the amount of bytes to be allocated 
/// @return nullptr on error
extern void*    MemCleardAlloc( const size_t in_size );

/// @brief Reallocate a already allocated pointer to a new value
/// @param in_old 
/// @param in_newSize 
/// @return the new pointer 
extern void*    MemRealloc( void* in_old, const size_t in_newSize );

/// @brief Release memory pointer
/// @param in_source the pointer of memory to release
extern void     MemFree( void* in_source );

extern void     MemFreeAligned( void* in_source );

/// @brief Allocate a array of elements
/// @tparam __type__ the type of object to be allocated
/// @param in_count array count
/// @return the pointer of the array of the elements 
//template<typename __type__>
//inline __type__* MemAlloc( const uint32_t in_count )
//{
//    return static_cast<__type__*>( MemAlloc( sizeof( __type__ ) * in_count ) );
//}

//template<typename __type__>
//inline __type__* MemAllocAligned( const uint32_t in_count, const size_t in_align )
//{
//    return static_cast<__type__*>( MemAllocAligned( sizeof( __type__ ) * in_count, in_align ) );
//}

//template<typename __type__>
//inline __type__* MemRealloc( __type__ * &in_oldPtr, const uint32_t in_count )
//{
//    return static_cast<__type__*>( MemRealloc( reinterpret_cast<void*>( in_oldPtr ), sizeof( __type__ ) * in_count ) );
//}

#endif //!__HEAP_HPP__