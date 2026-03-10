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
#ifndef __MODEL_TERRAIN_HPP__
#define __MODEL_TERRAIN_HPP__

class crModelTerrain : public crModel
{
public:
    crModelTerrain( void );
    ~crModelTerrain( void );

    virtual void        View( const crVec3f in_viewPoint );
    virtual void        Update( void );

private:


	void	SetParameters(	const float in_maxPixelError, 
							const float in_maxTexelSize, 
							const float in_ScreenWidth, 
							const float in_HorizontalFOVDegrees );
};



#endif //!__MODEL_TERRAIN_HPP__