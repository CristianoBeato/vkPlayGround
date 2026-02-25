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

#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

class crGeometry
{
public:
    crGeometry( void );
    ~crGeometry( void );

    crBounds        Bounds( void ) const { return m_bounds; }
    crMatrix4       ModelMatrix( void ) { return m_modelMatrix; }
    crGeometry*     Next( void ) const { return m_next; }
    crMesh*         Mesh( void ) const { return m_mesh; }
    crModel*        Model( void ) const { return m_model; }
    crMaterial*     Material( void ) const { return m_material; }

private:
    crBounds        m_bounds;
    crMatrix4       m_modelMatrix;
    crGeometry*     m_next;
    crModel*        m_model;
    crMaterial*     m_material;
    crMesh*         m_mesh;
};

#endif //!__GEOMETRY_HPP__