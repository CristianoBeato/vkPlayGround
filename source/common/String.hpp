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

#ifndef __STRING_HPP__
#define __STRING_HPP__

class crString
{
public:
    crString( void );
    crString( const char* in_string );
    crString( const crString* &in_string );
    ~crString( void );

    inline size_t   Lengenth( void ) const { return m_lengenth; }
    inline bool     Compare( const crString& in_string ) const;
    inline bool     Comparei( const crString& in_string ) const;
    inline bool     operator == ( const crString& in_string ) const;
    inline bool     operator != ( const crString& in_string ) const;
    inline operator char*( void ) const { return m_string; }
    inline operator bool( void ) const { return m_string != nullptr; }
    inline const char*  c_str( void ) const { return m_string; }
private:
    size_t  m_lengenth;
    char*   m_string;
    
    void    Alloc( const size_t in_lenght );
    void    Release( void );
};

bool crString::Compare(const crString &in_string) const
{
    return std::strncmp( m_string, in_string.m_string, m_lengenth ) == 0;
}

bool crString::Comparei(const crString &in_string) const
{
    return SDL_strncasecmp( m_string, in_string.m_string, m_lengenth ) == 0;
}

bool crString::operator==(const crString &in_string) const
{
    return Compare( in_string );
}

bool crString::operator!=(const crString &in_string) const
{
    return !Compare( in_string );
}

#endif __STRING_HPP__