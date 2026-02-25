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
#ifndef __POINTERS_HPP__
#define __POINTERS_HPP__


/// @brief Base pointer estructure 
/// @tparam __type__ the pointer holding type
/// @tparam __tag__ Memory tag definition
template< typename __type__ >
class crPointer
{
public:
    typedef __type__*       pointer;
    typedef const __type__* const_pointer; 
    typedef __type__&       reference;
    typedef const __type__& const_reference;
    
    crPointer( void ) : m_data( nullptr ), m_size( 0 )
    {
    }

    virtual ~crPointer( void )
    {
    }

    /// @brief Fill the pointer with zero values.
    inline void              SetZero( void )
    {
        std::memset( m_data, 0x00, m_size );
    }

    /// @brief Fills the pointer with values ​​defined by
    /// @param in_val the value to be set in the pointer.
    inline void              SetValue( const int in_val )
    {
        std::memset( m_data, in_val, m_size );
    }

    /// @brief Perfor a memcpy to the pointer 
    /// @param in_src the source data buffer
    /// @param in_offset the offset in the destination pointer ( in bytes )
    /// @param in_bytes the bytes count to copy
    /// @return 
    inline void              Memcpy( const void *in_src, const size_t in_offset, const size_t in_bytes )
    {
        /// check for overflow
        assert( ( in_offset + in_bytes ) < m_size );
        std::memcpy( reinterpret_cast<byte*>( m_data ) + in_offset, in_src, in_bytes );
    }

    inline int               Memcmp( const void *in_src, const size_t in_offset, const size_t in_bytes )
    {
        /// check for overflow
        assert( ( in_offset + in_bytes ) < m_size );
        return std::memcmp( reinterpret_cast<byte*>( m_data ) + in_offset, in_src, in_bytes );
    }

    /// @brief The size of data allocated in the pointer.
    inline const size_t      Size( void ) const { return m_size; }

    /// @brief Manual access to the pointer 
    inline const_pointer     Data( void ) const { return m_data; }

    /// @brief Operator overloading -> to access members of the pointed object
    inline pointer           operator->( void ) { return m_data; }

    /// @brief Operator overloading -> to access members of the pointed object.
    inline const_pointer     operator->( void ) const { return m_data; }

    /// @brief Overloading the "*" operator to dereference the pointer.
    inline reference         operator*( void ) { return *m_data; }

    /// @brief Overloading the "*" operator to dereference the pointer.
    inline const_reference   operator*( void ) const { return *m_data; }

    /// @brief 
    /// @param i 
    /// @return 
    inline reference         operator[]( const int &i ) { return m_data[i]; }

    /// @brief 
    /// @param i 
    /// @return 
    inline const_reference   operator[]( const int &i ) const {  return m_data[i]; }
    
    /// @brief 
    /// @param  
    inline operator __type__*( void ) const { return m_data; }

    /// @brief 
    /// @param  
    inline operator bool( void ) const { return m_data != nullptr; }

protected:
    size_t      m_size;
    pointer     m_data;
};

/// @brief Base pointer estructure ( RAII patern )
/// @tparam __type__ the pointer holding type
/// @tparam __tag__ Memory tag definition
template< typename __type__ >
class crStaticPointer : public crPointer< __type__ >
{
public:
    typedef __type__*       pointer;
    typedef const __type__* const_pointer; 
    typedef __type__&       reference;
    typedef const __type__& const_reference;

    crStaticPointer( void ) : crPointer<__type__>()
    {
    }

    explicit crStaticPointer( const uint32_t in_count, const size_t in_alignament ) : crPointer<__type__>()
    {
        this->m_data = static_cast<pointer>( MemAlloc<__type__>( in_count ), in_alignament );
        this->m_size = sizeof( this->m_data ); // TODO: align size first, and get it to allocation
    }

