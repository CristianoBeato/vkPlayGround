#version 460 core

/// render target output
layout( location = 0 ) out vec4 render_taget0;

/// material uniform
layout( std420, binding = 2 ) uniform UniformBufferObject
{
    vec4 color;
};

void main( void )
{
    render_taget0 = color;
}