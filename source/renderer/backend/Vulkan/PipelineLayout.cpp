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

/*
================================================================================================
crSamplerSlot
================================================================================================
*/
crSamplerSlot::crSamplerSlot( void ) : m_index( 0xFFFFFFFF ) 
{
}

crSamplerSlot::crSamplerSlot(const VkImageView in_imageView, const VkSampler in_sampler, const VkImageLayout in_imageLayout)
{
    m_descriptorImageInfo.imageLayout = in_imageLayout;
    m_descriptorImageInfo.imageView = in_imageView;
    m_descriptorImageInfo.sampler = in_sampler;
}

crSamplerSlot::~crSamplerSlot(void)
{
    m_descriptorImageInfo.imageView = nullptr;
    m_descriptorImageInfo.sampler = nullptr;
}

/*
================================================================================================
crSamplerSlotArray
================================================================================================
*/
crSamplerSlotArray::crSamplerSlotArray( void )
{
}

crSamplerSlotArray::~crSamplerSlotArray( void )
{
}

bool crSamplerSlotArray::Create( void )
{
    VkResult result = VK_SUCCESS;
    auto device = crContext::Get()->Device();
    
    ///
    /// VkDescriptorBindingFlags
    /// configure descriptor binding flags 
    VkDescriptorBindingFlags bindingFlagsArr = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;
    VkDescriptorSetLayoutBindingFlagsCreateInfo flagsCreateInfo{};
    flagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    flagsCreateInfo.bindingCount = 1;
    flagsCreateInfo.pBindingFlags = &bindingFlagsArr;

    VkDescriptorSetLayoutBinding binding{};
    binding.binding = 0; // layout( set = 0, binding = 0 ) uniform sampler texturasGlobais[N]; 
    binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    binding.descriptorCount = 1; // placeholder (variable at alloc time)
    binding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; // can be acessed by the vertex or frament shaders
    binding.pImmutableSamplers = nullptr;

    ///
    /// VkDescriptorSetLayoutCreateInfo
    /// configure layout bindless descriptor
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = &flagsCreateInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &binding;
    layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT; // enable update-after-bind pool behavior:
    result = vkCreateDescriptorSetLayout( *device, &layoutInfo, k_allocationCallbacks, &m_bindlessLayout );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "vkCreateDescriptorSetLayout %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    ///
    /// VkDescriptorPoolSize
    /// store total image sampler binding count
    VkDescriptorPoolSize    poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = MAX_BINDING_SAMPLERS; 
    
    ///
    /// VkDescriptorPoolCreateInfo
    ///
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT; /// we can update the descriptor after binding 
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 1; /// A global set at begin of the render 
    result = vkCreateDescriptorPool( *device, &poolInfo, nullptr, &m_bindlessPool );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "vkCreateDescriptorPool %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    ///
    ///
    ///
    static uint32_t descriptorCountForBinding0 = MAX_BINDING_SAMPLERS;
    VkDescriptorSetVariableDescriptorCountAllocateInfo  varCountAlloc{};    
    varCountAlloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    varCountAlloc.descriptorSetCount = 1;
    varCountAlloc.pDescriptorCounts = &descriptorCountForBinding0;
    
    ///
    ///
    ///
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = &varCountAlloc;
    allocInfo.descriptorPool = m_bindlessPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_bindlessLayout;

    result = vkAllocateDescriptorSets( *device, &allocInfo, &m_bindlessSet );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "vkAllocateDescriptorSets %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    return true;
}

void crSamplerSlotArray::Destroy(void)
{
    auto device = crContext::Get()->Device();

    if( m_bindlessSet != nullptr )
    {
        vkFreeDescriptorSets( *device, m_bindlessPool, 1, &m_bindlessSet );
        m_bindlessSet = nullptr;
    }

    if ( m_bindlessPool != nullptr )
    {
        vkDestroyDescriptorPool( *device, m_bindlessPool, k_allocationCallbacks );
        m_bindlessPool = nullptr;
    }

    if( m_bindlessLayout != nullptr )
    {
        vkDestroyDescriptorSetLayout( *device, m_bindlessLayout, k_allocationCallbacks );
        m_bindlessLayout = nullptr;
    }
}

