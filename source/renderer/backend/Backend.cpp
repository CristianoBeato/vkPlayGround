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

crBackend::crBackend( void ) :
    m_frame( 0 ),
    m_graphicCMD( nullptr ),
    m_geometrFB( nullptr ),
    m_defaultFB( nullptr ),
    m_swapchain( nullptr ),
    m_staticMeshAdditive( nullptr ),
    m_dyanmicMeshAdditive( nullptr )
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

void crBackend::StartUp( void )
{
    m_graphicCMD = new crCommandbuffer();
    if ( !m_graphicCMD->Create() )
        throw crException( "Failed to initialize Graphic Command Buffer" );

    m_swapchain = new crSwapchain();
    if( !m_swapchain->Create( 800, 600, false ) )
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

    if ( m_graphicCMD != nullptr )
    {
        m_graphicCMD->Destroy();
        delete m_graphicCMD;
        m_graphicCMD = nullptr;
    }
}

void crBackend::SetBuffer(void)
{
    /// begin record
    m_graphicCMD->Begin( m_frame );
    m_swapchain->Acquire( m_frame );

    auto presentImages = m_swapchain->Image(); 

#if 0
    /// the image is changed outside of the scope
    m_presentImages[m_currentImage].layout = VK_IMAGE_LAYOUT_UNDEFINED;
    m_presentImages[m_currentImage].stageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    m_presentImages[m_currentImage].accessMask = 0;
    VkImageStateTransition( GetImage(), CommandBuffer(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_NONE );
#else
    presentImages->State( *m_graphicCMD, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_NONE );
#endif

    m_defaultFB->Bind();
}

void crBackend::SwapBuffers(void)
{
    VkResult result = VK_SUCCESS;

    /// release frame buffer
    m_defaultFB->Unbind();

    auto presentImages = m_swapchain->Image(); 

    /// clea presnet image
    VkRenderingAttachmentInfo colorAttachment{};
    colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO; 
    colorAttachment.pNext = nullptr; 
    colorAttachment.imageView = m_swapchain->Image()->View();
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
    vkCmdBeginRendering( *m_graphicCMD, &renderingInfo );

    ///
    ///
    /// End frame rendering
    vkCmdEndRendering( *m_graphicCMD );

    m_defaultFB->BlitColorAttachament( { 0, 0, 0, 10, 10, 0, 800, 600, 1, 790, 590, 1, presentImages } );

    ///
    /// prepare image to be presented
    presentImages->State( *m_graphicCMD, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_2_NONE );

    ///
    /// If image is available to render, begin execute commands 
    m_graphicCMD->Submit( m_swapchain->ImageAvailable() );

    ///
    /// Wen render is done, present to screen
    m_swapchain->Present( m_graphicCMD->SubmitFinishe() );

    m_frame++;
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
    assert( in_surface != nullptr );
    uint32_t        elements;
    VkDeviceSize    offsets[VERTEX_BINDING_COUNT]{};
    VkDeviceSize    sizes[VERTEX_BINDING_COUNT]{};
    VkDeviceSize    strides[VERTEX_BINDING_COUNT]{};
    VkBuffer        buffers[VERTEX_BINDING_COUNT]{};
    crSubBuffer*    indexCache = in_surface->IndexesCache();
    crSubBuffer*    postionCache = in_surface->VertexPosition();
    crSubBuffer*    normalCache = in_surface->VertexNormals();
    crSubBuffer*    weightCache = in_surface->VertexWeights();

    /// bind index buffer
    vkCmdBindIndexBuffer( *m_graphicCMD, indexCache->Handle(), indexCache->Offset(), VK_INDEX_TYPE_UINT16 );
 
    /// Vertex Position binding 0
    offsets[VERTEX_BINDING_POSI] = postionCache->Offset();
    sizes[VERTEX_BINDING_POSI] = postionCache->Size();
    strides[VERTEX_BINDING_POSI] = sizeof( VertPos_t );
    buffers[VERTEX_BINDING_POSI] = postionCache->Handle();

    /// Vertex Normal binding 1
    if ( normalCache != nullptr ) /// we have a vertex cache binding
    {
        /// Vertex Normal binding 1
        buffers[VERTEX_BINDING_NORM] = normalCache->Handle();
        offsets[VERTEX_BINDING_NORM] = normalCache->Offset();
        sizes[VERTEX_BINDING_NORM] = normalCache->Size();
        strides[VERTEX_BINDING_NORM] = sizeof( VertNor_t );
    }
    else
    {
        /// bind vertex buffer as null position in vertex buffer
        buffers[VERTEX_BINDING_NORM] = postionCache->Handle();
        offsets[VERTEX_BINDING_NORM] = 0;
        sizes[VERTEX_BINDING_NORM] = postionCache->Size();
        strides[VERTEX_BINDING_NORM] = sizeof( VertNor_t );
    }
    
    if( weightCache != nullptr )
    {
        buffers[VERTEX_BINDING_SKIN] = normalCache->Handle();
        offsets[VERTEX_BINDING_SKIN] = normalCache->Offset();
        sizes[VERTEX_BINDING_SKIN] = normalCache->Size();
        strides[VERTEX_BINDING_SKIN] = sizeof( VertSkin_t );
    }
    else
    {   
        buffers[VERTEX_BINDING_SKIN] = postionCache->Handle();
        offsets[VERTEX_BINDING_SKIN] = 0;
        sizes[VERTEX_BINDING_SKIN] = postionCache->Size();
        strides[VERTEX_BINDING_SKIN] = sizeof( VertSkin_t );
    }

    ///
    vkCmdBindVertexBuffers2( *m_graphicCMD, 0, VERTEX_BINDING_COUNT, buffers, offsets, sizes, strides );

    /// submit draw call
    vkCmdDrawIndexed( *m_graphicCMD, in_surface->Indexes(), 1, 0, in_surface->BaseVertex(), 0 );
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
