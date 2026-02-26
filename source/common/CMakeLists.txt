##
##===========================================================================================
##    This file is part of vkPlayGround a Vulkan testing c++ engine.
##
##    Copyright (c) 2026 Cristiano B. Santos <cristianobeato_dm@hotmail.com>
##    Contributor(s): none yet.
##
##-------------------------------------------------------------------------------------------
##
## This file is part of the vkPlayGround library and is licensed under the
## MIT License with Attribution Requirement.
##
## You are free to use, modify, and distribute this file (even commercially),
## as long as you give credit to the original author:
##
##     “Based on vkPlayGround by Cristiano Beato – https://github.com/CristianoBeato”
##
## For full license terms, see the LICENSE file in the root of this repository.
##===============================================================================================
##

## library configurarion 
##################################################################

set( COMMON_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/Array.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Exception.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/HalfFloat.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Heap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Heap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Pointers.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Quaternion.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Quaternion.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Matrices.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Matrices.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/List.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/String.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/String.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Vectors.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Vectors.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Lib.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Lib.hpp
	)

add_library( common STATIC ${COMMON_SOURCES} )

#set the common specific definitions
target_compile_definitions( common PRIVATE __COMMON_LIB__ ) 

#set the common root dir as include
target_include_directories( common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled common
target_precompile_headers( common 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/system/Platform.hpp
        ${CMAKE_SOURCE_DIR}/system/Includes.hpp
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/Lib.hpp
	) 
