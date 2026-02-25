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

#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

/// @brief a sub buffer represent a region of a buffer
class crSubBuffer
{
public:
    crSubBuffer( void );
    ~crSubBuffer( void );
    void        State( const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask );
    size_t      Size( void ) const { return m_size; }
    uintptr_t   Offset( void ) const { return m_offset; }
    VkBuffer    Handle( void ) const { return m_buffer; }

private:
    VkPipelineStageFlags2   m_stage;
    VkAccessFlags2          m_access;
    size_t                  m_size;     // size of the buffer
    uintptr_t               m_offset;   // offset in the buffer
    VkBuffer                m_buffer;   // handle
};

/// @brief 
class crBuffer
{
public:
    crBuffer( void );
    ~crBuffer( void );
    void            Create( void );
    void            Destroy( void );
    crSubBuffer*    Alloc( const size_t in_size );
    void            Free( crSubBuffer* in_buff );

    void    State( const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask );
    
    /// access the buffer handler
    inline VkBuffer Buffer( void ) const { return m_buffer; }

private:
    VkPipelineStageFlags2   m_stage;
    VkAccessFlags2          m_access;
    VkBuffer                m_buffer;
    VkDeviceMemory          m_memory;
};

#endif //!__BUFFER_HPP__