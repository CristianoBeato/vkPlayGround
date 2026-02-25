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

#ifndef __THREADS_HPP__
#define __THREADS_HPP__

#include <SDL3/SDL_atomic.h>
#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_thread.h>

class crSignal
{
public:
    crSignal( void )
    {
        m_lock = SDL_CreateMutex();
        m_signal = SDL_CreateCondition();
    }

    ~crSignal( void )
    {
        if ( m_signal != nullptr )
        {
            SDL_DestroyCondition( m_signal );
            m_signal = nullptr;
        }

        if ( m_lock != nullptr )
        {
            SDL_DestroyMutex( m_lock );
            m_lock = nullptr;
        }        
    }

    inline void    Signal( void )
    {
        if ( SDL_GetAtomicInt( &m_waiting ) > 0 )
        {
            // wake thread
            SDL_SignalCondition( m_signal );
        }
        else
        {
            // we already have a signal let
            if ( SDL_GetAtomicInt( &m_signaled ) == 0 )
                SDL_AtomicIncRef( &m_signaled );
        }
    }

    inline void    Wait( void )
    {
        SDL_LockMutex( m_lock );

        if ( SDL_GetAtomicInt( &m_signaled ) > 0 )
        {
            // consume signal and exit
            SDL_AtomicDecRef( &m_signaled );
        }
        else
        {
            SDL_AtomicIncRef( &m_waiting );
            SDL_WaitCondition( m_signal, m_lock );
            SDL_AtomicDecRef( &m_waiting );
        }

        SDL_UnlockMutex( m_lock );
    }

private:
    SDL_AtomicInt   m_signaled;
    SDL_AtomicInt   m_waiting;
    SDL_Mutex*      m_lock;
    SDL_Condition*  m_signal;
};

class crThread
{
public:
    crThread( void );
    ~crThread( void );

    void Start( const char* in_name );
    void Wait( void );

protected:
    virtual void Run( void ) = 0;

private:
    SDL_ThreadID    m_threadID;
    SDL_Thread*     m_thread;
    static int ThreadEntryPoint( void* entry );
};

class crWorkerThread : public crThread
{
public:
    crWorkerThread( void );
    ~crWorkerThread( void );

    void    Create( void );
    void    Destroy( void );

    void    Wake( void );

protected:
    virtual void Work( void ) = 0;

private:
    volatile bool   m_running;
    volatile bool   m_iddle;               
    crSignal*       m_newWork;
    
    virtual void Run( void ) override;
};


#endif //!__THREADS_HPP__