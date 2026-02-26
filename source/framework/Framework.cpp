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

#include "Framework.hpp"

crFramework *crFramework::Get(void)
{
    static crFramework gFramework = crFramework();
    return &gFramework;
}

crFramework::crFramework(void)
{
}

crFramework::~crFramework( void )
{
}

void crFramework::StartUp(void)
{
    auto console = crConsole::Get();
    console->StartUp();
}

void crFramework::ShutDown(void)
{
    auto console = crConsole::Get();
    console->ShutDown();
}

void crFramework::DoFrame(void)
{
}
