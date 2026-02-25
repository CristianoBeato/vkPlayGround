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

## Game logic 
##################################################################

set( GAME_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Damange.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Damange.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Protection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Protection.hpp
    )

add_library( game SHARED ${GAME_SOURCES} )

#set the sound specific definitions
target_compile_definitions( game PRIVATE __GAMED_DLL__ ) 

#set the sound root dir as include
target_include_directories( game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled common
target_precompile_headers( game 
	PUBLIC
		${CMAKE_SOURCE_DIR}/system/Platform.hpp
        ${CMAKE_SOURCE_DIR}/system/Includes.hpp
		${CMAKE_SOURCE_DIR}/common/Lib.hpp
		${CMAKE_SOURCE_DIR}/system/Systems.hpp
		${CMAKE_SOURCE_DIR}/framework/Framework.hpp
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/game_common.hpp
	) 