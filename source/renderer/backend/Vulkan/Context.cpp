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

#include "Context.hpp"
#include "system/Video.hpp"
#include "Core.hpp"

#define NO_SDL_VULKAN_TYPEDEFS
#include <SDL3/SDL_vulkan.h>

crCVar vk_deviceID( "vk_deviceID", "-1", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER, " -1 select the device with the highest score, +0 select the device by index" );

static VKAPI_ATTR void* VKAPI_CALL vkAllocation( void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope );
static VKAPI_ATTR void* VKAPI_CALL vkReallocation( void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
static VKAPI_ATTR void  VKAPI_CALL vkFree( void* pUserData, void* pMemory );
static VKAPI_ATTR void  VKAPI_CALL vkInternalAllocation( void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope);
static VKAPI_ATTR void  VKAPI_CALL vkInternalFree( void* pUserData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope );

// our onw allocation structure using SDL_malloc
static const VkAllocationCallbacks allocationCallbacksLocal = 
{
    nullptr,
    vkAllocation,
    vkReallocation,
    vkFree,
    vkInternalAllocation,
    vkInternalFree
};

#if 1 // TODO: a define to easy control
const VkAllocationCallbacks* k_allocationCallbacks = &allocationCallbacksLocal;
#else 
const VkAllocationCallbacks* k_allocationCallbacks = nullptr;
#endif 

/*
==============
crContext::Get
==============
*/
crContext *crContext::Get(void)
{
    static crContext gContext = crContext();
    return &gContext;
}

/*
==============
crContext::crContext
==============
*/
crContext::crContext(void) : m_hasDebugUtils(false),
                             m_portabilityEnumerationAvailable(false),
                             m_instance(nullptr),
                             m_debugCallback(nullptr),
                             m_surface(nullptr)
{
}

crContext::~crContext(void)
{
}

/*
==============
crContext::Init
==============
*/
bool crContext::StarUp( void )
{    
    VkResult                            result = VK_SUCCESS;
    uint32_t                            instanceExtensionCount = 0;
    uint32_t                            instanceLayerCount = 0;
    uint32_t                            physicalDeviceCount = 0;
    uint32_t                            device = 0;
    Uint32                              SDL3ExtensionCount = 0;
    const char* const*                  SDL3Extensions = nullptr;
	crList<const char*>                 requiredInstanceExtensions;
    crList<const char*>                 requiredDeviceExtensions;
    crList<const char*>                 requestedInstanceLayers;
    VkApplicationInfo                   applicationInfo{};
	VkInstanceCreateInfo                instanceCI{};
    VkDebugUtilsMessengerCreateInfoEXT  debugUtilsMessengerCI{};
    
    printf( "Initializing Vulkan instance.\n");

    InitInstanceProcs();
    requiredInstanceExtensions.Append( VK_KHR_SURFACE_EXTENSION_NAME );
    requiredInstanceExtensions.Append( VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME );
    requiredInstanceExtensions.Append( VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME );

    /// enumerate instance extensions
    result = vkEnumerateInstanceExtensionProperties( nullptr, &instanceExtensionCount, nullptr);
    if ( !ResultCheck( result, "vkEnumerateInstanceExtensionProperties" ) )
        return false;

    m_availableInstanceExtensions.Resize( instanceExtensionCount );
	result = vkEnumerateInstanceExtensionProperties( nullptr, &instanceExtensionCount, m_availableInstanceExtensions.GetData() );
    if ( !ResultCheck( result, "vkEnumerateInstanceExtensionProperties" ) )
        return false;

    /// instance enumerate layers 
    result = vkEnumerateInstanceLayerProperties( &instanceLayerCount, nullptr );
    if ( !ResultCheck( result, "vkEnumerateInstanceExtensionProperties" ) )
        return false;

    m_supportedInstanceLayers.Resize( instanceLayerCount );
    result = vkEnumerateInstanceLayerProperties( &instanceLayerCount, m_supportedInstanceLayers.GetData());
    if ( !ResultCheck( result, "vkEnumerateInstanceExtensionProperties" ) )
        return false;

	if ( ExtensionAvailable( VK_EXT_DEBUG_UTILS_EXTENSION_NAME ) )
    {
        m_hasDebugUtils = true;
		requiredInstanceExtensions.Append( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
    }
    
    if( ExtensionAvailable( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME ) )
    {
        m_portabilityEnumerationAvailable = true;
        requiredInstanceExtensions.Append( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
    }

    // copy and enable system extensios
    SDL3Extensions = SDL_Vulkan_GetInstanceExtensions( &SDL3ExtensionCount );
    printf( "SDL3 VkExt Found:\n" );    
    for ( uint32_t i = 0; i < SDL3ExtensionCount; i++)
    {
        requiredInstanceExtensions.Append( SDL3Extensions[i] );
        printf( " - %s\n", SDL3Extensions[i] );
    }

    /// apend layers 
    if( LayersAvailable( "VK_LAYER_KHRONOS_validation" ) )
        requestedInstanceLayers.Append( "VK_LAYER_KHRONOS_validation" );

    ///
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "Hello Triangle V1.3";
    applicationInfo.pEngineName = "Vulkan Samples";
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    ///
	instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCI.pApplicationInfo = &applicationInfo;
	instanceCI.enabledLayerCount = requestedInstanceLayers.Count();
	instanceCI.ppEnabledLayerNames = requestedInstanceLayers.GetData();
	instanceCI.enabledExtensionCount = requiredInstanceExtensions.Count();
	instanceCI.ppEnabledExtensionNames = requiredInstanceExtensions.GetData();

    ///
    debugUtilsMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugUtilsMessengerCI.pNext = nullptr;
    debugUtilsMessengerCI.flags = 0;

    /// if available, enable the debug output utils
    if ( m_hasDebugUtils )
	{
		debugUtilsMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		debugUtilsMessengerCI.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugUtilsMessengerCI.pfnUserCallback = DebugCallback;
        debugUtilsMessengerCI.pUserData = this;
        instanceCI.pNext = &debugUtilsMessengerCI;
	}

	if ( m_portabilityEnumerationAvailable )
        instanceCI.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	// Create the Vulkan instance
	result = vkCreateInstance(&instanceCI, k_allocationCallbacks, &m_instance );
    if ( !ResultCheck( result, "vkCreateInstance" ) )
        return false;
    
    LoadVulkanProcs();

    if ( m_hasDebugUtils )
    {
		result = vkCreateDebugUtilsMessengerEXT( m_instance, &debugUtilsMessengerCI, k_allocationCallbacks, &m_debugCallback );
        if ( !ResultCheck( result, "vkCreateDebugUtilsMessengerEXT" ) )
            m_hasDebugUtils = false;
    }

    auto video = crVideo::Get();
    if ( !SDL_Vulkan_CreateSurface( video->MainWindow(), m_instance, k_allocationCallbacks, &m_surface ) )
    {
        printf( "Failed to create window surface %s\n", SDL_GetError() );
        return false;
    }

    result = vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, nullptr);
    if ( !ResultCheck( result, "vkEnumeratePhysicalDevices" ) )
        return false;
    
    if ( physicalDeviceCount < 1 )
    {
        printf( "No vulkan suported physical device found." );
        return false;
    }
    
    m_availablePhysicalDevices.Resize( physicalDeviceCount );
    m_devices.Resize( physicalDeviceCount );
    result = vkEnumeratePhysicalDevices( m_instance, &physicalDeviceCount, m_availablePhysicalDevices.GetData() );    
    if ( !ResultCheck( result, "vkEnumeratePhysicalDevices" ) )
        return false;
    
    for ( uint32_t i = 0; i < physicalDeviceCount; i++)
    {
        m_devices[i].Init( i, m_availablePhysicalDevices[i], m_surface ); 
    }
    
    printf( " %i Vulkan suported devices found\n", physicalDeviceCount );
    
    auto FallBack = [physicalDeviceCount, &device]( crList<crRenderDevice> &devices, crList<const char*> layers, const crList<const char*> extensions )
    {   
        for ( uint32_t i = 0; i < physicalDeviceCount; i++)
        {
            if( devices[i].StartUp( layers, extensions ) )
            {
                device = i;
                break;
            }
        }

        /// no suitable device initialized
        device = UINT32_MAX;
    };

    /// We require a device whit swap chain suport, an whit the extension initialized
    requiredDeviceExtensions.Append( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

    if( vk_deviceID.GetInteger() > -1 )
    {
        device = std::clamp( static_cast<uint32_t>( vk_deviceID.GetInteger() ), 0u,  physicalDeviceCount -1 );
        // TODO: need create a fallback, to try initalize another device
        if( !m_devices[device].StartUp( requestedInstanceLayers, requiredDeviceExtensions ) )
        {   
            //TODO:
            // common->FatalError( "Can't initialize selected device ID: %i, name: %s\n", device, m_devices[device].Name() );
            throw crException( "Can't initialize selected device ID: %i, name: %s\n", device, m_devices[device].Name() );

        }
    }
    else
    {
        uint32_t bestScore = 0;
        uint32_t bestDevice = 0;
        /// find the best
        for ( uint32_t i = 0; i < physicalDeviceCount; i++)
        {
            uint32_t score = m_devices[i].Score();
            if ( bestScore < score )
            {
                bestScore = score;
                bestDevice = i;
            }
        }

        device = bestDevice;

        // TODO: need create a fallback, to try initalize another device
        if( !m_devices[device].StartUp( requestedInstanceLayers, requiredDeviceExtensions ) )
        {
            // common->FatalError( "Can't initialize selected device ID: %i, name: %s\n", device, m_devices[device].Name() );
            throw crException( "Can't initialize selected device ID: %i, name: %s\n", device, m_devices[device].Name() );
        }
    }

    m_currentDevice = device;

    return true;
}

/*
==============
crContext::Shutdown
==============
*/
void crContext::Shutdown( void )
{
    if ( m_debugCallback != nullptr )
    {
        vkDestroyDebugUtilsMessengerEXT( m_instance, m_debugCallback, k_allocationCallbacks );
        m_debugCallback = nullptr;
    }
    
    if ( m_surface != nullptr )
    {
        vkDestroySurfaceKHR( m_instance, m_surface, k_allocationCallbacks );
        m_surface = nullptr;
    }
    
    if ( m_instance != nullptr )
    {
        vkDestroyInstance( m_instance, k_allocationCallbacks );
        m_instance = nullptr;
    }
}

/*
==============
crContext::ExtensionAvailable
==============
*/
bool crContext::ExtensionAvailable(const crString &in_ext) const
{
    for ( uint32_t i = 0; i < m_availableInstanceExtensions.Count(); i++)
    {
        crString ext = m_availableInstanceExtensions[i].extensionName;
        if( ext == in_ext )
            return true;
    }
    
    printf( "%s not found!\n", in_ext.c_str() );
    return false;
}

/*
==============
crContext::LayersAvailable
==============
*/
bool crContext::LayersAvailable(const crString &in_layer) const
{
    for ( uint32_t i = 0; i < m_supportedInstanceLayers.Count(); i++)
    {
        crString layer = m_supportedInstanceLayers[i].layerName;
        if( layer == in_layer )
        {
            printf( "%s found\n", in_layer.c_str() );
            return true;
        }
    }
    
    printf( "%s not found\n", in_layer.c_str() );
    return false;
}

/*
==============================================
vkAllocation
==============================================
*/
void *VKAPI_ATTR vkAllocation( void * in_userData, size_t in_size, size_t in_alignment, VkSystemAllocationScope in_allocationScope )
{
    void* memptr = nullptr;
#if 1
    void* original = SDL_malloc( in_size + in_alignment - 1 + sizeof(void*) );
    uintptr_t aligned = ( reinterpret_cast<uintptr_t>( original ) + sizeof(void*) + in_alignment - 1) & ~( in_alignment - 1 );
    (reinterpret_cast<void**>(aligned))[-1] = original;
    memptr = reinterpret_cast<void*>( aligned );
    // check if memory is aligned 
    assert( memptr && ( (uintptr_t)memptr % in_alignment ) == 0 );
#else
    memptr = SDL_aligned_alloc( in_alignment, in_size );
#endif
    return memptr;
}

/*
==============================================
vkReallocation
==============================================
*/
void* VKAPI_CALL vkReallocation( void* in_userData, void* in_original, size_t in_size, size_t in_alignment, VkSystemAllocationScope in_allocationScope )
{
    void* memptr = nullptr;
#if 1
    void* original = static_cast<void**>( in_original )[-1];
    if( original != nullptr )
        original = SDL_realloc( original, in_size + in_alignment - 1 + sizeof(void*) );
    else
        original = SDL_malloc( in_size + in_alignment - 1 + sizeof(void*) );

    uintptr_t aligned = ( reinterpret_cast<uintptr_t>( original ) + sizeof(void*) + in_alignment - 1) & ~( in_alignment - 1 );
    (reinterpret_cast<void**>(aligned))[-1] = original;
    memptr = reinterpret_cast<void*>( aligned );

    // check if memory is aligned 
    assert( memptr && ( (uintptr_t)memptr % in_alignment ) == 0 );
#else
    memptr = SDL_aligned_alloc( in_alignment, in_size );
    SDL_memcpy( memptr, in_original, sizeof( in_original ) );
    SDL_free( in_original );    
#endif
    return memptr;
}

/*
==============================================
vkFree
==============================================
*/
void VKAPI_CALL vkFree( void* in_userData, void* in_memory )
{
#if 1
    if ( in_memory ) 
    {
        void* original = static_cast<void**>( in_memory )[-1];
        SDL_free( original );
    }
#else
    SDL_aligned_free( in_memory );
#endif
}

/*
==============================================
vkInternalAllocation
==============================================
*/
void VKAPI_CALL vkInternalAllocation( void* in_userData, size_t in_size, VkInternalAllocationType in_allocationType, VkSystemAllocationScope in_allocationScope )
{
    //vkCtx.allocedMemory += size;
    //std::printf("[Vulkan] Internal allocation of %zu bytes, total %i\n", in_size, 0 );
}

/*
==============================================
vkInternalFree
==============================================
*/
void VKAPI_CALL vkInternalFree( void* in_userData, size_t in_size, VkInternalAllocationType in_allocationType, VkSystemAllocationScope in_allocationScope )
{
    // vkCtx.allocedMemory -= size; 
    //std::printf("[Vulkan] Internal free of %zu bytes, total %i\n", in_size, 0 );
}
