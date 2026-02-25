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

#ifndef __PIPELINE_HPP__
#define __PIPELINE_HPP__

#define GLS_ALPHATEST_MAKE_REF( x ) ( ( (uint64)(x) << GLS_ALPHATEST_FUNC_REF_SHIFT ) & GLS_ALPHATEST_FUNC_REF_BITS )

enum pipeline_state_e : uint64_t
{
    PLS_DEFAULT = 0,

    // one/zero is flipped on src/dest so a gl state of 0 is SRC_ONE,DST_ZERO
    PLS_SRCBLEND_ONE					= 0 << 0,
    PLS_SRCBLEND_ZERO					= 1 << 0,
    PLS_SRCBLEND_DST_COLOR				= 2 << 0,
    PLS_SRCBLEND_ONE_MINUS_DST_COLOR	= 3 << 0,
    PLS_SRCBLEND_SRC_ALPHA				= 4 << 0,
    PLS_SRCBLEND_ONE_MINUS_SRC_ALPHA	= 5 << 0,
    PLS_SRCBLEND_DST_ALPHA				= 6 << 0,
    PLS_SRCBLEND_ONE_MINUS_DST_ALPHA	= 7 << 0,
    PLS_SRCBLEND_BITS					= 7 << 0,

    PLS_DSTBLEND_ZERO					= 0 << 3,
    PLS_DSTBLEND_ONE					= 1 << 3,
    PLS_DSTBLEND_SRC_COLOR				= 2 << 3,
    PLS_DSTBLEND_ONE_MINUS_SRC_COLOR	= 3 << 3,
    PLS_DSTBLEND_SRC_ALPHA				= 4 << 3,
    PLS_DSTBLEND_ONE_MINUS_SRC_ALPHA	= 5 << 3,
    PLS_DSTBLEND_DST_ALPHA				= 6 << 3,
    PLS_DSTBLEND_ONE_MINUS_DST_ALPHA	= 7 << 3,
    PLS_DSTBLEND_BITS					= 7 << 3,

    //------------------------
    // these masks are the inverse, meaning when set the glColorMask value will be 0,
    // preventing that channel from being written
    //------------------------
    PLS_DEPTHMASK						= 1 << 6,
    PLS_REDMASK							= 1 << 7,
    PLS_GREENMASK						= 1 << 8,
    PLS_BLUEMASK						= 1 << 9,
    PLS_ALPHAMASK						= 1 << 10,
    PLS_COLORMASK						= ( PLS_REDMASK | PLS_GREENMASK | PLS_BLUEMASK ),

    PLS_POLYMODE_LINE					= 1 << 11,
    PLS_POLYGON_OFFSET					= 1 << 12,

    PLS_DEPTHFUNC_LESS					= 0 << 13,
    PLS_DEPTHFUNC_ALWAYS				= 1 << 13,
    PLS_DEPTHFUNC_GREATER				= 2 << 13,
    PLS_DEPTHFUNC_EQUAL					= 3 << 13,
    PLS_DEPTHFUNC_BITS					= 3 << 13,

    PLS_BLENDOP_ADD						= 0 << 18,
    PLS_BLENDOP_SUB						= 1 << 18,
    PLS_BLENDOP_MIN						= 2 << 18,
    PLS_BLENDOP_MAX						= 3 << 18,
    PLS_BLENDOP_BITS					= 3 << 18,

    // stencil bits
    PLS_STENCIL_FUNC_REF_SHIFT			= 20,
    PLS_STENCIL_FUNC_REF_BITS			= 0xFFll << PLS_STENCIL_FUNC_REF_SHIFT,

    PLS_STENCIL_FUNC_MASK_SHIFT			= 28,
    PLS_STENCIL_FUNC_MASK_BITS			= 0xFFll << PLS_STENCIL_FUNC_MASK_SHIFT,

    PLS_STENCIL_FUNC_ALWAYS				= 0ull << 36,
    PLS_STENCIL_FUNC_LESS				= 1ull << 36,
    PLS_STENCIL_FUNC_LEQUAL				= 2ull << 36,
    PLS_STENCIL_FUNC_GREATER			= 3ull << 36,
    PLS_STENCIL_FUNC_GEQUAL				= 4ull << 36,
    PLS_STENCIL_FUNC_EQUAL				= 5ull << 36,
    PLS_STENCIL_FUNC_NOTEQUAL			= 6ull << 36,
    PLS_STENCIL_FUNC_NEVER				= 7ull << 36,
    PLS_STENCIL_FUNC_BITS				= 7ull << 36,

