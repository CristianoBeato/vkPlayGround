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
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_filesystem.h>

static crString savePath = crString();
static crString basepath = crString();

static crString GetLocalDir( const crString &global, const crString &full_path ) 
{
    crString output = crString();
    // Verifica se o full_path começa com o global_path
    if ( std::strncmp( global, full_path, global.Lengenth() ) == 0) 
    {
        // Aponta para o início da parte local
        const char* local_part = full_path.c_str() + global.Lengenth();
        
        //Copia para o output
        output = local_part;

        // Remove o nome do arquivo para deixar apenas o diretório
        // output.StripFileName();
    }

    return output;
}


/*
===============================================================================================
crFile 
===============================================================================================
*/
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

/*
===============================================================================================
crFileLocal
===============================================================================================
*/
class crFileLocal : public crFile
{
public:
    crFileLocal( const crString in_path, const uint32_t in_mode  );
    ~crFileLocal( void );

    bool                IsOpen( void ) const { return m_fstream != nullptr; }
    virtual uintptr_t   Read( void*  in_data, const size_t in_size, const uint32_t in_count );
    virtual uintptr_t   Write( const void* in_data, const size_t in_size, const uint32_t in_count );
    virtual uintptr_t   Seek( const uintptr_t in_offset, const whence_e in_whence );
    virtual uintptr_t   Tell( void ) const;
    virtual size_t      Size( void ) const;
    virtual void        Flush( void ) const;

private:
    SDL_IOStream*   m_fstream;
};

crFileLocal::crFileLocal( const crString in_path, const uint32_t in_mode ) : m_fstream( nullptr )
{
    const char* mode = nullptr;
    if ( ( in_mode & crFileSystem::FS_OPEN_READ ) && ( in_mode & crFileSystem::FS_OPEN_WRITE ) )
    {
        if( in_mode & crFileSystem::FS_OPEN_BINARY )
            mode = "ab+";
        else
            mode = "a+";
    }
    else if ( in_mode & crFileSystem::FS_OPEN_READ )
    {
        if( in_mode & crFileSystem::FS_OPEN_BINARY )
            mode = "rb";
        else
            mode = "r";
    }
    else if ( in_mode & crFileSystem::FS_OPEN_WRITE )
    {
        if( in_mode & crFileSystem::FS_OPEN_BINARY )
            mode = "wb";
        else
            mode = "w";
    }
    
    m_fstream = SDL_IOFromFile( in_path.c_str(), mode );
    if ( !m_fstream )
        crConsole::Warning( "Error: %s\n", SDL_GetError() );
}

crFileLocal::~crFileLocal( void )
{
    if ( m_fstream != nullptr )
    {
        SDL_CloseIO( m_fstream );
        m_fstream = nullptr;
    }
}

uintptr_t crFileLocal::Read( void *in_data, const size_t in_size, const uint32_t in_count )
{
    size_t readed = 0;
    auto outStream = static_cast<byte*>( in_data );
    for ( uint32_t i = 0; i < in_count; i++)
    {
        auto read = SDL_ReadIO( m_fstream, outStream, in_size );
        outStream += read;
        readed += read;
    }
    
    auto offset = SDL_TellIO( m_fstream );
    return ( offset < 0 ) ? 0 : offset;
}

uintptr_t crFileLocal::Write(const void *in_data, const size_t in_size, const uint32_t in_count)
{
    size_t writed = 0;
    auto inStream = static_cast<const byte*>( in_data );

    for ( uint32_t i = 0; i < in_count; i++)
    {
        auto write = SDL_WriteIO( m_fstream, inStream, in_size );
        inStream += write;
        writed += write;
    }
    

    auto offset = SDL_TellIO( m_fstream );
    return ( offset < 0 ) ? 0 : offset;
}

/// the value of crFile::whence_e match 1:1 of SDL_IOWhence
/// but we think is more secure use a static table
static const SDL_IOWhence k_SEEK_WHENCE_TABLE[3] =
{
    SDL_IO_SEEK_SET,
    SDL_IO_SEEK_CUR,
    SDL_IO_SEEK_END
};

uintptr_t crFileLocal::Seek( const uintptr_t in_offset, const whence_e in_whence )
{
    auto offset = SDL_SeekIO( m_fstream, in_offset, k_SEEK_WHENCE_TABLE[in_whence] );
    return ( offset < 0 ) ? 0 : offset;
}

uintptr_t crFileLocal::Tell(void) const
{
    auto offset = SDL_TellIO( m_fstream );
    return ( offset < 0 ) ? 0 : offset;
}

