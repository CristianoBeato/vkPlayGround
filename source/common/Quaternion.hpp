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

#ifndef __QUARTERNIO_HPP__
#define __QUARTERNIO_HPP__

class alignas( 16 ) crQuaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    crQuaternion( void );
    crQuaternion( const float in_x, const float in_y, const float in_z, const float in_w );
    ~crQuaternion( void );

    void            Identity( void );
    void            Normalize( void );
    float           Dot( void ) const;
    float           DotProduct( const crQuaternion &q ) const;
    crQuaternion    Inverse( void ) const;

    inline crQuaternion Conjugate( void ) const 
    { 
        return {-x, -y, -z, w }; 
    }

    inline crQuaternion operator+( const crQuaternion& q ) const 
    {
        return { x + q.x, y + q.y, z + q.z, w + q.w };
    }

    inline crQuaternion operator-( const crQuaternion& q ) const 
    {
        return { x - q.x, y - q.y, z - q.z, w - q.w };
    }

    inline crQuaternion operator*(float s) const 
    {
        return { x * s, y * s, z * s, w * s };
    }

    crQuaternion operator*(const crQuaternion& q) const;
};

#endif //!__QUARTERNIO_HPP__