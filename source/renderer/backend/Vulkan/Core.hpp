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

#ifndef __VK_CORE_HPP__
#define __VK_CORE_HPP__

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

extern PFN_vkEnumerateInstanceLayerProperties           vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumerateInstanceExtensionProperties       vkEnumerateInstanceExtensionProperties;

// VkInstance
extern PFN_vkCreateInstance                             vkCreateInstance;
extern PFN_vkDestroyInstance                            vkDestroyInstance;
extern PFN_vkGetInstanceProcAddr                        vkGetInstanceProcAddr;
extern PFN_vkEnumeratePhysicalDevices                   vkEnumeratePhysicalDevices;

extern PFN_vkDestroySurfaceKHR                          vkDestroySurfaceKHR;

// VkPhysicalDevice
extern PFN_vkGetPhysicalDeviceMemoryProperties2         vkGetPhysicalDeviceMemoryProperties2;
extern PFN_vkGetPhysicalDeviceProperties2               vkGetPhysicalDeviceProperties2;
extern PFN_vkGetPhysicalDeviceFeatures2                 vkGetPhysicalDeviceFeatures2;
extern PFN_vkEnumerateDeviceExtensionProperties         vkEnumerateDeviceExtensionProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2    vkGetPhysicalDeviceQueueFamilyProperties2;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR    vkGetPhysicalDeviceSurfacePresentModesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR   vkGetPhysicalDeviceSurfaceCapabilities2KHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormats2KHR        vkGetPhysicalDeviceSurfaceFormats2KHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR         vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkGetPhysicalDeviceFormatProperties2         vkGetPhysicalDeviceFormatProperties2;

// VkDevice
extern PFN_vkCreateDevice                               vkCreateDevice;
extern PFN_vkDestroyDevice                              vkDestroyDevice;
extern PFN_vkDeviceWaitIdle                             vkDeviceWaitIdle;
extern PFN_vkGetDeviceProcAddr                          vkGetDeviceProcAddr;

// VkQueue
extern PFN_vkGetDeviceQueue                             vkGetDeviceQueue;
extern PFN_vkGetDeviceQueue2                            vkGetDeviceQueue2;
extern PFN_vkQueuePresentKHR                            vkQueuePresentKHR;
extern PFN_vkQueueSubmit                                vkQueueSubmit;
extern PFN_vkQueueSubmit2                               vkQueueSubmit2;
extern PFN_vkQueueWaitIdle                              vkQueueWaitIdle;
extern PFN_vkCreateCommandPool                          vkCreateCommandPool;
extern PFN_vkDestroyCommandPool                         vkDestroyCommandPool;

// VkFence
extern PFN_vkCreateFence                                vkCreateFence;
extern PFN_vkDestroyFence                               vkDestroyFence;
extern PFN_vkResetFences                                vkResetFences;
extern PFN_vkWaitForFences                              vkWaitForFences;
extern PFN_vkGetFenceStatus                             vkGetFenceStatus;

// VkSemaphore
extern PFN_vkCreateSemaphore                            vkCreateSemaphore;
extern PFN_vkDestroySemaphore                           vkDestroySemaphore;
extern PFN_vkSignalSemaphore                            vkSignalSemaphore;
extern PFN_vkWaitSemaphores                             vkWaitSemaphores;
extern PFN_vkGetSemaphoreCounterValue                   vkGetSemaphoreCounterValue;

// VkShaderModule
extern PFN_vkCreateShaderModule                         vkCreateShaderModule;
extern PFN_vkDestroyShaderModule                        vkDestroyShaderModule;

// VkSwapchainKHR
extern PFN_vkCreateSwapchainKHR                         vkCreateSwapchainKHR;
extern PFN_vkDestroySwapchainKHR                        vkDestroySwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR                      vkGetSwapchainImagesKHR;
extern PFN_vkAcquireNextImage2KHR                       vkAcquireNextImage2KHR;

// VkDeviceMemory
extern PFN_vkAllocateMemory                             vkAllocateMemory;
extern PFN_vkFreeMemory                                 vkFreeMemory;
extern PFN_vkMapMemory                                  vkMapMemory;
extern PFN_vkUnmapMemory                                vkUnmapMemory;
extern PFN_vkFlushMappedMemoryRanges                    vkFlushMappedMemoryRanges;