/*
void crSamplerSlotArray::Bind(void)
{
    auto command = crBackend::Get()->Commandbuffer();
    
    VkPipelineBindPoint pipelineBindPoint{};
    VkPipelineLayout layout{};
    uint32_t firstSet{};
    uint32_t descriptorSetCount{};
    VkDescriptorSet* pDescriptorSets{}, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets

    vkCmdBindDescriptorSets( *command,  )
}
*/

void crSamplerSlotArray::Update( void )
{
    auto device = crContext::Get()->Device();
    VkWriteDescriptorSet descriptorWrites{};
    descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites.pNext = nullptr;
    descriptorWrites.dstSet = m_bindlessSet;
    descriptorWrites.dstBinding = 0;
    descriptorWrites.dstArrayElement = MAX_BINDING_SAMPLERS;
    descriptorWrites.descriptorCount = 1;
    descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites.pImageInfo = m_imageInfos.Ptr();
    descriptorWrites.pBufferInfo = nullptr;
    descriptorWrites.pTexelBufferView = nullptr;
    vkUpdateDescriptorSets( *device, 1, &descriptorWrites, 0, nullptr );
}

/*
================================================================================================
crPipelineLayout
================================================================================================
*/
crShaderStorageLayout::crShaderStorageLayout( void )
{
}

crShaderStorageLayout::~crShaderStorageLayout( void )
{
}

bool crShaderStorageLayout::Create( const uint32_t in_bindingCount )
{
    VkResult result = VK_SUCCESS;
    uint32_t i = 0;
    crList<VkDescriptorBindingFlags>        bindingFlagsArr;
    crList<VkDescriptorSetLayoutBinding>    bindings;
    auto device = crContext::Get()->Device();

    /// configure bindings and flags
    bindingFlagsArr.Resize( in_bindingCount );
    bindings.Resize( in_bindingCount );
    for ( i = 0; i < in_bindingCount; i++)
    {
        VkDescriptorSetLayoutBinding binding{};
        binding.binding = i;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        binding.descriptorCount = 1;
        binding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS; // generic for now
        binding.pImmutableSamplers = nullptr;

        bindings[i] = binding;
        bindingFlagsArr[i] = 0; // Shader Storage buffers, has no active flags
    }
    
    ///
    /// VkDescriptorSetLayoutBindingFlagsCreateInfo 
    /// configure layout bind flags 
    VkDescriptorSetLayoutBindingFlagsCreateInfo flagsCreateInfo{};
    flagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    flagsCreateInfo.bindingCount = bindingFlagsArr.Count();
    flagsCreateInfo.pBindingFlags = bindingFlagsArr.GetData();

    ///
    /// VkDescriptorSetLayoutCreateInfo
    /// Explicitly describe the interface of the resources accessed by the shader.
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = &flagsCreateInfo;
    layoutInfo.bindingCount = bindings.Count();
    layoutInfo.pBindings = bindings.GetData();
    layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT; // enable update-after-bind pool behavior:
    result = vkCreateDescriptorSetLayout( *device, &layoutInfo, k_allocationCallbacks, &m_descriptorSetLayout );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "vkCreateDescriptorSetLayout %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    ///
    /// VkDescriptorPoolSize
    /// Describe which descriptor types our descriptor sets are going to contain and how many of them
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;  // storage buffers 
    poolSize.descriptorCount = SMP_FRAMES * in_bindingCount; // 4 storages buffers by 3 frames   

    /// VkDescriptorPoolCreateInfo
    /// It reserves blocks of GPU resources in advance so that the allocation of descriptor 
    /// sets is fast and does not cause crashes during the rendering loop.
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = SMP_FRAMES; // probably 1 "global" set per frame
    result = vkCreateDescriptorPool( *device, &poolInfo, k_allocationCallbacks, &m_descriptorPool );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "vkCreateDescriptorPool %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    /// 
    /// VkDescriptorSetVariableDescriptorCountAllocateInfo
    ///    
    VkDescriptorSetVariableDescriptorCountAllocateInfo  varCountAlloc{};
    varCountAlloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    varCountAlloc.descriptorSetCount = 1;
    varCountAlloc.pDescriptorCounts = &in_bindingCount;

    ///
    /// VkDescriptorSetAllocateInfo
    ///
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = &varCountAlloc;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_descriptorSetLayout;
    for( i = 0; i < SMP_FRAMES; i++ )
    {
        result = vkAllocateDescriptorSets( *device, &allocInfo, &m_descriptorSet[i] );
        if( result != VK_SUCCESS )
        {
            crConsole::Error( "vkAllocateDescriptorSets %s\n", VulkanErrorString( result ).c_str() );
            return false;
        }
    }

    return true;
}

