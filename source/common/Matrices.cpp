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

#include "Matrices.hpp"

alignas( 16 ) constexpr float k_IDENTITY3x3[9] 
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

alignas( 16 ) constexpr float k_IDENTITY4x4[16] 
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

crMatrix3::crMatrix3( void )
{
    Identity();
}

crMatrix3::crMatrix3(const crQuaternion &in_quaternion)
{
    // Pre-calculates repeated values ​​for optimization.
    float x2 = in_quaternion.x + in_quaternion.x;  
    float y2 = in_quaternion.y + in_quaternion.y;
    float z2 = in_quaternion.z + in_quaternion.z;
    float xx = in_quaternion.x * x2; 
    float xy = in_quaternion.x * y2; 
    float xz = in_quaternion.x * z2;
    float yy = in_quaternion.y * y2; 
    float yz = in_quaternion.y * z2; 
    float zz = in_quaternion.z * z2;
    float wx = in_quaternion.w * x2; 
    float wy = in_quaternion.w * y2; 
    float wz = in_quaternion.w * z2;

    // Column 0
    m_mat[0] = 1.0f - (yy + zz);
    m_mat[1] = xy + wz;
    m_mat[2] = xz - wy;

    // Column 1
    m_mat[3] = xy - wz;
    m_mat[4] = 1.0f - (xx + zz);
    m_mat[5] = yz + wx;

    // Column 2
    m_mat[6] = xz + wy;
    m_mat[7] = yz - wx;
    m_mat[8] = 1.0f - (xx + yy);
}

crMatrix3::crMatrix3(const crVec3f &in_position)
{
    // Coluna 0 (Eixo X)
    m_mat[0] = 1.0f;
    m_mat[1] = 0.0f;
    m_mat[2] = 0.0f;

    // Coluna 1 (Eixo Y)
    m_mat[3] = 0.0f;
    m_mat[4] = 1.0f;
    m_mat[5] = 0.0f;

    // Coluna 2 (Eixo Z)
    m_mat[6] = in_position.x;
    m_mat[7] = in_position.y;
    m_mat[8] = in_position.z;
}

crMatrix3::~crMatrix3( void )
{
    Zero();
}

void crMatrix3::Identity(void)
{
    std::memcpy( m_mat, k_IDENTITY3x3, sizeof( float ) * 9 );
}

void crMatrix3::Zero( void )
{
    std::memset( m_mat, 0x00, sizeof( float ) * 9 );
}

crMatrix3 crMatrix3::operator*(const crMatrix3 &in_ref) const
{
    crMatrix3 res;
    const float* a = Ptr();         // Matriz da esquerda
    const float* b = in_ref.Ptr();  // Matriz da direita
    float* r = res.Ptr();

  // Coluna 0
    r[0] = a[0] * b[0] + a[3] * b[1] + a[6] * b[2];
    r[1] = a[1] * b[0] + a[4] * b[1] + a[7] * b[2];
    r[2] = a[2] * b[0] + a[5] * b[1] + a[8] * b[2];

    // Coluna 1
    r[3] = a[0] * b[3] + a[3] * b[4] + a[6] * b[5];
    r[4] = a[1] * b[3] + a[4] * b[4] + a[7] * b[5];
    r[5] = a[2] * b[3] + a[5] * b[4] + a[8] * b[5];

    // Coluna 2
    r[6] = a[0] * b[6] + a[3] * b[7] + a[6] * b[8];
    r[7] = a[1] * b[6] + a[4] * b[7] + a[7] * b[8];
    r[8] = a[2] * b[6] + a[5] * b[7] + a[8] * b[8];

    return res;
}

crMatrix3 crMatrix3::operator*=(const crMatrix3 &in_ref)
{
    crMatrix3 a = *this;
    crMatrix3 b = in_ref;
    *this = a * b;
    return *this;
}

crMatrix4::crMatrix4( void )
{
    Identity();
}

