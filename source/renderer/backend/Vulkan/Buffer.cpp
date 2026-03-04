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

#include "Buffer.hpp" 

// include VMA Here, since, his include of other headers 
// can lead to naming polution
#include <vk_mem_alloc.h>

static const VkBufferUsageFlags k_USAGE_TABLE[] = 
{
    0, // BUFFER_TYPE_NONE
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // BUFFER_STAGING_SOURCE
    VK_BUFFER_USAGE_TRANSFER_DST_BIT, // BUFFER_STAGING_DESTINATION
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // BUFFER_ELEMENT_ARRAY
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // BUFFER_VERTEX_ARRAY
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, // BUFFER_SHADER_STORAGE
};

#if USE_VMA_BUFFERS

static const VmaMemoryUsage k_MEMORY_USAGE_TABLE[] = 
{
    VMA_MEMORY_USAGE_UNKNOWN,               // BUFFER_TYPE_NONE
    VMA_MEMORY_USAGE_AUTO_PREFER_HOST,      // BUFFER_STAGING_SOURCE
    VMA_MEMORY_USAGE_AUTO_PREFER_HOST,      // BUFFER_STAGING_DESTINATION
    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,    // BUFFER_ELEMENT_ARRAY
    VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,    // BUFFER_VERTEX_ARRAY
    VMA_MEMORY_USAGE_AUTO_PREFER_HOST       // BUFFER_SHADER_STORAGE
};

static const VmaAllocationCreateFlags k_ALLOCATION_FLAGS[] = 
{
    0, // BUFFER_TYPE_NONE
    VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT, // BUFFER_STAGING_SOURCE
    VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT, // BUFFER_STAGING_DESTINATION
    0, // BUFFER_ELEMENT_ARRAY
    0, // BUFFER_VERTEX_ARRAY
    VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT // BUFFER_SHADER_STORAGE
};

#else

const VkMemoryPropertyFlags k_MEMORY_PROPERTY_TABLE[] = 
{
    0, // BUFFER_TYPE_NONE
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // BUFFER_STAGING_SOURCE
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // BUFFER_STAGING_DESTINATION
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, // BUFFER_ELEMENT_ARRAY
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, // BUFFER_VERTEX_ARRAY
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // BUFFER_SHADER_STORAGE
};

#endif //!#if USE_VMA_BUFFERS

/*
===============================================================================================
crBuffer 
===============================================================================================
*/
crBuffer::crBuffer( void ) : 
    m_size( 0 ),
    m_usage( 0 ),
    m_stage( VK_PIPELINE_STAGE_2_NONE ),
    m_access( VK_ACCESS_2_NONE ),
    m_buffer( nullptr ),
    m_memory(  nullptr )
{
}

crBuffer::~crBuffer( void )
{
    Destroy();
}

bool crBuffer::Create( const size_t in_size, const buffer_type_e in_type )
{   
    VkResult result = VK_SUCCESS; 
    uniqueQueue_t queues;
    VkMemoryRequirements requirements;

    auto context = crContext::Get();
    auto device = context->Device();
    auto graphicQ = device->GraphicQueue();
    auto transferQ = device->TransferQueue();
    auto computeQ = device->TransferQueue();
    
    /// If we have a transfer family, we can use to copy operations
    if( transferQ )
        queues.Append( transferQ->Family() );
    
    /// If we have a compute family, make buffer ready to be used in compute operatiosn
    if ( computeQ )
        queues.Append( computeQ->Family() );

    m_size = in_size;
    m_usage = k_USAGE_TABLE[in_type];

    ///
    ///
    /// Create the buffer handler
    VkBufferCreateInfo  bufferCI{};
    bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCI.pNext = nullptr;
    bufferCI.flags = 0;
    bufferCI.size = m_size;
    bufferCI.usage = m_usage;
    bufferCI.sharingMode = queues.SharingMode();
    bufferCI.queueFamilyIndexCount = queues.count;
    bufferCI.pQueueFamilyIndices = queues.families;

#if USE_VMA_BUFFERS
    /// Vulkan Memory Allocator allocation structures
    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = k_MEMORY_USAGE_TABLE[in_type];
    allocInfo.flags = k_ALLOCATION_FLAGS[in_type];    
    vmaCreateBuffer( context->VMAAllocator(), &bufferCI, &allocInfo, &m_buffer, &m_memory, nullptr);
#else
    result = vkCreateBuffer( *device, &bufferCI, k_allocationCallbacks, &m_buffer );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkCreateBuffer %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    // Aquire buffer memory requirements
    vkGetBufferMemoryRequirements( *device, m_buffer, &requirements );

    ///
    ///
    /// Allocate buffer Memory
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = requirements.size;
    // We need VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT (to map) and 
    // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT (so we don't need to manually flush)
    allocInfo.memoryTypeIndex = device->FindMemoryType( requirements.memoryTypeBits, k_MEMORY_PROPERTY_TABLE[in_type] ); 
    result = vkAllocateMemory( *device, &allocInfo, k_allocationCallbacks, &m_memory );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkAllocateMemory %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }

    VkBindBufferMemoryInfo bindInfos{};
    bindInfos.sType = VK_STRUCTURE_TYPE_BIND_BUFFER_MEMORY_INFO;
    bindInfos.pNext = nullptr;
    bindInfos.buffer = m_buffer;
    bindInfos.memory = m_memory;
    bindInfos.memoryOffset = 0;
    result = vkBindBufferMemory2( *device, 1, &bindInfos );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkBindBufferMemory %s\n", VulkanErrorString( result ).c_str() );
        return false;
    }
