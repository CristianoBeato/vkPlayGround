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

#include "ModelManager.hpp"

constexpr size_t k_SURFACE_STAGING_BUFFER_SIZE = 128 * 1024 * 1024;

halfFloat_t ONEhf = 0x3C00;

// base face vertices 
static const VertPos_t k_QUAD_VERTS[4] = 
{ 
    { -1.0f, -1.0f, 0.0f, 0x0000, 0x0000 }, // 0 top left
    {  1.0f, -1.0f, 0.0f, 0x3C00, 0x0000 }, // 1 top right
    { -1.0f,  1.0f, 0.0f, 0x0000, 0x3C00 }, // 2 botton left
    {  1.0f,  1.0f, 0.0f, 0x3C00, 0x3C00 }, // 3 botton right
};

/// base face triangles
static const uint16_t   faces[6] = 
{       ///    ___
    0,  /// 0 |  / 1 
    2,  ///   | /
    1,  /// 2 |/
    1,  ///      /|1
    2,  ///     / |
    3,  ///   2/__|3
};

crModelManager *crModelManager::Get(void)
{
    static crModelManager gModelManager = crModelManager();
    return &gModelManager;
}

crModelManager::crModelManager(void) : m_geometryStaging(nullptr)
{
}

crModelManager::~crModelManager( void )
{
}

void crModelManager::StartUp( void )
{
    /// Create the meshes staging buffer
    m_geometryStaging = new crBufferRing();
    if( !m_geometryStaging->Create( 
        k_SURFACE_STAGING_BUFFER_SIZE, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // We use this buffer to copy to geometry buffer
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT // make the client copy visible to server
    ) )
    {
        throw crException( "Failed to create geometry staging buffer\n" );
    }
}

void crModelManager::ShutDown( void )
{
    if ( m_geometryStaging != nullptr )
    {
        delete m_geometryStaging;
        m_geometryStaging = nullptr;
    }
}
