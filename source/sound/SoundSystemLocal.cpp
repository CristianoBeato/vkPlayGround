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

#include "SoundSystemLocal.hpp"
#include "sound_common.hpp"

static crCVar s_device( "s_device", "-1", CVAR_SOUND | CVAR_ARCHIVE | CVAR_INTEGER, "Select the sound device index" ); 

crSoundSystem *crSoundSystem::Get(void)
{
    static crSoundSystemLocal gSoundSystem = crSoundSystemLocal();
    return &gSoundSystem;
}

crSoundSystemLocal::crSoundSystemLocal(void)
{
}

crSoundSystemLocal::~crSoundSystemLocal(void)
{
}

void crSoundSystemLocal::StartUp(void)
{
    SDL_AudioSpec specs{};
    specs.format = SDL_AUDIO_F32;
    specs.channels = 2; // TODO: implement surround
    specs.freq = 48000;

    auto selectedDevice = s_device.GetInteger();
    if ( selectedDevice >= 0 )
    {
        int numPlayDevices = 0; 
        SDL_AudioDeviceID* deviceIDs = SDL_GetAudioPlaybackDevices( &numPlayDevices );
        for ( auto i = 0; i < numPlayDevices; i++)
        {
            const char* dName = SDL_GetAudioDeviceName( deviceIDs[i] );   

        }

        int device = std::min( numPlayDevices, selectedDevice - 1 );

        m_openAudioDeviceID = SDL_OpenAudioDevice( deviceIDs[device], &specs );

    }
    else
    {
        m_openAudioDeviceID = SDL_OpenAudioDevice( SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &specs );
    }

    for ( uint32_t i = 0; i < NUM_VOICES; i++)
    {
        m_audioStreams[i] = SDL_CreateAudioStream( &specs, &specs );
    }
    

    //SDL_PauseAudioDevice
    //SDL_ResumeAudioDevice
    //SDL_AudioDevicePaused
}

void crSoundSystemLocal::ShutDown(void)
{
    for ( uint32_t i = 0; i < NUM_VOICES; i++)
    {
        SDL_DestroyAudioStream( m_audioStreams[i] );
        m_audioStreams[i] = nullptr;
    }
    
    if ( m_openAudioDeviceID != 0 )
    {
        SDL_CloseAudioDevice( m_openAudioDeviceID );
        m_openAudioDeviceID = 0;
    }
}

void crSoundSystemLocal::Mute(const bool in_mude)
{
}

crSharedPointer<crSoundWorld> crSoundSystemLocal::AllocWorld(void)
{
    return crSharedPointer<crSoundWorld>();
}

void crSoundSystemLocal::DeallocWorld(crSharedPointer<crSoundWorld> in_sworld)
{
}
