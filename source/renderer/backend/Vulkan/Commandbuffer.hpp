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
    VkCommandBuffer     CommandBuffer( void ) const { return m_commandBuffers[m_frame]; }

private:
    uint32_t            m_frame;
    uint64_t            m_frameCount;
    crRenderDevicep     m_device;
    crDeviceQueuep      m_graphicQueue;
    VkSemaphore         m_frameSemaphore;
    VkCommandBuffer     m_commandBuffers[SMP_FRAMES];
};

#endif //!__COMMAND_BUFFER_HPP__