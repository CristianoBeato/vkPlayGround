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

#include <SDL3/SDL_stdinc.h>
#include "Heap.hpp"

void *MemAlloc(const size_t in_size)
{
    return SDL_malloc( in_size );
}

void *MemAllocAligned(const size_t in_size, const size_t in_align)
{
    return SDL_aligned_alloc( in_align, in_size );
}

void *MemCleardAlloc(const size_t in_size)
{
    /// alloc data pointer
    void* bytes = SDL_malloc( in_size );
    std::memset( bytes, 0x00, in_size ); 
    return bytes;
}

void * MemRealloc(void * in_old, const size_t in_newSize)
{
    return SDL_realloc( in_old, in_newSize );
}

void MemFree(void *in_source)
{
    SDL_free( in_source );
}

void MemFreeAligned(void *in_source)
{
    SDL_aligned_free( in_source );
}
