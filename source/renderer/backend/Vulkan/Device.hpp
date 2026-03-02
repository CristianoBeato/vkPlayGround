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

#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

struct queueInfo_t
{
    bool        present = false;    // is a present queue
    bool        graphic = false;    // is a graphic queue
    bool        transfer = false;   // is a transfer queue
    bool        compute = false;    // compute queue 
    uint32_t    index = 0;          // queue index 
    uint32_t    family = 0;         // quque family
};

typedef class crDeviceQueue* crDeviceQueuep;

/// @brief 
class crDeviceQueue
{
public:
    crDeviceQueue( const uint32_t in_family, const uint32_t in_index );
    ~crDeviceQueue( void );
    bool            Init( const VkDevice in_device );
    bool            WaitSemaphore( const uint64_t in_value, const uint64_t in_timeout );
    uint64_t        IncrementTimeline( void );
    uint32_t        Index( void ) const { return m_index; }
    uint32_t        Family( void ) const { return m_index; }
    uint64_t        Timeline( void ) const { return m_timeline; }
    VkQueue         Queue( void ) const { return m_queue; }
    VkCommandPool   CommandPool( void ) const { return m_commandPool; }
    VkSemaphore     Semaphore( void ) const { return m_semaphore; }

private:
    uint32_t                m_index;        // index in the family 
    uint32_t                m_family;       // the family index
    std::atomic_uint64_t    m_timeline;     // events timeline counter
    VkQueue                 m_queue;        // queue hanlde
    VkCommandPool           m_commandPool;  // queue command pool
    VkSemaphore             m_semaphore;    // queue semaphore
    VkDevice                m_device;       // parent device
};

struct device_info_t
{
    uint32_t vendorID = 0;
    uint32_t deviceID = 0;
    uint32_t driverVersion = 0;
    uint32_t driverABI = 0;
    uint8_t uuid[VK_UUID_SIZE];
};

typedef class crRenderDevice* crRenderDevicep;

class crRenderDevice
{
public:
    crRenderDevice( void );
    ~crRenderDevice( void );

    bool        Init( const uint32_t in_ID, const VkPhysicalDevice in_device, const VkSurfaceKHR in_surface );
    bool        StartUp( const crList<const char*> &in_layers, const crList<const char*> &in_enabledExtensions );
    void        ShutDown( void );

    uint32_t    Score(void) const;

    uint32_t    Mask( void ) const { return m_id + 1; }

    /// @brief check for extension if available in the device 
    /// @return 
    bool        ExtensionAvailable( const crString &in_ext ) const;

    /// @brief Find device memory type
    /// @param type_filter 
    /// @param properties 
    /// @return the index of the type 
    uint32_t FindMemoryType( const uint32_t in_filter, const VkMemoryPropertyFlags in_properties ) const;

    const bool                      SupportedFormat( const VkFormat in_format, const VkColorSpaceKHR in_colorSpace ) const;

    const bool                      SupportedPresentMode( const VkPresentModeKHR in_mode );

    const VkSurfaceFormatKHR        GetPresentFormat( const uint32_t in_formatID );
    
    /// @brief 
    /// @param depthFormat 
    /// @return 
    const bool                      SupportedDepthFormat( VkFormat *depthFormat ) const;

    /// @brief 
    /// @param depthStencilFormat 
    /// @return 
    const bool                      SupportedDepthStencilFormat( VkFormat *depthStencilFormat) const;

    const uint32_t                  MinMemoryMapAlignment( void ) const;
    const uint32_t                  MinTexelBufferOffsetAlignment( void ) const;
    const uint32_t                  MinStorageAlignment( void ) const;
    
    ///
    const device_info_t             DevInfo( void ) const;

    /// @brief 
    /// @param in_format 
    /// @param tiling 
    /// @return 
    const bool                      FormatIsFilterable(const VkFormat in_format, const VkImageTiling tiling) const;

    /// @brief Device name
    inline const char*              Name( void ) const { return m_propertiesv10.properties.deviceName; }

    /// @brief the type of the gpu
    inline VkPhysicalDeviceType     Type( void ) const { return m_propertiesv10.properties.deviceType; }
    
    /// @brief current suported device version
    inline uint32_t                 ApiVersion( void ) const { return m_propertiesv10.properties.apiVersion; }

    /// @brief Max msaa samples suported
    uint32_t                        MaxSamples( void ) const;

    /// @brief logic device handler
    inline VkDevice                 Device( void ) const { return m_logic; }
    
    inline crDeviceQueue*   PresentQueue( void ) const { return m_present; }
    inline crDeviceQueue*   GraphicQueue( void ) const { return m_graphic; }
    inline crDeviceQueue*   ComputeQueue( void ) const { return ( m_compute != nullptr ) ? m_compute : m_graphic; }
    inline crDeviceQueue*   TransferQueue( void ) const { return ( m_transfer != nullptr ) ? m_transfer : m_graphic; }

    /// @brief Physic device handler
    inline VkPhysicalDevice PhysicalDevice( void ) const { return m_physical; }
        
    inline operator VkDevice( void ) const { return m_logic; }
    inline operator VkPhysicalDevice( void ) const { return m_physical; }

private:
    uint32_t                                        m_id;

    // device properties
    VkPhysicalDeviceProperties2                     m_propertiesv10;
    VkPhysicalDeviceVulkan11Properties              m_propertiesv11;
    VkPhysicalDeviceVulkan12Properties              m_propertiesv12;
    VkPhysicalDeviceVulkan13Properties              m_propertiesv13;
    
    // device enabled features
    VkPhysicalDeviceFeatures2                       m_featuresv10;
    VkPhysicalDeviceVulkan12Features                m_featuresv11;
    VkPhysicalDeviceVulkan12Features                m_featuresv12;
    VkPhysicalDeviceVulkan13Features                m_featuresv13;

    // device suface sapabilities 
    VkSurfaceCapabilities2KHR                       m_surfaceCapabilities;

    // device memory info
    VkPhysicalDeviceMemoryProperties2               m_memoryProperties;
    VkPhysicalDevice                                m_physical; // Physical Device handler 
    VkDevice                                        m_logic;    // Logic Device handler
    
    // device queues
    crDeviceQueue*                                  m_present;
    crDeviceQueue*                                  m_graphic;
    crDeviceQueue*                                  m_compute;
    crDeviceQueue*                                  m_transfer;

    crList<VkQueueFamilyProperties2>                m_queueFamilyPropertiesList;
    crList<VkExtensionProperties>                   m_deviceExtensions;
    crList<VkSurfaceFormat2KHR>                     m_surfaceFormats;
    crList<VkPresentModeKHR>                        m_presentModes;
    crList<queueInfo_t>                             m_queues;

    void SelectDeviceQueues( crList<VkDeviceQueueCreateInfo> &in_queueList );
};

#endif //!__DEVICE_HPP__