void crShaderStorageLayout::Bind( const VkCommandBuffer in_commandBuffer, const VkPipelineLayout in_pipelineLayout )
{
    vkCmdBindDescriptorSets( in_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, in_pipelineLayout, 0, 1, &m_descriptorSet[m_bufferID], 0, nullptr  );
}

void crShaderStorageLayout::Update( const uint32_t in_bufferID, const VkBuffer* in_buffers, const uint32_t in_count )
{
    

    m_bufferID = in_bufferID;
    bufferDescriptors.Resize( in_count );
    writeDescriptors.Resize( in_count );
    for (size_t i = 0; i < in_count ; i++)
    {
        /// we make the whole buffer accessible
        VkDescriptorBufferInfo descriptorBuffer{};
        descriptorBuffer.buffer = in_buffers[i];
        descriptorBuffer.offset = 0;
        descriptorBuffer.range = VK_WHOLE_SIZE;
        bufferDescriptors[i] = descriptorBuffer;

        VkWriteDescriptorSet set{};
        set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        set.pNext = nullptr;
        set.dstSet = m_descriptorSet[m_bufferID];
        set.dstBinding = i;
        set.dstArrayElement = 0;
        set.descriptorCount = 1;
        set.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        set.pBufferInfo = &bufferDescriptors[i];
        writeDescriptors[i] = set;
    }

    
}

/*
================================================================================================
crPipelineLayout
================================================================================================
*/
crPipelineLayout::crPipelineLayout( void ) : m_layout( nullptr )
{
}

crPipelineLayout::~crPipelineLayout( void )
{
    Destroy();
}

bool crPipelineLayout::Create( const crSamplerSlotArray* in_bindlessArrays, const crShaderStorageLayout* in_bindingStorage )
{
    VkResult result = VK_SUCCESS;
    VkDescriptorSetLayout descriptorSetLayout[2]{ nullptr, nullptr };
    auto device = crContext::Get()->Device();

    /// Set 0 (High Frequency)
    m_storage = const_cast<crShaderStorageLayout*>( in_bindingStorage );
    descriptorSetLayout[0] = *m_storage;
    m_descriptorSets[0] = *m_storage;

    /// Set 1 (Low Frequency/Global)
    m_samples = const_cast<crSamplerSlotArray*>( in_bindlessArrays );
    descriptorSetLayout[1] = *m_samples;
    m_descriptorSets[1] = *m_samples;

    ///
    /// Pipeline Layout
    ///
    VkPipelineLayoutCreateInfo pipelineLayoutCI{};
    pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCI.pNext = nullptr;
    pipelineLayoutCI.flags = 0;
    pipelineLayoutCI.setLayoutCount = 2;
    pipelineLayoutCI.pSetLayouts = descriptorSetLayout;
    pipelineLayoutCI.pushConstantRangeCount = 0;
    pipelineLayoutCI.pPushConstantRanges = nullptr;
    result = vkCreatePipelineLayout( *device, &pipelineLayoutCI, k_allocationCallbacks, &m_layout );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crPipelineLayout::Create:vkCreatePipelineLayout Failed\n%s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    return true;
}

void crPipelineLayout::Destroy(void)
{
    auto device = crContext::Get()->Device();

    if( m_layout != nullptr )
    {
        vkDestroyPipelineLayout( *device, m_layout, k_allocationCallbacks );
        m_layout = nullptr;
    }
}