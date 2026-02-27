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

#ifndef __FILE_SYSTEM_HPP__
#define __FILE_SYSTEM_HPP__

/// @brief File base class
class crFile
{
public:
    crFile( void );
    ~crFile( void );

    /// read signed integers 
    uintptr_t   ReadInt8( int8_t* out_values, const uint32_t in_count );
    uintptr_t   ReadInt16( int16_t* out_values, const uint32_t in_count );
    uintptr_t   ReadInt32( int32_t* out_values, const uint32_t in_count );
    uintptr_t   ReadInt64( int64_t* out_values, const uint32_t in_count );
    
    /// read unsigned integers
    uintptr_t   ReadUint8( uint8_t* out_values, const uint32_t in_count );
    uintptr_t   ReadUint16( uint16_t* out_values, const uint32_t in_count );
    uintptr_t   ReadUint32( uint32_t* out_values, const uint32_t in_count );
    uintptr_t   ReadUint64( uint64_t* out_values, const uint32_t in_count );

    /// read float
    uintptr_t   ReadFloat( float* out_values, const uint32_t in_count );

    /// Write signed integers
    uintptr_t   WriteInt8( const int8_t* in_values, const uint32_t in_count );
    uintptr_t   WriteInt16( const int16_t* in_values, const uint32_t in_count );
    uintptr_t   WriteInt32( const int32_t* in_values, const uint32_t in_count );
    uintptr_t   WriteInt64( const int64_t* in_values, const uint32_t in_count );

    /// write unsigned integers
    uintptr_t   writeUint8( const uint8_t* in_values, const uint32_t in_count );
    uintptr_t   writeUint16( const uint16_t* in_values, const uint32_t in_count );
    uintptr_t   writeUint32( const uint32_t* in_values, const uint32_t in_count );
    uintptr_t   writeUint64( const uint64_t* in_values, const uint32_t in_count );

    /// write float
    uintptr_t   WriteFloat( const float* in_values, const uint32_t in_count );

    virtual uintptr_t   Read( void*  in_data, const size_t in_size, const uint32_t in_count ) = 0;
    virtual uintptr_t   Write( const void* in_data, const size_t in_size, const uint32_t in_count ) = 0;
    virtual uintptr_t   Tell( void ) const = 0;
    virtual size_t      Size( void ) const = 0;

    // Disable class copy
    crFile( const crFile & ) = delete;
    crFile operator =( const crFile& ) = delete;
};

class crFileSystem
{
public:
    enum openFlags_t
    {
        /// @brief Open file for read operations
        FS_OPEN_READ = 1 << 0,

        /// @brief Open file for write operations
        FS_OPEN_WRITE = 1 << 1,

        /// @brief Open file from save path
        FS_OPEN_SAVE_PATH = 1 << 2,

        /// @brief Open file as a memory blob and read from
        FS_OPEN_MAPED = 1 << 2
    };

    static crFileSystem* Get( void );
    crFileSystem( void );
    ~crFileSystem( void );

    crFile*  Open( const crString in_path, const uint32_t in_flags );
    void    Close( crFile* in_file );

    crString    BasePath( void );
    crString    SavePath( void );
};

#endif //!__FILE_SYSTEM_HPP__