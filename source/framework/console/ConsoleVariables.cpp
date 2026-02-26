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

#include "ConsoleVariables.hpp"
#include "Console.hpp"

crCvarManager::crCvarManager( void ) 
{
}

crCvarManager::~crCvarManager( void )
{
}

crCVar::crCVar( const char* in_name, const char* in_defalt, const uint32_t in_flags, const char* in_decription  ) : 
    m_name( in_name ),
    m_flags( in_flags ),
    m_description( in_decription )
{
    if ( ( m_flags & CVAR_BOOL ) == CVAR_BOOL )
        SetBool( std::atoi( in_defalt ) == 0 ? false : true );
    else if( ( m_flags & CVAR_INTEGER ) == CVAR_INTEGER )
        SetInteger( std::atoi( in_defalt ) );
    else if( ( m_flags & CVAR_FLOAT ) == CVAR_FLOAT )
        SetFloat( std::atof( in_defalt ) );
    else if( ( m_flags & CVAR_STRING ) == CVAR_STRING )
        SetString( in_defalt );
}

crCVar::~crCVar( void )
{
}

void crCVar::SetBool( const bool in_value )
{
    m_value = in_value;
}

void crCVar::SetInteger( const int in_value )
{
    m_value = in_value;
}

void crCVar::SetFloat( const float in_value )
{
    m_value = in_value;
}

void crCVar::SetString( const char* in_value )
{
    m_value = in_value;
}

const bool crCVar::GetBool(void) const
{
    assert( ( m_flags & CVAR_BOOL ) == CVAR_BOOL );
    return std::get<bool>( m_value );
}

const int crCVar::GetInteger(void) const
{
    assert( ( m_flags & CVAR_INTEGER ) == CVAR_INTEGER );
    return std::get<int>( m_value );
}

const float crCVar::GetFloat(void) const
{
    assert( ( m_flags & CVAR_FLOAT ) == CVAR_FLOAT );
    return std::get<float>( m_value );
}

const char *crCVar::GetString(void) const
{
    assert( ( m_flags & CVAR_STRING ) == CVAR_STRING );
#if 0
    return std::get<crString>( m_value ).c_str();
#else
    return std::get<const char*>( m_value );
#endif
}
