
#ifndef __CONSOLE_VARIABLES_HPP__
#define __CONSOLE_VARIABLES_HPP__

#include <variant>

class crCvarManager
{
public:
    crCvarManager( void );
    ~crCvarManager( void );

private:

};

enum cvarFlags_t
{
    CVAR_BOOL       = 1 << 0,
    CVAR_INTEGER    = 1 << 1,
    CVAR_FLOAT      = 1 << 2,
    CVAR_STRING     = 1 << 3,
    CVAR_RENDERER   = 1 << 4,
    CVAR_SOUND      = 1 << 5,
    CVAR_ARCHIVE    = 1 << 6,
};

class crCVar
{    
public:
    crCVar( const crString in_name, const crString in_defalt, const uint32_t in_flags, const crString in_decription );
    ~crCVar( void );

    void            SetBool( const bool in_value );
    void            SetInteger( const int in_value );
    void            SetFloat( const float in_value );
    void            SetString( const crString in_value );
    const bool      GetBool( void ) const;
    const int       GetInteger( void ) const;
    const float     GetFloat( void ) const;
    const char*     GetString( void ) const;

private:
    uint32_t                                    m_flags;
    crString                                    m_name;
    crString                                    m_description;
    std::variant<bool, int, float, crString>    m_value;
};


#endif //!__CONSOLE_VARIABLES_HPP__