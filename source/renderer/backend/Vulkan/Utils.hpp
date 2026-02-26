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

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

extern crString VulkanErrorString( VkResult errorCode );

inline void ReturnError( const VkResult in_errorCode, const char* in_fn, const char* in_file, const int in_line )
{
    auto error = VulkanErrorString( in_errorCode );
    printf( "VkResult is \"%s\" in \"%s\" at line %i\n", error.c_str(), in_file, in_line );
}

inline bool ResultCheck( const VkResult in_result, const char* in_fnName )
{
    /// no erro just pass
    if( in_result == VK_SUCCESS )
        return true;
    
    /// error found, what
    auto err = VulkanErrorString( in_result );
    printf( " Vulkan %s result: %s\n", in_fnName, err.c_str() );
    return false;
}

#define VK_CHECK(f)                                 \
{                                                   \
	VkResult res = (f);                             \
	if ( res != VK_SUCCESS )	                    \
	{					                            \
        ReturnError( res, #f, __FILE__, __LINE__ ); \
 	}											    \
}

struct uniqueQueue_t
{
    uint32_t    count = 0;
    uint32_t    families[4]{ UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX }; // graphic, compute, transfer, present

    inline VkSharingMode SharingMode( void ) const
    { 
        return count > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE; 
    } 

    inline void    Append( const uint32_t in_familyIndex )
    {
        // If the index is invalid (VULKAN_UNDEFINED_FAMILY) or exceeds the limit
        if ( in_familyIndex == uint32_t(-1) || count >= 4 ) 
            return;

        for ( uint32_t i = 0; i < count; i++)
        {
            // found, not insert
            if( families[i] == in_familyIndex )
                return;
        }
        
        families[count++] = in_familyIndex;
    }
};

#endif //!__UTILS_HPP__