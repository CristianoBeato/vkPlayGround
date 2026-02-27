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

#include "FileSystem.hpp"

crFile::crFile( void )
{
}

crFile::~crFile( void )
{
}

uintptr_t crFile::ReadInt8( int8_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( int8_t ), in_count );
}

uintptr_t crFile::ReadInt16( int16_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( int16_t ), in_count );
}

uintptr_t crFile::ReadInt32( int32_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( int32_t ), in_count );
}

uintptr_t crFile::ReadInt64( int64_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( int64_t ), in_count );
}

uintptr_t crFile::ReadUint8( uint8_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( uint8_t ), in_count );
}

uintptr_t crFile::ReadUint16( uint16_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( uint16_t ), in_count );
}

uintptr_t crFile::ReadUint32( uint32_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( uint32_t ), in_count );
}

uintptr_t crFile::ReadUint64( uint64_t *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( uint64_t ), in_count );
}

uintptr_t crFile::ReadFloat( float *out_values, const uint32_t in_count )
{
    return Read( out_values, sizeof( uint32_t ), in_count );
}

uintptr_t crFile::WriteInt8( const int8_t *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( int8_t ), in_count );
}

uintptr_t crFile::WriteInt16(const int16_t *in_values, const uint32_t in_count)
{
    return Write( in_values, sizeof( int16_t ), in_count );
}

uintptr_t crFile::WriteInt32( const int32_t *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( int32_t ), in_count );
}

uintptr_t crFile::WriteInt64( const int64_t *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( int64_t ), in_count );
}

uintptr_t crFile::writeUint8( const uint8_t *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( uint8_t ), in_count );
}

uintptr_t crFile::writeUint16( const uint16_t *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( uint16_t ), in_count );
}

uintptr_t crFile::writeUint32( const uint32_t *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( uint32_t ), in_count );
}

uintptr_t crFile::writeUint64( const uint64_t *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( uint64_t ), in_count );
}

uintptr_t crFile::WriteFloat( const float *in_values, const uint32_t in_count )
{
    return Write( in_values, sizeof( float ), in_count );
}

crFileSystem *crFileSystem::Get(void)
{
    static crFileSystem gFileSystem = crFileSystem();
    return &gFileSystem;
}

crFileSystem::crFileSystem(void)
{
}

crFileSystem::~crFileSystem( void )
{
}