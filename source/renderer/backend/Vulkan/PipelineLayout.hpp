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

class crPipelineLayout
{
public:
    crPipelineLayout( void );
    ~crPipelineLayout( void );

    bool    Create( const crList<VkDescriptorSetLayoutBinding> &in_storageBindings, const crList<VkDescriptorBindingFlags> &in_bindingFlags );
    void    Destroy( void );

    operator VkPipelineLayout( void ) const { return m_layout; }

private:
    VkDescriptorSet			m_bindlessSet;
    VkDescriptorSetLayout	m_descriptor;
    VkDescriptorPool		m_descriptorPool;
    VkPipelineLayout        m_layout;
};

#endif //!__PIPELINE_LAYOUT_HPP__