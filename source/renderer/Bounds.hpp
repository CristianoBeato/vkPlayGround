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

#ifndef __BOUNDS_HPP__
#define __BOUNDS_HPP__

class crBounds
{
public:
    enum type_e
    {
        BOUNDS_NONE,
        BOUNDS_VOLUME,
        BOUNDS_SPHERE,

    };

    crBounds( void );
    ~crBounds( void );
    virtual type_e  Type( void ) const { return BOUNDS_NONE; }
    virtual crVec3f Origin( void ) const { return crVec3f( 0.0f, 0.0f, 0.0f ); } 
};


/// @brief 
class crBoundsVolume : public crBounds
{
public:
    crBoundsVolume( void );
    ~crBoundsVolume( void );
    virtual type_e  Type( void ) const override { return BOUNDS_SPHERE; }
    virtual crVec3f Origin( void ) const override { return m_max - m_min; }
    crVec3f Min( void ) const { return m_min; }
    crVec3f Max( void ) const { return m_max; }
    
private:
    crVec3f     m_min;
    crVec3f     m_max;
};

class crBoundsSphere : public crBounds
{
public:
    crBoundsSphere( void );
    crBoundsSphere( const crVec3f &in_origin, const float in_radius );
    ~crBoundsSphere( void );
    virtual type_e  Type( void ) const override { return BOUNDS_NONE; }
    virtual crVec3f Origin( void ) const override { return m_center.ToVec3(); }
    const float Radius( void ) const { return m_center.w; }

private:
    crVec4f m_center; // w is radius
};



#endif //!__BOUNDS_HPP__