#endif // !USE_VMA_BUFFERS
    return true;
}

void crBuffer::Destroy(void)
{
    auto context = crContext::Get();
    auto device = context->Device();

#if USE_VMA_BUFFERS
    if( m_buffer != nullptr )
    {
        vmaDestroyBuffer( context->VMAAllocator(), m_buffer, m_memory );
        m_buffer = nullptr;
        m_memory = nullptr;
    }
#else
    if ( m_memory != nullptr )
    {
        vkFreeMemory( *device, m_memory, k_allocationCallbacks );
        m_memory = nullptr;
    }
    
    if ( m_buffer != nullptr )
    {
        vkDestroyBuffer( *device, m_buffer, k_allocationCallbacks );
        m_buffer = nullptr;
    }
#endif // USE_VMA_BUFFERS
    
    m_stage = VK_PIPELINE_STAGE_2_NONE;
    m_access = VK_ACCESS_2_NONE;
}

void *crBuffer::Map(void)
{
    void* data = nullptr;
    
#if USE_VMA_BUFFERS
    auto context = crContext::Get();
    vmaMapMemory( context->VMAAllocator(), m_memory, &data );
#else

    auto device = crContext::Get()->Device();
    auto result = vkMapMemory( *device, m_memory, 0, VK_WHOLE_SIZE, 0, &data );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkBindBufferMemory %s\n", VulkanErrorString( result ).c_str() );
        return nullptr;
    }

#endif //USE_VMA_BUFFERS
    return data;
}

void crBuffer::Unmap(void)
{
#if USE_VMA_BUFFERS
    auto context = crContext::Get();
    vmaUnmapMemory( context->VMAAllocator(), m_memory );
#else
    auto device = crContext::Get()->Device();
    vkUnmapMemory( *device, m_memory );
#endif // USE_VMA_BUFFERS
}

void crBuffer::State(const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask)
{
    assert( m_buffer != nullptr );
    VkBufferMemoryBarrier2 destinationBarrier{}; 
    destinationBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
    destinationBarrier.pNext = nullptr;
    destinationBarrier.srcStageMask = m_stage;
    destinationBarrier.srcAccessMask = m_access;
    destinationBarrier.dstStageMask = in_stageMask;
    destinationBarrier.dstAccessMask = in_accessMask;
    destinationBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.buffer = m_buffer;

    // update whole buffer, no region change
    destinationBarrier.offset = 0;  
    destinationBarrier.size = VK_WHOLE_SIZE;

    /// perform a state transition to destination
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.pNext = nullptr;
    dependencyInfo.dependencyFlags = 0;
    dependencyInfo.memoryBarrierCount = 0;
    dependencyInfo.pMemoryBarriers = nullptr;
    dependencyInfo.bufferMemoryBarrierCount = 1;
    dependencyInfo.pBufferMemoryBarriers = &destinationBarrier;
    dependencyInfo.imageMemoryBarrierCount = 0;
    dependencyInfo.pImageMemoryBarriers = nullptr;
    vkCmdPipelineBarrier2( in_commandBuffer, &dependencyInfo );

    // update buffer stage
    m_stage = in_stageMask;
    m_access = in_accessMask;
}

/*
===============================================================================================
crSubBuffer 
===============================================================================================
*/
crSubBuffer::crSubBuffer( void ) : 
    m_stage( VK_PIPELINE_STAGE_2_NONE ),
    m_access( VK_ACCESS_2_NONE ),
    m_size( 0 ),
    m_offset( 0 ),
    m_buffer( nullptr )
{
}

crSubBuffer::crSubBuffer(const VkBuffer in_buffer, const size_t in_size, const uintptr_t in_offset)
{
}

crSubBuffer::~crSubBuffer( void )
{
    m_stage = VK_PIPELINE_STAGE_2_NONE;
    m_access = VK_ACCESS_2_NONE;
    m_size = 0;
    m_offset = 0;
    m_buffer = nullptr;
}

