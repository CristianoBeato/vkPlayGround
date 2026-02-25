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

#include "Swapchain.hpp"
#include "Core.hpp"
#include "renderer/RenderSystemLocal.hpp"

static struct scMode_t
{
    const char*         mode;
    VkPresentModeKHR    present;
} modes[] =
{
    { "Imediate",           VK_PRESENT_MODE_IMMEDIATE_KHR },
    { "Mail Box",           VK_PRESENT_MODE_MAILBOX_KHR },
    { "Fifo",               VK_PRESENT_MODE_FIFO_KHR },
    { "Fifo Relaxed",       VK_PRESENT_MODE_FIFO_RELAXED_KHR },
    { "Shared Demand",      VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR },
    { "Shared Continuous",  VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR },
};

//static struct scFormat_t
//{
//    const char*         decr;
//    VkSurfaceFormatKHR  format;
//} formats[]
//{
//    { "", { VK_FORMAT_B8G8R8A8_SRGB,    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }},
//    { "", { VK_FORMAT_R8G8B8A8_SRGB,    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }},
//    { "", { VK_FORMAT_B8G8R8A8_UNORM,   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }},
//    { "", { VK_FORMAT_A2B10G10R10_UNORM_PACK32, VK_COLOR_SPACE_HDR10_ST2084_EXT }},
//    { "", { VK_FORMAT_R16G16B16A16_SFLOAT,  }}
//}

crCVar vk_swapchainFormat( "vk_swapChainFormat", "5", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER, " -1 select the device with the highest score, +0 select the device by index" );
crCVar vk_swapchainPresent( "vk_swapchainPresent", "1", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER, " -1 select the device with the highest score, +0 select the device by index" );

crSwapchain::crSwapchain( void ) : 
    m_frame( 0 ),
    m_width( 0 ),
    m_height( 0 ),
    m_currentImage( 0 ),
    m_presentQueue( nullptr ),
    m_graphicQueue( nullptr ),
    m_swapchain( nullptr ),
    m_device( nullptr )
{
    
    std::memset( m_commandBuffers, 0x00, sizeof(VkCommandBuffer) * SMP_FRAMES );
    std::memset( m_imageAvailable, 0x00, sizeof(VkSemaphore) * SMP_FRAMES );
    std::memset( m_renderFinished, 0x00, sizeof(VkSemaphore) * SMP_FRAMES );
    std::memset( m_frameFences, 0x00, sizeof(VkFence) * SMP_FRAMES );
}

crSwapchain::~crSwapchain( void )
{
    Destroy();
}

bool crSwapchain::Create( const uint32_t in_width, const uint32_t in_height )
{
    VkSurfaceFormatKHR  format{};
    VkPresentModeKHR    presentMode;
    crRenderSystemLocal* renderer =  dynamic_cast<crRenderSystemLocal*>( crRenderSystem::Get() );

    auto context = renderer->GetContext();
    m_device = renderer->GetDevice();
    m_width = in_width;
    m_height = in_height;

    // Get queues 
    m_presentQueue = m_device->PresentQueue();
    m_graphicQueue = m_device->GraphicQueue();

    if ( m_presentQueue == nullptr )
    {
        printf( "can't create a swapchain no valid present queue" );
        return false;
    }

    if ( m_graphicQueue == nullptr )
    {
        printf( "can't create a swapchain no valid graphic queue" );
        return false;
    }
    
    /// Get present mode
    auto selected = modes[0]; 
    if( m_device->SupportedPresentMode( modes[vk_swapchainPresent.GetInteger()].present ) )
        selected = modes[vk_swapchainPresent.GetInteger()]; 

    printf( "Using Present Mode: %s\n", selected.mode );
    presentMode = selected.present;

    // Get Surface format
    format = m_device->GetPresentFormat( vk_swapchainFormat.GetInteger() );

    ///
    /// Create Swapchain 
    /// ==========================================================================
    if( !CreateSwapChain( context->Surface(), format, presentMode ) )
        return false;
        
    ///
    /// Create Imageviews
    /// ==========================================================================
    if( !PrepareImages( false, format.format ) )
        return false;

    ///
    /// Create Main Graphic Command Buffer  
    /// ==========================================================================
    if( !CreateCommandBuffers() )
        return false;

    ///
    /// Create race condition structures
    /// ========================================================================== 
    if( !CreateFences() )
        return false;

    return true;
}

