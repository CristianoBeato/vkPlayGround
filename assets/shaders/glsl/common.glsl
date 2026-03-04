
#ifndef VERTEX_SHADER
#   ifdef __SHADER_STAGE_VERTEX__
#       define VERTEX_SHADER
#   endif //__SHADER_STAGE_VERTEX__
#endif //!VERTEX_SHADER

#ifndef FRAGMENT_SHADER
#   ifdef __SHADER_STAGE_FRAGMENT__
#       define FRAGMENT_SHADER
#   endif // __SHADER_STAGE_FRAGMENT__
#endif //!FRAGMENT_SHADER

#ifdef VERTEX_SHADER

struct mesh_uniforms_t
{
    vec4    uViewPosition;  /// View Origin
    mat4    uModelMatrix;   /// model for normal
    mat4    uMVP;           /// model MVP
    mat4    uTextureMatrix; /// Texture Matrix
};

#endif //VERTEX_SHADER

#ifdef FRAGMENT_SHADER

#endif //FRAGMENT_SHADER