    ~crStaticPointer( void )
    {
        /// Release data at poiter exit
        if ( crPointer<__type__>::m_data != nullptr )
        {
            MemFreeAligned( this->m_data );
            this->m_data = nullptr;
        }

        this->m_size = 0;
    }

    inline void Duplicate( const crStaticPointer &in_copySource )
    {
        this->m_size = in_copySource.m_size;
        this->m_data = static_cast<pointer>( Mem_Alloc16( this->m_size ) );
        std::memcpy( this->m_data, in_copySource.m_data, this->m_size );
    }

private:
    // Prevents copying to avoid two objects attempting to delete the same pointer.
    crStaticPointer( const crStaticPointer& ) = delete;
    crStaticPointer& operator = ( const crStaticPointer& ) = delete;
};

template< typename __type__ >
class crSharedPointer : public crPointer< __type__ >
{
public:
    typedef __type__*       pointer;
    typedef const __type__* const_pointer; 
    typedef __type__&       reference;
    typedef const __type__& const_reference;

    crSharedPointer( void ) : crPointer<__type__>()
    {
    }

    explicit crSharedPointer( const uint32_t in_count, const size_t in_alignament ) : crPointer<__type__>()
    {
        this->m_data = static_cast<pointer>( MemAllocAligned( sizeof( __type__), in_alignament ) );
        this->m_size = sizeof( this->m_data ); // TODO: align size first, and get it to allocation
    }

    crSharedPointer( const crSharedPointer& in_ref )
    {
        this->m_data = in_ref.m_data;
        this->m_size = in_ref.m_size;
    }

    static void Free( crSharedPointer& in_ref )
    {
        if ( in_ref.m_data != nullptr )
        {
            MemFreeAligned( in_ref.m_data );
            in_ref.m_data = nullptr;
        }

        in_ref.m_size = 0;
    }

    crSharedPointer& operator = ( const crSharedPointer& in_ref )
    {
        this->m_data = in_ref.m_data;
        this->m_size = in_ref.m_size;
    }

    ~crSharedPointer( void )
    {
        this->m_data = nullptr;
        this->m_size = 0;
    }

    //inline void Duplicate( const crStaticPointer &in_copySource )
    //{
    //    this->m_size = in_copySource.m_size;
    //    this->m_data = static_cast<pointer>( MemAllocAligned( this->m_size ) );
    //    std::memcpy( this->m_data, in_copySource.m_data, this->m_size );
    //}
};


// BEATO HELP:
// GCC atomic
// __sync_fetch_and_add: atomically increase and return the old value ( old = x, x += n, old )
// __sync_fetch_and_sub: atomically decrement and return the old value ( old = x, x -= n, old )
// __sync_add_and_fetch atomically increase and return the new value ( x += n, x )
// __sync_sub_and_fetch atomically decrements and return the new value ( x -= n, x )

// STD C11 atomic
// atomic_fetch_add: Atomically replaces the value pointed by obj with the result of addition of arg to the old value of obj
// atomic_fetch_sub: Atomically replaces the value pointed by obj with the result of subtraction of arg from the old value of obj

/// @brief Reference countin pointer
/// @tparam __type__ the pointer holding type
template< typename __type__>
class crAutoPointer : public crPointer< __type__ >
{
public:
    typedef __type__*       pointer;
    typedef const __type__* const_pointer; 
    typedef __type__&       reference;
    typedef const __type__& const_reference;

    crAutoPointer( void ) : crPointer< __type__ >()
    {
    }

    crAutoPointer( const crAutoPointer< __type__> &in_ref )
    {
        // if the pointer are in use, release the old reference
        if ( this->m_data != nullptr )
        {
            // decrease reference
		    if ( DecRefCount( this->m_data ) < 1 )
			    Delete( *this );
        }

        // copy pointer endress
	    this->m_data = in_ref.m_data;

        // incrase pointer reference
	    if( this->m_data != nullptr )
		    IncRefCount( this->m_data );
    }