void crSwapchain::Destroy(void)
{
    uint32_t i = 0;
    for ( i = 0; i < SMP_FRAMES; i++)
    {
        vkDestroySemaphore( m_device->Device(), m_imageAvailable[i], k_allocationCallbacks );
        vkDestroySemaphore( m_device->Device(), m_renderFinished[i], k_allocationCallbacks );
        vkDestroyFence( m_device->Device(), m_frameFences[i], k_allocationCallbacks );
    }
    
    if ( m_commandBuffers[0] != nullptr )
        vkFreeCommandBuffers( m_device->Device(), m_graphicQueue->CommandPool(), SMP_FRAMES, m_commandBuffers );
    
    for ( i = 0; i < m_presentImages.Count(); i++ )
    {
        // release color image view 
        vkDestroyImageView( m_device->Device(), m_presentImages[i].view, k_allocationCallbacks );
    }

    if ( m_swapchain != nullptr )
    {
        vkDestroySwapchainKHR( m_device->Device(), m_swapchain, k_allocationCallbacks );
        m_swapchain = nullptr;
    }
}

void crSwapchain::BeginFrame( void )
{
    VkResult result = VK_SUCCESS;
 
    //
    // Wait for the device finish last render in previous match frame, before reuse command buffer
    result = vkWaitForFences( m_device->Device(), 1, &m_frameFences[m_frame], VK_TRUE, UINT64_MAX );
    if ( result != VK_SUCCESS )
        printf( "vkSwapchain::AcquireImage::vkWaitForFences: %s\n", VulkanErrorString( result ).c_str() );
    else
        vkResetFences(m_device->Device(), 1, &m_frameFences[m_frame] );

    //
    //
    // Aquire the current frame image idex
    VkAcquireNextImageInfoKHR   acquireNextImageInfo{};
    acquireNextImageInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;
    acquireNextImageInfo.pNext = nullptr;
    acquireNextImageInfo.swapchain = m_swapchain;
    acquireNextImageInfo.timeout = UINT64_MAX;
    acquireNextImageInfo.semaphore = m_imageAvailable[m_frame];
    acquireNextImageInfo.fence = nullptr;
    acquireNextImageInfo.deviceMask = m_device->Mask();
    result = vkAcquireNextImage2KHR( m_device->Device(), &acquireNextImageInfo, &m_currentImage );
    if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
        printf( "vkSwapchain::AcquireImage::vkAcquireNextImage2KHR %s\n", VulkanErrorString( result ).c_str() );

    ///
    ///
    /// Reset the main render command buffer
    result = vkResetCommandBuffer( m_commandBuffers[m_frame], 0 );
    if( result != VK_SUCCESS )
        printf( "vkResetCommandBuffer::FAILED!\n" );

    ///
    /// 
    /// Begin register commands in curren buffer
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT /*VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT*/; // we only submit one time per frame 
    result = vkBeginCommandBuffer( m_commandBuffers[m_frame], &beginInfo );
    if( result != VK_SUCCESS )
        printf( "vkCommandBuffer::Begin FAILED to begin!\n" );
       
    ///
    ///
    ///
#if 1
    /// the image is changed outside of the scope
    m_presentImages[m_currentImage].layout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_presentImages[m_currentImage].stageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    m_presentImages[m_currentImage].accessMask = 0;
    VkImageStateTransition( GetImage(), CommandBuffer(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_NONE );
#else
    VkImageMemoryBarrier2 acquireBarrier{};
    acquireBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    acquireBarrier.srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; // Veio do SO
    acquireBarrier.srcAccessMask = 0;
    acquireBarrier.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    acquireBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    acquireBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Sempre trate como Undefined ao adquirir
    acquireBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    acquireBarrier.image = GetImage();
    acquireBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    acquireBarrier.subresourceRange.levelCount = 1;
    acquireBarrier.subresourceRange.layerCount = 1;

    VkDependencyInfo depInfo{};
    depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    depInfo.imageMemoryBarrierCount = 1;
    depInfo.pImageMemoryBarriers = &acquireBarrier;

    vkCmdPipelineBarrier2( CommandBuffer(), &depInfo);
#endif
}

void crSwapchain::SwapBuffers(void)
{
    VkResult result = VK_SUCCESS;

    ///
    ///
    /// Perform a state transition in the present image
#if 1
    VkImageStateTransition( GetImage(), CommandBuffer(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_NONE );
#else
    VkImageMemoryBarrier2 barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.pNext = nullptr;
    barrier.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    barrier.dstAccessMask = 0;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //m_graphicQueue->Family();
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //m_presentQueue->Family();
    barrier.image = m_presentImages[m_frame].image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.pNext = nullptr;
    dependencyInfo.dependencyFlags = 0;
    dependencyInfo.memoryBarrierCount = 0;
    dependencyInfo.pMemoryBarriers = nullptr;
    dependencyInfo.bufferMemoryBarrierCount = 0;
    dependencyInfo.pBufferMemoryBarriers = nullptr;
    dependencyInfo.imageMemoryBarrierCount = 1;
    dependencyInfo.pImageMemoryBarriers = &barrier;
    vkCmdPipelineBarrier2( CommandBuffer(), &dependencyInfo );
#endif

    /// 
    /// Finish record draw commands
    result = vkEndCommandBuffer( m_commandBuffers[m_frame] );
    if( result != VK_SUCCESS )
        printf( "vkCommandBuffer::Begin FAILED!\n" );

    ///
    /// We wait for swap chain aquire a image
    VkSemaphoreSubmitInfo       wait{};
    wait.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    wait.pNext = nullptr;
    wait.semaphore = m_imageAvailable[m_frame];
    wait.value = 0;
    wait.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT; 
    wait.deviceIndex = 0; 

    ///
    /// Signal that render is done
    VkSemaphoreSubmitInfo       signal{};
    signal.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    signal.pNext = nullptr;
    signal.semaphore = m_renderFinished[m_frame];
    signal.value = 0;
    signal.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT; 
    signal.deviceIndex = 0; 

    ///
    /// configure command to be submited
    VkCommandBufferSubmitInfo   commandBufferSubmit{};
    commandBufferSubmit.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    commandBufferSubmit.pNext = nullptr;
    commandBufferSubmit.commandBuffer = m_commandBuffers[m_frame];
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
    result = vkQueueSubmit2( m_graphicQueue->Queue(), 1, &submitInfo, nullptr );
    if( result != VK_SUCCESS )
        printf( "vkQueueSubmit2 FAILED!\n" );

    ///
    /// present render to the window
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_renderFinished[m_frame];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_currentImage;
    vkQueuePresentKHR( m_presentQueue->Queue(), &presentInfo );

    m_frame = ( m_frame + 1 ) % SMP_FRAMES;
}

bool crSwapchain::CreateSwapChain( const VkSurfaceKHR in_surface, const VkSurfaceFormatKHR in_format, const VkPresentModeKHR in_presentMode )
{
    VkResult result = VK_SUCCESS;
    uint32_t queueFamilyIndices[2] { m_presentQueue->Family(), m_graphicQueue->Family() };
    VkSwapchainKHR old = m_swapchain;
    VkSwapchainCreateInfoKHR swapchainCI{};
    swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCI.pNext = nullptr;
    swapchainCI.flags = 0;
    swapchainCI.surface = in_surface;
    swapchainCI.minImageCount = std::min( SMP_FRAMES, 3u ); //TODO: check max device suported frames
    swapchainCI.imageFormat = in_format.format;
    swapchainCI.imageColorSpace = in_format.colorSpace;
    swapchainCI.imageExtent.width = m_width;
    swapchainCI.imageExtent.height = m_height;
    swapchainCI.imageArrayLayers = 1;
    swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    // if we have a independent present queue 
    swapchainCI.imageSharingMode = ( queueFamilyIndices[0] != queueFamilyIndices[1] ) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
    swapchainCI.queueFamilyIndexCount = ( queueFamilyIndices[0] != queueFamilyIndices[1] ) ? 2 : 1;
    swapchainCI.pQueueFamilyIndices = queueFamilyIndices;
    swapchainCI.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; // todo: get from context
    swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCI.presentMode = in_presentMode;
    swapchainCI.clipped = VK_TRUE;
    swapchainCI.oldSwapchain = old;

    result = vkCreateSwapchainKHR( m_device->Device(), &swapchainCI, k_allocationCallbacks, &m_swapchain );
    if ( result != VK_SUCCESS )
    { 
        printf( "vkCreateSwapchainKHR FAILED! %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    // we are updating, recreating a new
    if ( old != nullptr )
        vkDestroySwapchainKHR( m_device->Device(), old, k_allocationCallbacks );

    return true;
}

bool crSwapchain::PrepareImages(const bool in_recreate, const VkFormat in_format )
{
    uint32_t i = 0;
    VkResult result = VK_SUCCESS;
    uint32_t numImages = 0;

        // Get the available image count 
    vkGetSwapchainImagesKHR( m_device->Device(), m_swapchain, &numImages, nullptr );
    
    // prepare the arrays
    m_presentImages.Resize( numImages );
    m_imagesArray.Resize( numImages );
    
    // Get the image array 
    vkGetSwapchainImagesKHR( m_device->Device(), m_swapchain, &numImages, m_imagesArray.GetData() );
    
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = in_format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    // create the swap chain views
    for ( i = 0; i < numImages; i++) 
    {
        m_presentImages[i].image = m_imagesArray[i];

        // destroy old view 
        if ( in_recreate )
            vkDestroyImageView( m_device->Device(), m_presentImages[i].view, k_allocationCallbacks );
    
        createInfo.image = m_presentImages[i];    
        result = vkCreateImageView( m_device->Device(), &createInfo, k_allocationCallbacks, &m_presentImages[i].view ); 
        if ( result != VK_SUCCESS ) 
            printf( "vkSwapchain::PrepareImages::vkCreateImageView ERROR: %s\n", VulkanErrorString( result ).c_str() );
    }

    return true;
}

bool crSwapchain::CreateFences(void)
{
    VkResult result = VK_SUCCESS;
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
        result = vkCreateSemaphore( m_device->Device(), &semaphoreInfo, k_allocationCallbacks, &m_imageAvailable[i] ); 
        if( result != VK_SUCCESS )
        {
            printf( "crvkSwapchain::Create::vkCreateSemaphore %s\n", VulkanErrorString( result ).c_str() );
            return false;
        }

        // create the semaphore object
        result = vkCreateSemaphore( m_device->Device(), &semaphoreInfo, k_allocationCallbacks, &m_renderFinished[i] ); 
        if( result != VK_SUCCESS )
        {
            printf( "crvkSwapchain::Create::vkCreateSemaphore %s\n", VulkanErrorString( result ).c_str() );
            return false;
        }

        // create the fence object
        result = vkCreateFence( m_device->Device(), &fenceCI, k_allocationCallbacks, &m_frameFences[i] );
        if( result != VK_SUCCESS )
        {
            printf( "crvkSwapchain::Create::vkCreateFence %s\n", VulkanErrorString( result ).c_str() );
            return false;
        }
    }

    return true;
}

bool crSwapchain::CreateCommandBuffers(void)
{
    VkResult result = VK_SUCCESS;
    // allocate command buffers
    VkCommandBufferAllocateInfo commandBufferAllocateCI{};
    commandBufferAllocateCI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateCI.pNext = nullptr;
    commandBufferAllocateCI.commandPool = m_graphicQueue->CommandPool();
    commandBufferAllocateCI.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateCI.commandBufferCount = SMP_FRAMES;
    result = vkAllocateCommandBuffers( m_device->Device(), &commandBufferAllocateCI, m_commandBuffers );
    if( result != VK_SUCCESS )
    {
        printf( "crvkCommandBuffer::Create::vkAllocateCommandBuffers:%s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    return true;
}
