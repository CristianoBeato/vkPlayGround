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

#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

class crException
{
public:
    crException( void )
    {
        m_error[0] = 0x00; // '\0' null terminator;
    }

    crException( const char* in_format, ... )
    {
        va_list args;
        va_start( args, in_format );
        // O segredo é o reinterpret_cast para que a vsnprintf aceite o buffer de char8_t
        std::vsnprintf( m_error, k_MAX_ERR_STRING_LEN, in_format, args);
        va_end(args);
    }

    ~crException( void )
    {
    }

    inline const char* What( void ) const { return m_error; }

private:
    char    m_error[k_MAX_ERR_STRING_LEN];
};

#endif __EXCEPTION_HPP__