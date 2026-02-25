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

#include "Threads.hpp"

crThread::crThread( void ) : m_thread( nullptr )
{
}

crThread::~crThread( void )
{
}

void crThread::Start( const char* in_name )
{
    m_thread = SDL_CreateThread( ThreadEntryPoint, in_name, reinterpret_cast<void*>( this ) );
    if( !m_thread )
        throw crException( SDL_GetError() );

    m_threadID = SDL_GetThreadID( m_thread );
}

void crThread::Wait(void)
{
    // wait thread exit
    SDL_WaitThread( m_thread, nullptr );
}

int crThread::ThreadEntryPoint(void *entry)
{
    crThread* thread = static_cast<crThread*>( entry );
    try
    {
        thread->Run();
    }
    catch( const crException& e)
    {
        printf( "EXCEPTION TROW>%s\n", e.What() );
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

crWorkerThread::crWorkerThread( void )
{
}

crWorkerThread::~crWorkerThread( void )
{
}

void crWorkerThread::Create(void)
{
    m_newWork = new crSignal();
    crThread::Start( "Woker#" );
}

void crWorkerThread::Destroy(void)
{
    m_running = false;
    Wake();
    crThread::Wait();
    delete m_newWork;
}

void crWorkerThread::Wake(void)
{
    m_newWork->Signal();
}

void crWorkerThread::Run(void)
{
    /// woker loop
    while ( m_running )
    {
        // wait for work signal
        m_newWork->Wait();
        m_iddle = false;
        Work();
        m_iddle = true;
    }
}
