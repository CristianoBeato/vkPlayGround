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

/// the cache will be stored in the user space path
static const char* k_CACHE_FILE = { "generated/cache/pipeline.pcf" };
static const char* k_CACHE_PATH = { "generated/cache/" };

crPipelineManager *crPipelineManager::Get(void)
{
    static crPipelineManager gPipelineManager = crPipelineManager();
    return &gPipelineManager;
}

crPipelineManager::crPipelineManager(void) : m_cacheModified( false )
{
}

crPipelineManager::~crPipelineManager( void )
{
}

void crPipelineManager::StartUp(void)
{
    m_cache = new crPipelineCache();

    auto fs = crFileSystem::Get();

    /// create cache storage path if don't exist
    if ( !fs->PathExist( k_CACHE_PATH, true ) )
        fs->CreatePath( k_CACHE_PATH, true );
    

    if( !m_cache->OpenCache( k_CACHE_FILE ) )
        crConsole::Error( "Failed to create pipeline cache\n");

    m_cacheModified = false;
}

void crPipelineManager::ShutDown(void)
{
    if ( m_cache != nullptr )
    {
        /// save pipeline cache before exit, if we have changes
        if( m_cacheModified || !m_cache->Loaded() )
            m_cache->SaveCache( k_CACHE_FILE );

        delete m_cache;
        m_cache = nullptr;
    }
    
}

crPipeline *crPipelineManager::Pipelines(const uint64_t in_flags, const uint32_t in_vertexShader, const uint32_t in_fragmentShader)
{
    
    return nullptr;    
}

crProgram *crPipelineManager::GetProgram(const uint32_t in_progID)
{
    return m_programs[in_progID];
}

uint32_t crPipelineManager::GetNewIndex(void)
{
    return m_pipelineCount++;
}
