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

#ifndef __COMMAND_BUFFER_HPP__
#define __COMMAND_BUFFER_HPP__

class crCommandbuffer
{
public:
    crCommandbuffer( void );
    ~crCommandbuffer( void );
    bool                Create( void );
    void                Destroy( void );
    void                Begin( const uint64_t in_frame );
    void                Submit( const VkSemaphore in_wait );
    VkCommandBuffer     CommandBuffer( void ) const { return m_commandBuffers[m_bufferID]; }
    VkSemaphore         SubmitFinishe( void ) const { return m_renderFinished[m_bufferID]; }
    operator VkCommandBuffer( void ) const { return m_commandBuffers[m_bufferID]; }

private:
    uint32_t                                m_bufferID;
    crRenderDevicep                         m_device;
    crDeviceQueuep                          m_graphicQueue;
    crArray<VkCommandBuffer, SMP_FRAMES>    m_commandBuffers;
    crArray<VkSemaphore, SMP_FRAMES>        m_renderFinished;
    crArray<VkFence, SMP_FRAMES>            m_frameFences;

};

#endif //!__COMMAND_BUFFER_HPP__