    PLS_STENCIL_OP_FAIL_KEEP			= 0ull << 39,
    PLS_STENCIL_OP_FAIL_ZERO			= 1ull << 39,
    PLS_STENCIL_OP_FAIL_REPLACE			= 2ull << 39,
    PLS_STENCIL_OP_FAIL_INCR			= 3ull << 39,
    PLS_STENCIL_OP_FAIL_DECR			= 4ull << 39,
    PLS_STENCIL_OP_FAIL_INVERT			= 5ull << 39,
    PLS_STENCIL_OP_FAIL_INCR_WRAP		= 6ull << 39,
    PLS_STENCIL_OP_FAIL_DECR_WRAP		= 7ull << 39,
    PLS_STENCIL_OP_FAIL_BITS			= 7ull << 39,

    PLS_STENCIL_OP_ZFAIL_KEEP			= 0ull << 42,
    PLS_STENCIL_OP_ZFAIL_ZERO			= 1ull << 42,
    PLS_STENCIL_OP_ZFAIL_REPLACE		= 2ull << 42,
    PLS_STENCIL_OP_ZFAIL_INCR			= 3ull << 42,
    PLS_STENCIL_OP_ZFAIL_DECR			= 4ull << 42,
    PLS_STENCIL_OP_ZFAIL_INVERT			= 5ull << 42,
    PLS_STENCIL_OP_ZFAIL_INCR_WRAP		= 6ull << 42,
    PLS_STENCIL_OP_ZFAIL_DECR_WRAP		= 7ull << 42,
    PLS_STENCIL_OP_ZFAIL_BITS			= 7ull << 42,

    PLS_STENCIL_OP_PASS_KEEP			= 0ull << 45,
    PLS_STENCIL_OP_PASS_ZERO			= 1ull << 45,
    PLS_STENCIL_OP_PASS_REPLACE			= 2ull << 45,
    PLS_STENCIL_OP_PASS_INCR			= 3ull << 45,
    PLS_STENCIL_OP_PASS_DECR			= 4ull << 45,
    PLS_STENCIL_OP_PASS_INVERT			= 5ull << 45,
    PLS_STENCIL_OP_PASS_INCR_WRAP		= 6ull << 45,
    PLS_STENCIL_OP_PASS_DECR_WRAP		= 7ull << 45,
    PLS_STENCIL_OP_PASS_BITS			= 7ull << 45,

    PLS_ALPHATEST_FUNC_REF_SHIFT		= 48,
    PLS_ALPHATEST_FUNC_REF_BITS			= 0xFFll << PLS_ALPHATEST_FUNC_REF_SHIFT,

    PLS_ALPHATEST_FUNC_ALWAYS			= 0ull << 56,
    PLS_ALPHATEST_FUNC_LESS				= 1ull << 56,
    PLS_ALPHATEST_FUNC_GREATER			= 2ull << 56,
    PLS_ALPHATEST_FUNC_EQUAL			= 3ull << 56,
    PLS_ALPHATEST_FUNC_BITS				= 3ull << 56,

    PLS_STENCIL_OP_BITS					= PLS_STENCIL_OP_FAIL_BITS | PLS_STENCIL_OP_ZFAIL_BITS | PLS_STENCIL_OP_PASS_BITS,

    PLS_OVERRIDE						= 1ull << 63		// override the render prog state
};

#define GLS_STENCIL_MAKE_REF( x ) ( ( (uint64)(x) << GLS_STENCIL_FUNC_REF_SHIFT ) & GLS_STENCIL_FUNC_REF_BITS )
#define GLS_STENCIL_MAKE_MASK( x ) ( ( (uint64)(x) << GLS_STENCIL_FUNC_MASK_SHIFT ) & GLS_STENCIL_FUNC_MASK_BITS )

class crPipeline
{
public:
    crPipeline( void );
    ~crPipeline( void );
    bool    Create( );
    void    Destroy( void );

private:
    VkPipeline  m_pipeline;
    uint64_t    m_flags;
    uint16_t    m_vertexShader;
    uint16_t    m_fragmentShader;
};

#endif //!__PIPELINE_HPP__