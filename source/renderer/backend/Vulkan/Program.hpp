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

#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

class crProgram
{
public:
    crProgram( void );
    ~crProgram( void );

    bool    Create( const uint32_t* in_source, const size_t in_size, const VkShaderStageFlagBits in_stage, const uint16_t in_ID );
    void    Destroy( void );

    const VkPipelineShaderStageCreateInfo   Program( void ) const { return m_shaderStageInfo; }
    uint16_t    ID( void ) const { return m_programID; }
    
private:
    uint16_t                        m_programID;
    VkPipelineShaderStageCreateInfo m_shaderStageInfo;
};

#endif //!__PROGRAM_HPP__