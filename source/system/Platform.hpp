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

#ifndef __PLATFORM_HPP__
#define __PLATFORM_HPP__

/*
#define _ARCH_x86_32_ 0
#define _ARCH_ARM_32_ 0
#define _ARCH_PPC_32_ 0
#define _ARCH_x86_64_ 0
#define _ARCH_ARM_64_ 0
#define _ARCH_PPC_64_ 0
*/
//Get current architecture, detectx nearly every architecture. Coded by Freak
// https://stackoverflow.com/questions/152016/detecting-cpu-architecture-compile-time
#if defined(__x86_64__) || defined(_M_X64)
#define _ARCH_x86_64_ 1
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define _ARCH_x86_32_ 1
#elif defined(__arm__) || defined(_M_ARM)
#elif defined(__aarch64__) || defined(_M_ARM64)
#define _ARCH_ARM_64_ 1
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
#define _ARCH_PPC_32_ 1
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define _ARCH_PPC_64_ 1
#endif

/*
#define __PLATFORM_WINDOWS__ 0
#define __PLATFORM_LINUX__ 0 
#define __PLATFORM_FBSD__ 0 
#define __PLATFORM_ANDROID__ 0
*/

// SO
#if defined(WIN32) || defined(_WIN32) // Windows
#   define __PLATFORM_WINDOWS__ 1
#elif defined( __linux__ )
#   define __PLATFORM_LINUX__ 1
#elif defined( __FreeBSD__ )
#   define __PLATFORM_FBSD__ 1
#elif defined( __ANDROID__ )
#   define __PLATFORM_ANDROID__ 1
#endif

/*
#define __COMPILER_MSVC__ 0
#define __COMPILER_GCC__ 0
#define __COMPILER_CLANG__ 0
#define __COMPILER_INTEL__ 0
*/
// COMPILER PLATFORM
#if defined( _MSC_VER ) || defined( _MSVC_LANG )
#define __COMPILER_MSVC__ 1
#elif defined( __clang__ )
#define __COMPILER_CLANG__ 1
#elif defined(__GNUC__) || defined(__GNUG__)
#define __COMPILER_GCC__ 1
#elif defined( __INTEL_COMPILER )
#define __COMPILER_INTEL__ 1
#endif

#if _ARCH_x86_32_ || _ARCH_ARM_32_ || _ARCH_PPC_32_
inline constexpr size_t pointer_size = sizeof( void* );
static_assert( pointer_size == 4, "Invalid pointer size" );
#elif _ARCH_x86_64_ || _ARCH_ARM_64_ || _ARCH_PPC_64_
inline constexpr size_t pointer_size = sizeof( void* );
static_assert( pointer_size == 8, "Invalid pointer size" );
#endif

typedef unsigned char byte;
typedef SDL_Time    time_t; // time stamp type

#endif //!__PLATFORM_HPP__