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

#ifndef __MODEL_MANAGER_HPP__
#define __MODEL_MANAGER_HPP__

class crModel;
class crModelManager
{
public:
    static crModelManager* Get( void );
    crModelManager( void );
    ~crModelManager( void );
    void    BeginLevelLoad( void );
    void    EndLevelLoad( void );
    void    Touch( crModel* in_model );
    
private:

};

#endif //!__MODEL_MANAGER_HPP__