// VkBuffer
extern PFN_vkCreateBuffer                               vkCreateBuffer;
extern PFN_vkDestroyBuffer                              vkDestroyBuffer;
extern PFN_vkGetBufferMemoryRequirements                vkGetBufferMemoryRequirements;
extern PFN_vkBindBufferMemory                           vkBindBufferMemory;
extern PFN_vkCmdUpdateBuffer                            vkCmdUpdateBuffer;
extern PFN_vkCmdFillBuffer                              vkCmdFillBuffer;

// VkImage
extern PFN_vkCreateImage                                vkCreateImage;
extern PFN_vkDestroyImage                               vkDestroyImage;
extern PFN_vkGetImageMemoryRequirements                 vkGetImageMemoryRequirements;
extern PFN_vkBindImageMemory                            vkBindImageMemory;
extern PFN_vkCmdClearColorImage                         vkCmdClearColorImage;
extern PFN_vkCmdClearDepthStencilImage                  vkCmdClearDepthStencilImage;
extern PFN_vkCmdResolveImage2                           vkCmdResolveImage2;

// VkImageView
extern PFN_vkCreateImageView                            vkCreateImageView;
extern PFN_vkDestroyImageView                           vkDestroyImageView;

// VkSampler
extern PFN_vkCreateSampler                              vkCreateSampler;
extern PFN_vkDestroySampler                             vkDestroySampler;

// VkPipeline 
extern PFN_vkCreatePipelineLayout                       vkCreatePipelineLayout;
extern PFN_vkDestroyPipelineLayout                      vkDestroyPipelineLayout;
extern PFN_vkCreateGraphicsPipelines                    vkCreateGraphicsPipelines;
extern PFN_vkDestroyPipeline                            vkDestroyPipeline;

// VkRenderPass
extern PFN_vkCreateRenderPass                           vkCreateRenderPass;
extern PFN_vkCreateRenderPass2                          vkCreateRenderPass2;
extern PFN_vkDestroyRenderPass                          vkDestroyRenderPass;
extern PFN_vkCmdBeginRenderPass2                        vkCmdBeginRenderPass2;
extern PFN_vkCmdEndRenderPass2                          vkCmdEndRenderPass2;

// VkFramebuffer
extern PFN_vkCreateFramebuffer                          vkCreateFramebuffer;
extern PFN_vkDestroyFramebuffer                         vkDestroyFramebuffer;

// VkCommandBuffer
extern PFN_vkAllocateCommandBuffers                     vkAllocateCommandBuffers;
extern PFN_vkFreeCommandBuffers                         vkFreeCommandBuffers;
extern PFN_vkResetCommandBuffer                         vkResetCommandBuffer;
extern PFN_vkBeginCommandBuffer                         vkBeginCommandBuffer;
extern PFN_vkEndCommandBuffer                           vkEndCommandBuffer;
extern PFN_vkCmdExecuteCommands                         vkCmdExecuteCommands;
extern PFN_vkCmdBindIndexBuffer                         vkCmdBindIndexBuffer;
extern PFN_vkCmdBindVertexBuffers2                      vkCmdBindVertexBuffers2;
extern PFN_vkCmdDraw                                    vkCmdDraw;
extern PFN_vkCmdDrawIndexed                             vkCmdDrawIndexed;
extern PFN_vkCmdDrawIndirect                            vkCmdDrawIndirect;
extern PFN_vkCmdDrawIndexedIndirect                     vkCmdDrawIndexedIndirect;
extern PFN_vkCmdDispatch                                vkCmdDispatch;
extern PFN_vkCmdDispatchIndirect                        vkCmdDispatchIndirect;
extern PFN_vkCmdPipelineBarrier2                        vkCmdPipelineBarrier2;

