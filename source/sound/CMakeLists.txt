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

## Sound system 
##################################################################

set( SOUND_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/Emiter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Emiter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Ocluder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Ocluder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/sound_common.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundSystemLocal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundSystemLocal.hpp
    )

add_library( sound STATIC ${SOUND_SOURCES} )

#set the sound specific definitions
target_compile_definitions( sound PRIVATE __SOUND_LIB__ ) 

#set the sound root dir as include
target_include_directories( sound PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled common
target_precompile_headers( sound 
	PUBLIC
		${CMAKE_SOURCE_DIR}/system/Platform.hpp
        ${CMAKE_SOURCE_DIR}/system/Includes.hpp
		${CMAKE_SOURCE_DIR}/common/Lib.hpp
		${CMAKE_SOURCE_DIR}/system/Systems.hpp
		${CMAKE_SOURCE_DIR}/framework/Framework.hpp
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/sound_common.hpp
	) 