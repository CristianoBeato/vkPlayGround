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

#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

struct alignas( 16 ) material_t
{
    uint32_t    samp0 = 0; // sampler location 0 ( albedo )
    uint32_t    samp1 = 0; // sampler location 1 ( normal )
    uint32_t    samp2 = 0; // sampler location 2 ( RMAO )
    uint32_t    samp3 = 0; // sampler location 3 ( R)
    float       matrix[16];
};

class crMaterial
{
public:
    crMaterial( void );
    ~crMaterial( void );

    /// @brief material don't have transparency
    bool    Opaque( void ) const;

    /// @brief material perform a additive pass, 
    bool    Emissive( void ) const;

private:

};

#endif //!__MATERIAL_HPP__