    ~crAutoPointer( void )
    {
        // if is the last pointer reference delete the object
	    if( this->m_data != nullptr )
	    {
	    	if ( DecRefCount( this->m_data ) < 1 )
	    		Delete( *this );
	    }
    }

    crAutoPointer& operator = ( const crAutoPointer< __type__ > &in_ref )
    {
        // if the pointer are in use, release the old reference
        if ( this->m_data != nullptr )
        {
            // decrease reference
		    if ( DecRefCount( this->m_data ) < 1 )
			    Delete( *this );
        }

        // copy pointer endress
	    this->m_data = in_ref.m_data;

        // incrase pointer reference
	    if( this->m_data != nullptr )
		    IncRefCount( this->m_data );

        return *this;
    }

    /// @brief Allocate a new data array  
    /// @param in_count number of the elements
    /// @return 
    static crAutoPointer<__type__ > Malloc( const uint32_t in_count )
    {
        // new reference pointer holder 
        crAutoPointer<__type__ > newptr = crAutoPointer<__type__ >();
        
        // Allocate the atomic counter at pointer begining 
	    void* ptr = MemAllocAligned( ( sizeof(__type__) * in_count ) + sizeof( uint32_t ), 8 );
        assert(ptr != nullptr); //

        // set counter to zero
        *static_cast<uint32_t*>( ptr ) = 0;
        
	    // hide atomic counter
	    newptr.m_data = reinterpret_cast<pointer>( reinterpret_cast<uintptr_t>( ptr ) + sizeof(uint32_t) );
    
        return newptr;
    }

    template<typename... Args>
    static crAutoPointer<__type__> New( Args&&... args )
    {
        // new reference pointer holder 
        crAutoPointer<__type__> newptr = crAutoPointer<__type__>();
        
        // Allocate the atomic counter at pointer begining 
	    void* ptr = MemAllocAligned( sizeof(__type__) + sizeof( uint32_t ), 8 );
        assert( ptr != nullptr ); //

        // set counter to zero
        *static_cast<uint32_t*>( ptr ) = 0;
        
	    // hide atomic counter
	    newptr.m_data = reinterpret_cast<pointer>( reinterpret_cast<uintptr_t>( ptr ) + sizeof(uint32_t) );
    
        // construct the class 
        new( newptr.m_data ) __type__( std::forward<Args>(args)... ); 

        return newptr;
    }

    /// @brief Delete object pointer
    /// @param ref reference to the pointer
    /// @return 
    inline static void                   Delete(  crAutoPointer<__type__> &in_ref )
    {
        if ( in_ref.m_data != nullptr )
        {
            /// cast the destructor
            reinterpret_cast<pointer>( in_ref.m_data )->~__type__();

            // get whole pointer
            void* ptr = reinterpret_cast<void*>( reinterpret_cast<uintptr_t>( in_ref.m_data ) + sizeof( uint32_t ) );

            // release memory 
            MemFreeAligned( ptr );
            in_ref.m_data = nullptr;
        }   
    }

private:
    uint32_t IncRefCount( pointer in_ptr )
    {
	    assert( in_ptr != nullptr );
	    uint32_t* val = reinterpret_cast<uint32_t*>( reinterpret_cast<uintptr_t>( in_ptr ) - sizeof( uint32_t ) );
#ifdef USE_GCC_ATTOMIC
		return atomic_fetch_add( val, 1 ) + 1;
#else
		return __sync_fetch_and_add( val , 1 ) + 1;
#endif
    }

	uint32_t DecRefCount( pointer in_ptr )
    {
        assert( in_ptr != nullptr );
        uint32_t* val = reinterpret_cast<uint32_t*>( reinterpret_cast<uintptr_t>( in_ptr ) - sizeof( uint32_t ) );
#ifdef USE_GCC_ATTOMIC
        return atomic_fetch_sub( val, 1 ) - 1;
#else
		return __sync_sub_and_fetch( val, 1 ) - 1;
#endif
    }
};

#endif //!__POINTERS_HPP__