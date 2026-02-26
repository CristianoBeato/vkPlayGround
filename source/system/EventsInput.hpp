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

#ifndef __EVENTS_INPUT_HPP__
#define __EVENTS_INPUT_HPP__

class crEventsInput
{
public:
    static crEventsInput*   Get( void );

    crEventsInput( void );
    ~crEventsInput( void ); 
    void    StartUp( void );
    void    ShutDown( void );
    bool    Pool( void );

private:

};

#endif //!__EVENTS_INPUT_HPP__