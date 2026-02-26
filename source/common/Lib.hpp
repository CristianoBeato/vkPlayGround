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

#endif //!__LIB_HPP__