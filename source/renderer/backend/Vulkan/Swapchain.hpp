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

    bool                    Create( const uint32_t in_width, const uint32_t in_height, bool in_recreate );
    void                    Destroy( void );
    void                    Acquire( const uint64_t in_frame );
    void                    Present( const VkSemaphore in_renderFinish );
    inline crImage*         Image( void ) const { return const_cast<crImage*>( &m_presentImages[m_currentImage] ); }
    inline VkSemaphore      ImageAvailable( void ) const { return m_imageAvailable[m_bufferID]; }

private:
    uint32_t                            m_bufferID; 
    uint32_t                            m_width;
    uint32_t                            m_height;
    uint32_t                            m_currentImage;
    crDeviceQueuep                      m_presentQueue;
    crDeviceQueuep                      m_graphicQueue;
    crRenderDevicep                     m_device;
    VkSwapchainKHR                      m_swapchain;
    crArray<VkSemaphore, SMP_FRAMES>    m_imageAvailable;
    crList<VkImage>                     m_imagesArray;
    crList<crImage>                     m_presentImages;
};

#endif //!__SWAPCHAIN_HPP__