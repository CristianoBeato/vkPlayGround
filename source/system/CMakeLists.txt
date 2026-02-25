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

## Main system 
##################################################################

set( SYSTEM_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Includes.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Platform.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Main.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Video.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Video.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/EventsInput.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EventsInput.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Threads.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Threads.hpp
)

add_executable( vkPlayGround ${SYSTEM_SOURCES} )

#set the vkPlayGround executable specific definitions
target_compile_definitions( vkPlayGround PRIVATE __SYSTEM_EXE__ )

set( LIBRARIES 
		SDL3::SDL3 
		framework
		renderer
		sound
		common 
	)

target_link_libraries( vkPlayGround PRIVATE -Wl,--start-group ${LIBRARIES} -Wl,--end-group )

#set the vkPlayGround executable root dir as include
target_include_directories( vkPlayGround PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled common
target_precompile_headers( vkPlayGround 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Platform.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Includes.hpp
		${CMAKE_SOURCE_DIR}/common/Lib.hpp
		${CMAKE_SOURCE_DIR}/renderer/RenderSystem.hpp
		${CMAKE_SOURCE_DIR}/sound/SoundSystem.hpp
	##PRIVATE 
	##	${CMAKE_CURRENT_SOURCE_DIR}/Lib.h
	) 