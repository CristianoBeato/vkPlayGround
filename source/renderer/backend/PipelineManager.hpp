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

#ifndef __PIPELINE_MANAGER_HPP__
#define __PIPELINE_MANAGER_HPP__

enum programs_e
{
    PROGRAM_VERTEX_COLOR,
    PROGRAM_VERTEX_TEXTURED,
    PROGRAM_VERTEX_TEXTURED_SKINED,
    PROGRAM_VERTEX_TEXTURED_MATERIAL,
    PROGRAM_VERTEX_GEOMETRYPASS,
    PROGRAM_VERTEX_GEOMETRYPASS_SKINED,
    PROGRAM_VERTEX_GEOMETRYPASS_MATERIAL,
    PROGRAM_FRAGMENT_COLOR,
    PROGRAM_FRAGMENT_TEXTURED,
    PROGRAM_FRAGMENT_GEOMETRYPASS,
    PROGRAM_FRAGMENT_ADDITIVE,
    PROGRAM_COUNT    
};

class crPipelineManager
{
public:
    static crPipelineManager* Get( void );

    crPipelineManager( void );
    ~crPipelineManager( void );
    void    StartUp( void );
    void    ShutDown( void );
    void    Reload( void );
    void    BindPipeline( void)

private:
    bool    LoadProgram( void );
    
    
};

#endif //!__PIPELINE_MANAGER_HPP__