void crSubBuffer::State(const VkCommandBuffer in_commandBuffer, const VkPipelineStageFlags2 in_stageMask, const VkAccessFlags2 in_accessMask)
{
    assert( m_buffer != nullptr );

    VkBufferMemoryBarrier2 destinationBarrier{}; 
    destinationBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
    destinationBarrier.pNext = nullptr;
    destinationBarrier.srcStageMask = m_stage;
    destinationBarrier.srcAccessMask = m_access;
    destinationBarrier.dstStageMask = in_stageMask;
    destinationBarrier.dstAccessMask = in_accessMask;
    destinationBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destinationBarrier.buffer = m_buffer;

    // update whole buffer, no region change
    destinationBarrier.offset = m_offset;
    destinationBarrier.size = m_size;

    /// perform a state transition to destination
    VkDependencyInfo dependencyInfo{};
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.pNext = nullptr;
    dependencyInfo.dependencyFlags = 0;
    dependencyInfo.memoryBarrierCount = 0;
    dependencyInfo.pMemoryBarriers = nullptr;
    dependencyInfo.bufferMemoryBarrierCount = 1;
    dependencyInfo.pBufferMemoryBarriers = &destinationBarrier;
    dependencyInfo.imageMemoryBarrierCount = 0;
    dependencyInfo.pImageMemoryBarriers = nullptr;
    vkCmdPipelineBarrier2( in_commandBuffer, &dependencyInfo );

    // update buffer stage
    m_stage = in_stageMask;
    m_access = in_accessMask;
}


/*
===============================================================================================
crBufferAllocator 
===============================================================================================
*/
crBufferAllocator::crBufferAllocator( void ) : crBuffer()
{
}

crBufferAllocator::~crBufferAllocator( void )
{
    Destroy();
}

bool crBufferAllocator::Create(const size_t in_size, const buffer_type_e in_type )
{
    if ( !crBuffer::Create( in_size, in_type ) )
        return false;

    return true;
}

void crBufferAllocator::Destroy(void)
{
}

crSubBuffer* crBufferAllocator::Alloc( const size_t in_size, const size_t in_alignment )
{
    // Finding the best (best-fit) block
    // We pass the size + alignment to ensure the block can accommodate the padding.
    block_t* target = FindBestFit( in_size, in_alignment ); 

    if (!target) 
    {
        crConsole::Error( "crBufferAllocator::Alloc::Error Buffer Out of Memory\n" );
        return nullptr;
    }

    // Calculate the aligned offset within this block (offset + (align-1)) & ~(align-1).
    uintptr_t alignedOffset = (target->offset + ( in_alignment - 1)) & ~( in_alignment - 1);
    size_t padding = alignedOffset - target->offset;
    size_t totalNeeded = in_size + padding;

    // Remove the tree block before modifying it.
    RemoveFromTree(target);
    target->free = false;

    // Check if there is enough space left to create a new free block (Split).
    // We define a "Min Split Size" (e.g., 64 bytes) to avoid unnecessary tiny blocks.
    if ( target->size > totalNeeded + MIN_BUFFER_SIZE ) 
    {
        block_t* nextBlock = new block_t(); //TODO: Ideally, use a pool here.
        nextBlock->offset = target->offset + totalNeeded;
        nextBlock->size   = target->size - totalNeeded;
        nextBlock->free   = true;

        // Update Physical List (Insert between 'target' and 'target->nextPhys')
        nextBlock->prevPhys = target;
        nextBlock->nextPhys = target->nextPhys;
        if (target->nextPhys) 
            target->nextPhys->prevPhys = nextBlock;
        
        target->nextPhys = nextBlock;

        // Adjust the size of the current block.
        target->size = totalNeeded;

        // Inserir o novo bloco remanescente na árvore
        InsertToTree(nextBlock);
    }

    // 5. Configurar o sub-buffer de saída
    // Aqui você usaria os métodos da sua classe crSubBuffer
    // Ex: out_sub.Set(target->Handle(), alignedOffset, size);
    
    return nullptr;
}

void crBufferAllocator::Free( crSubBuffer *in_buffer )
{
    block_t* block = in_buffer->Block();
    block->free = true;

    // Try to merge with the NEXT physical block.
    if ( block->nextPhys && block->nextPhys->free )
    {
        block_t* neighbor = block->nextPhys;

        RemoveFromTree( neighbor );

        block->size += neighbor->size;
        block->nextPhys = neighbor->nextPhys;
        if (neighbor->nextPhys) neighbor->nextPhys->prevPhys = block;
        
        delete neighbor; // Ou retorne para um pool de memória
    }

    // Try to merge with the PREVIOUS physical block.
    if ( block->prevPhys && block->prevPhys->free )
    {
        block_t* neighbor = block->prevPhys;
        
        // Remove neighbor from tree
        RemoveFromTree(neighbor);

        neighbor->size += block->size;
        neighbor->nextPhys = block->nextPhys;
        if ( block->nextPhys ) 
            block->nextPhys->prevPhys = neighbor;
        
        delete block; 
        block = neighbor; // O bloco atual agora é o vizinho fundido
    }

    // Reinsert the resulting block (whether it's fused or not) into the tree.
    InsertToTree( block );
}

