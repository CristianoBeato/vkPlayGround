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

#ifndef __LIGTH_HPP__
#define __LIGTH_HPP__

struct lightUniform_t
{
    uint32_t    type;
    float       constant;
    float       linear;
    float       quadratic;
    float       cutOff;
    crVec4f     color;
    crVec3f     position;
    crVec3f     direction;
};

class crLight
{
public:
    enum type_t
    {
        LIGTH_AMBIENT = 0,  // ambient ligth
        LIGTH_POINT,        // point ligth
        LIGTH_DIRECTIONAL,  // directional ligth
        LIGTH_SPOT,         //
        LIGTH_AREA          //
    };

    crLight( void );
    ~crLight( void );

    type_t      Type( void ) const { return m_type; }
    crVec4f     Color( void ) const { return m_ligth.color; }
    crVec3f     Position( void ) const { return m_ligth.position; }
    crVec3f     Direction( void ) const { return m_ligth.direction; }
    crBounds    Bounds( void ) const { return m_bounds; }

private:
    type_t          m_type;
    crBounds        m_bounds;
    lightUniform_t  m_ligth;
};

#endif //!__LIGTH_HPP__