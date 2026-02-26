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

#include "Console.hpp"
#include <iostream>

// TODO: Flush error log to file

constexpr uint32_t MAX_STRING_LENGTH = 4086;
static const char RED_STRING[] = { "\033[31m" };
static const char YELOW_STRING[] = { "\033[33m" };
static const char RESET_STRIMG[] = { "\033[0m" };

crConsole *crConsole::Get(void)
{
    static crConsole gConsole = crConsole();
    return &gConsole;
}

crConsole::crConsole(void)
{
}

crConsole::~crConsole( void )
{
}

void crConsole::StartUp(void)
{
}

void crConsole::ShutDown(void)
{
    uint32_t i = 0;

    // clear error registry
    for ( i = 0; i < m_errors.Count(); i++)
    {
        MemFree( m_errors[i] );
    }

    m_errors.Clear();

    // clear warnings registry
    for ( i = 0; i < m_warnings.Count(); i++)
    {
        MemFree( m_warnings[i] );
    }

    m_warnings.Clear();

    // clear miscelaneous registry
    for ( i = 0; i < m_errors.Count(); i++)
    {
        MemFree( m_errors[i] );
    }
    
    m_errors.Clear();
}

void crConsole::Verbose(const char *in_format, ...)
{
    char message[MAX_STRING_LENGTH]{};
    auto con = crConsole::Get();

    va_list args;
    va_start( args, in_format );
    std::vsnprintf( message, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    con->AppendPrint( message, std::strlen( message ) );
    std::cout << message;
}

void crConsole::Print(const char *in_format, ...)
{
    char message[MAX_STRING_LENGTH]{};
    auto con = crConsole::Get();

    va_list args;
    va_start( args, in_format );
    std::vsnprintf( message, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    con->AppendPrint( message, std::strlen( message ) );
    std::cout << message;
}

void crConsole::Warning( const char *in_format, ... )
{
    char warn[MAX_STRING_LENGTH]{};
    auto con = crConsole::Get();

    va_list args;
    va_start( args, in_format );
    std::vsnprintf( warn, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    con->Warning( warn, std::strlen( warn ) );
    std::cout << YELOW_STRING << warn << RESET_STRIMG;
}

void crConsole::Error( const char *in_format, ... )
{
    char error[MAX_STRING_LENGTH]{};
    auto con = crConsole::Get();

    va_list args;
    va_start( args, in_format );
    std::vsnprintf( error, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    con->Error( error, std::strlen( error ) );
    std::cerr << RED_STRING << error << RESET_STRIMG;
}

void crConsole::AppendPrint(const char *in_msg, const size_t in_legenth )
{
    // made a copy of the string
    char* message = static_cast<char*>( MemAlloc( in_legenth + 1 /* nullchar ('/0') */ ) );
    std::memset( message, 0x00, in_legenth + 1 );
    
    // store string
    m_prints.Append( message );
}

void crConsole::AppendWarning( const char *in_warn, const size_t in_legenth )
{
    // made a copy of the string
    char* message = static_cast<char*>( MemAlloc( in_legenth + 1 /* nullchar ('/0') */ ) );
    std::memset( message, 0x00, in_legenth + 1 );
    
    // store string
    m_warnings.Append( message );
}

void crConsole::AppendError(const char *in_error, const size_t in_legenth )
{
    // made a copy of the string
    char* message = static_cast<char*>( MemAlloc( in_legenth + 1 /* nullchar ('/0') */ ) );
    std::memset( message, 0x00, in_legenth + 1 );
    
    // store string
    m_errors.Append( message );
}
