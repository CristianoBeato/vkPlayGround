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

#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

inline constexpr size_t MIN_BUFFER_SIZE = 64;

/// @brief base buffer class
class crBuffer
{
public:
    crBuffer( void );
    ~crBuffer( void );
    virtual bool    Create( const size_t in_size, const VkBufferUsageFlags in_usage, const VkMemoryPropertyFlags in_memoryProperty );
    virtual void    Destroy( void );
    void*           Map( void );
    void            Unmap( void );
    void            State( const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask );
    
    /// access the buffer handler
    inline VkBuffer Buffer( void ) const { return m_buffer; }

    /// @brief Retrive the buffer usable size
    inline size_t   Size( void ) const {  return m_size; }

private:
    size_t                  m_size;
    VkBufferUsageFlags      m_usage;
    VkPipelineStageFlags2   m_stage;
    VkAccessFlags2          m_access;
    VkBuffer                m_buffer;
    VkDeviceMemory          m_memory;
};

struct block_t
{
    bool        free = true;
    size_t      size = 0;
    uintptr_t   offset = 0;

    /// Binary Search Tree (Sorted by Size) 
    block_t*    parent = nullptr;
    block_t*    left = nullptr;
    block_t*    right = nullptr;

    /// Doubly Linked List (Physical Order in Buffer)
    block_t*    prevPhys = nullptr;
    block_t*    nextPhys = nullptr;
};

/// @brief a sub buffer represent a region of a buffer
class crSubBuffer
{
public:
    crSubBuffer( void );
    ~crSubBuffer( void );
    void        State( const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask );
    size_t      Size( void ) const { return m_size; }
    uintptr_t   Offset( void ) const { return m_offset; }
    VkBuffer    Handle( void ) const { return m_buffer; }

protected:
    friend class crBufferAllocator;
    friend class crBufferRing;
    crSubBuffer( const VkBuffer in_buffer, const size_t in_size, const uintptr_t in_offset );
    block_t*                Block( void ) { return m_block; }

private:
    block_t*                m_block;    //
    VkPipelineStageFlags2   m_stage;    //
    VkAccessFlags2          m_access;   //
    size_t                  m_size;     // size of the buffer
    uintptr_t               m_offset;   // offset in the buffer ( aligned )
    VkBuffer                m_buffer;   // handle
};

class crBufferAllocator : public crBuffer
{
public:
    crBufferAllocator( void );
    ~crBufferAllocator( void );
    virtual bool    Create( const size_t in_size, const VkBufferUsageFlags in_usage, const VkMemoryPropertyFlags in_memoryProperty ) override;
    virtual void    Destroy( void ) override;
    ///
    crSubBuffer*    Alloc( const size_t in_size, const size_t in_alignment );

    ///
    void            Free( struct crSubBuffer* in_buffer );
private:
    block_t*    m_treeRoot;

    ///Insert into the tree based on size.
    void InsertToTree( block_t* in_node );

    ///
    void RemoveFromTree( block_t* in_node );

    /// 
    block_t* FindBestFit( const size_t in_requiredSize, const size_t in_alignment );
};

class crBufferRing : public crBuffer
{
public:
    crBufferRing( void );
    ~crBufferRing( void );
    virtual bool    Create( const size_t in_size, const VkBufferUsageFlags in_usage, const VkMemoryPropertyFlags in_memoryProperty ) override;
    virtual void    Destroy( void ) override;
    crSubBuffer*    Alloc( const size_t in_size, const size_t in_alignment );

private:
    uintptr_t       m_offset;
    uint64_t        m_operationCount;
    VkSemaphore     m_operationSemaphore;
};


#endif //!__BUFFER_HPP__