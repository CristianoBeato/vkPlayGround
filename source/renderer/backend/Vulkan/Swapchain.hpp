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

#ifndef __SWAPCHAIN_HPP__
#define __SWAPCHAIN_HPP__

class crSwapchain
{
public:
    crSwapchain( void );
    ~crSwapchain( void );

    bool                    Create( const uint32_t in_width, const uint32_t in_height );
    void                    Destroy( void );
    void                    BeginFrame( void );
    void                    SwapBuffers( void );
    inline uint32_t         Frame( void ) const { return m_frame; }
    inline VkCommandBuffer  CommandBuffer( void ) const { return m_commandBuffers[m_frame]; }
    inline vkImageHandle_t* GetImage( void ) const { return const_cast<vkImageHandle_t*>( &m_presentImages[m_currentImage] ); }
    
private:
    uint32_t                m_frame;
    uint32_t                m_width;
    uint32_t                m_height;
    uint32_t                m_currentImage;
    crDeviceQueuep          m_presentQueue;
    crDeviceQueuep          m_graphicQueue;
    crRenderDevicep         m_device;
    VkSwapchainKHR          m_swapchain;
    VkCommandBuffer         m_commandBuffers[SMP_FRAMES];
    VkSemaphore             m_imageAvailable[SMP_FRAMES];
    VkSemaphore             m_renderFinished[SMP_FRAMES];
    VkFence                 m_frameFences[SMP_FRAMES];
    crList<VkImage>         m_imagesArray;
    crList<vkImageHandle_t> m_presentImages;

    bool    CreateSwapChain( const VkSurfaceKHR in_surface, const VkSurfaceFormatKHR in_format, const VkPresentModeKHR in_presentMode );
    bool    PrepareImages( const bool in_recreate, const VkFormat in_format );
    bool    CreateFences( void );
    bool    CreateCommandBuffers( void );
};

#endif //!__SWAPCHAIN_HPP__