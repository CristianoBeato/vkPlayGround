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

#ifndef __VIDEO_HPP__
#define __VIDEO_HPP__

typedef struct SDL_Window SDL_Window;

struct windowMode_t
{
    float       scale;
    crString    name;
};

class crVideo
{
public:
    static crVideo* Get( void );

    crVideo( void );
    ~crVideo( void );

    void            StartUp( void );
    void            ShutDown( void );
    void            ShowWindow( void );
    void            HideWindow( void );
    uint32_t        Width( void ) const { return m_width; }
    uint32_t        Height( void ) const { return m_heigth; }
    SDL_Window *    MainWindow( void ) const { return m_mainWindow; }
    SDL_Window *    EditorWindow( void ) const { return m_editorWindow; }

private:
    uint16_t        m_currentMode;
    uint32_t        m_width;
    uint32_t        m_heigth;
    SDL_Window *    m_mainWindow;
    SDL_Window *    m_editorWindow;

    void    ListModes( void );
};

#endif //!__VIDEO_HPP__