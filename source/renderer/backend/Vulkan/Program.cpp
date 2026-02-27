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

#include "Program.hpp"

/// we use GLSL4.6 compiled source, aways a void main( void ) { ... } entry
static const char* pName = "main"; 

crProgram::crProgram( void )
{
    std::memset( &m_shaderStageInfo, 0x00, sizeof(VkPipelineShaderStageCreateInfo) );
}

crProgram::~crProgram( void )
{
    Destroy();
}

bool crProgram::Create( const uint32_t* in_source, const size_t in_size, const VkShaderStageFlagBits in_stage, const uint16_t in_ID )
{
    auto device = crContext::Get()->Device();

    m_shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_shaderStageInfo.pNext = nullptr;
    m_shaderStageInfo.flags = 0;
    m_shaderStageInfo.stage = in_stage;
    m_shaderStageInfo.module = nullptr;
    m_shaderStageInfo.pName = pName;
    m_shaderStageInfo.pSpecializationInfo = nullptr;

    VkShaderModuleCreateInfo shaderModuleCI{};
    shaderModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCI.pNext = nullptr;
    shaderModuleCI.flags = 0;
    shaderModuleCI.codeSize = in_size;
    shaderModuleCI.pCode = in_source;

    auto result = vkCreateShaderModule( *device, &shaderModuleCI, k_allocationCallbacks, &m_shaderStageInfo.module );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crProgram::Create::vkCreateShaderModule %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    return true;
}

void crProgram::Destroy(void)
{
    auto device = crContext::Get()->Device();
    if ( m_shaderStageInfo.module != nullptr )
    {
        vkDestroyShaderModule( *device, m_shaderStageInfo.module, k_allocationCallbacks );
        m_shaderStageInfo.module = nullptr;
    }
}
