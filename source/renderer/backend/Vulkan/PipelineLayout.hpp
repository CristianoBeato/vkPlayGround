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
    struct storage_binging_t
    {
    };

    crShaderStorageLayout( void );
    ~crShaderStorageLayout( void );
    bool    Create( crList<storage_binging_t> in_bingings );
    void    Destroy( void );
    void    Bind( void );
    void    Update( void );
    operator VkDescriptorSetLayout( void ) const { return m_descriptorSetLayout; }
    operator VkDescriptorSet( void ) const { return m_descriptorSet; }
private:
    VkDescriptorSet			                                    m_descriptorSet;
    VkDescriptorSetLayout	                                    m_descriptorSetLayout;
    VkDescriptorPool		                                    m_descriptorPool;
};

class crPipelineLayout
{
public:
    crPipelineLayout( void );
    ~crPipelineLayout( void );

    bool    Create( const crSamplerSlotArray* in_bindlessArrays, const crShaderStorageLayout* in_bindingStorage );
    void    Destroy( void );
    void    SetBuffers( const crList<VkWriteDescriptorSet > in_buffers );
    void    Bind( void ) const;

    operator VkPipelineLayout( void ) const { return m_layout; }

private:
    crShaderStorageLayout*  m_storage;
    crSamplerSlotArray*     m_samples;
    VkDescriptorSet         m_descriptorSets[2];
    VkPipelineLayout        m_layout;
};

#endif //!__PIPELINE_LAYOUT_HPP__