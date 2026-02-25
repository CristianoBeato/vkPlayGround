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

#include "Video.hpp"

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>


crVideo *crVideo::Get(void)
{
    static crVideo video = crVideo();
    return &video;
}

crVideo::crVideo( void ) : 
    m_mainWindow( nullptr ),
    m_editorWindow( nullptr )
{
}

crVideo::~crVideo( void )
{
}

void crVideo::StartUp(void)
{
    SDL_WindowFlags windowProps = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS;

    // Initialize Vulkan Libary
    if ( !SDL_Vulkan_LoadLibrary( nullptr ) )
        throw crException( SDL_GetError() );
    
    /// list suportated window modes 
    ListModes();
    
    m_width = 800;
    m_heigth = 600;
    // create main window
    m_mainWindow = SDL_CreateWindow( "NAME", m_width, m_heigth, windowProps ); // TODO: Create Hiden
    if( m_mainWindow == nullptr )
        throw crException( SDL_GetError() );
}

void crVideo::ShutDown(void)
{
    ///
    if ( m_editorWindow == nullptr )
    {
        SDL_DestroyWindow( m_editorWindow );
        m_editorWindow = nullptr;
    }
    
    ///
    if ( m_mainWindow == nullptr )
    {
        SDL_DestroyWindow( m_mainWindow );
        m_mainWindow = nullptr;
    }

    SDL_Vulkan_UnloadLibrary();
}

void crVideo::ShowWindow(void)
{
    if ( !m_mainWindow )
        return;

    SDL_ShowWindow( m_mainWindow );
}

void crVideo::HideWindow(void)
{
    if ( !m_mainWindow )
        return;

    SDL_HideWindow( m_mainWindow );
}

void crVideo::ListModes(void)
{
    int displayCount = 0;
    auto displaysIDs = SDL_GetDisplays( &displayCount );

    for ( int i = 0; i < displayCount; i++)
    {
        int modeCount = 0;
        auto displayID = displaysIDs[i];
        auto modeList = SDL_GetFullscreenDisplayModes( displayID, &modeCount );

        // SDL_GetDisplayBounds;
        // SDL_GetDisplayUsableBounds;
        // SDL_GetNaturalDisplayOrientation;
        // SDL_GetCurrentDisplayOrientation;
        // SDL_GetDisplayContentScale; 
        // crString name = crString( SDL_GetDisplayName( displayID ) );
        // auto 
    }
}
