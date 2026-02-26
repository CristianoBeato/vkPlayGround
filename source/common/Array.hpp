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

#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__
template< typename __type__, uint32_t __count__>
class crArray
{
public:
    typedef __type__&       reference;
    typedef __type__*       pointer;
    typedef const __type__& const_reference;
    typedef const __type__* const_pointer;
     
    crArray( void );
    ~crArray( void );

    inline uint32_t         Count( void ) const { return __count__; }
    inline pointer          Ptr( void );
    inline const_pointer    Ptr( void ) const;
    inline reference        operator[]( const uint32_t in_index );
    inline const_reference  operator[]( const uint32_t in_index ) const;
    inline pointer          operator&( void );
    inline const_pointer    operator&( void ) const;

private:
    __type__    m_elements[__count__];
};

template< typename __type__, uint32_t __count__>
crArray<__type__, __count__>::crArray( void )
{
    std::memset( m_elements, 0x00, sizeof(__type__) * __count__ );
}

template< typename __type__, uint32_t __count__>
crArray<__type__, __count__>::~crArray( void )
{
    std::memset( m_elements, 0x00, sizeof(__type__) * __count__ );
}

template <typename __type__, uint32_t __count__>
crArray<__type__, __count__>::pointer crArray<__type__, __count__>::Ptr(void)
{
    return m_elements ;
}

template <typename __type__, uint32_t __count__>
crArray<__type__, __count__>::const_pointer crArray<__type__, __count__>::Ptr(void) const
{
    return m_elements;
}

template <typename __type__, uint32_t __count__>
crArray<__type__, __count__>::reference crArray<__type__, __count__>::operator[](const uint32_t in_index)
{
    static_assert( in_index < __count__ );
    return m_elements[in_index];
}

template <typename __type__, uint32_t __count__>
crArray<__type__, __count__>::const_reference crArray<__type__, __count__>::operator[](const uint32_t in_index) const
{
    static_assert( in_index < __count__ );
    return m_elements[in_index];
}

template <typename __type__, uint32_t __count__>
crArray<__type__, __count__>::pointer crArray<__type__, __count__>::operator&( void )
{
    return m_elements;
}

template <typename __type__, uint32_t __count__>
crArray<__type__, __count__>::const_pointer crArray<__type__, __count__>::operator&( void ) const
{
    return m_elements;
}

#endif //!__ARRAY_HPP__