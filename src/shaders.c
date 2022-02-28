#include "shaders.h"

gs_handle_gs_graphics_shader_t mbfv_load_shaders()
{
  char *vs = gs_platform_read_file_contents("res/shaders/base.vs", "rb", 0);
  char *fs = gs_platform_read_file_contents("res/shaders/base.fs", "rb", 0);
  gs_graphics_shader_source_desc_t shadersrc[] = {
      {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = vs},
      {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = fs},
  };

  gs_graphics_shader_desc_t shaderdesc = {
      .sources = shadersrc,
      .size = sizeof(shadersrc),
      .name = "triangle",
  };
  return gs_graphics_shader_create(&shaderdesc);
}
