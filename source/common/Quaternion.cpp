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

#include "Quaternion.hpp"

crQuaternion::crQuaternion( void )
{
    Identity();
}

crQuaternion::crQuaternion(const float in_x, const float in_y, const float in_z, const float in_w ) :
    x( in_x ),
    y( in_y ),
    z( in_z ),
    w( in_w )
{
}

crQuaternion::~crQuaternion( void )
{
    Identity();
}

void crQuaternion::Identity( void )
{
    x = y = z = 0;
    w = 1.0f;
}

void crQuaternion::Normalize( void )
{
    float mag = x * x + y * y + z * z + w * w;
    if ( mag > 0.0f )
    {
        mag = std::sqrt( mag );
        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;
    } 
    else    /// not valid quarternion
        Identity();
}

float crQuaternion::Dot( void ) const
{    
    return x * x + y * y + z * z + w * w;
}

float crQuaternion::DotProduct(const crQuaternion &q) const
{
    return x * q.x + y * q.y + z * q.z + w * q.w;
}

crQuaternion crQuaternion::Inverse(void) const
{
    float norm = Dot();
    return Conjugate() * ( 1.0f / norm );
}

crQuaternion crQuaternion::operator*(const crQuaternion &q) const
{
    crQuaternion n;
    n.x = w * q.x + x * q.w + y * q.z - z * q.y; // New X
    n.y = w * q.y - x * q.z + y * q.w + z * q.x; // New Y
    n.z = w * q.z + x * q.y - y * q.x + z * q.w; // New Z
    n.w = w * q.w - x * q.x - y * q.y - z * q.z;  // New W
    return n;
}
