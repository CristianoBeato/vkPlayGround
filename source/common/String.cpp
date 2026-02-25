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

#include "String.hpp"

crString::crString( void ) : m_lengenth( 0 ), m_string( nullptr )
{
}

crString::crString(const char *in_string)
{
    /// get string legenth 
    Alloc( std::strlen( in_string ) );

    /// allocate string memory
    std::memcpy( m_string, in_string, m_lengenth );
}

crString::crString(const crString *&in_string)
{
   
}

crString::~crString( void )
{
    Release();
}

void crString::Alloc(const size_t in_lenght)
{
     /// get string legenth 
    m_lengenth = in_lenght;
    
    /// allocate string memory
    m_string = static_cast<char*>( MemAlloc( m_lengenth + 1  ) ); //MemAlloc<char>( m_lengenth + 1 );
    std::memset( m_string, 0x00, m_lengenth + 1 );
}

void crString::Release(void)
{
    if ( m_string != nullptr )
    {
        MemFree( m_string );
        m_string = nullptr;
    }

    m_lengenth = 0;
}
