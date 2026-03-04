#version 460 core

/// Vertex attribute 
layout( location = 0 ) in vec3 attrPos; // vertex position
layout( location = 1 ) in vec2 attrTco; // texture coordinate

layout( std420, binding = 1 ) uniform UniformBufferObject
{
    vec4 viewPos;
    mat4 model;
    mat4 MVP;
};

void main( void )
{
    gl_Position = MVP * vec4( attrPos, 1 );
}