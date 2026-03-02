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

static uint32_t descriptorCountForBinding0 = 4096; // ex: 4096
static uint32_t k_MAX_BINDING_SAMPLERS = 4096;

crPipelineLayout::crPipelineLayout( void ) : m_layout( nullptr )
{
}

crPipelineLayout::~crPipelineLayout( void )
{
    Destroy();
}

bool crPipelineLayout::Create( const crList<VkDescriptorSetLayoutBinding> &in_storageBindings, const crList<VkDescriptorBindingFlags> &in_bindingFlags )
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
    result = vkCreateDescriptorSetLayout( *device, &descriptorSetLayout, k_allocationCallbacks, &m_descriptor );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create:vkCreateDescriptorSetLayout Failed!\n%s\n", VulkanErrorString( result ) );
        return false;
    }

    ///
    /// Descriptor Pool Sizes
    /// 
    VkDescriptorPoolSize poolSizes[2]{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = k_MAX_BINDING_SAMPLERS;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = 16; // quantos SSBOs você planeja (normalmente 1 por binding)

    ///
    /// Descriptor Pool
    ///
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 1; // provavelmente 1 set global

    result = vkCreateDescriptorPool( *device, &poolInfo, nullptr, &m_descriptorPool );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "vkCreateDescriptorPool %s\n", VulkanErrorString( result ) );
        return false;
    }

    ///
    /// variable descriptor count
    ///
    VkDescriptorSetVariableDescriptorCountAllocateInfo  varCountAlloc{};
    varCountAlloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    varCountAlloc.descriptorSetCount = 1;
    varCountAlloc.pDescriptorCounts = &descriptorCountForBinding0;

    ///
    /// Descriptor Set Allocate Info
    ///
    VkDescriptorSetAllocateInfo descriptorSet{};
    descriptorSet.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSet.pNext = &varCountAlloc;
    descriptorSet.descriptorPool = m_descriptorPool;
    descriptorSet.descriptorSetCount = 1;
    descriptorSet.pSetLayouts = &m_descriptor;
    result = vkAllocateDescriptorSets( *device, &descriptorSet, &m_bindlessSet );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create::vkAllocateDescriptorSets failed\n %s\n", VulkanErrorString( result ) );
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
    pipelineLayoutCI.pSetLayouts = &m_descriptor;
    pipelineLayoutCI.pushConstantRangeCount = 0;
    pipelineLayoutCI.pPushConstantRanges = nullptr;
    result = vkCreatePipelineLayout( *device, &pipelineLayoutCI, k_allocationCallbacks, &m_layout );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create: Failed\n%s\n", VulkanErrorString( result ) );
        return false;
    }

    return true;
}

void crPipelineLayout::Destroy(void)
{
    auto device = crContext::Get()->Device();

    if( m_bindlessSet != nullptr )
    {
        vkFreeDescriptorSets( *device, m_descriptorPool, 2, &m_bindlessSet );
        m_bindlessSet = nullptr;
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
