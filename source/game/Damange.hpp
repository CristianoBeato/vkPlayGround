
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

#ifndef __DAMANGE_HPP__
#define __DAMANGE_HPP__

struct damange_t
{
    enum type_t : uint16_t
    {
        DM_PHISIC       = 1 << 0, /// Phisic damange
        DM_PIERCING     = 1 << 1, /// Armour piercing
        DM_FIRE         = 1 << 2, /// Fire damange
        DM_MAGIC        = 1 << 3, /// Magic cast damange
        DM_EXPLOSIVE    = 1 << 4, /// Explosion damange 
    };

    type_t      type;       /// @brief the type of the damange caused
    uint16_t    ammount;    /// @brief the amount of the damange caused 
};

#endif //__DAMANGE_HPP__