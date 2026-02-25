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

#ifndef __VIEW_HPP__
#define __VIEW_HPP__

class crView
{
public:
    crView( void );
    ~crView( void );

    crView*         Next( void ) const { return m_next; }
    crGeometry*     Geometry( void ) const { return m_geometryChain; }
    crMatrix4       ViewMatrix( void ) const;
    crVec3f         Position( void ) const { return m_position; }
    crVec3f         Orientation( void ) const { return m_orientation; }
    
    
private:
    crMatrix4       m_viewMatrix;
    crVec3f         m_position;
    crVec3f         m_orientation;
    crView*         m_next;
    crGeometry*     m_geometryChain;
};

#endif //!__VIEW_HPP__