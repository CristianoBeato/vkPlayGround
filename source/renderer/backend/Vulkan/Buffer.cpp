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

bool crBuffer::Create( const size_t in_size, const VkBufferUsageFlags in_usage, const VkMemoryPropertyFlags in_memoryProperty )
{   
    VkResult result = VK_SUCCESS; 
    uniqueQueue_t queues;
    VkMemoryRequirements requirements;

    auto device = crContext::Get()->Device();
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
    m_usage = in_usage;

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
    result = vkCreateBuffer( *device, &bufferCI, k_allocationCallbacks, &m_buffer );
    if ( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkCreateBuffer %s\n", VulkanErrorString( result ) );
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
    allocInfo.memoryTypeIndex = device->FindMemoryType( requirements.memoryTypeBits, in_memoryProperty ); 
    result = vkAllocateMemory( *device, &allocInfo, nullptr, &m_memory );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkAllocateMemory %s\n", VulkanErrorString( result ) );
        return false;
    }

    result = vkBindBufferMemory( *device, m_buffer, m_memory, 0 );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkBindBufferMemory %s\n", VulkanErrorString( result ) );
        return false;
    }

    return true;
}

void crBuffer::Destroy(void)
{
    auto device = crContext::Get()->Device();
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

    m_stage = VK_PIPELINE_STAGE_2_NONE;
    m_access = VK_ACCESS_2_NONE;
}

void *crBuffer::Map(void)
{
    void* data = nullptr;
    auto device = crContext::Get()->Device();
    auto result = vkMapMemory( *device, m_memory, 0, VK_WHOLE_SIZE, 0, &data );
    if( result != VK_SUCCESS )
    {
        crConsole::Error( "crBuffer::Create::vkBindBufferMemory %s\n", VulkanErrorString( result ) );
        return nullptr;
    }

    return data;
}

void crBuffer::Unmap(void)
{
    auto device = crContext::Get()->Device();
    vkUnmapMemory( *device, m_memory );
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

bool crBufferAllocator::Create(const size_t in_size, const VkBufferUsageFlags in_usage, const VkMemoryPropertyFlags in_memoryProperty)
{
    if ( !crBuffer::Create( in_size, in_usage, in_memoryProperty ) )
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

bool crBufferRing::Create(const size_t in_size, const VkBufferUsageFlags in_usage, const VkMemoryPropertyFlags in_memoryProperty)
{
    auto device = crContext::Get()->Device();
    if( !crBuffer::Create( in_size, in_usage, in_memoryProperty ) )
        return false;

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
        /* code */
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
