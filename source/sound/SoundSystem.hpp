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
#ifndef __SOUND_SYSTEM_HPP__
#define __SOUND_SYSTEM_HPP__

class crSoundFilter
{
public:
    crSoundFilter( void );
    ~crSoundFilter( void ){}
};

class crSoundOccluder
{
public:
    crSoundOccluder( void ) {};
    ~crSoundOccluder( void ) {};
};

class crSoundEmiter
{
public:
    crSoundEmiter( void ){};
    ~crSoundEmiter( void ){};

    /// @brief Start stream the sound emiter
    virtual void    Play( void ) const = 0;

    /// @brief Pause the emiter streaming 
    virtual void    Pause( void ) const = 0;

    /// @brief  Stop streaming and move carret to stream start
    virtual void    Stop( void ) const = 0;

    /// @brief Is current streaming ? 
    /// @return  true if not or stoped 
    virtual bool    Playing( void ) const = 0;
    
    /// @brief  Is current paused ? 
    /// @return true if is paused ( false on stoped )
    virtual bool    Paused( void ) const = 0;

    /// @brief Is playng in loop ? 
    /// @return true if play in loop ( even stoped or paused )
    virtual bool    Loop( void ) const = 0;
};

class crSoundWorld
{
public:
    crSoundWorld( void ) {};
    ~crSoundWorld( void ) {};

    /// @brief play all emiters
    virtual void    PlayAll( void ) const = 0;

    /// @brief stop all emiters
    virtual void    StopAll( void ) const = 0;
};

class crSoundSystem
{
public:
    static crSoundSystem*  Get( void );
    crSoundSystem( void ) {};
    ~crSoundSystem( void ) {};

    /// @brief Load up audio device output
    virtual void    StartUp( void ) = 0;

    /// @brief Release audio device
    virtual void    ShutDown( void ) = 0;
    
    /// @brief mute unmute audio ( doesn't stop stream play, just don't)
    /// @param in_mude true to mute, false to unmute
    virtual void    Mute( const bool in_mude ) = 0;
    
    /// @brief Create a new sound world, 
    /// @param  
    /// @return 
    virtual crSharedPointer<crSoundWorld>   AllocWorld( void ) = 0;
    virtual void                            DeallocWorld( crSharedPointer<crSoundWorld> in_sworld ) = 0;
};

#endif //!__SOUND_SYSTEM_HPP__