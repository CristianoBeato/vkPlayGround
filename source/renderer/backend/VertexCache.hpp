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

#ifndef __VERTEX_CACHE_HPP__
#define __VERTEX_CACHE_HPP__

#include "Vulkan/Core.hpp"

struct cache_t
{
    uintptr_t           offset = 0;
    size_t              size = 0;
};

class crVertexCache
{
public:
    static crVertexCache*  Get( void );

    crVertexCache( void );
    ~crVertexCache( void );

private:

};

#endif //!__VERTEX_CACHE_HPP__