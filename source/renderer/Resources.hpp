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

#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__

class crResources
{
public:
    crResources( void );
    ~crResources( void );

    virtual bool    Create( const uint32_t in_flags, const crString in_name, const crString in_sourceFile ) = 0;
    virtual void    Destroy( void ) = 0;
    virtual bool    Load( void ) = 0;
    
    /// name of the resource
    crString        Name( void ) const { return m_name; }
    
    /// source file path
    crString        File( void ) const { return m_sourceFile; }

protected:
    crString    m_name;
    crString    m_sourceFile;
};

#endif //!__RESOURCES_HPP__