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

#include "Backend.hpp"

crBackend::crBackend( void ) : m_swapchain( nullptr )
{
}

crBackend::~crBackend( void )
{
}

crBackend *crBackend::Get(void)
{
    static crBackend gBackend = crBackend();
    return &gBackend;
}

void crBackend::StartUp(void)
{
    m_swapchain = new crSwapchain();
    if( !m_swapchain->Create( 800, 600 ) )
        throw crException( "Failed to initialize SwapChain" );

    m_defaultFB = new crFramebuffer();
    m_defaultFB->Create( { 800,  600, 1, VK_FORMAT_R8G8B8A8_SNORM, VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT } );
}

void crBackend::ShutDown(void)
{
    if ( m_defaultFB != nullptr )
    {
        m_defaultFB->Destroy();
        delete m_defaultFB;
        m_defaultFB = nullptr;
    }
    

    if ( m_swapchain != nullptr )
    {
        m_swapchain->Destroy();
        delete m_swapchain;
        m_swapchain = nullptr;
    }
}

void crBackend::BeginFrame(void)
{
    m_swapchain->BeginFrame();
    m_defaultFB->Bind();
}

void crBackend::SwapBuffers(void)
{
    VkResult result = VK_SUCCESS;

    /// release frame buffer
    m_defaultFB->Unbind();

    /// clea presnet image
    VkRenderingAttachmentInfo colorAttachment{};
    colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO; 
    colorAttachment.pNext = nullptr; 
    colorAttachment.imageView = m_swapchain->GetImage()->view;
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.resolveMode = VK_RESOLVE_MODE_NONE; 
    colorAttachment.resolveImageView = nullptr;
    colorAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    // clear to black
    colorAttachment.clearValue.color.float32[0] = 0.1f; 
    colorAttachment.clearValue.color.float32[1] = 0.2f; 
    colorAttachment.clearValue.color.float32[2] = 0.6f; 
    colorAttachment.clearValue.color.float32[3] = 1.0f; 

    VkRenderingInfo renderingInfo{};
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.pNext = nullptr;
    renderingInfo.flags = 0;
    renderingInfo.renderArea = { 0, 0, 800, 600 };
    renderingInfo.layerCount = 1;
    renderingInfo.viewMask = 0;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = nullptr;
    renderingInfo.pStencilAttachment = nullptr;
    vkCmdBeginRendering( m_swapchain->CommandBuffer(), &renderingInfo );

    ///
    ///
    /// End frame rendering
    vkCmdEndRendering( m_swapchain->CommandBuffer() );

    m_defaultFB->BlitColorAttachament( { 0, 0, 0, 10, 10, 0, 800, 600, 1, 790, 590, 1, m_swapchain->GetImage() } );

    m_swapchain->SwapBuffers();
}

void crBackend::DrawView(void)
{
    // Draw all non interative surfaces

    // draw all light interations
    LighPass();

    // draw all aditive surfaces 
    AdditivePass();   
}

void crBackend::DrawSurface( const crSurface *in_surface )
{
    auto cmd = m_swapchain->CommandBuffer();
    assert( in_surface != nullptr );
    uint32_t        elements;
    VkDeviceSize    offsets[VERTEX_BINDING_COUNT]{};
    VkDeviceSize    sizes[VERTEX_BINDING_COUNT]{};
    VkDeviceSize    strides[VERTEX_BINDING_COUNT]{};
    VkBuffer        vertexesBuffers[VERTEX_BINDING_COUNT]{ nullptr, nullptr };
    cache_t*        indexCache = in_surface->IndexesCache();
    cache_t*        postionCache = in_surface->VertexPosition();
    cache_t*        normalCache = in_surface->VertexNormals();
    cache_t*        normalCache = in_surface->VertexNormals();
    cache_t*        normalCache = in_surface->VertexNormals();
    

    /// bind index buffer
    vkCmdBindIndexBuffer( cmd, indexCache->buffer->buffer, indexCache->offset, VK_INDEX_TYPE_UINT16 );
 
    /// Vertex Position binding 0
    offsets[VERTEX_BINDING_POSI] = postionCache->offset;
    sizes[VERTEX_BINDING_POSI] = postionCache->size;
    strides[VERTEX_BINDING_POSI] = sizeof( VertPos_t );

    /// Vertex Normal binding 1
    offsets[VERTEX_BINDING_NORM] = normalCache->offset;
    sizes[VERTEX_BINDING_NORM] = normalCache->size;
    strides[VERTEX_BINDING_NORM] = sizeof( VertNor_t );

    /// Vertex Skinning binding 2
    offsets[VERTEX_BINDING_SKIN] = normalCache->offset;
    sizes[VERTEX_BINDING_SKIN] = normalCache->size;
    strides[VERTEX_BINDING_SKIN] = sizeof( VertSkin_t );

    /// Vertex Materil ID binding 4 
    offsets[VERTEX_BINDING_SKIN] = normalCache->offset;
    sizes[VERTEX_BINDING_SKIN] = normalCache->size;
    strides[VERTEX_BINDING_SKIN] = sizeof( VertMat_t );

    ///
    vkCmdBindVertexBuffers2( cmd, 0, 2, vertexesBuffers, offsets, sizes, strides );

    /// submit draw call
    vkCmdDrawIndexed( m_swapchain->CommandBuffer(), in_surface->Indexes(), 1, 0, in_surface->BaseVertex(), 0 );
}

void crBackend::LighPass(void)
{
    FillGeometryBuffer();
    LighBlending();
}

void crBackend::OpaqueGeometryPass(void)
{
    /// render all opaque geometry
    for ( crView* view = m_viewChain; view != nullptr; view = view->Next() )
    {
        for ( crGeometry* geometry = view->Geometry(); geometry != nullptr; geometry = geometry->Next() )
        {
            crMaterial* material = geometry->Material();
            crMesh*     mesh = geometry->Mesh();
            /// ignore non opaque material
            if ( !material->Opaque() )
                continue;

            
        }
    }
}

void crBackend::TransparenGeometryPass(void)
{
}

void crBackend::FillGeometryBuffer(void)
{
    /// bind to draw geometry buffer
    m_geometrFB->Bind();

    /// draw all opaque textures first
    OpaqueGeometryPass();

    /// now draw all 
    TransparenGeometryPass();
    
    /// finish geometry rendering 
    m_geometrFB->Unbind();
}

void crBackend::LighBlending(void)
{
}

void crBackend::Translucent(void)
{
}

void crBackend::AdditivePass(void)
{
}
