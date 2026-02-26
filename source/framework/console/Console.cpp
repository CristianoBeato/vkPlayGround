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

void crConsole::Verbose(const char *in_format, ...)
{
    char message[MAX_STRING_LENGTH]{};
    va_list args;
    va_start( args, in_format );
    std::vsnprintf( message, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    std::cout << message;
}

void crConsole::Print(const char *in_format, ...)
{
    char message[MAX_STRING_LENGTH]{};
    va_list args;
    va_start( args, in_format );
    std::vsnprintf( message, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    std::cout << message;
}

void crConsole::Warning( const char *in_format, ... )
{
    char warn[MAX_STRING_LENGTH]{};
    va_list args;
    va_start( args, in_format );
    std::vsnprintf( warn, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    std::cout << YELOW_STRING << warn << RESET_STRIMG;
}

void crConsole::Error( const char *in_format, ... )
{
    char error[MAX_STRING_LENGTH]{};
    va_list args;
    va_start( args, in_format );
    std::vsnprintf( error, k_MAX_ERR_STRING_LEN, in_format, args);
    va_end( args );

    std::cerr << RED_STRING << error << RESET_STRIMG;
}
