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

#ifndef __BACKEND_HPP__
#define __BACKEND_HPP__

#include "Vulkan/Core.hpp"

class crBackend
{
public:
    crBackend( void );
    ~crBackend( void );
    
    static crBackend*  Get( void );

    void    StartUp( void );
    void    ShutDown( void );
    void    SetBuffer( void );
    void    SwapBuffers( void );
    void    DrawView( void );
    crSwapchain*    Swapchain( void ) const { return m_swapchain; }

private:
    uint64_t            m_frame;
    crView*             m_viewChain;
    crSwapchain*        m_swapchain;
    crCommandbuffer*    m_graphicCMD;
    crFramebuffer*      m_geometrFB;
    crFramebuffer*      m_defaultFB;
    crPipeline*         m_staticMeshAdditive;
    crPipeline*         m_dyanmicMeshAdditive;
    
    void    DrawSurface( const crSurface* in_surface );
    void    LighPass( void );

    /// @brief draw all opaque geometries 
    void    OpaqueGeometryPass( void );

    /// @brief draw all transparent geometies
    void    TransparenGeometryPass( void );

    /// @brief fill geomtry buffer
    void    FillGeometryBuffer( void );
 
    // perform the light pass on geometry buffer surfaces
    void    LighBlending( void );
 
    /// @brief Translucent pass ( Glass/Water )
    void    Translucent( void );

    /// Perform a additive pass
    void    AdditivePass( void );
};


#endif //!__BACKEND_HPP__