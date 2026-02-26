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

#ifndef __RENDER_SYSTEM_LOCAL_HPP__
#define __RENDER_SYSTEM_LOCAL_HPP__

#include "Bounds.hpp"
#include "RenderSystem.hpp"
#include "backend/Vulkan/Core.hpp"
#include "backend/Backend.hpp"
#include "material/Material.hpp"
#include "world/Light.hpp"

class crRenderSystemLocal : 
    public crRenderSystem,
    public crThread
{
public:
    crRenderSystemLocal( void );
    ~crRenderSystemLocal( void );
    virtual bool                            StartUp( void );
    virtual void                            ShutDown( void );
    virtual crSharedPointer<crRenderWorld>  AllocRenderWorld( void );
    virtual void                            FreeRenderWorld( crSharedPointer<crRenderWorld> &in_renderWorl );

protected:
    virtual void Run( void ); /// render thread loop

private:
    volatile bool   m_running;
    bool            m_renderInitialized;
    void            InitRendersystem( void );
    void            ReleaseRenderSystem( void );
    void            BeginFrame( void );
    void            EndFrame( void );
};


#endif //!__RENDER_SYSTEM_LOCAL_HPP__