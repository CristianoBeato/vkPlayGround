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

#include "Commandbuffer.hpp"
#include "Core.hpp"

crCommandbuffer::crCommandbuffer( void ) : 
    m_bufferID( 0 ),
    m_device( nullptr ),
    m_graphicQueue( nullptr )
{
}

crCommandbuffer::~crCommandbuffer( void )
{
}

bool crCommandbuffer::Create(void)
{
    VkResult result = VK_SUCCESS;
    m_device = crContext::Get()->Device();
    m_graphicQueue = m_device->GraphicQueue();

    // allocate command buffers
    VkCommandBufferAllocateInfo commandBufferAllocateCI{};
    commandBufferAllocateCI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateCI.pNext = nullptr;
    commandBufferAllocateCI.commandPool = m_graphicQueue->CommandPool();
    commandBufferAllocateCI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateCI.commandBufferCount = m_commandBuffers.Count();
    result = vkAllocateCommandBuffers( m_device->Device(), &commandBufferAllocateCI, &m_commandBuffers );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crCommandbuffer::Create::vkAllocateCommandBuffers\n%s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    // Semaphore configuration
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    // Fence configuration
    VkFenceCreateInfo fenceCI{};
    fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCI.pNext = nullptr;
    fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Signaled, so we don't stuck at first frames 

        // alloc the structures arrays 
    for ( uint32_t i = 0; i < SMP_FRAMES; i++)
    {
        // create the semaphore object
        result = vkCreateSemaphore( m_device->Device(), &semaphoreInfo, k_allocationCallbacks, &m_renderFinished[i] ); 
        if( result != VK_SUCCESS )
        {
            crConsole::Error( "crCommandbuffer::Create::vkCreateSemaphore %s\n", VulkanErrorString( result ).c_str() );
            return false;
        }

        // create the fence object
        result = vkCreateFence( m_device->Device(), &fenceCI, k_allocationCallbacks, &m_frameFences[i] );
        if( result != VK_SUCCESS )
        {
            crConsole::Error( "crCommandbuffer::Create::vkCreateFence %s\n", VulkanErrorString( result ).c_str() );
            return false;
        }
    }

    return true;
}

void crCommandbuffer::Destroy(void)
{
    for ( uint32_t i = 0; i < SMP_FRAMES; i++)
    {
        vkDestroySemaphore( *m_device, m_renderFinished[i], k_allocationCallbacks );
        vkDestroyFence( m_device->Device(), m_frameFences[i], k_allocationCallbacks );
    }

    vkFreeCommandBuffers( m_device->Device(), m_graphicQueue->CommandPool(), m_commandBuffers.Count(), &m_commandBuffers );
}

void crCommandbuffer::Begin(const uint64_t in_frame)
{
    VkResult result = VK_SUCCESS;
 
    // get buffer 
    m_bufferID = in_frame % SMP_FRAMES;

    //
    // Wait for the device finish last render in previous match frame, before reuse command buffer
    result = vkWaitForFences( *m_device, 1, &m_frameFences[m_bufferID], VK_TRUE, UINT64_MAX );
    if ( result != VK_SUCCESS )
        crConsole::Error( "crCommandbuffer::Begin::vkWaitForFences: %s\n", VulkanErrorString( result ).c_str() );
    else
        vkResetFences( *m_device, 1, &m_frameFences[m_bufferID] );

    ///
    ///
    /// Reset the main render command buffer
    result = vkResetCommandBuffer( m_commandBuffers[m_bufferID], 0 );
    if( result != VK_SUCCESS )
        crConsole::Error( "crCommandbuffer::Begin::vkResetCommandBuffer FAILED!\n%s\n", VulkanErrorString( result ).c_str() );

    ///
    /// 
    /// Begin register commands in curren buffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT /*VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT*/; // we only submit one time per frame 
    result = vkBeginCommandBuffer( m_commandBuffers[m_bufferID], &beginInfo );
    if( result != VK_SUCCESS )
        crConsole::Error( "crCommandbuffer::Begin::vkBeginCommandBuffer FAILED!\n%s\n", VulkanErrorString( result ).c_str() );
}

void crCommandbuffer::Submit( const VkSemaphore in_wait )
{
    VkResult result = VK_SUCCESS;
    
    /// 
    /// Finish record draw commands
    result = vkEndCommandBuffer( m_commandBuffers[m_bufferID] );
    if( result != VK_SUCCESS )
        crConsole::Error( "crCommandbuffer::Submit::vkEndCommandBuffer FAILED!\n" );

    ///
    /// We wait for swap chain aquire a image
    VkSemaphoreSubmitInfo wait{};
    wait.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    wait.pNext = nullptr;
    wait.semaphore = in_wait;
    wait.value = 0;
    wait.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT; 
    wait.deviceIndex = 0; 

    ///
    /// Signal that render is done
    VkSemaphoreSubmitInfo       signal{};
    signal.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    signal.pNext = nullptr;
    signal.semaphore = m_renderFinished[m_bufferID];
    signal.value = 0;
    signal.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT; 
    signal.deviceIndex = 0; 

    ///
    /// configure command to be submited
    VkCommandBufferSubmitInfo   commandBufferSubmit{};
    commandBufferSubmit.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    commandBufferSubmit.pNext = nullptr;
    commandBufferSubmit.commandBuffer = m_commandBuffers[m_bufferID];
    commandBufferSubmit.deviceMask = 0;

    ///
    /// sumit command buffer of this frame
    VkSubmitInfo2               submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submitInfo.waitSemaphoreInfoCount = 1;
    submitInfo.pWaitSemaphoreInfos = &wait;
    submitInfo.commandBufferInfoCount = 1;
    submitInfo.pCommandBufferInfos = &commandBufferSubmit;
    submitInfo.signalSemaphoreInfoCount = 1;
    submitInfo.pSignalSemaphoreInfos = &signal;
    result = vkQueueSubmit2( m_graphicQueue->Queue(), 1, &submitInfo, m_frameFences[m_bufferID] );
    if( result != VK_SUCCESS )
        crConsole::Error( "crCommandbuffer::Submit::vkQueueSubmit2 FAILED!\n" );
}
