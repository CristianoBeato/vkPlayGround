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

#ifndef __LIST_HPP__
#define __LIST_HPP__

template< typename __type__>
class crList
{
public:
    typedef __type__&           reference;
    typedef const __type__&     const_reference;
    typedef __type__*           pointer;
    typedef const __type__*     const_pointer;
    
    crList( void );
    ~crList( void );

    inline void     Clear( void );
    inline void     Reserve( const uint32_t in_num );   // Allocate buffer for num objects
    inline void     Resize( const uint32_t in_num ); // Set the number of valid entries. New entries will be zero
    inline uint32_t Append( const_reference val );
    inline uint32_t Count( void ) const;
    inline size_t   Size( void ) const;
    inline size_t   AllocatedSize( void ) const;
 
    inline reference        operator[]( const int i );
    inline const_reference  operator[]( const int i ) const;
    inline pointer          operator&( void ) { return m_data; }
    inline const_pointer    operator&( void ) const { return m_data; }
    pointer GetData( void ) const  { return m_data; }
    
private:
    uint32_t    m_granularity;  //
    uint32_t    m_numEntries;   // Number of objects stored
    uint32_t    m_numAllocated; // 
    pointer     m_data;
};

template< typename __type__>
crList<__type__>::crList( void ) :
    m_granularity( 8 ),
    m_numEntries( 0 ),
    m_numAllocated( 0 ),
    m_data( nullptr )
{
}

template< typename __type__>
crList<__type__>::~crList( void )
{
    Clear();
}

template <typename __type__>
void crList<__type__>::Clear(void)
{
    if ( m_data != nullptr )
    {
        MemFree( m_data );
        m_data = nullptr;
    }

    m_numEntries = 0;
    m_numAllocated = 0;
}

template <typename __type__>
void crList<__type__>::Reserve(const uint32_t in_num)
{
    uint32_t aligned_num = 0;

    //
    if ( in_num == 0 )
        return;

    // Fórmula de teto (ceil) para inteiros:
    // (x + y - 1) / y resulta em ceil(x/y)
    // O número de blocos necessários:
    uint32_t num_blocks = (in_num + m_granularity - 1) / m_granularity;
    aligned_num = num_blocks * m_granularity;

    // never scale down
    if ( aligned_num > m_numAllocated )
    {
        if ( m_numAllocated > 0 )
            m_data = static_cast<pointer>( MemRealloc( m_data, sizeof( __type__ ) * aligned_num ) );
        else
            m_data = static_cast<pointer>( MemAlloc( sizeof( __type__ ) * aligned_num ) );

        m_numAllocated = aligned_num;
    }
}

template <typename __type__>
void crList<__type__>::Resize( const uint32_t in_num )
{
    Reserve( in_num );
    m_numEntries = in_num;
}

template <typename __type__>
uint32_t crList<__type__>::Append(const_reference val)
{
    uint32_t index = 0;

    if ( m_numEntries < m_numAllocated )
    {
        index = m_numEntries++;
        m_data[index] = val;
    }
    else
    {
        Reserve( m_numAllocated + m_granularity );    
        return Append( val );
    }
    
    return index;
}

template <typename __type__>
uint32_t crList<__type__>::Count(void) const
{
    return m_numEntries;
}

template <typename __type__>
size_t crList<__type__>::Size(void) const
{
    return m_numEntries * sizeof( __type__ );
}

template <typename __type__>
size_t crList<__type__>::AllocatedSize(void) const
{
    return m_numAllocated * sizeof( __type__ );
}

template <typename __type__>
crList<__type__>::reference crList<__type__>::operator[](const int i)
{
    assert( ( i > -1 && i < m_numEntries ) && "Inalid Range" );
    return m_data[i];
}

template <typename __type__>
crList<__type__>::const_reference crList<__type__>::operator[](const int i) const
{
    assert( ( i > -1 && i < m_numEntries ) && "Inalid Range" );
    return m_data[i];
}

#endif //!__LIST_HPP__