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

#include "PipelineCache.hpp"
#include "Core.hpp"

constexpr uint32_t k_PIPELINE_CACHE_FILE_MAGIC = 'PCF/0';
constexpr uint32_t k_PIPELINE_CACHE_FILE_VERSION = 10; // 1.0
constexpr uint32_t k_PIPELINE_CAHCE_FILE_SEED = 0x1337;

crPipelineCache::crPipelineCache( void ) :
    m_loaded( false ),
    m_cache( nullptr )
{
}

crPipelineCache::~crPipelineCache( void )
{
}

bool crPipelineCache::OpenCache( const crString &in_path )
{
    crFile* cacheFile = nullptr;
    cache_header_t header{};
    VkPipelineCacheCreateInfo pipelineCacheCI{};
    auto device = crContext::Get()->Device(); // retrieve current device
    auto inf = device->DevInfo();
    auto fs = crFileSystem::Get();

    /// 
    pipelineCacheCI.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheCI.pNext = nullptr;
    pipelineCacheCI.flags = 0;
    
    // no cache available
    pipelineCacheCI.initialDataSize = 0;    
    pipelineCacheCI.pInitialData = nullptr;
    m_loaded = false;

    /// Try read cache file from save dir
    cacheFile = fs->Open( "generated/cache/pipeline.pcf", crFileSystem::FS_OPEN_READ | crFileSystem::FS_OPEN_SAVE_PATH ); 
    if ( cacheFile != nullptr )
    {
        /// retrieve cache header
        cacheFile->Read( &header, sizeof( cache_header_t), 1 );

        /// check device cache compatibility
        if( ( header.magic == k_PIPELINE_CACHE_FILE_MAGIC ) || 
            ( header.version == k_PIPELINE_CACHE_FILE_VERSION ) || 
            ( header.length == sizeof( cache_header_t ) ) )
        {
            /// validate cache compatibility
            if( ( header.driverABI != inf.driverABI ) ||
                ( header.vendorID != inf.vendorID ) ||
                ( header.deviceID != inf.deviceID ) ||
                ( header.driverVersion != inf.driverVersion ) ||
                ( std::memcmp( header.uuid, inf.uuid, sizeof(uint8_t) * VK_UUID_SIZE ) != 0 ) )
            {
                void* cacheData = MemAlloc( header.dataSize );
                /// read the cache 
                cacheFile->Read( cacheData, header.dataSize, 1 );

                auto hash = SDL_murmur3_32( cacheData, header.dataSize, k_PIPELINE_CAHCE_FILE_SEED );
                if ( hash == header.dataHash )
                {
                    /// A valid cache available
                    pipelineCacheCI.initialDataSize = header.dataSize;
                    pipelineCacheCI.pInitialData = cacheData;
                    m_loaded = true;
                }
                else
                {
                    MemFree( cacheData );
                    crConsole::Error( "Pipeline cache invalid data hash!\n" );
                }
            }
            else
                crConsole::Warning( "Pipeline Cache Out of date, rebuilt!\n" );
        }
        else
            crConsole::Warning( "Unconpatible pipeline cache, engine may updated, rebuild!\n" );
        
        fs->Close( cacheFile );
    }
    else
        crConsole::Warning( "Can't read cache file from disk, pipeline conpilation can be slow\n" );
    
    /// create a cache from source, or allocate for new one
    auto result = vkCreatePipelineCache( *device, &pipelineCacheCI, k_allocationCallbacks, &m_cache );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineManager::StartUp::vkCreatePipelineCache %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    /// don't leak memory 
    if( pipelineCacheCI.pInitialData != nullptr )
        MemFree( const_cast<void*>( pipelineCacheCI.pInitialData ) );

    return true;
}

bool crPipelineCache::SaveCache( const crString &in_path )
{
    size_t cacheSize = 0;
    VkResult result = VK_SUCCESS;
    void* cacheData = nullptr;
    crFile* cacheFile = nullptr;
    if( m_loaded )
        return true;

    auto fs = crFileSystem::Get();
    auto device = crContext::Get()->Device(); // retrieve current device
    auto inf = device->DevInfo();

    cache_header_t header{};
    header.magic = k_PIPELINE_CACHE_FILE_MAGIC;
    header.version = k_PIPELINE_CACHE_FILE_VERSION;
    header.length = sizeof( cache_header_t );
    header.dataSize = 0; 
    header.dataHash = 0;
    header.vendorID = inf.vendorID;
    header.deviceID = inf.deviceID;
    header.driverVersion = inf.driverVersion;
    header.driverABI = inf.driverABI;

    /// retrieve the cache data size 
    result = vkGetPipelineCacheData( *device, m_cache, &cacheSize, nullptr );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "Failed to retrive pipeline cache size\n%s\n", VulkanErrorString( result ).c_str() );
        return false;
    }
    
    /// alloc cache data
    cacheData = MemAlloc( cacheSize );

    /// retrieve the cache source data 
    result = vkGetPipelineCacheData( *device, m_cache, &cacheSize, cacheData );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "Failed to retrive pipeline cache data\n%s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    /// store cache size
    header.dataSize = cacheSize;
    
    /// hash cache data to prevent file corruption
    header.dataHash =  SDL_murmur3_32( cacheData, cacheSize, k_PIPELINE_CAHCE_FILE_SEED );

    /// Try create cache file in the save directory
    cacheFile = fs->Open( "generated/cache/pipeline.pcf", crFileSystem::FS_OPEN_WRITE | crFileSystem::FS_OPEN_SAVE_PATH ); 
    if ( !cacheFile )
    {
        MemFree( cacheData );
        crConsole::Error( "Can't write cache file to disk\n" );
        return false;
    }

    /// write the header
    cacheFile->Write( &header, header.length, 1 );
    cacheFile->Write( cacheData, cacheSize, 1 );

    MemFree( cacheData );
    fs->Close( cacheFile );

    return true;
}
