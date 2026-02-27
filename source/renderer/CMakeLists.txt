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

## Renderer system 
##################################################################

set( BASE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Bounds.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Bounds.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mesh.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer_common.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Surface.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Surface.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderSystemLocal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderSystemLocal.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Vertexes.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Vertexes.hpp
	)

set( BACKEND_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Backend.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Backend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/PipelineManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/PipelineManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/VertexCache.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/VertexCache.hpp
	)

set( BACKEND_VULKAN_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Buffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Buffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Commandbuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Commandbuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Context.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Context.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Core.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Core.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Device.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Device.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Framebuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Framebuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Image.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Image.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Pipeline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Pipeline.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Program.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Program.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Swapchain.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Swapchain.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Utils.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/backend/Vulkan/Utils.cpp
	)

set( MATERIAL_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/material/Material.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/material/Material.hpp
	)

set( MODEL_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/models/Model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/models/Model.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/models/ModelManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/models/ModelManager.hpp
	)

set( TEXTURE_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/textures/Texture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/textures/Texture.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/textures/TextureManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/textures/TextureManager.hpp
	)

set( RENDERWORLD_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/world/Light.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/world/Light.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/world/Geometry.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/world/Geometry.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/world/View.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/world/View.hpp
	)

set( RENDERER_SOURCES
	${BASE_SOURCES}
	${BACKEND_SOURCES}
	${BACKEND_VULKAN_SOURCES}
	${MODEL_SOURCES}
	${TEXTURE_SOURCES}
	${MATERIAL_SOURCES}
	${RENDERWORLD_SOURCES}
	)

add_library( renderer STATIC ${RENDERER_SOURCES} )

#set the renderer specific definitions
target_compile_definitions( renderer PRIVATE __RENDERER_LIB__ ) 

#set the renderer root dir as include
target_include_directories( renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ) 

# set the precompiled common
target_precompile_headers( renderer 
	PUBLIC
		${CMAKE_SOURCE_DIR}/system/Platform.hpp
        ${CMAKE_SOURCE_DIR}/system/Includes.hpp
		${CMAKE_SOURCE_DIR}/common/Lib.hpp
		${CMAKE_SOURCE_DIR}/system/Systems.hpp
		${CMAKE_SOURCE_DIR}/framework/Framework.hpp
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/renderer_common.hpp
	) 