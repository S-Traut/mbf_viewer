#include "texture.h"

gs_graphics_texture_desc_t mbfv_create_texture(char *file)
{
  int width, height, channels;
  void *img_data;
  gs_util_load_texture_data_from_file(file, &width, &height, &channels, &img_data, 1);
  gs_graphics_texture_desc_t description = {
      .data = img_data,
      .format = GS_GRAPHICS_TEXTURE_FORMAT_RGBA8,
      .height = height,
      .width = width,
      .min_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST,
      .mag_filter = GS_GRAPHICS_TEXTURE_FILTER_NEAREST,
  };
  return description;
}

gs_handle_gs_graphics_uniform_t mbfv_create_texture_uniform()
{
  gs_graphics_uniform_desc_t description = {
      .layout = &(gs_graphics_uniform_layout_desc_t){
          .type = GS_GRAPHICS_UNIFORM_SAMPLER2D,
      },
      .stage = GS_GRAPHICS_SHADER_STAGE_FRAGMENT,
      .name = "u_tex",
  };
  return gs_graphics_uniform_create(&description);
}