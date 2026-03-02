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

#include "PipelineLayout.hpp"
#include "Core.hpp"

crPipelineLayout::crPipelineLayout( void ) : m_layout( nullptr )
{
}

crPipelineLayout::~crPipelineLayout( void )
{
    Destroy();
}

bool crPipelineLayout::Create( const crList<VkDescriptorSetLayoutBinding> &in_storageBindings, const crList<VkDescriptorBindingFlags> &in_bindingFlags, const uint32_t in_maxBindingSamples )
{
    VkResult result = VK_SUCCESS;
    auto device = crContext::Get()->Device();

    ///
    /// Descriptor Set Layout Binding Flags 
    /// configure layout bind flags 
    VkDescriptorSetLayoutBindingFlagsCreateInfo descriptorSetLayoutBindingFlags{};
    descriptorSetLayoutBindingFlags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    descriptorSetLayoutBindingFlags.bindingCount = in_bindingFlags.Count();
    descriptorSetLayoutBindingFlags.pBindingFlags = in_bindingFlags.GetData();

    ///
    /// Descriptor Set Layout
    ///
    VkDescriptorSetLayoutCreateInfo descriptorSetLayout{};
    descriptorSetLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayout.pNext = &descriptorSetLayoutBindingFlags;
    descriptorSetLayout.bindingCount = in_storageBindings.Count();
    descriptorSetLayout.pBindings = in_storageBindings.GetData();
    descriptorSetLayout.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT; // permit update-after-bind pool behavior:
    result = vkCreateDescriptorSetLayout( *device, &descriptorSetLayout, k_allocationCallbacks, &m_descriptorLayout );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create:vkCreateDescriptorSetLayout Failed!\n%s\n", VulkanErrorString( result ) );
        return false;
    }

    ///
    /// Pipeline Layout
    ///
    VkPipelineLayoutCreateInfo pipelineLayoutCI{};
    pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCI.pNext = nullptr;
    pipelineLayoutCI.flags = 0;
    pipelineLayoutCI.setLayoutCount = 1;
    pipelineLayoutCI.pSetLayouts = &m_descriptorLayout;
    pipelineLayoutCI.pushConstantRangeCount = 0;
    pipelineLayoutCI.pPushConstantRanges = nullptr;
    result = vkCreatePipelineLayout( *device, &pipelineLayoutCI, k_allocationCallbacks, &m_layout );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create:vkCreatePipelineLayout Failed\n%s\n", VulkanErrorString( result ) );
        return false;
    }

    ///
    /// Descriptor Pool Sizes
    /// Describe which descriptor types our descriptor sets are going to contain and how many of them.
    VkDescriptorPoolSize poolSizes[2]{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = in_maxBindingSamples;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = 3; // how many SSBO we want bind ( normally 1 per binding )

    ///
    /// Descriptor Pool
    /// allocate one of these descriptors for every frame. This pool size structure is referenced by the main VkDescriptorPoolCreateInfo
    VkDescriptorPoolCreateInfo descriptorPoolCI{};
    descriptorPoolCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCI.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
    descriptorPoolCI.poolSizeCount = 2;
    descriptorPoolCI.pPoolSizes = poolSizes;
    descriptorPoolCI.maxSets = 1; // one global set
    result = vkCreateDescriptorPool( *device, &descriptorPoolCI, nullptr, &m_descriptorPool );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create:vkCreateDescriptorPool %s\n", VulkanErrorString( result ) );
        return false;
    }

    ///
    /// variable descriptor count
    ///
    VkDescriptorSetVariableDescriptorCountAllocateInfo descriptorVariableDescriptorCount{};
    descriptorVariableDescriptorCount.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    descriptorVariableDescriptorCount.descriptorSetCount = 1;
    descriptorVariableDescriptorCount.pDescriptorCounts = &in_maxBindingSamples;

    /// Descriptor Set Allocate Info
    /// You need to specify the descriptor pool to allocate from, the number of descriptor sets to allocate, 
    /// and the descriptor set layout to base them on
    VkDescriptorSetAllocateInfo descriptorSet{};
    descriptorSet.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSet.pNext = &descriptorVariableDescriptorCount;
    descriptorSet.descriptorPool = m_descriptorPool;
    descriptorSet.descriptorSetCount = 1;
    descriptorSet.pSetLayouts = &m_descriptorLayout;
    result = vkAllocateDescriptorSets( *device, &descriptorSet, &m_descriptorSet );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create::vkAllocateDescriptorSets failed\n %s\n", VulkanErrorString( result ) );
        return false;
    }

    return true;
}

void crPipelineLayout::Destroy(void)
{
    auto device = crContext::Get()->Device();

    if( m_descriptorSet != nullptr )
    {
        vkFreeDescriptorSets( *device, m_descriptorPool, 2, &m_descriptorSet );
        m_descriptorSet = nullptr;
    }

    if( m_descriptorLayout != nullptr )
    {
        vkDestroyDescriptorSetLayout( *device, m_descriptorLayout, k_allocationCallbacks );
        m_descriptorSet = nullptr;
    }

    if( m_descriptorPool != nullptr )
    {
        vkDestroyDescriptorPool( *device, m_descriptorPool, k_allocationCallbacks );
        m_descriptorPool = nullptr;
    }

    if( m_layout != nullptr )
    {
        vkDestroyPipelineLayout( *device, m_layout, k_allocationCallbacks );
        m_layout = nullptr;
    }
}

void crPipelineLayout::SetBuffers( const crList<VkWriteDescriptorSet> in_buffers )
{
    auto device = crContext::Get()->Device();
    vkUpdateDescriptorSets( *device, in_buffers.Count(), in_buffers.GetData(), 0, nullptr);
}

void crPipelineLayout::Bind(const VkCommandBuffer in_commandBuffer )
{
    uint32_t firstSet = 0;
    VkPipelineBindPoint bindPoint{};
    vkCmdBindDescriptorSets( in_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_layout, firstSet, 1, &m_descriptorSet, 0, nullptr );
}