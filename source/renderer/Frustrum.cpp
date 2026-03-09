/*
===========================================================================================
    This file is part of vkPlayGround a Vulkan testing c++ engine.

    Copyright (c) 2026 Cristiano B. Santos <cristianobeato_dm@hotmail.com>
    Contributor(s): none yet.

-------------------------------------------------------------------------------------------

 This file is part of the vkPlayGround library and is licensed under the
 MIT License with Attribution Requirement.

 You are free to use, modify, and distribute this file (even commercially),
 as long as you give credit to the original author:

     “Based on vkPlayGround by Cristiano Beato – https://github.com/CristianoBeato”

 For full license terms, see the LICENSE file in the root of this repository.
===============================================================================================
*/

#include "Frustrum.hpp"

crFrustrum::crFrustrum( void )
{
}

crFrustrum::~crFrustrum( void )
{
}

void crFrustrum::Update( const crMatrix4 &in_viewProj )
{
    const float* m = in_viewProj.FloatPtr(); // Assume que retorna float[16]
    // Atalhos para as linhas da matriz (padrão column-major)
    // m[3], m[7], m[11], m[15] é a 4ª linha (W)
    // m[0], m[4], m[8], m[12] é a 1ª linha (X)
    // m[1], m[5], m[9], m[13] é a 2ª linha (Y)
    // m[2], m[6], m[10], m[14] é a 3ª linha (Z)

    auto SetupPlane = [&]( planes_e plane, const float a, const float b, const float c, const float d ) 
    {
        float length = std::sqrtf( a * a + b * b + c * c );
        m_faces[plane].normal = crVec3f(a / length, b / length, c / length);
        m_faces[plane].distance = -d / length; // Ajuste o sinal conforme sua convenção de Dot(p)-dist
    };

    // Left: Row4 + Row1
    SetupPlane( PLANE_LEFT,   m[3] + m[0], m[7] + m[4], m[11] + m[8],  m[15] + m[12] );

    // Right: Row4 - Row1
    SetupPlane( PLANE_RIGHT,  m[3] - m[0], m[7] - m[4], m[11] - m[8],  m[15] - m[12] );

    // Bottom: Row4 + Row2
    SetupPlane( PLANE_BOTTOM, m[3] + m[1], m[7] + m[5], m[11] + m[9],  m[15] + m[13] );

    // Top: Row4 - Row2
    SetupPlane( PLANE_TOP,    m[3] - m[1], m[7] - m[5], m[11] - m[9],  m[15] - m[13] );

    // Near: Row4 + Row3 (OpenGL range -1 a 1)
    SetupPlane( PLANE_BACK,   m[3] + m[2], m[7] + m[6], m[11] + m[10], m[15] + m[14] );

    // Far: Row4 - Row3
    SetupPlane( PLANE_FRONT,  m[3] - m[2], m[7] - m[6], m[11] - m[10], m[15] - m[14] );
}
