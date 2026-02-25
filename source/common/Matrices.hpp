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

#ifndef __MATRICES_HPP__
#define __MATRICES_HPP__

class alignas( 16 ) crMatrix3
{
public:
    crMatrix3( void );
    crMatrix3( const crQuaternion& in_quaternion );
    crMatrix3( const crVec3f& in_position );
    ~crMatrix3( void );

    /// @brief set matrix to identity
    void    Identity( void );
    
    /// @brief set matrix to zero
    void    Zero( void );

    inline float*  Ptr( void ) { return &m_mat[0]; }
    inline float*  Ptr( void ) const { return const_cast<float*>( &m_mat[0] ); }

    crMatrix3 operator*(const crMatrix3& in_ref ) const;
    crMatrix3 operator*=( const crMatrix3& in_ref );

private:
    float m_mat[9];
};

class alignas( 16 ) crMatrix4
{
public:
    crMatrix4( void );
    crMatrix4( const crQuaternion& in_quaternion );
    crMatrix4( const crVec3f& in_position );

    ~crMatrix4( void );
    
    /// @brief set matrix to identity
    void    Identity( void );

    /// @brief set matrix to zero
    void    Zero( void );

    inline float*  Ptr( void ) { return &m_mat[0]; }
    inline float*  Ptr( void ) const { return const_cast<float*>( &m_mat[0] ); }

    crMatrix4   operator*( const crMatrix4 &in_ref ) const;
    crMatrix4   operator*=( const crMatrix4 &in_ref );
    
private:
    float m_mat[16];
};

#endif //!__MATRICES_HPP__