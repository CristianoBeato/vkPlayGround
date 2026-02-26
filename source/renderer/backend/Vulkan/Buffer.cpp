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

#include "Buffer.hpp" 

crSubBuffer::crSubBuffer( void ) : 
    m_stage( VK_PIPELINE_STAGE_2_NONE ),
    m_access( VK_ACCESS_2_NONE ),
    m_size( 0 ),
    m_offset( 0 ),
    m_buffer( nullptr )
{
}

crSubBuffer::~crSubBuffer( void )
{
    m_stage = VK_PIPELINE_STAGE_2_NONE;
    m_access = VK_ACCESS_2_NONE;
    m_size = 0;
    m_offset = 0;
    m_buffer = nullptr;
}

void crSubBuffer::State(const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask)
{
    assert( m_buffer != nullptr );

    VkBufferMemoryBarrier2 destinationBarrier{}; 
    destinationBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
    destinationBarrier.pNext = nullptr;
    destinationBarrier.srcStageMask = m_stage;
    destinationBarrier.srcAccessMask = m_access;
    destinationBarrier.dstStageMask = in_stageMask;
    destinationBarrier.dstAccessMask = in_accessMask;
    destinationBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.buffer = m_buffer;

    // update whole buffer, no region change
    destinationBarrier.offset = m_offset;
    destinationBarrier.size = m_size;

    /// perform a state transition to destination
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.pNext = nullptr;
    dependencyInfo.dependencyFlags = 0;
    dependencyInfo.memoryBarrierCount = 0;
    dependencyInfo.pMemoryBarriers = nullptr;
    dependencyInfo.bufferMemoryBarrierCount = 1;
    dependencyInfo.pBufferMemoryBarriers = &destinationBarrier;
    dependencyInfo.imageMemoryBarrierCount = 0;
    dependencyInfo.pImageMemoryBarriers = nullptr;
    vkCmdPipelineBarrier2( in_commandBuffer, &dependencyInfo );

    // update buffer stage
    m_stage = in_stageMask;
    m_access = in_accessMask;
}

crBuffer::crBuffer( void ) : 
    m_stage( VK_PIPELINE_STAGE_2_NONE ),
    m_access( VK_ACCESS_2_NONE ),
    m_buffer( nullptr ),
    m_memory(  nullptr )
{
}

crBuffer::~crBuffer( void )
{
    Destroy();
}

void crBuffer::Destroy(void)
{
    auto device = crContext::Get()->Device();
    if ( m_memory != nullptr )
    {
        vkFreeMemory( *device, m_memory, k_allocationCallbacks );
        m_memory = nullptr;
    }
    
    if ( m_buffer != nullptr )
    {
        vkDestroyBuffer( *device, m_buffer, k_allocationCallbacks );
        m_buffer = nullptr;
    }

    m_stage = VK_PIPELINE_STAGE_2_NONE;
    m_access = VK_ACCESS_2_NONE;
}

void crBuffer::State(const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask)
{
    assert( m_buffer != nullptr );

    VkBufferMemoryBarrier2 destinationBarrier{}; 
    destinationBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
    destinationBarrier.pNext = nullptr;
    destinationBarrier.srcStageMask = m_stage;
    destinationBarrier.srcAccessMask = m_access;
    destinationBarrier.dstStageMask = in_stageMask;
    destinationBarrier.dstAccessMask = in_accessMask;
    destinationBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.buffer = m_buffer;

    // update whole buffer, no region change
    destinationBarrier.offset = 0;  
    destinationBarrier.size = VK_WHOLE_SIZE;

    /// perform a state transition to destination
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.pNext = nullptr;
    dependencyInfo.dependencyFlags = 0;
    dependencyInfo.memoryBarrierCount = 0;
    dependencyInfo.pMemoryBarriers = nullptr;
    dependencyInfo.bufferMemoryBarrierCount = 1;
    dependencyInfo.pBufferMemoryBarriers = &destinationBarrier;
    dependencyInfo.imageMemoryBarrierCount = 0;
    dependencyInfo.pImageMemoryBarriers = nullptr;
    vkCmdPipelineBarrier2( in_commandBuffer, &dependencyInfo );

    // update buffer stage
    m_stage = in_stageMask;
    m_access = in_accessMask;
}