// Pipeline
extern PFN_vkCmdBindPipeline                            vkCmdBindPipeline;
extern PFN_vkCmdSetViewport                             vkCmdSetViewport;
extern PFN_vkCmdSetScissor                              vkCmdSetScissor;
extern PFN_vkCmdSetLineWidth                            vkCmdSetLineWidth;
extern PFN_vkCmdSetDepthBias                            vkCmdSetDepthBias;
extern PFN_vkCmdSetBlendConstants                       vkCmdSetBlendConstants;
extern PFN_vkCmdSetDepthBounds                          vkCmdSetDepthBounds;
extern PFN_vkCmdSetCullMode                             vkCmdSetCullMode;
extern PFN_vkCmdSetStencilCompareMask                   vkCmdSetStencilCompareMask;
extern PFN_vkCmdSetStencilWriteMask                     vkCmdSetStencilWriteMask;
extern PFN_vkCmdSetStencilReference                     vkCmdSetStencilReference;

//
extern PFN_vkCreateDescriptorSetLayout                  vkCreateDescriptorSetLayout;
extern PFN_vkDestroyDescriptorSetLayout                 vkDestroyDescriptorSetLayout;
extern PFN_vkCreateDescriptorPool                       vkCreateDescriptorPool;
extern PFN_vkDestroyDescriptorPool                      vkDestroyDescriptorPool;
extern PFN_vkResetDescriptorPool                        vkResetDescriptorPool;

// VkDescriptorSet
extern PFN_vkAllocateDescriptorSets                     vkAllocateDescriptorSets;
extern PFN_vkFreeDescriptorSets                         vkFreeDescriptorSets;
extern PFN_vkUpdateDescriptorSets                       vkUpdateDescriptorSets;

// VkEvent
extern PFN_vkCmdSetEvent2                               vkCmdSetEvent2;
extern PFN_vkCmdResetEvent2                             vkCmdResetEvent2;
extern PFN_vkCmdWaitEvents2                             vkCmdWaitEvents2;

// VkQueryPool
extern PFN_vkCmdBeginQuery                              vkCmdBeginQuery;
extern PFN_vkCmdResetQueryPool                          vkCmdResetQueryPool;
extern PFN_vkCmdEndQuery                                vkCmdEndQuery;
extern PFN_vkCmdCopyQueryPoolResults                    vkCmdCopyQueryPoolResults;

// VK_KHR_copy_commands2
extern PFN_vkCmdBlitImage2                              vkCmdBlitImage2;
extern PFN_vkCmdCopyBuffer2                             vkCmdCopyBuffer2;
extern PFN_vkCmdCopyBufferToImage2                      vkCmdCopyBufferToImage2;
extern PFN_vkCmdCopyImage2                              vkCmdCopyImage2;
extern PFN_vkCmdCopyImageToBuffer2                      vkCmdCopyImageToBuffer2;

// VK_KHR_dynamic_rendering
extern PFN_vkCmdBeginRendering                          vkCmdBeginRendering;
extern PFN_vkCmdEndRendering                            vkCmdEndRendering;

// VK_EXT_debug_utils
extern PFN_vkCreateDebugUtilsMessengerEXT               vkCreateDebugUtilsMessengerEXT;
extern PFN_vkDestroyDebugUtilsMessengerEXT              vkDestroyDebugUtilsMessengerEXT;

extern VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT in_severity, VkDebugUtilsMessageTypeFlagsEXT in_types, const VkDebugUtilsMessengerCallbackDataEXT* in_data, void *in_user );

// Helper to manage texture 
typedef struct vkImageHandle_s
{
    VkImageLayout           layout      = VK_IMAGE_LAYOUT_UNDEFINED;
    VkPipelineStageFlags2   stageMask   = VK_PIPELINE_STAGE_NONE;
    VkAccessFlags2          accessMask  = VK_ACCESS_NONE;
    VkImage                 image       = nullptr;
    VkImageView             view        = nullptr;

    operator VkImage( void ) const { return image; }
    operator VkImageView( void ) const { return view; }
} vkImageHandle_t;

extern void VkImageStateTransition( vkImageHandle_t* in_image, const VkCommandBuffer in_commandBuffer, const VkImageLayout in_newLayout, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask );

#include "Utils.hpp"
#include "Device.hpp"
#include "Context.hpp"
#include "Commandbuffer.hpp"
#include "Buffer.hpp"
#include "Image.hpp"
#include "Framebuffer.hpp"
#include "Swapchain.hpp"
#include "Pipeline.hpp"

#endif //!__VK_CORE_HPP__