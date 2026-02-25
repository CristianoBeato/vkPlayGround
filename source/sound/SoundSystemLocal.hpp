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

#ifndef __SOUND_SYSTEM_LOCAL_HPP__
#define __SOUND_SYSTEM_LOCAL_HPP__

#include <SDL3/SDL_audio.h>

inline constexpr uint32_t NUM_VOICES = 64;

class crSoundSystemLocal : public crSoundSystem
{
public:
    crSoundSystemLocal( void );
    ~crSoundSystemLocal( void );

    /// @brief Load up audio device output
    virtual void    StartUp( void );

    /// @brief Release audio device
    virtual void    ShutDown( void );
    
    /// @brief mute unmute audio ( doesn't stop stream play, just don't)
    /// @param in_mude true to mute, false to unmute
    virtual void    Mute( const bool in_mude );
    
    /// @brief Create a new sound world, 
    /// @param  
    /// @return 
    virtual crSharedPointer<crSoundWorld>   AllocWorld( void );
    virtual void                            DeallocWorld( crSharedPointer<crSoundWorld> in_sworld );

private:
    SDL_AudioDeviceID       m_openAudioDeviceID;
    SDL_AudioStream*        m_audioStreams[NUM_VOICES];
};

#endif //!__SOUND_SYSTEM_LOCAL_HPP__