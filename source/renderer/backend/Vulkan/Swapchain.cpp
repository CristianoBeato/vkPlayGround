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
    m_bufferID( 0 ),
    m_width( 0 ),
    m_height( 0 ),
    m_currentImage( 0 ),
    m_presentQueue( nullptr ),
    m_graphicQueue( nullptr ),
    m_swapchain( nullptr ),
    m_device( nullptr )
{
}

crSwapchain::~crSwapchain( void )
{
    Destroy();
}

bool crSwapchain::Create( const uint32_t in_width, const uint32_t in_height, bool in_recreate )
{
    uint32_t                i = 0;
    uint32_t                numImages = 0;
    VkResult                result = VK_SUCCESS;
    VkSurfaceFormatKHR      format{};
    VkPresentModeKHR        presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
    VkSwapchainKHR          old = m_swapchain;
    crRenderSystemLocal*    renderer =  dynamic_cast<crRenderSystemLocal*>( crRenderSystem::Get() );

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
    uint32_t queueFamilyIndices[2] { m_presentQueue->Family(), m_graphicQueue->Family() };
    
    VkSwapchainCreateInfoKHR swapchainCI{};
    swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCI.pNext = nullptr;
    swapchainCI.flags = 0;
    swapchainCI.surface = context->Surface();
    swapchainCI.minImageCount = std::min( SMP_FRAMES, 3u ); //TODO: check max device suported frames
    swapchainCI.imageFormat = format.format;
    swapchainCI.imageColorSpace = format.colorSpace;
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
    swapchainCI.presentMode = presentMode;
    swapchainCI.clipped = VK_TRUE;
    swapchainCI.oldSwapchain = old;

    result = vkCreateSwapchainKHR( m_device->Device(), &swapchainCI, k_allocationCallbacks, &m_swapchain );
    if ( result != VK_SUCCESS )
    { 
        printf( "vkCreateSwapchainKHR FAILED! %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    // we are updating, recreating a new
    if ( in_recreate )
        vkDestroySwapchainKHR( m_device->Device(), old, k_allocationCallbacks );
    
    // Get the available image count 
    vkGetSwapchainImagesKHR( *m_device, m_swapchain, &numImages, nullptr );
    
    // prepare the arrays
    m_presentImages.Resize( numImages );
    m_imagesArray.Resize( numImages );
    
    // Get the image array 
    vkGetSwapchainImagesKHR( *m_device, m_swapchain, &numImages, m_imagesArray.GetData() );
    
    ///
    /// Create Imageviews
    /// ==========================================================================
    
    // create the swap chain views
    for ( i = 0; i < numImages; i++) 
    {
         // if we are re creatin a frame buffer or the swap chain
        if ( in_recreate )
            vkDestroyImageView( *m_device, m_presentImages[i].View(), k_allocationCallbacks );
        
        m_presentImages[i] = crImage(); // prevent gargbage
        m_presentImages[i].Create( m_imagesArray[i], format.format, VK_IMAGE_VIEW_TYPE_2D );
    }

    ///
    /// Create race condition structures
    /// ========================================================================== 
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    // alloc the structures arrays 
    for ( uint32_t i = 0; i < SMP_FRAMES; i++)
    {
        // create the semaphore object
        result = vkCreateSemaphore( m_device->Device(), &semaphoreInfo, k_allocationCallbacks, &m_imageAvailable[i] ); 
        if( result != VK_SUCCESS )
        {
            crConsole::Error( "crSwapchain::Create::vkCreateSemaphore %s\n", VulkanErrorString( result ).c_str() );
            return false;
        }
    }

    return true;
}

void crSwapchain::Destroy(void)
{
    uint32_t i = 0;
    for ( i = 0; i < SMP_FRAMES; i++)
    {
        vkDestroySemaphore( m_device->Device(), m_imageAvailable[i], k_allocationCallbacks );
    }
        
    for ( i = 0; i < m_presentImages.Count(); i++ )
    {
        // release color image view 
        vkDestroyImageView( m_device->Device(), m_presentImages[i].View(), k_allocationCallbacks );
    }

    if ( m_swapchain != nullptr )
    {
        vkDestroySwapchainKHR( m_device->Device(), m_swapchain, k_allocationCallbacks );
        m_swapchain = nullptr;
    }
}

void crSwapchain::Acquire( const uint64_t in_frame )
{
    VkResult result = VK_SUCCESS;
    m_bufferID = in_frame % SMP_FRAMES;
    
    //
    //
    // Aquire the current frame image idex
    VkAcquireNextImageInfoKHR   acquireNextImageInfo{};
    acquireNextImageInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;
    acquireNextImageInfo.pNext = nullptr;
    acquireNextImageInfo.swapchain = m_swapchain;
    acquireNextImageInfo.timeout = UINT64_MAX;
    acquireNextImageInfo.semaphore = m_imageAvailable[m_bufferID];
    acquireNextImageInfo.fence = nullptr;
    acquireNextImageInfo.deviceMask = m_device->Mask();
    result = vkAcquireNextImage2KHR( m_device->Device(), &acquireNextImageInfo, &m_currentImage );
    if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
        printf( "vkSwapchain::AcquireImage::vkAcquireNextImage2KHR %s\n", VulkanErrorString( result ).c_str() );
}

void crSwapchain::Present( const VkSemaphore in_renderFinish )
{
    VkResult result = VK_SUCCESS;

    ///
    /// present render to the window
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &in_renderFinish;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_currentImage;
    vkQueuePresentKHR( m_presentQueue->Queue(), &presentInfo );
}
