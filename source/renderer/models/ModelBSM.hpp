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

#ifndef __MODEL_BSM_HPP__
#define __MODEL_BSM_HPP__



/// @brief Load ou internal model
/// Beato Static Mesh
class crModelBSM : 
    public crModel,
    public crResource
{
public:
    crModelBSM( void );
    ~crModelBSM( void );

    virtual bool    Create( const uint32_t in_flags, const crString in_name, const crString in_sourceFile ) override;
    virtual void    Destroy( void ) override;
    virtual bool    Load( void ) override;

private:

};

#endif //!__MODEL_BSM_HPP__