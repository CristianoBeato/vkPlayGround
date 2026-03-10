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

#ifndef __UNIFORM_MANAGER_HPP__
#define __UNIFORM_MANAGER_HPP__

inline constexpr uint32_t k_BINDLESS_SAMPLERS_BINDING   = 00u; /// bindless sampler location
inline constexpr uint32_t k_MESH_UNIFORM_BINDING        = 00u; ///
inline constexpr uint32_t k_MATERIAL_UNIFORM_BINDING    = 01u; /// 
inline constexpr uint32_t k_LIGTH_UNIFORM_BINDING       = 02u; ///
inline constexpr uint32_t k_JOINT_UNIFORM_BINDING       = 03u; ///
inline constexpr uint32_t k_NUM_SHADER_STORAGE          = 04u; ///

/// @brief Mesh uniform
struct alignas( 16 ) uMesh_t
{
    crVec4f     viewPos;    // view position in the world
    crMatrix4   model;      // model matrix
    crMatrix4   MVP;        // mode view projection
};

/// @brief Material uniform
struct alignas( 16 ) uMaterial_t
{
    crVec4f     color; //   
};

struct alignas( 16 ) uLight_t
{
    crVec4f ligthPos;   // Light Position
    crVec4f lightColor; // Light color
};

template< typename _t >
class crUniform
{
public:
    typedef _t* pointer;

    crUniform( void ) : 
        m_count( 0 ),
        m_current( 0 ),
        m_array( nullptr )
    {
    }

    ~crUniform( void )
    {
    }

    /// acess buffer position
    inline pointer Get( void ) const 
    { 
        return &m_array[m_current]; 
    }

    /// get next element 
    inline uint32_t Next( void ) 
    { 
        uint32_t a = m_current; 
        m_current = ( m_current + 1) % m_count;
        return a; 
    }

protected:
    crUniform( const uint32_t in_count, const pointer in_array )
    {
    }

private:
    uint32_t    m_count;    /// uniform array size
    uint32_t    m_current;
    pointer     m_array;    ///

    // don't permit class copy 
    crUniform( const crUniform& ) = delete;
    crUniform operator = ( const crUniform& ) = delete;
};

class crUniformManager
{
public:
    static crUniformManager* Get( void );

    crUniformManager( void );
    ~crUniformManager( void );
    void    StartUp( void );
    void    ShutDown( void );

    // Mesh properties uniform array 
    crUniform<uMesh_t>*     Mesh( void ) const { return const_cast<crUniform<uMesh_t>*>( &m_mesh ); }

    // material properties uniform array
    crUniform<uMaterial_t>* Material( void ) const { return const_cast<crUniform<uMaterial_t>*>( &m_material ); }

    // light protperties uniforms
    crUniform<uLight_t>*    Light( void ) const { return const_cast<crUniform<uLight_t>*>( &m_ligth ); }

protected:
    friend class crPipeline;
    crPipelineLayout*   Layout( void ) const { return m_layout; }

private:
    crUniform<uMesh_t>      m_mesh;
    crUniform<uMaterial_t>  m_material;
    crUniform<uLight_t>     m_ligth;
    crShaderStorageLayout*  m_storageLayout;
    crSamplerSlotArray*     m_samplerSlotArray; // store combined sampler arrai location
    crPipelineLayout*       m_layout;
    crBuffer*               m_meshSSBO;     // mesh shader storage buffer
    crBuffer*               m_materialSSBO; // material shader storage buffer
    crBuffer*               m_lightSSBO;    // light shader storage buffer
    uMesh_t*                m_meshUniformMap;
    uMaterial_t*            m_materialUniformMap;
    uLight_t*               m_lightUniformMap;
};

#endif //!__UNIFORM_MANAGER_HPP__