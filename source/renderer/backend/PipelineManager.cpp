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

#include "PipelineManager.hpp"

constexpr uint32_t k_PIPELINE_CACHE_FILE_VERSION = 10; // 1.0

enum programs_e
{
    PROGRAM_VERTEX_COLOR,
    PROGRAM_VERTEX_TEXTURED,
    PROGRAM_VERTEX_TEXTURED_SKINED,
    PROGRAM_VERTEX_TEXTURED_MATERIAL,
    PROGRAM_VERTEX_GEOMETRYPASS,
    PROGRAM_VERTEX_GEOMETRYPASS_SKINED,
    PROGRAM_VERTEX_GEOMETRYPASS_MATERIAL,
    PROGRAM_FRAGMENT_COLOR,
    PROGRAM_FRAGMENT_TEXTURED,
    PROGRAM_FRAGMENT_GEOMETRYPASS,
    PROGRAM_FRAGMENT_ADDITIVE,
    PROGRAM_COUNT    
};

struct cache_header_t
{
    // an arbitrary magic header to make sure this is actually our file
    uint32_t magic = 0;
    // version of the cache manager
    uint32_t version = 0;
    // this header length
    uint32_t length = 0;
    // equal to *pDataSize returned by vkGetPipelineCacheData   
    uint32_t dataSize = 0;
    // a hash of pipeline cache data, including the header
    uint64_t dataHash = 0;
    // equal to VkPhysicalDeviceProperties::vendorID
    uint32_t vendorID = 0;
    // equal to VkPhysicalDeviceProperties::deviceID
    uint32_t deviceID = 0;
    // VkPhysicalDeviceProperties::driverVersion
    uint32_t driverVersion = 0;
    // equal to sizeof(void*)
    uint32_t driverABI = 0;
    // equal to VkPhysicalDeviceProperties::pipelineCacheUUID 
    uint8_t uuid[VK_UUID_SIZE]; // Device driver info
};

crPipelineManager::crPipelineManager( void )
{
}

crPipelineManager::~crPipelineManager( void )
{
}

void crPipelineManager::StartUp(void)
{
    auto device = crContext::Get()->Device();

    VkPipelineCacheCreateInfo pipelineCacheCI{};
    pipelineCacheCI.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheCI.pNext = nullptr;
    pipelineCacheCI.flags = 0;
    pipelineCacheCI.initialDataSize = 0; // 0 para cache novo
    pipelineCacheCI.pInitialData = nullptr;
    auto result = vkCreatePipelineCache( *device, &pipelineCacheCI, k_allocationCallbacks, &m_cache );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineManager::StartUp::vkCreatePipelineCache %s\n", VulkanErrorString( result ).c_str() );
        throw crException( "Failed to create pipeline cache" );
    }
}

void crPipelineManager::ShutDown(void)
{
    auto device = crContext::Get()->Device();
    if ( m_cache != nullptr )
    {
        vkDestroyPipelineCache( *device, m_cache, k_allocationCallbacks );
        m_cache = nullptr;
    }
}

crPipeline *crPipelineManager::Pipelines(const uint64_t in_flags, const uint32_t in_vertexShader, const uint32_t in_fragmentShader)
{
    return nullptr;

    
}

bool crPipelineManager::SaveCache(void)
{
    cache_header_t header{};

    

    return false;
}
