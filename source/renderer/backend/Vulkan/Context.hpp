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

#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

extern const VkAllocationCallbacks* k_allocationCallbacks;

VK_DEFINE_HANDLE(VmaAllocator)

class crContext
{
public:
    static crContext*   Get( void ); 

    crContext( void );
    ~crContext( void );

    bool                StarUp( void );
    void                Shutdown( void );

    VkInstance          Instance( void ) const { return m_instance; }
    VkSurfaceKHR        Surface( void )  const { return m_surface; }
    crRenderDevice *    Device( void ) { return &m_devices[m_currentDevice]; }

#if USE_VMA
    VmaAllocator        VMAAllocator( void ) const { return m_VMAallocator; }
#endif

private:
    bool                            m_hasDebugUtils;    // Enable debug layer
    bool                            m_portabilityEnumerationAvailable; //
    uint32_t                        m_currentDevice;    // current initialized device 
    VkInstance                      m_instance;
    VkDebugUtilsMessengerEXT        m_debugCallback;    // The debug utility messenger callback.
	VkSurfaceKHR                    m_surface;          // The surface we will render to.
    crList<VkExtensionProperties>   m_availableInstanceExtensions;  //
    crList<VkLayerProperties>       m_supportedInstanceLayers;
    crList<VkPhysicalDevice>        m_availablePhysicalDevices; // list of vulkan compatible devices
	crList<crRenderDevice>          m_devices; // our interal device list 

#if USE_VMA
    VmaAllocator                    m_VMAallocator;
#endif //USE_VMA

    void    InitInstanceProcs( void );
    void    LoadVulkanProcs( void );
    bool    ExtensionAvailable( const crString &in_ext ) const;
    bool    LayersAvailable( const crString &in_layer ) const;

#if USE_VMA
    void    SetVMAallocator( void );
#endif //USE_VMA
};

#endif //!__CONTEXT_HPP__