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

#ifndef __RENDER_SYSTEM_HPP__
#define __RENDER_SYSTEM_HPP__

inline constexpr unsigned int SMP_FRAMES = 3;

struct lightInfo_t
{
    crVec3f     position;   // light position
    crVec4f     orientation;
};

class crRenderLight
{
public:
    crRenderLight( void ) {};
    ~crRenderLight( void ) {};

private: 
    lightInfo_t m_lightInfor;
};

struct entityInfo_t
{
    crVec3f     position;
    crVec4f     orientation;
};

class crRenderEntity
{
public:
    crRenderEntity( void ) {};
    ~crRenderEntity( void ) {};
    
private:
    entityInfo_t    m_entityInfo;
};

class crRenderWorld
{
public:
    crRenderWorld( void ) {};
    ~crRenderWorld( void ) {};
};

class crRenderSystem
{
public:
    crRenderSystem( void ){};
    ~crRenderSystem( void ){};

    /// @brief Render System Singleton acess
    /// @return 
    static crRenderSystem* Get( void );

    virtual bool                            StartUp( void ) = 0;
    virtual void                            ShutDown( void ) = 0;
    virtual crSharedPointer<crRenderWorld>  AllocRenderWorld( void ) = 0;
    virtual void                            FreeRenderWorld( crSharedPointer<crRenderWorld> &in_renderWorl ) = 0;
};

#endif //!__RENDER_SYSTEM_HPP__