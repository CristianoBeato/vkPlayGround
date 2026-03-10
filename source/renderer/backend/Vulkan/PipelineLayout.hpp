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

#ifndef __PIPELINE_LAYOUT_HPP__
#define __PIPELINE_LAYOUT_HPP__

inline constexpr uint32_t MAX_BINDING_SAMPLERS = 8192;

///
/// Texture Sampler binding slot 
class crSamplerSlot
{
public:
	crSamplerSlot( void );
	crSamplerSlot( const VkImageView in_imageView, const VkSampler in_sampler, const VkImageLayout in_imageLayout );
    ~crSamplerSlot( void );
    
    void    SetIndex( const uint32_t in_index ) { m_index = in_index; }
    uint32_t GetIndex( void ) const { return m_index; }
    VkDescriptorImageInfo GetHandle( void ) const { return m_descriptorImageInfo; };

    ///
    operator uint32_t( void ) const { return m_index; }  

private:
    uint32_t 				m_index;				// logic index
	VkDescriptorImageInfo   m_descriptorImageInfo; 
};

class crSamplerSlotArray
{
public:
    crSamplerSlotArray( void );
    ~crSamplerSlotArray( void );
    bool    Create( void );
    void    Destroy( void );
    // void    Bind( void );
    void    Update( void );

    operator VkDescriptorSetLayout( void ) const { return m_bindlessLayout; }
    operator VkDescriptorSet( void ) const { return m_bindlessSet; }

private:
    VkDescriptorSet			                                    m_bindlessSet;
    VkDescriptorSetLayout	                                    m_bindlessLayout;
    VkDescriptorPool		                                    m_bindlessPool;
	crArray<VkDescriptorImageInfo, MAX_BINDING_SAMPLERS>        m_imageInfos;
    crList<uint32_t>                                            m_freeSlots;
};

class crShaderStorageLayout
{
public:
    crShaderStorageLayout( void );
    ~crShaderStorageLayout( void );

    /// @brief Create a shader storage layout
    /// @param in_bindingCount 
    /// @return true on success, false on error
    bool    Create( const uint32_t in_bindingCount );

    /// @brief Destroy pipeline layout object
    void    Destroy( void );

    /// @brief 
    /// @param in_commandBuffer 
    /// @param in_pipelineLayout 
    void    Bind( const VkCommandBuffer in_commandBuffer, const VkPipelineLayout in_pipelineLayout );

    /// @brief Update descriptor set layout buffers
    /// @param in_bufferID 
    /// @param in_buffers 
    /// @param in_count 
    void    Update( const uint32_t in_bufferID, const VkBuffer* in_buffers, const uint32_t in_count );

    operator VkDescriptorSetLayout( void ) const { return m_descriptorSetLayout; }
    operator VkDescriptorSet( void ) const { return m_descriptorSet[m_bufferID]; }

private:
    uint32_t                                    m_bufferID;
    uint32_t                                    m_bindings;
    
    /// @brief Explicitly describe the interface of the resources accessed by the shader.
    VkDescriptorSetLayout                       m_descriptorSetLayout;

    /// @brief It reserves blocks of GPU resources in advance so that the allocation of descriptor 
    /// sets is fast and does not cause crashes during the rendering loop.
    VkDescriptorPool		                    m_descriptorPool;

    /// @brief Real Resource Binding, this is where you associate a specific VkBuffer 
    /// with a binding number defined in the shader.
    crArray<VkDescriptorSet, SMP_FRAMES>        m_descriptorSet;
};

class crPipelineLayout
{
public:
    crPipelineLayout( void );
    ~crPipelineLayout( void );

    bool    Create( const crSamplerSlotArray* in_bindlessArrays, const crShaderStorageLayout* in_bindingStorage );
    void    Destroy( void );
    operator VkPipelineLayout( void ) const { return m_layout; }

private:
    crShaderStorageLayout*  m_storage;
    crSamplerSlotArray*     m_samples;
    VkDescriptorSet         m_descriptorSets[2];
    VkPipelineLayout        m_layout;
};

#endif //!__PIPELINE_LAYOUT_HPP__