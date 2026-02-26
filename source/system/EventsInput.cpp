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

#include "EventsInput.hpp"

#include <SDL3/SDL_events.h>

crEventsInput *crEventsInput::Get(void)
{
    static crEventsInput eventsInput = crEventsInput();
    return &eventsInput;
}

crEventsInput::crEventsInput(void)
{
}

crEventsInput::~crEventsInput( void )
{
}

void crEventsInput::StartUp(void)
{
}

void crEventsInput::ShutDown(void)
{
}

bool crEventsInput::Pool(void)
{
    SDL_Event event{};
    while ( SDL_PollEvent( &event ) )
    {
        switch ( event.type )
        {
            /// quit event
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            case SDL_EVENT_QUIT:
                return false;
        
        default:
            break;
        }
    }

    return true;
}
