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

#include "UniformManager.hpp"

static uint32_t descriptorCountForBinding0 = 4096; // ex: 4096
static uint32_t k_MAX_BINDING_SAMPLERS = 4096;

constexpr size_t    k_MESH_UNIFORM_BUFFER_SIZE = sizeof( uMesh_t ) * 2048u * SMP_FRAMES;        // ~864kb
constexpr size_t    k_MATERIAL_UNIFORM_BUFFER_SIZE = sizeof( uMaterial_t ) * 2048u * SMP_FRAMES; // ~96kb
constexpr size_t    k_LIGTH_UNIFORM_BUFFER_SIZE = sizeof( uLight_t ) * 2048u * SMP_FRAMES;      // ~192kb
constexpr uint32_t  k_MAX_SAMPLERS_BINDING = 8172u;
constexpr uint32_t  k_DESCRIPTOR_BINDIN_COUNT = 4u;

crUniformManager* crUniformManager::Get( void )
{
    static crUniformManager gUniformManager = crUniformManager();
    return &gUniformManager;
}

crUniformManager::crUniformManager( void )
{
}

crUniformManager::~crUniformManager( void )
{
}

void crUniformManager::StartUp( void )
{
    // get minimum alignament of Vulkan storage buffers 
    auto device = crContext::Get()->Device();
    size_t alignment = device->MinStorageAlignment(); 

    m_meshSSBO = new crBuffer();
    if( !m_meshSSBO->Create( _align( k_MESH_UNIFORM_BUFFER_SIZE, alignment ), crBuffer::BUFFER_SHADER_STORAGE ) )
        throw crException( "Failed to create Mesh Shader Storage Buffer\n" );

    // get the persistent map of the mesh uniform buffer
    m_meshUniformMap = static_cast<uMesh_t*>( m_meshSSBO->Map() );

    m_materialSSBO = new crBuffer();
    if( !m_materialSSBO->Create( _align( k_MATERIAL_UNIFORM_BUFFER_SIZE, alignment ), crBuffer::BUFFER_SHADER_STORAGE ) )
        throw crException( "Failed to create Material Shader Storage Buffer\n" );

    // get the persistent map of the material uniform buffer
    m_materialUniformMap = static_cast<uMaterial_t*>( m_materialSSBO->Map() );

    m_lightSSBO = new crBuffer();
    if( !m_lightSSBO->Create( _align( k_LIGTH_UNIFORM_BUFFER_SIZE, alignment ), crBuffer::BUFFER_SHADER_STORAGE ) )
        throw crException( "Failed to create Light Shader Storage Buffer\n" );

    // get the persistent map of the material uniform buffer
    m_lightUniformMap = static_cast<uLight_t*>( m_lightSSBO->Map() ); 

    /// Create storage layout
    m_storageLayout = new crShaderStorageLayout();
    if( m_storageLayout->Create( k_NUM_SHADER_STORAGE ) )
        throw crException( "Failed to create shader storage layout" );

    /// create the sampler location array
    m_samplerSlotArray = new crSamplerSlotArray();
    if( m_samplerSlotArray->Create() )

    /// create the pipeline layout
    m_layout = new crPipelineLayout();
    if( !m_layout->Create( m_samplerSlotArray, m_storageLayout ) )
        throw crException( "Failed to create the uniform buffers pipeline layout\n" );
}

void crUniformManager::ShutDown( void )
{
    /// Release pipeline layout
    if ( m_layout != nullptr )
    {
        delete m_layout;
        m_layout = nullptr;
    }

    /// Release storage layout
    if( m_storageLayout != nullptr )
    {
        delete m_storageLayout;
        m_storageLayout = nullptr;
    }

    /// Release bindless sampler storage array
    if( m_samplerSlotArray != nullptr )
    {
        delete m_samplerSlotArray;
        m_samplerSlotArray = nullptr;
    }

    /// Release light storage buffer
    if( m_lightSSBO != nullptr )
    {
        delete m_lightSSBO;
        m_lightSSBO = nullptr;
    }

    /// Release material storage buffer
    if( m_materialSSBO != nullptr )
    {
        delete m_materialSSBO;
        m_materialSSBO = nullptr;
    }
    
    /// Release mesh storage buffer
    if( m_meshSSBO != nullptr )
    {
        delete m_meshSSBO;
        m_meshSSBO = nullptr;
    }
}