size_t crFileLocal::Size(void) const
{
    return SDL_GetIOSize( m_fstream );
}

void crFileLocal::Flush(void) const
{
    SDL_FlushIO( m_fstream );
}

/*
===============================================================================================
crFileSystem 
===============================================================================================
*/
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

void crFileSystem::StartUp(void)
{
    crConsole::Print( "|---Initializing FileSystem---|\n");
    /// configure paths
    auto base = BasePath();
    crConsole::Print( "Base Path: \"%s\"\n", base.c_str() );
    auto save = SavePath();
    crConsole::Print( "Save Path: \"%s\"\n", save.c_str() );

    BuildDirectoryTree();
}

void crFileSystem::ShutDown(void)
{
}

crFile *crFileSystem::Open( const crString in_path, const uint32_t in_flags )
{
    crString location;
    crFile* newFile = nullptr;

    /// Open file from save or base dir
    if ( in_flags & FS_OPEN_SAVE_PATH )
        location = SavePath() + in_path;
    else
        location = BasePath() + in_path;
    
    newFile = new crFileLocal( location, in_flags );
    if ( !dynamic_cast<crFileLocal*>( newFile )->IsOpen() )
    {
        delete newFile;
        newFile = nullptr;
    }
    
    return newFile;
}

void crFileSystem::Close(crFile *in_file)
{
    delete in_file;
}

bool crFileSystem::PathExist(const crString in_path, const bool in_savepath) const
{
    SDL_PathInfo info{};
    crString fullPath;
    if ( in_savepath )
        fullPath = SavePath() + in_path;
    else
        fullPath = BasePath() + in_path;
    
    return SDL_GetPathInfo( fullPath.c_str(), &info );
}

void crFileSystem::CreatePath( const crString in_path, const bool in_savepath )
{
    crString newpath;
    if ( in_savepath )
        newpath = SavePath() + in_path;
    else
        newpath = BasePath() + in_path;

    if( !SDL_CreateDirectory( newpath.c_str() ) )
        throw crException( SDL_GetError() );
}

crString crFileSystem::BasePath(void) const
{
    if( basepath.Empty() )
    {
        auto path = SDL_GetBasePath();
        basepath = crString( path ); 
#if __PLATFORM_WINDOWS__    /// we ignore windows \\ path separator
        basepath.Replace( '\\', '/' );
#endif
    }

    return basepath;
}

crString crFileSystem::SavePath(void) const
{
    if ( savePath.Empty() )
    {
        auto path = SDL_GetPrefPath( "vkPlayground", "GameTest" );
        savePath = crString( path );
#if __PLATFORM_WINDOWS__    /// we ignore windows \\ path separator
        savePath.Replace( '\\', '/' );
#endif
    }
    
    return savePath;
}

void crFileSystem::BuildDirectoryTree(void)
{
    /// list base path directories
    auto base = BasePath();
    BuildPath( base );
    
    auto save = SavePath();
    BuildPath( save );
}

void crFileSystem::BuildPath( const crString in_path )
{
    int count = 0;
    uint32_t i = 0;
    crList<crString> subdirs;

    /// list paths
    char** path_list = SDL_GlobDirectory( in_path.c_str(), nullptr, SDL_GLOB_CASEINSENSITIVE, &count );
    for (  i = 0; i < count; i++)
    {
        SDL_PathInfo info{};
        crString local = crString();
        crString fullpath = crString();

        crString path = crString( path_list[i] );
        path.Replace( '\\', '/');

        // SDL_GlobDirectory lists paths only in a relative way, 
        // so we have to reference the path based on the parent path.
        fullpath = in_path + path;
        local = GetLocalDir( BasePath(), fullpath );

        /// TODO: Fix save path path list
        if ( local.Empty() )
                continue;

        /// retrieve subdirs properties
        if( !SDL_GetPathInfo( fullpath, &info ) )
        {
            crConsole::Error( "%s %s\n", fullpath.c_str(), SDL_GetError() );
            continue;
        }

        if ( info.type == SDL_PATHTYPE_FILE )
        {
            fileInfo_t finfo{};
            finfo.size = info.size;
            finfo.modify = info.modify_time;
            finfo.fullpath = fullpath;
            finfo.localPath = local;
            m_files.Append( finfo );
        }
        else if( info.type == SDL_PATHTYPE_DIRECTORY )
        {
            subdirs.Append( local );
        }
    }
    
    for ( i = 0; i < subdirs.Count(); i++)
    {
        BuildPath( subdirs[i] );
    }
}