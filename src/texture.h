#ifndef MBFV_TEXTURE
#define MBFV_TEXTURE

#include <gs/gs.h>

gs_graphics_texture_desc_t mbfv_create_texture(char *file);
gs_handle_gs_graphics_uniform_t mbfv_create_texture_uniform();

#endif