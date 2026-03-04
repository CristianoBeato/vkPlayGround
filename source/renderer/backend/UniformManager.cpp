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
    crList<VkDescriptorBindingFlags>        bindingFlags;
    crList<VkDescriptorSetLayoutBinding>    storageBindings;
    crList<VkDescriptorBufferInfo>          descriptorBufferInfo; 
    crList<VkWriteDescriptorSet>            writeDescriptorSet;

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

    /// 
    bindingFlags.Resize( k_DESCRIPTOR_BINDIN_COUNT );
    storageBindings.Resize( k_DESCRIPTOR_BINDIN_COUNT );
    writeDescriptorSet.Resize( k_DESCRIPTOR_BINDIN_COUNT );

    /// bindless texture samples
    bindingFlags[k_BINDLESS_SAMPLERS_BINDING] = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT | VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT; //
    storageBindings[k_BINDLESS_SAMPLERS_BINDING].binding = 0;
    storageBindings[k_BINDLESS_SAMPLERS_BINDING].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    storageBindings[k_BINDLESS_SAMPLERS_BINDING].descriptorCount = k_MAX_SAMPLERS_BINDING;
    storageBindings[k_BINDLESS_SAMPLERS_BINDING].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; // we can use samples for vertex displace
    storageBindings[k_BINDLESS_SAMPLERS_BINDING].pImmutableSamplers = nullptr;

    descriptorBufferInfo[k_BINDLESS_SAMPLERS_BINDING];

    /// Mesh uniforms 
    bindingFlags[k_MESH_UNIFORM_BINDING] = 0;
    storageBindings[k_MESH_UNIFORM_BINDING].binding = k_MESH_UNIFORM_BINDING;
    storageBindings[k_MESH_UNIFORM_BINDING].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    storageBindings[k_MESH_UNIFORM_BINDING].descriptorCount = 1;
    storageBindings[k_MESH_UNIFORM_BINDING].stageFlags = VK_SHADER_STAGE_VERTEX_BIT; /// only in vertex shader stage
    storageBindings[k_MESH_UNIFORM_BINDING].pImmutableSamplers = nullptr;

    // Material uniforms 
    bindingFlags[k_MATERIAL_UNIFORM_BINDING] = 0;
    storageBindings[k_MATERIAL_UNIFORM_BINDING].binding = k_MATERIAL_UNIFORM_BINDING;
    storageBindings[k_MATERIAL_UNIFORM_BINDING].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    storageBindings[k_MATERIAL_UNIFORM_BINDING].descriptorCount = 1;
    storageBindings[k_MATERIAL_UNIFORM_BINDING].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; /// only in fragment shader stage
    storageBindings[k_MATERIAL_UNIFORM_BINDING].pImmutableSamplers = nullptr;

    // Light uniforms 
    bindingFlags[k_LIGTH_UNIFORM_BINDING] = 0;
    storageBindings[k_LIGTH_UNIFORM_BINDING].binding = k_LIGTH_UNIFORM_BINDING;
    storageBindings[k_LIGTH_UNIFORM_BINDING].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    storageBindings[k_LIGTH_UNIFORM_BINDING].descriptorCount = 1;
    storageBindings[k_LIGTH_UNIFORM_BINDING].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; /// only in fragment shader stage
    storageBindings[k_LIGTH_UNIFORM_BINDING].pImmutableSamplers = nullptr;

    /// create the pipeline layout
    m_layout = new crPipelineLayout();
    if( !m_layout->Create( storageBindings, bindingFlags, k_MAX_SAMPLERS_BINDING ) )
        throw crException( "Failed to create the uniform buffers pipeline layout\n" );

    /// buffer bindings
    m_layout->SetBuffers( writeDescriptorSet );
}

void crUniformManager::ShutDown( void )
{
    if ( m_layout != nullptr )
    {
        delete m_layout;
        m_layout = nullptr;
    }

    if( m_lightSSBO != nullptr )
    {
        delete m_lightSSBO;
        m_lightSSBO = nullptr;
    }

    if( m_materialSSBO != nullptr )
    {
        delete m_materialSSBO;
        m_materialSSBO = nullptr;
    }
    
    if( m_meshSSBO != nullptr )
    {
        delete m_meshSSBO;
        m_meshSSBO = nullptr;
    }
}