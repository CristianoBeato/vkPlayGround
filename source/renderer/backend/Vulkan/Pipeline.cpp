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

#include "Pipeline.hpp"
#include "Core.hpp"

crPipeline::crPipeline( void )
{
}

crPipeline::~crPipeline( void )
{
}

bool crPipeline::Create( const uint64_t in_flags, const uint32_t in_vertexProgramID, const uint32_t in_fragmentProgramID )
{
    auto device = crContext::Get()->Device();
    auto cache = crPipelineManager::Get()->Cache();
    VkDynamicState dynamicStates[] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    m_flags = in_flags;
    m_vertexShader = in_vertexProgramID;
    m_fragmentShader = in_fragmentProgramID;

    /// Dynamic state
    /// While most of the pipeline state needs to be baked into the pipeline state, a
    /// limited amount of the state can actually be changed without recreating the
    /// pipeline at draw time.
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    ///
    /// Bindings
    /// Describe vertex shader attributes buffer bindings
    VkVertexInputBindingDescription vertexInputBindingDescription[VERTEX_BINDING_COUNT]
    {
        { VERTEX_BINDING_POSI, sizeof( VertPos_t ), VK_VERTEX_INPUT_RATE_VERTEX },
        { VERTEX_BINDING_NORM, sizeof( VertSkin_t ), VK_VERTEX_INPUT_RATE_VERTEX },
        { VERTEX_BINDING_SKIN, sizeof( VertSkin_t ), VK_VERTEX_INPUT_RATE_VERTEX }
    };

    ///
    /// Attribute Description
    /// Describe vertex components
    VkVertexInputAttributeDescription vertexInputAttributeDescription[VERTEX_ATTRIBUTE_COUNT]
    {
        { VERTEX_ATTRIBUTE_POSI, VERTEX_BINDING_POSI, VK_FORMAT_R32G32B32_SFLOAT,   0 },
        { VERTEX_ATTRIBUTE_TEXT, VERTEX_BINDING_POSI, VK_FORMAT_R16G16_SFLOAT,      sizeof( float ) * 3 },
        { VERTEX_ATTRIBUTE_NORM, VERTEX_BINDING_NORM, VK_FORMAT_R8G8B8A8_UNORM,     0 },
        { VERTEX_ATTRIBUTE_TANG, VERTEX_BINDING_NORM, VK_FORMAT_R8G8B8A8_UNORM,     sizeof( uint8_t ) * 4 },
        { VERTEX_ATTRIBUTE_STAR, VERTEX_BINDING_SKIN, VK_FORMAT_R32_SINT,           0 },
        { VERTEX_ATTRIBUTE_COUN, VERTEX_BINDING_SKIN, VK_FORMAT_R32_SINT,           sizeof( int32_t ) }
    };

    /// 
    /// Vertex input
    /// describes the format of the vertex data that will be passed to the vertex shader
    VkPipelineVertexInputStateCreateInfo vertexInputStateCI{};
    vertexInputStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;;
    vertexInputStateCI.pNext = nullptr;
    vertexInputStateCI.flags = 0;
    vertexInputStateCI.vertexBindingDescriptionCount = VERTEX_BINDING_COUNT;
    vertexInputStateCI.pVertexBindingDescriptions = vertexInputBindingDescription;
    vertexInputStateCI.vertexAttributeDescriptionCount = VERTEX_ATTRIBUTE_COUNT;
    vertexInputStateCI.pVertexAttributeDescriptions = vertexInputAttributeDescription;

    ///
    /// Input Assembly
    /// Describes what kind of geometry will be drawn from the vertices and if primitive restart
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;;
    inputAssemblyState.pNext = nullptr;
    inputAssemblyState.flags = 0;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    ///
    ///
    ///
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr; // Dynamically defined 
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr; // Dynamically defined 

    ///
    ///
    ///
    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.pNext = nullptr;
    rasterizationState.flags = 0;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode = ( m_flags & PLS_POLYMODE_LINE ) == PLS_POLYMODE_LINE ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_NONE;
    rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthBiasEnable = ;
    rasterizationState.depthBiasConstantFactor = ;
    rasterizationState.depthBiasClamp = ;
    rasterizationState.depthBiasSlopeFactor = ;
    rasterizationState.lineWidth = 1.0f;

    if (  m_flags & PLS_CULLFACE_BACK )
        rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    else if(  m_flags & PLS_CULLFACE_FRONT )
        rasterizationState.cullMode = VK_CULL_MODE_FRONT_BIT;
    else if(  m_flags & PLS_CULLFACE_TWO )
        rasterizationState.cullMode = VK_CULL_MODE_FRONT_AND_BACK;

    VkGraphicsPipelineCreateInfo pipelineCI{};

    auto result = vkCreateGraphicsPipelines( *device, cache, 1, &pipelineCI, m_pipeline )
    return true;
}
