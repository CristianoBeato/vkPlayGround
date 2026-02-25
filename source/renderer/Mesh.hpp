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

#ifndef __MESH_HPP__
#define __MESH_HPP__

class crMesh
{
public:
    enum type_t : uint8_t
    {
        MESH_STATIC,        // static mesh
        MESH_ANIMATE,       // animate model
        MESH_PARTICLE,      // particle mesh
        MESH_TERRAIN        // terrain
    };

    crMesh( void );
    ~crMesh( void );

    /// @brief Type of the mesh  
    virtual type_t  Type( void ) const { return MESH_STATIC; }
 
private:
    crMaterial*     m_material;
    
};



#endif //!__RENDER_MESH_HPP__