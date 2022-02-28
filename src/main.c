/*================================================================
    * Copyright: 2020 John Jackson
    * gs_static_lib
    This example compiles gunslinger as a static library then
    links it against the example program to run.
    Press `esc` to exit the application.
================================================================*/
#include <gs/gs.h>
#include "sprite.h"
#include "texture.h"
#include "shaders.h"
#include "data.h"

int play = 0;
int t = 0;

void init()
{

  appdata *app = gs_user_data(appdata);
  app->command_buffer = gs_command_buffer_new();

  app->d_vbo = NULL;
  app->d_ibo = NULL;
  app->textures = NULL;
  app->textures_h = NULL;

  // Generating textures
  // (TODO): Abstraction from mbf
  gs_dyn_array_push(app->textures, mbfv_create_texture("res/img/img.jpg"));
  gs_dyn_array_push(app->textures, mbfv_create_texture("res/img/flush3d.png"));
  //////////

  for (int i = 0; i < gs_dyn_array_size(app->textures); i++)
  {
    gs_graphics_texture_desc_t description = app->textures[i];
    gs_handle_gs_graphics_texture_t texture = gs_graphics_texture_create(&description);
    gs_dyn_array_push(app->textures_h, texture);
  }

  app->texture_uniform = mbfv_create_texture_uniform();
  app->shader = mbfv_load_shaders();

  // PIPELINE
  gs_graphics_vertex_attribute_desc_t attributes[] = {
      {
          .format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2,
          .name = "a_pos",
      },
      {
          .format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2,
          .name = "a_uv",
      },
      {
          .format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT,
          .name = "a_texid",
      },
  };

  gs_graphics_pipeline_desc_t pipedesc = {
      .raster = {.shader = app->shader},
      .layout =
          {
              .attrs = attributes,
              .size = sizeof(attributes),
          },

  };
  app->pipeline = gs_graphics_pipeline_create(&pipedesc);
}

void update()
{
  appdata *app = gs_user_data(appdata);
  gs_command_buffer_t *cb = &app->command_buffer;

  if (gs_platform_key_pressed(GS_KEYCODE_ESC))
    gs_quit();

  if (gs_platform_key_pressed(GS_KEYCODE_SPACE))
    play = !play;

  if (!play)
    return;

  //////// BATCH
  // To be optimized for generating > unit texture size for a single frame in the case this
  // happens on lower GCards.

  gs_dyn_array_clear(app->d_vbo);
  gs_dyn_array_clear(app->d_ibo);

  mbfv_sprite_desc_t sprite1 = {
      .texture_id = 0,
      .position = {.x = -300, .y = 0},
      .opacity = 1.0f,
  };

  mbfv_sprite_desc_t sprite2 = {
      .texture_id = 1,
      .position = {.x = 300, .y = 0},
      .opacity = 1.0f,
  };

  mbfv_create_sprite(app, sprite1);
  mbfv_create_sprite(app, sprite2);

  // Need to understand why I have to go reverse to insert the binds
  int textures_size = gs_dyn_array_size(app->textures_h);
  int b = 0;
  gs_graphics_bind_uniform_desc_t unibinds[textures_size];
  for (int i = textures_size - 1; i >= 0; i--)
  {
    gs_graphics_bind_uniform_desc_t utexture_description = {
        .uniform = app->texture_uniform,
        .data = &app->textures_h[i],
        .binding = b,
    };
    unibinds[b] = utexture_description;
    b++;
  }

  gs_graphics_vertex_buffer_desc_t vbdesc = {
      .data = app->d_vbo,
      .size = sizeof(float) * gs_dyn_array_size(app->d_vbo),
  };

  gs_graphics_index_buffer_desc_t ibdesc = {
      .data = app->d_ibo,
      .size = sizeof(float) * gs_dyn_array_size(app->d_ibo),
      .usage = GS_GRAPHICS_BUFFER_USAGE_DYNAMIC,
      .update = {
          .offset = 0,
          .type = GS_GRAPHICS_BUFFER_UPDATE_RECREATE,
      },
  };

  gs_graphics_bind_vertex_buffer_desc_t vbinds = {.buffer = gs_graphics_vertex_buffer_create(&vbdesc)};
  gs_graphics_bind_index_buffer_desc_t ibinds = {.buffer = gs_graphics_index_buffer_create(&ibdesc)};

  gs_graphics_bind_desc_t binds = {
      .vertex_buffers = &vbinds,
      .index_buffers = &ibinds,
      .uniforms = {.desc = unibinds, .size = sizeof(unibinds)},
  };

  gs_graphics_clear_action_t clear_action = {.color = {0.1f, 0.1f, 0.1f, 1.f}};
  gs_graphics_clear_desc_t clear = {.actions = &clear_action};

  const gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
  float viewx = fbs.x;
  float viewy = fbs.x * 9 / 16;
  float viewox = 0;
  float viewoy = (fbs.y - viewy) / 2;
  if (viewy > fbs.y)
  {
    viewx = fbs.y * 16 / 9;
    viewy = fbs.y;
    viewox = (fbs.x - viewx) / 2;
    viewoy = 0;
  }

  // Creating a draw description
  gs_graphics_draw_desc_t drawdesc = {.start = 0, .count = 12};
  gs_graphics_begin_render_pass(cb, GS_GRAPHICS_RENDER_PASS_DEFAULT);
  gs_graphics_set_viewport(cb, viewox, viewoy, viewx, viewy);
  gs_graphics_clear(cb, &clear);
  gs_graphics_bind_pipeline(cb, app->pipeline);
  gs_graphics_apply_bindings(cb, &binds);
  gs_graphics_draw(cb, &drawdesc);
  gs_graphics_end_render_pass(cb);
  gs_graphics_submit_command_buffer(cb);

  t++;
}

gs_app_desc_t gs_main(int32_t argc, char **argv)
{
  érique(IEC958) | ~ / s / p / C / GS01 | (master) return (gs_app_desc_t){
                                              .user_data = gs_malloc_init(appdata),
                                              .init = init,
                                              .update = update,
                                              .window_title = "GST",
                                              .frame_rate = 100,
                                          };
}
