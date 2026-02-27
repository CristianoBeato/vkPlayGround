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

#include "VertexCache.hpp"

constexpr size_t k_INDEX_BUFFER_SIZE = 1024 * 1024 * 16;    // 16 mb for index buffer
constexpr size_t k_VERTEX_BUFFER_SIZE = 1024 * 1024 * 128;  // 128mb for vertex buffer
constexpr size_t k_STAGING_BUFFER_SIZE = 1024 * 1024 * 64;  // 64mb for streaming

crVertexCache *crVertexCache::Get(void)
{
    static crVertexCache gVertexCache = crVertexCache();
    return &gVertexCache;
}

crVertexCache::crVertexCache( void ) : 
    m_vertexBuffer( nullptr ),
    m_indexBuffer( nullptr )
{
}

crVertexCache::~crVertexCache( void )
{
}

void crVertexCache::StartUp(void)
{
    /// our vertex cache buffer
    m_vertexBuffer = new crBufferAllocator();
    if( !m_vertexBuffer->Create( k_VERTEX_BUFFER_SIZE, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT ) )

    /// our index cache buffer 
    m_indexBuffer = new crBufferAllocator();
    if ( !m_indexBuffer->Create( k_INDEX_BUFFER_SIZE, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT ) )
        throw crException( "crVertexCache ( Failed to create index buffer! )" );

    /// create the source ring buffer for vertex data streaming
    m_stagingBuffer = new crBufferRing();
    if( !m_stagingBuffer->Create( k_STAGING_BUFFER_SIZE, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
        throw crException( "crVertexCache ( Failed to create stagin buffer! )" );
}

void crVertexCache::ShutDown(void)
{
    /// Release staging buffer
    if ( m_stagingBuffer != nullptr )
    {
        delete m_stagingBuffer;
        m_stagingBuffer = nullptr;
    }
    
    /// Release index buffer 
    if ( m_indexBuffer != nullptr )
    {
        delete m_indexBuffer;
        m_indexBuffer = nullptr;
    }
    
    /// Release vertex buffer
    if ( m_vertexBuffer )
    {
        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
    }
}
