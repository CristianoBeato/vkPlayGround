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

#define _DEBUG_STRING_COPY 0

crString::crString( void ) : m_allocated( 0 ), m_string( nullptr )
{
}

crString::crString( const char *in_string ) : m_allocated( 0 ), m_string( nullptr )
{
    /// get string legenth 
    Alloc( std::strlen( in_string ) );

    /// allocate string memory
    std::strncpy( m_string, in_string, m_allocated - 1 );

#if _DEBUG_STRING_COPY
    assert( std::memcmp( m_string, in_string, m_lengenth ) == 0 );
#endif 
}

crString::crString( const crString &in_string ) : m_allocated( 0 ), m_string( nullptr )
{
   Alloc( in_string.Lengenth() );
   std::strncpy( m_string, in_string.m_string, m_allocated - 1 );

#if _DEBUG_STRING_COPY
    assert( std::memcmp( m_string, in_string.c_str(), m_lengenth ) == 0 );
#endif 
}

crString::~crString( void )
{
    Release();
}

void crString::Replace( const char in_from, const char in_to )
{
    char* str = m_string;
    while ( *str != '\0' )
    {
        if( *str == in_from )
            *str = in_to;
        str++;
    }
}

void crString::StripFileName( void )
{
    // Search for the last occurrence of '/'
    char* last_slash = std::strrchr( m_string, '/');

    /// make the rest of string null
    while ( last_slash != nullptr && ( *( last_slash + 1 ) != '\0' ) )
    {
        *(++last_slash) = '\0'; // Keep the trailing slash, or '0' to remove
    } 
}

void crString::StripFileExtension(void)
{
    // Search for the last occurrence of '.'
    char* last_dot = std::strrchr( m_string, '.');
    while ( ( last_dot != nullptr ) && (*last_dot != '\0' ) )
    {
        /// make the rest of string null
        *(last_dot) = '\0';
    }
}

void crString::Alloc( const size_t in_size )
{
    if( m_allocated == 0 )
    {
        /// set string legenth 
        m_allocated = in_size + 1;
    
        /// allocate string memory
        m_string = MemAlloc<char>( m_allocated );
        /// fill string whith null teminator
        std::memset( m_string, '\0', m_allocated );
    }
    else
    {
        m_string = MemRealloc<char>( m_string, m_allocated );

        /// fill the rest o the string whith null terminator
        std::memset( m_string + m_allocated, '\0', in_size - m_allocated );
        m_allocated = in_size + 1;
    }
}

void crString::Release(void)
{
    if ( m_string != nullptr )
    {
        MemFree( m_string );
        m_string = nullptr;
    }

    m_allocated = 0;
}
