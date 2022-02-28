#ifndef MBFV_DATA
#define MBFV_DATA

typedef struct appdata
{
  int t;
  gs_command_buffer_t command_buffer;
  gs_handle_gs_graphics_pipeline_t pipeline;

  gs_handle_gs_graphics_uniform_t texture_uniform;
  gs_handle_gs_graphics_shader_t shader;

  gs_dyn_array(float) d_vbo;
  gs_dyn_array(uint32_t) d_ibo;
  gs_dyn_array(gs_graphics_texture_desc_t) textures;
  gs_dyn_array(gs_handle_gs_graphics_texture_t) textures_h;
} appdata;

#endif