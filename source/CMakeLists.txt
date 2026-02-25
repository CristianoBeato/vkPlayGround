cmake_minimum_required(VERSION 3.13)

##
## Project Name
##
project( vkPlayGround )

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
## engine render sytems
##
add_subdirectory( renderer )

##
## engine system entry
##
add_subdirectory( system )

##
## game logic system
##
add_subdirectory( game )
