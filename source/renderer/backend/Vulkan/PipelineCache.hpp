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

#ifndef __PIPELINE_CACHE_HPP__
#define __PIPELINE_CACHE_HPP__

#pragma pack(push, 1)

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
    uint32_t dataHash = 0;
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

#pragma pop()

class crPipelineCache
{
public:
    crPipelineCache( void );
    ~crPipelineCache( void );

    bool    OpenCache( const crString &in_path );
    bool    SaveCache( const crString &in_path );

    operator VkPipelineCache( void ) const { return m_cache; }
private:
    bool                m_loaded;   // if cache is loaded, we don't save it again
    VkPipelineCache     m_cache;
};

#endif //__PIPELINE_CACHE_HPP__