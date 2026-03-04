#version 460 core

#extension GL_ARB_shading_language_include : require

//
#include "common.glsl"

/// Vertex attribute 
layout( location = 0 ) in vec3 attrPos;    // vertex position 

layout( std140, binding = 1 ) uniform UniformBufferObject
{
    mat4 model;
    mat4 MVP;
};

void main( void )
{

    gl_Position = MVP * vec4( attrPos, 1 );
}