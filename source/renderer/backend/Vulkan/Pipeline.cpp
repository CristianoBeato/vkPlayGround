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

crPipeline::crPipeline( void ) : 
    m_identity( 0 ),
    m_flags( 0 ),
    m_vertexShader( 0 ),
    m_fragmentShader( 0 ),
    m_pipeline( nullptr )
{
}

crPipeline::~crPipeline( void )
{
}

bool crPipeline::Create( const uint64_t in_flags, const uint32_t in_vertexProgramID, const uint32_t in_fragmentProgramID, const crPipeline* in_base )
{
    auto device = crContext::Get()->Device();
    VkPipelineShaderStageCreateInfo pipelineShaderStage[2]{};
    crPipelineManager*              pipelineManager = crPipelineManager::Get();
    crUniformManager*               uniformManager = crUniformManager::Get();
    auto cache = pipelineManager->Cache();
   
    VkDynamicState dynamicStates[] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE
    };

    m_flags = in_flags;
    m_vertexShader = in_vertexProgramID;
    m_fragmentShader = in_fragmentProgramID;

    /// shader stage programs
    auto vertexProg = pipelineManager->GetProgram( m_vertexShader );
    auto fragmentProg = pipelineManager->GetProgram( m_fragmentShader );

    // if programs are loaded
    if ( vertexProg == nullptr || fragmentProg == nullptr )
    {
        crConsole::Error("");

    }

    pipelineShaderStage[0] = vertexProg->Program();
    pipelineShaderStage[1] = fragmentProg->Program();
    

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
    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;;
    vertexInput.pNext = nullptr;
    vertexInput.flags = 0;
    vertexInput.vertexBindingDescriptionCount = VERTEX_BINDING_COUNT;
    vertexInput.pVertexBindingDescriptions = vertexInputBindingDescription;
    vertexInput.vertexAttributeDescriptionCount = VERTEX_ATTRIBUTE_COUNT;
    vertexInput.pVertexAttributeDescriptions = vertexInputAttributeDescription;

    ///
    /// Input Assembly
    /// Describes what kind of geometry will be drawn from the vertices and if primitive restart
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;;
    inputAssembly.pNext = nullptr;
    inputAssembly.flags = 0;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    ///
    /// Tessellation State
    /// Control tesselation path
    VkPipelineTessellationStateCreateInfo tessellation{};
    tessellation.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellation.pNext = nullptr;
    tessellation.flags = 0;
    tessellation.patchControlPoints = 0;

    ///
    /// Viewport State
    /// Viewport and scissor configuration ( not set in pipeline, dynamic )
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = nullptr; // Dynamically defined 
    viewportState.scissorCount = 1;
    viewportState.pScissors = nullptr; // Dynamically defined 

    ///
    /// Rasterization State
    ///
    VkPipelineRasterizationStateCreateInfo rasterization{};
    rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization.pNext = nullptr;
    rasterization.flags = 0;
    rasterization.depthClampEnable = VK_FALSE;
    rasterization.rasterizerDiscardEnable = VK_FALSE;
    rasterization.polygonMode = ( m_flags & PLS_POLYMODE_LINE ) == PLS_POLYMODE_LINE ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
    rasterization.cullMode = VK_CULL_MODE_NONE;
    rasterization.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterization.depthBiasEnable = VK_FALSE;
    rasterization.depthBiasConstantFactor = 0.0f;
    rasterization.depthBiasClamp = 0.0f;
    rasterization.depthBiasSlopeFactor = 0.0f;
    rasterization.lineWidth = 1.0f;

    uint64_t cullVal = (m_flags & PLS_CULLFACE_BITS) >> 0;
    if (cullVal == ( PLS_CULLFACE_BACK >> 0 )) 
        rasterization.cullMode = VK_CULL_MODE_BACK_BIT;
    else if (cullVal == ( PLS_CULLFACE_FRONT >> 0 ) )
        rasterization.cullMode = VK_CULL_MODE_FRONT_BIT;
    else 
        rasterization.cullMode = VK_CULL_MODE_NONE;

    ///
    /// Multisample State
    /// configure multisample state
    VkPipelineMultisampleStateCreateInfo multisample{};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.pNext = nullptr;
    multisample.flags = 0;
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //TODO: cvar for multisampling ( on that we required )
    multisample.sampleShadingEnable = VK_FALSE;
    multisample.minSampleShading = 0.1;
    multisample.pSampleMask = 0;
    multisample.alphaToCoverageEnable = VK_FALSE;
    multisample.alphaToOneEnable = VK_FALSE;

    ///
    ///
    ///
    VkPipelineDepthStencilStateCreateInfo depthStencil{};

    ///
    ///
    ///
    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    
    ///
    ///
    ///
    VkGraphicsPipelineCreateInfo pipelineCI{};
    pipelineCI.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCI.pNext = nullptr;
    pipelineCI.flags = 0;
    pipelineCI.stageCount = 2;
    pipelineCI.pStages = pipelineShaderStage;
    pipelineCI.pVertexInputState = &vertexInput;
    pipelineCI.pInputAssemblyState = &inputAssembly;
    pipelineCI.pTessellationState = &tessellation;
    pipelineCI.pViewportState = &viewportState;
    pipelineCI.pRasterizationState = &rasterization;
    pipelineCI.pMultisampleState = &multisample;
    pipelineCI.pDepthStencilState = &depthStencil;
    pipelineCI.pColorBlendState = &colorBlendState;
    pipelineCI.pDynamicState = &dynamicState;
    pipelineCI.layout = *uniformManager->Layout();
    pipelineCI.renderPass = nullptr;
    pipelineCI.subpass = 0;

    if( in_base != nullptr )
    {
        pipelineCI.basePipelineHandle = *in_base;
        pipelineCI.basePipelineIndex = in_base->PipelineID();
    }
    else
    {
        pipelineCI.basePipelineHandle = nullptr;
        pipelineCI.basePipelineIndex = 0;
    }

    auto result = vkCreateGraphicsPipelines( *device, *cache, 1, &pipelineCI, k_allocationCallbacks, &m_pipeline );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipeline::Create::vkCreateGraphicsPipelines Failed\n %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    return true;
}

bool crPipeline::operator==(const crPipeline &in_reference) const
{
    return ( m_flags == in_reference.m_flags ) && ( m_vertexShader == in_reference.m_flags ) && ( m_fragmentShader == in_reference.m_fragmentShader );
}

bool crPipeline::operator!=(const crPipeline &in_reference) const
{
    return ( m_flags != in_reference.m_flags ) && ( m_vertexShader != in_reference.m_flags ) && ( m_fragmentShader != in_reference.m_fragmentShader );
}
