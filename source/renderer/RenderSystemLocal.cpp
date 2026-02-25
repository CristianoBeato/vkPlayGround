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

#include "RenderSystemLocal.hpp"

crRenderSystem *crRenderSystem::Get(void)
{
    static crRenderSystemLocal local = crRenderSystemLocal();
    return &local;
}

crRenderSystemLocal::crRenderSystemLocal( void ) : 
    m_context( nullptr )
{
}

crRenderSystemLocal::~crRenderSystemLocal( void )
{
}

bool crRenderSystemLocal::StartUp(void)
{
    m_running = true;
    crThread::Start( "Renderer" );
    return true;
}

void crRenderSystemLocal::ShutDown(void)
{
    // singal to terminate render thread
    m_running = false;
    // wait thread exit 
    crThread::Wait();    
}

crSharedPointer<crRenderWorld> crRenderSystemLocal::AllocRenderWorld(void)
{
    crSharedPointer<crRenderWorld> world = crSharedPointer<crRenderWorld>( 1, 8 ); 
    return world;
}

void crRenderSystemLocal::FreeRenderWorld( crSharedPointer<crRenderWorld> &in_renderWorl )
{
    crSharedPointer<crRenderWorld>::Free( in_renderWorl );
}

void crRenderSystemLocal::Run(void)
{
    InitRendersystem();

    /// after video initialization we can show the window
    auto video = crVideo::Get();
    video->ShowWindow();

    while ( m_running )
    {
        BeginFrame();
        EndFrame();
    }
    ReleaseRenderSystem();
}

void crRenderSystemLocal::InitRendersystem(void)
{
    m_context = new vkContext();
    m_context->StarUp();
    
    auto backend = crBackend::Get();
    backend->StartUp();

    m_renderInitialized = true;
}

void crRenderSystemLocal::ReleaseRenderSystem(void)
{
    auto backend = crBackend::Get();
    backend->ShutDown();

    if ( m_context != nullptr )
    {
        m_context->Shutdown();
        delete m_context;
        m_context = nullptr;
    }
    
    m_renderInitialized = false;
}

void crRenderSystemLocal::BeginFrame(void)
{
    auto backend = crBackend::Get();
    backend->BeginFrame();
}

void crRenderSystemLocal::EndFrame(void)
{
    auto backend = crBackend::Get();
    
    /// present to window and swap buffers 
    backend->SwapBuffers();
}
