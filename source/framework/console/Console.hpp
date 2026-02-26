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

    void    StartUp( void );
    void    ShutDown( void );

    /// @brief Print debug info messages, that can be ignored via cVar
    static void Verbose( const char* in_format, ... );

    /// @brief Print geral message information
    static void Print( const char* in_format, ... );

    /// @brief Print warning info  
    static void Warning( const char* in_format, ... );

    /// @brief Error message handling 
    static void Error( const char* in_format, ... );

protected:
    void    AppendPrint( const char* in_msg, const size_t in_legenth );
    void    AppendWarning( const char* in_warn, const size_t in_legenth );
    void    AppendError( const char* in_error, const size_t in_legenth );

private:
    crList<char*>   m_prints;   // store all messages
    crList<char*>   m_warnings; // store all warnings
    crList<char*>   m_errors;
};

#endif //!__CONSOLE_HPP__