crMatrix4::crMatrix4(const crQuaternion &in_quaternion)
{
    // Pre-calculates products for performance and clarity.
    float x2 = in_quaternion.x + in_quaternion.x;  
    float y2 = in_quaternion.y + in_quaternion.y;  
    float z2 = in_quaternion.z + in_quaternion.z;
    float xx = in_quaternion.x * x2; 
    float xy = in_quaternion.x * y2; 
    float xz = in_quaternion.x * z2;
    float yy = in_quaternion.y * y2; 
    float yz = in_quaternion.y * z2; 
    float zz = in_quaternion.z * z2;
    float wx = in_quaternion.w * x2; 
    float wy = in_quaternion.w * y2; 
    float wz = in_quaternion.w * z2;

    // --- Column 0 ---
    m_mat[0] = 1.0f - (yy + zz);
    m_mat[1] = xy + wz;
    m_mat[2] = xz - wy;
    m_mat[3] = 0.0f; // No translation along the W axis.

    // --- Column 1 ---
    m_mat[4] = xy - wz;
    m_mat[5] = 1.0f - (xx + zz);
    m_mat[6] = yz + wx;
    m_mat[7] = 0.0f;

    // --- Column 2 ---
    m_mat[8] = xz + wy;
    m_mat[9] = yz - wx;
    m_mat[10] = 1.0f - (xx + yy);
    m_mat[11] = 0.0f;

    // --- Column 3 (Translation and Scale W) ---
    m_mat[12] = 0.0f; // Translation X
    m_mat[13] = 0.0f; // Translation Y
    m_mat[14] = 0.0f; // Translation Z
    m_mat[15] = 1.0f; // Homogeneous W component
}

crMatrix4::crMatrix4(const crVec3f &in_position)
{
    // Coluna 0 (Eixo X)
    m_mat[0] = 1.0f;
    m_mat[1] = 0.0f;
    m_mat[2] = 0.0f;
    m_mat[3] = 0.0f;

    // Coluna 1 (Eixo Y)
    m_mat[4] = 0.0f;
    m_mat[5] = 1.0f;
    m_mat[6] = 0.0f;
    m_mat[7] = 0.0f;

    // Coluna 2 (Eixo Z)
    m_mat[8] = 0.0f;
    m_mat[9] = 0.0f;
    m_mat[10] = 1.0f;
    m_mat[11] = 0.0f;

    // Coluna 3 (Posição / Translação) -> Onde o crVec3 entra
    m_mat[12] = in_position.x;
    m_mat[13] = in_position.y;
    m_mat[14] = in_position.z;
    m_mat[15] = 1.0f;
}

crMatrix4::~crMatrix4( void )
{
    Zero();
}

void crMatrix4::Identity( void )
{
    std::memcpy( m_mat, k_IDENTITY4x4, sizeof( float ) * 9 );
}

void crMatrix4::Zero(void)
{
    std::memset( m_mat, 0x00, sizeof( float ) * 16 );
}

crMatrix4 crMatrix4::operator*(const crMatrix4 &in_ref) const
{
    crMatrix4 res;
    const float* a = Ptr();         // Matriz da esquerda
    const float* b = in_ref.Ptr();  // Matriz da direita
    float* r = res.Ptr();

     // Coluna 0
    r[0]  = a[0]*b[0] + a[4]*b[1] + a[8]*b[2]  + a[12]*b[3];
    r[1]  = a[1]*b[0] + a[5]*b[1] + a[9]*b[2]  + a[13]*b[3];
    r[2]  = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];
    r[3]  = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];

    // Coluna 1
    r[4]  = a[0]*b[4] + a[4]*b[5] + a[8]*b[6]  + a[12]*b[7];
    r[5]  = a[1]*b[4] + a[5]*b[5] + a[9]*b[6]  + a[13]*b[7];
    r[6]  = a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];
    r[7]  = a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];

    // Coluna 2
    r[8]  = a[0]*b[8] + a[4]*b[9] + a[8]*b[10] + a[12]*b[11];
    r[9]  = a[1]*b[8] + a[5]*b[9] + a[9]*b[10] + a[13]*b[11];
    r[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];
    r[11] = a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];

    // Coluna 3
    r[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14]  + a[12]*b[15];
    r[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14]  + a[13]*b[15];
    r[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
    r[15] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];

    return res;
}
