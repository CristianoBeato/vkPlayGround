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

#include "Main.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

crAppMain::crAppMain( void )
{
    // Initialize SDL
    if( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD | SDL_INIT_AUDIO ) )
        throw crException( "Error on SDL_Init:%s\n", SDL_GetError() );
    
    // initialize engine framework system
    auto framework = crFramework::Get();
    framework->StartUp();

    // initialize video system
    auto video = crVideo::Get();
    video->StartUp();

    // initialize events system
    auto events = crEventsInput::Get();    
    events->StartUp();

    // initialize audio 
    auto sound = crSoundSystem::Get();
    sound->StartUp();

    // initialize renderer
    auto renderer = crRenderSystem::Get();
    renderer->StartUp();

}

crAppMain::~crAppMain( void )
{
    auto renderer = crRenderSystem::Get();
    renderer->ShutDown();

    auto sound = crSoundSystem::Get();
    sound->ShutDown();

    auto video = crVideo::Get();
    video->ShutDown();

    auto events = crEventsInput::Get();
    events->ShutDown();

    auto framework = crFramework::Get();
    framework->ShutDown();

    SDL_Quit();
}

void crAppMain::Run(void)
{
    auto events = crEventsInput::Get();
    auto framework = crFramework::Get();
    while ( true )
    {
        // handle events
        if( !events->Pool() )
            break;

        // sumit engine logic
        framework->DoFrame();
    }
}

int main(int argc, char *argv[])
{
    try
    {
        crAppMain app = crAppMain();
        app.Run();
    }
    catch( const crException& e)
    {
        printf( "EXCEPTION TROW>%s\n", e.What() );
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}