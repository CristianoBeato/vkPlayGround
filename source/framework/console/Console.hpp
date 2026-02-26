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

#ifndef __CONSOLE_HPP__
#define __CONSOLE_HPP__

class crConsole
{
public:
    static crConsole* Get( void );

    crConsole( void );
    ~crConsole( void );
    
    static void Verbose( const char* in_format, ... );
    static void Print( const char* in_format, ... );
    static void Warning( const char* in_format, ... );
    static void Error( const char* in_format, ... );

private:
    crList<char*>   m_prints;   // store all messages
    crList<char*>   m_warnings; // store all warnings
    crList<char*>   m_errors;
};

#endif //!__CONSOLE_HPP__