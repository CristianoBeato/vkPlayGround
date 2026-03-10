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

#include "ModelTerrain.hpp"

// this is to make chunks overlap by about a millimeter, to avoid cracks.
constexpr float EXTRA_BOX_SIZE = 1e-3f;	

/*
===============================================================================================
crVertexInfo 
Structure for storing morphable vertex mesh info.
===============================================================================================
*/
class crVertexInfo 
{
public:
	struct vertex 
    {
		int16_t	x[3];
		int16_t y_delta;	// delta, to get to morphed y

		void	Read( const crFile* in_source )
		{
            in_source->ReadInt16( x, 3 );
            in_source->ReadInt16( &y_delta );

//			// xxxxxx TEST QUANTIZATION xxxxxxx
//			// lose the bottom 8 bits by rounding
//			y_delta = (Sint16) iclamp((y_delta + 128) & ~0x0FF, -32768, 32767);
//			// xxxxxx TEST QUANTIZATION xxxxxxx
		}
	};

	crVertexInfo( void );
	~crVertexInfo( void );

    void	Read( const crFile* in_source );

	// Return the data bytes used by this object.
	inline size_t	GetDataSize( void ) const
	{
		return sizeof(*this) + m_vertexCount * sizeof( vertex ) + m_indexCount * sizeof( uint16_t );
	}

private:
    uint32_t    m_triangleCount;	// for statistics.
    uint32_t    m_vertexCount;
	uint32_t    m_indexCount;
	vertex*     m_vertices;
	uint16_t*   m_indices;
};

crVertexInfo::crVertexInfo( void ) : 
    m_vertexCount(0),
    m_indexCount(0),
    m_triangleCount(0),
    m_vertices( nullptr ),
    m_indices( nullptr )
{
}

crVertexInfo::~crVertexInfo(void)
{
    if ( m_vertices ) 
    {
        MemFree( m_vertices );
        m_vertices = nullptr;
    }

    if ( m_indices ) 
    {
        MemFree( m_indices );
        m_indices = nullptr;
    }
}

// Read vert info from the given file.
void crVertexInfo::Read( const crFile* in_source )
{
    /// retrieve the vertex count
    in_source->ReadUint32( &m_vertexCount );
	m_vertices = MemAlloc<vertex>( m_vertexCount );
	for (int i = 0; i < m_vertexCount; i++)
    {
		m_vertices[i].Read( in_source );
	}
		
	// Load indices.
    in_source->ReadUint32( &m_indexCount );
	if ( m_indexCount > 0) 
    {
		m_indices = MemAlloc<uint16_t>( m_indexCount );
        in_source->ReadUint16( m_indices, m_indexCount );
	} 
    else 
		m_indices = nullptr;

	// Load the real triangle count, for computing statistics.
    in_source->ReadUint32( &m_triangleCount );
}

/*
===============================================================================================
crTextureQuadTree 
Manages a disk-based texture quadtree.
===============================================================================================
*/
class crTextureQuadTree
{
public:
	crTextureQuadTree(const char* filename);
	~crTextureQuadTree( void );
	const bool	IsValid( void ) const { return m_source != NULL; }
	int	get_depth() const { return m_depth; }
	int	get_tile_size() const { return m_tileSize; }

	unsigned int	get_texture_id(int level, int col, int row) const;
	image::rgb*	load_image(int level, int col, int row) const;

	// Static utility functions.
	static bool	tqt::is_tqt_file(const char* filename);
	static int	tqt::node_count(int depth);
	static int	tqt::node_index(int level, int col, int row);
	
private:
	array<unsigned int>	m_toc;
	int		m_depth;
	int		m_tileSize;
	crFile*	m_source;
};


/*
===============================================================================================
crLodChunkTree 
Use this class as the UI to a chunked-LOD object.
!!! turn this into an interface class and get the data into the .cpp file !!!
===============================================================================================
*/
class crLodChunkTree 
{
public:
	crLodChunkTree( const crFile* in_src, const crTextureQuadTree* texture_quadtree );
	~crLodChunkTree( void );

	// External interface.
	void	SetParameters( const float max_pixel_error, float max_texel_size, float screen_width, float horizontal_FOV_degrees);
	void	Update( const crVec3f& viewpoint );
	int	    Render( const view_state& v, render_options opt );
	crBounds	GetBoundingBox( void );

	// Internal interfaces, used by our contained chunks.
	uint16_t    ComputeLOD( const crVec3f& in_center, const crVec3f& in_extent, const crVec3f& in_viewPoint ) const;
	int         ComputeTextureLOD( const crVec3f& in_center, const crVec3f& in_extent, const crVec3f& in_viewPoint) const;

	// Call this to enable/disable loading in a background thread.
	//void	SetUseLoaderThread(bool use);

private:
    int	            			m_chunksAllocated;
    int	            			m_treeDepth;	// from chunk data.
    float	        			m_errorLODmax;	// from chunk data.
    float	        			m_distanceLODmax;	// computed from chunk data params and set_parameters() inputs --> controls displayed LOD level.
    float	        			m_textureDistanceLODmax;	// computed from texture quadtree params and set_parameters() --> controls when textures kick in.
    float	        			m_verticalScale;	// from chunk data; displayed_height = y_data * m_vertical_scale.
    float	        			m_baseChunkDimension;	// x/z size of highest LOD chunks.
    int	            			m_chunkCount;
    crLodChunk*	    			m_chunks;
	crLodChunk**				m_chunkTable;
	const crTextureQuadTree*	m_textureQuadtree;
	crModelTerrain*				m_loader;
};

