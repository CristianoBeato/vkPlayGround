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

#ifndef __FRAMEWORK_HPP__
#define __FRAMEWORK_HPP__

#include "console/Console.hpp"
#include "console/ConsoleVariables.hpp"

class crFramework
{
public:
    static crFramework* Get( void );
    crFramework( void );
    ~crFramework( void );

    void        StartUp( void );
    void        ShutDown( void );
    void        DoFrame( void );
    uint64_t    FrameCount( void ) const { return m_frame; }
private:
    uint64_t    m_frame;
};



#endif //!__FRAMEWORK_HPP__