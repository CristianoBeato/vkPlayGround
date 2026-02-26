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

#include "Image.hpp"
#include "Core.hpp"


static inline bool IsDepthFormat( const VkFormat format ) 
{
    static const VkFormat depthFormats[5] = 
    {
        VK_FORMAT_D16_UNORM,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT
    };
 
    for ( uint32_t i = 0; i < 5; i++)
    {
        if (format == depthFormats[i] )
            return true;
    }
    return false;
}

static inline bool IsStencilFormat( const VkFormat format ) 
{
    static const VkFormat stencilFormats[4] = 
    {
        VK_FORMAT_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT
    };

    for ( uint32_t i = 0; i < 4; i++)
    {
        if (format == stencilFormats[i] )
            return true;
    }
    return false;
}

crImage::crImage( void ) :
    m_layout( VK_IMAGE_LAYOUT_UNDEFINED ),
    m_stage( VK_PIPELINE_STAGE_NONE ),
    m_access( VK_ACCESS_NONE ),
    m_image( nullptr ),
    m_view( nullptr ),
    m_device( nullptr )
{
}

crImage::~crImage( void )
{
}

void crImage::Create( const VkImage in_image, const VkFormat in_format, const VkImageViewType in_viewType )
{
    VkResult    result = VK_SUCCESS;

    /// image is create externally
    m_image = in_image;
    m_format = in_format;
    m_type = in_viewType;
    m_levelCount = 1;
    m_layerCount = 1;

    if ( m_view != nullptr )
    {
        vkDestroyImageView( *m_device, m_view, k_allocationCallbacks );
        m_view = nullptr;
    }

    bool hasDepth = IsDepthFormat( m_format );
    bool hasStencil = IsStencilFormat( m_format );
    if (  hasDepth || hasStencil )
    {
        if ( hasDepth )
            m_aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        if( hasStencil )
            m_aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    else
        m_aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // is color image
    
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.viewType = m_type;
    createInfo.format = in_format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = m_aspectMask;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = m_levelCount;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = m_layerCount;
    createInfo.image = m_image;    
    
    result = vkCreateImageView( m_device->Device(), &createInfo, k_allocationCallbacks, &m_view ); 
    if ( result != VK_SUCCESS ) 
        crConsole::Error( "crImage::Create:\n%s\n", VulkanErrorString( result ).c_str() );
}

void crImage::Destroy(void)
{
    if ( m_view != nullptr )
    {
        vkDestroyImageView( *m_device, m_view, k_allocationCallbacks );
        m_view = nullptr;
    }

    if( m_image != nullptr )
    {
        vkDestroyImage( *m_device, m_image, k_allocationCallbacks );
        m_image = nullptr;
    }

    m_device = nullptr;
}

void crImage::State(const VkCommandBuffer in_commandBuffer, const VkImageLayout in_newLayout, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask)
{
    assert( m_image != nullptr );

    VkImageMemoryBarrier2 destinationBarrier{};
    destinationBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    destinationBarrier.pNext = nullptr;
    destinationBarrier.srcStageMask = m_stage;
    destinationBarrier.srcAccessMask = m_access;
    destinationBarrier.dstStageMask = in_stageMask;
    destinationBarrier.dstAccessMask = in_accessMask;
    destinationBarrier.oldLayout = m_layout;
    destinationBarrier.newLayout = in_newLayout;
    destinationBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //m_graphicQueue->Family();
    destinationBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //m_presentQueue->Family();
    destinationBarrier.image = m_image;
    destinationBarrier.subresourceRange.aspectMask = m_aspectMask;
    destinationBarrier.subresourceRange.baseMipLevel = 0;
    destinationBarrier.subresourceRange.levelCount = m_levelCount;
    destinationBarrier.subresourceRange.baseArrayLayer = 0;
    destinationBarrier.subresourceRange.layerCount = m_layerCount;
        
    /// perform a state transition to destination
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.pNext = nullptr;
    dependencyInfo.dependencyFlags = 0;
    dependencyInfo.memoryBarrierCount = 0;
    dependencyInfo.pMemoryBarriers = nullptr;
    dependencyInfo.bufferMemoryBarrierCount = 0;
    dependencyInfo.pBufferMemoryBarriers = nullptr;
    dependencyInfo.imageMemoryBarrierCount = 1;
    dependencyInfo.pImageMemoryBarriers = &destinationBarrier;
    vkCmdPipelineBarrier2( in_commandBuffer, &dependencyInfo );

    /// keep track of texture state
    m_layout = in_newLayout;
    m_stage = in_stageMask;
    m_access = in_accessMask;
}