void crBufferAllocator::InsertToTree( block_t *in_node )
{
    block_t* parent = nullptr;
    block_t** current = &m_treeRoot;

    while ( *current )
    {
        parent = *current;
        /// Sort by size; if equal, use offset to maintain determinism.
        if ( in_node->size < (*current)->size )
            current = &((*current)->left );
        else
            current = &((*current)->right );
    }

    in_node->parent = parent;
    *current = in_node;
}

void crBufferAllocator::RemoveFromTree( block_t *in_node )
{
    block_t* replacement = nullptr;

    if (!in_node) 
        return;

    if ( !in_node->left )
    {
        // No right child (or no children at all)
        replacement = in_node->right;
    }
    else if (!in_node->right)
    {
        // No left child
        replacement = in_node->left;
    }
    else
    {
        // Two children - Find the successor ( smaller in the right subtree )
        block_t* successor = in_node->right;
        while (successor->left) 
        {
            successor = successor->left;
        }

        // If the successor is not a direct descendant, reorganize the successor family tree.
        if (successor->parent != in_node) 
        {
            // The parent of the successor adopts the successor's right child.
            successor->parent->left = successor->right;
            if (successor->right) {
                successor->right->parent = successor->parent;
            }
            // The successor assumes the right child of the removed node.
            successor->right = in_node->right;
            successor->right->parent = successor;
        }

        // The successor takes over the left child of the removed node.
        replacement = successor;
        replacement->left = in_node->left;
        replacement->left->parent = replacement;
    }

    // Update the parent of the removed node to point to the replacement.
    if ( !in_node->parent )
        m_treeRoot = replacement;
    else if (in_node->parent->left == in_node)
        in_node->parent->left = replacement;
    else
        in_node->parent->right = replacement;

    if (replacement)
        replacement->parent = in_node->parent;

    // Clean the nodes pointers, removing them, for safety.
    in_node->left = nullptr;
    in_node->right = nullptr;
    in_node->parent = nullptr;
}

block_t *crBufferAllocator::FindBestFit( const size_t in_requiredSize, const size_t in_alignment )
{
    block_t* current = m_treeRoot;
    block_t* best = nullptr;

    while ( current )
    {
        size_t padding = ( in_alignment - ( current->offset % in_alignment ) ) % in_alignment;
        if ( current->size >= ( in_requiredSize + padding ) )
        {
            best = current; // Candidate found, but we tried an even shorter one.
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return best;
}

/*
===============================================================================================
crBufferRing 
===============================================================================================
*/
crBufferRing::crBufferRing( void )
{
}

crBufferRing::~crBufferRing( void )
{
    Destroy();
}

bool crBufferRing::Create(const size_t in_size, const buffer_type_e in_type )
{
    auto device = crContext::Get()->Device();
    if( !crBuffer::Create( in_size, in_type ) )
        return false;

    // Get Buffer Map
    m_map = crBuffer::Map();

    VkSemaphoreTypeCreateInfo timelineCreateInfo{};
    timelineCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    timelineCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    timelineCreateInfo.initialValue = 0; 

    VkSemaphoreCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.pNext = &timelineCreateInfo;
    auto result = vkCreateSemaphore( *device, &createInfo, k_allocationCallbacks, &m_operationSemaphore );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crBufferRing::vkCreateSemaphore failed\n%s\n", VulkanErrorString( result ).c_str() );
        return false;
    }
    
    return true;
}

void crBufferRing::Destroy(void)
{
    auto device = crContext::Get()->Device();
    if ( m_operationSemaphore != nullptr )
    {
        vkDestroySemaphore(*device, m_operationSemaphore, k_allocationCallbacks );
        m_operationSemaphore = nullptr;
    }
    
    // release buffer
    crBuffer::Unmap();
    crBuffer::Destroy();
}

crSubBuffer *crBufferRing::Alloc( const size_t in_size, const size_t in_alignment )
{
    crSubBuffer* subBuffer = nullptr;

    // Adjust the current offset to the next multiple of 'in_alignment'
    size_t aligned_offset = _align( m_offset, in_alignment ); 
    size_t aligned_size = _align( in_size, in_alignment );

    /// Overflow/wrap-around check
    if ( ( aligned_offset + aligned_size ) > Size() )
    {
        m_offset = 0;
        aligned_offset = _align( m_offset, in_alignment ); 
    }
    
    subBuffer = new crSubBuffer( Buffer(), aligned_size, aligned_offset );
    
    return subBuffer;
}
