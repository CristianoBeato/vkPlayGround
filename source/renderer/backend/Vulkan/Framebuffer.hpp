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

#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

class crFramebuffer
{    
public:
    struct createInfo_t
    {
        uint32_t                width;
        uint32_t                height;
        uint32_t                arrayLayers;
        VkFormat                colorFormat;
        VkFormat                depthStencilFormat;
        VkSampleCountFlagBits   samples;
    };

    struct blitInfo_t
    {
        int32_t             srcX = 0;
        int32_t             srcY = 0;
        int32_t             srcZ = 0;
        int32_t             dstX = 0;
        int32_t             dstY = 0;
        int32_t             dstZ = 0;
        uint32_t            srcWidth = 0;
        uint32_t            srcHeigth = 0;
        uint32_t            srcDepth = 0;
        uint32_t            dstWidth = 0;
        uint32_t            dstHeigth = 0;
        uint32_t            dstDepth = 0;
        vkImageHandle_t*    dstImage = nullptr;
    };

    crFramebuffer( void );
    ~crFramebuffer( void );

    void                Create( const createInfo_t &in_createInfo );
    void                Destroy( void );
    void                Bind( void );
    void                Unbind( void );
    void                Clear( void );
    void                BlitColorAttachament( const blitInfo_t &in_blitInfo );
    void                BlitDepthStencilAttachament( const blitInfo_t &in_blitInfo );
    vkImageHandle_t*    ImageColor( void ) const { return const_cast<vkImageHandle_t*>( &m_colorAttachament[m_frame] ); }
    vkImageHandle_t*    ImageDepthStencil( void ) const { return const_cast<vkImageHandle_t*>( &m_depthAttachament[m_frame] ); }

private:
    uint32_t        m_frame;
    createInfo_t    m_properties;
    VkDeviceMemory  m_unifiedMemory;
    vkImageHandle_t m_colorAttachament[SMP_FRAMES];
    vkImageHandle_t m_depthAttachament[SMP_FRAMES];
};

#endif //!__FRAMEBUFFER_HPP__