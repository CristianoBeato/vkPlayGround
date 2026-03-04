
#ifndef __VMA_HPP__
#define __VMA_HPP__

#define USE_VMA 0

// include VMA Here, since, his include of other headers 
// can lead to naming polution
#if USE_VMA

#define USE_VMA_BUFFERS 1
#define USE_VMA_FRAME_BUFFERS 1
#define USE_VMA_IMAGES 1

#   if __COMPILER_CLANG__
#       pragma clang diagnostic push
#       pragma clang diagnostic ignored "-Wnullability-completeness"
#   endif //__COMPILER_CLANG__

#   ifndef VMA_IMPLEMENTATION
#       define VMA_IMPLEMENTATION
#   endif //!VMA_IMPLEMENTATION
#   define VMA_VULKAN_VERSION 1003000 // VMA Vulkan 1.3specification
#   define VMA_STATIC_VULKAN_FUNCTIONS 0
#   define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#   include <vk_mem_alloc.h>

#   if __COMPILER_CLANG__
#       pragma clang diagnostic pop
#   endif //__COMPILER_CLANG__

#else

#define USE_VMA_BUFFERS 0
#define USE_VMA_FRAME_BUFFERS 0
#define USE_VMA_IMAGES 0

#endif //USE_VMA

#endif //!__VMA_HPP__