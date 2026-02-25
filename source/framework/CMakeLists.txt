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

## Framework  
##################################################################

set( BASE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Framework.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Framework.hpp
	)

set( CONSOLE_SOURCE 
	${CMAKE_CURRENT_SOURCE_DIR}/console/Console.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/console/Console.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/console/ConsoleVariables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/console/ConsoleVariables.hpp
	)

set( FRAMEWORK_SOURCES
	${BASE_SOURCES}
	${CONSOLE_SOURCE}
	)

add_library( framework STATIC ${FRAMEWORK_SOURCES} )

#set the renderer specific definitions
target_compile_definitions( framework PRIVATE __FRAMEWORK_LIB__ ) 

#set the renderer root dir as include
target_include_directories( framework PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled common
target_precompile_headers( framework 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/system/Platform.hpp
        ${CMAKE_SOURCE_DIR}/system/Includes.hpp
		${CMAKE_SOURCE_DIR}/common/Lib.hpp
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/Framework.hpp
	) 