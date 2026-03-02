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
cmake_minimum_required(VERSION 3.13)

##
## Project Name
##
project( vkPlayGround )

option( COMPILE_SHADERS FALSE )

##
## Uses STD c++ 20
##
set( CMAKE_CXX_STANDARD 20 )

##
## External Libraries
##
find_package( SDL3 REQUIRED CONFIG )
include_directories( SDL3::Headers )

##
## set the engine source root path
##
include_directories( ${CMAKE_SOURCE_DIR} )

##
## common librarires
##
add_subdirectory( common )

##
## engine framework systems
##
add_subdirectory( framework )

##
## engine render systems
##
add_subdirectory( renderer )

##
## engine sound system
## 
add_subdirectory( sound )

##
## engine system entry
##
add_subdirectory( system )

##
## game logic system
##
add_subdirectory( game )

if( COMPILE_SHADERS )
    find_package(Vulkan REQUIRED COMPONENTS glslc)

    set( SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/glsl )
    set( SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/assets/shaders/spirv )
    file( MAKE_DIRECTORY ${SHADER_BINARY_DIR} )

    #list shaders to compile
    set( SHADER_FILES  
        ${SHADER_SOURCE_DIR}/color.vert
        ${SHADER_SOURCE_DIR}/color.frag )

    set( SPV_OUTPUTS "" )

    set( VERTES_SHADER_DEFINITION -DVERTEX_SHADER )
    set( VERTES_SHADER_DEFINITION -DFRAGMENT_SHADER )

    foreach( SHADER_PATH ${SHADER_FILES} )
        get_filename_component( SHADER_NAME ${SHADER_PATH} NAME )
        get_filename_component( FILE_EXT ${SHADER_FILE} EXT )
        set( OUTPUT_FILE ${SHADER_BIN_DIR}/${FILE_NAME}.spv)
        add_custom_command( OUTPUT ${OUTPUT_FILE} COMMAND Vulkan::glslc ${SHADER_FILE} -o ${OUTPUT_FILE} DEPENDS ${SHADER_FILE} COMMENT "Vulkan Shader: Compiling ${FILE_NAME} to SPIR-V" )
    endforeach()

    add_custom_target(CompileShaders DEPENDS ${SPV_OUTPUTS})

endif( COMPILE_SHADERS )