crLodChunkTree::crLodChunkTree(const crFile *in_src, const crTextureQuadTree *texture_quadtree)
{
}

crLodChunkTree::~crLodChunkTree(void)
{
}

/*
===============================================================================================
crLodChunkTree 
Vertex/mesh data for a chunk.  
Can get paged in/out on demand.
===============================================================================================
*/
class crLodChunkData 
{
public:
	crLodChunkData( const crFile* in_file );
	~crLodChunkData( void );
	
	int	Render( const crLodChunkTree& c, 
                const crLodChunk& chunk, 
                const view_state& v, 
                cull::result_info cull_info, 
                render_options opt,
                const crBounds &in_bounds );

    // Return data bytes used by this object.
	inline size_t	GetDataSize( void ) const
	{
		return sizeof(*this) + m_verts.GetDataSize();
	}

private:
    crVertexInfo	m_verts;	// vertex and mesh info; vertex array w/ morph targets, indices
    //	lod_chunk_data* m_next_data;
	//	lod_chunk_data* m_prev_data;
};

// Constructor.  Read our data & set our texture id.
crLodChunkData::crLodChunkData( const crFile* in_source )
{
    // Load the main chunk data.
    m_verts.Read( in_source );
}

crLodChunkData::~crLodChunkData(void)
{
}

int crLodChunkData::Render( const crLodChunkTree &c, const crLodChunk &chunk, const view_state &v, cull::result_info cull_info, render_options opt, const crBounds &in_bounds )
{
    return 0;
}

class crLodChunk 
{
public:

	~crLodChunk( void )
	{
		if ( m_data != nullptr ) 
        {
			delete m_data;
			m_data = 0;
		}

		ReleaseTexture();
	}

	void	ReleaseTexture( void )
	{
		if (m_texture_id) 
        {
			lod_tile_freelist::free_texture(m_texture_id);
			m_texture_id = 0;
		}
	}

	void	Clear( void );
	void	Update( crLodChunkTree* tree, const crVec3f& viewpoint);
	void	UpdateTexture( crLodChunkTree* tree, const crVec3f& viewpoint);
	void	DoSplit( crLodChunkTree* tree, const crVec3f& viewpoint);
	bool	CanSplit( crLodChunkTree* tree);	// return true if this chunk can split.  Also, request the necessary data for future, if not.
	void	UnloadData();
	void	WarmUpData( crLodChunkTree* in_tree, const float in_priority );
	void	RequestUnloadSubtree( crLodChunkTree* in_tree);
	void	RequestUnloadTextures( crLodChunkTree* in_tree);
	int     Render( const crLodChunkTree& in_c, const view_state& v, cull::result_info cull_info, render_options opt, bool texture_bound );
	void	Read( const crFile* in_source, int in_recursionCount, crLodChunkTree* in_tree, const bool in_vertDataAtEnd );
	void	LookupNeighbors( crLodChunkTree* in_tree );

	// Utilities.

	inline bool	HasResidentData( void ) const { return m_data != nullptr; }

	inline bool	HasChildren( void ) const { return m_children[0] != nullptr; }

	inline uint8_t	GetLevel( void ) const { return m_level; }

	void	ComputeBoundingBox( const crLodChunkTree& tree, crVec3f* box_center, crVec3f* box_extent );

private:
    crLodChunk*	m_parent;
	crLodChunk*	m_children[4];

	union 
    {
		int	m_label;
		crLodChunk*	m_chunk;
	} m_neighbor[4];

    /// true if this node should be rendered by descendents.  @@ pack this somewhere as a bitflag.  LSB of lod?
	bool        m_split;	
	uint8_t     m_level;

    // LOD of this chunk.  high byte never changes; low byte is the morph parameter.
	uint16_t    m_lod;

	// Chunk "address" (its position in the quadtree).
	uint16_t	m_x;
    uint16_t    m_z;

	// Vertical bounds, for constructing bounding box.
	int16_t	    m_minY;
    int16_t     m_maxY;

	int32_t     m_dataFilePosition;
    // OpenGL texture id for this chunk's texture map.
	uint32_t    m_texture_id;		
	crLodChunkData* m_data;
};

void crLodChunk::ComputeBoundingBox( const crLodChunkTree& tree, crVec3f* box_center, crVec3f* box_extent )
{
		float	level_factor = (1 << (tree.m_treeDepth - 1 - m_level));

		box_center->y = ( m_maxY + m_minY ) * 0.5f * tree.m_verticalScale;
		box_extent->y = ( m_maxY - m_minY ) * 0.5f * tree.m_verticalScale;
			     
		box_center->x = ( m_x + 0.5f ) * level_factor * tree.m_baseChunkDimension;
		box_center->z = ( m_z + 0.5f ) * level_factor * tree.m_baseChunkDimension;
			     
		
		box_extent->x = level_factor * tree.m_baseChunkDimension * 0.5f + EXTRA_BOX_SIZE;
		box_extent->z = box_extent->x;
	}

crModelTerrain::crModelTerrain( void ) : crModel()
{
}

crModelTerrain::~crModelTerrain( void )
{
}

