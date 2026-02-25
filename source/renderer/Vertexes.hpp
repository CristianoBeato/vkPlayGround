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

#ifndef __INCLUDE_VERTICES_HPP__
#define __INCLUDE_VERTICES_HPP__

// bindings 
inline constexpr uint32_t VERTEX_BINDING_POSI = 0; //
inline constexpr uint32_t VERTEX_BINDING_NORM = 1; //
inline constexpr uint32_t VERTEX_BINDING_SKIN = 2; //
inline constexpr uint32_t VERTEX_BINDING_MATE = 3; //
inline constexpr uint32_t VERTEX_BINDING_COUNT = 4;

// attributes
inline constexpr uint32_t VERTEX_ATTRIBUTE_POSI = 0; // 
inline constexpr uint32_t VERTEX_ATTRIBUTE_TEXT = 1; // 
inline constexpr uint32_t VERTEX_ATTRIBUTE_NORM = 2; //
inline constexpr uint32_t VERTEX_ATTRIBUTE_TANG = 3; //
inline constexpr uint32_t VERTEX_ATTRIBUTE_STAR = 4; // 
inline constexpr uint32_t VERTEX_ATTRIBUTE_COUN = 5; // 
inline constexpr uint32_t VERTEX_ATTRIBUTE_MATE = 6; // 
inline constexpr uint32_t VERTEX_ATTRIBUTE_COUNT = 7;

/// @brief Store vertex position and texture coordinate
struct VertPos_t
{
    float       x, y, z;    // vertex position
    float16_t   s, t;       // texture coordinate
};

/// Store vertex normal
struct VertNor_t
{
    uint8_t				normal[4];
    uint8_t				tangent[4];
};

struct VertSkin_t
{
    int32_t start;  // Weigth start
    int32_t count;  // Weigth count
};

struct VertMat_t
{
    uint32_t    surfID; // the material of the 
};

#endif //!__INCLUDE_VERTICES_HPP__