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

#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

class crImage
{
public:
    struct dimension_t
    {
        uint16_t    layers;
        uint16_t    levels;
        uint32_t    width;
        uint32_t    height;
        uint32_t    depth;
    };

    crImage( void );
    ~crImage( void );

    void        Create( const dimension_t in_dimension, const VkFormat in_format, const VkImageViewType in_viewType );
    void        Create( const VkImage in_image, const VkFormat in_format, const VkImageViewType in_viewType );
    void        Destroy( void );
    void        State( const VkCommandBuffer in_commandBuffer, const VkImageLayout in_newLayout, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask );

    inline VkPipelineStageFlags2    StageMask( void ) const { return m_stage; }
    inline VkAccessFlags2           AccessMask( void ) const { return m_access; } 
    inline VkImageViewType          ViewType( void ) const { return m_type; }
    inline VkFormat                 Format( void ) const { return m_format; }
    inline VkImage                  Image( void ) const { return m_image; }
    inline VkImageView              View( void ) const { return m_view; }
    inline operator VkImage( void ) const { return m_image; }
    inline operator VkImageView( void ) const { return m_view; }

protected:
    friend class            crFramebuffer;
    VkImageAspectFlags      m_aspectMask;
    VkAccessFlags2          m_access;
    VkPipelineStageFlags2   m_stage;
    VkImageLayout           m_layout;
    uint32_t                m_levelCount;
    uint32_t                m_layerCount;
    VkImageViewType         m_type;
    VkFormat                m_format;
    VkImage                 m_image;
    VkImageView             m_view;
    crRenderDevicep         m_device;
};

#endif //!__IMAGE_HPP__