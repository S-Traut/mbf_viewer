/*================================================================
    * Copyright: 2020 John Jackson
    * gs_static_lib
    This example compiles gunslinger as a static library then
    links it against the example program to run.
    Press `esc` to exit the application.
================================================================*/
#include <gs/gs.h>

gs_command_buffer_t cb = {0};
gs_handle(gs_graphics_vertex_buffer_t) vbo = {0};
gs_handle(gs_graphics_index_buffer_t) ibo = {0};
gs_handle(gs_graphics_pipeline_t) pip = {0};
gs_handle(gs_graphics_shader_t) shader = {0};
gs_handle(gs_graphics_uniform_t) uniform = {0};

float v_data[] = {
    -0.5f, -0.5f, // 0
    0.5f,  -0.5f, // 1
    0.5f,  0.5f,  // 2
    -0.5f, 0.5f,  // 3
};

void init() {
  // Creating a new command buffer
  cb = gs_command_buffer_new();

  char *vs = gs_platform_read_file_contents("res/shaders/base.vs", "rb", 0);
  char *fs = gs_platform_read_file_contents("res/shaders/base.fs", "rb", 0);

  // VERTEX BUFFER
  // Le vertex buffer est un buffer de memoire que l'ont va
  // envoyer a la carte graphique pour quelle traitre rapidement ses
  // valeurs.
  //
  // Sa description va être simplement un tableau de float qui designe la
  // position de chaque vertices de notre forme.
  gs_graphics_vertex_buffer_desc_t vbdesc = {
      .data = v_data,
      .size = sizeof(v_data),
  };
  vbo = gs_graphics_vertex_buffer_create(&vbdesc);

  // INDEX BUFFER
  // L'index buffer est tout comme le vertex buffer un bout de memoire qui va
  // contenir une suite d'index que la carte graphique pourra interpreter pour
  // generer une suite de vertex, ce qui va permettre de reduire drastiquement
  // la taille de la memoire allouée pour chaque connexion de triangles.
  unsigned int indexes[] = {0, 1, 2, 2, 3, 0};
  gs_graphics_buffer_update_desc_t gbu = {
      .offset = 0,
      .type = GS_GRAPHICS_BUFFER_UPDATE_RECREATE,
  };

  gs_graphics_index_buffer_desc_t ibdesc = {
      .data = &indexes,
      .size = sizeof(indexes),
      .usage = GS_GRAPHICS_BUFFER_USAGE_STATIC,
      .update = gbu,
  };
  ibo = gs_graphics_index_buffer_create(&ibdesc);

  // SHADER
  // Les shaders son une manière d'attribuer des calcul graphiques sur un
  // element en modifiant ses informations (to be improved)
  // Les shaders sont simplement du code qui va être executé par le processeur
  // graphique.
  //
  // Vertex shader:
  // Les vertex shaders sont executés pour chaque vertex de notre buffer. ils
  // permettent principalement de positioner chaque vertices sur l'ecran.
  //
  // Fragment shader:
  // Les fragment shaders vont bouclés sur chaque pixels de notre window pour
  // definir la couleur de chaque pixel affiché, à la suite du vertex shader
  // dans notre exemple, le fragment va simplement tracer chaque pixel de notre
  // triangle pour le rendre visible.
  gs_graphics_shader_source_desc_t shadersrc[] = {
      {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = vs},
      {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = fs},
  };

  gs_graphics_shader_desc_t shaderdesc = {
      .sources = shadersrc,
      .size = 2 * sizeof(gs_graphics_shader_source_desc_t),
      .name = "triangle",
  };
  shader = gs_graphics_shader_create(&shaderdesc);

  // UNIFORMS
  // Les uniforms sont un element assez similaire aux attribus, ils permettent
  // d'envoyer des informations depuis le CPU vers le GPU, par exemple si l'ont
  // souhaite modifier la couleur de notre triangle sur le temps, on modifiera
  // sa valeur dans l'update, puis on enverra celle-ci dans l'uniform pour que
  // le shader puisse la traiter.
  gs_graphics_uniform_layout_desc_t uldesc = {
      .type = GS_GRAPHICS_UNIFORM_VEC4,
      .fname = "u_color",
      .count = 0,
  };
  gs_graphics_uniform_desc_t udesc = {
      .layout = &uldesc,
      .layout_size = sizeof(gs_graphics_uniform_layout_desc_t),
      .stage = GS_GRAPHICS_SHADER_STAGE_FRAGMENT,
  };
  uniform = gs_graphics_uniform_create(&udesc);

  // PIPELINE
  gs_graphics_vertex_attribute_desc_t attributes[] = {
      {
          .format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2,
          .name = "a_pos",
      },
  };

  gs_graphics_pipeline_desc_t pipedesc = {
      .raster = {.shader = shader},
      .layout =
          {
              .attrs = attributes,
              .size = sizeof(gs_graphics_vertex_attribute_desc_t),
          },
  };
  pip = gs_graphics_pipeline_create(&pipedesc);
}

void update() {
  if (gs_platform_key_pressed(GS_KEYCODE_ESC)) {
    gs_quit();
  }

  // Generation d'une action de clear pour fill le screen avec une couleur fixe.
  // Elle est executés au debut de chaque frame
  gs_graphics_clear_action_t clear_action = {.color = {0.1f, 0.1f, 0.1f, 1.f}};
  gs_graphics_clear_desc_t clear = {.actions = &clear_action};

  // Binding descriptor for vertex buffer
  gs_graphics_bind_vertex_buffer_desc_t vbinds = {.buffer = vbo};
  gs_graphics_bind_index_buffer_desc_t ibinds = {.buffer = ibo};

  const float t = gs_platform_elapsed_time() * 0.001f;
  const float r = cos(t) * 0.5f + 0.5f;
  const float b = sin(t) * 0.5f + 0.5f;
  gs_vec3 color_data = gs_v3(r, 0.5, b);
  gs_graphics_bind_uniform_desc_t unibinds = {
      .uniform = uniform,
      .data = &color_data,
  };

  gs_graphics_bind_desc_t binds = {
      .vertex_buffers = &vbinds,
      .index_buffers = &ibinds,
      .uniforms = &unibinds,
  };

  // Creating a draw description
  gs_graphics_draw_desc_t drawdesc = {.start = 0, .count = 6};

  gs_graphics_begin_render_pass(&cb, GS_GRAPHICS_RENDER_PASS_DEFAULT);
  gs_graphics_clear(&cb, &clear);
  gs_graphics_bind_pipeline(&cb, pip);
  gs_graphics_apply_bindings(&cb, &binds);
  gs_graphics_draw(&cb, &drawdesc);
  gs_graphics_end_render_pass(&cb);
  gs_graphics_submit_command_buffer(&cb);
}

gs_app_desc_t gs_main(int32_t argc, char **argv) {
  return (gs_app_desc_t){
      .init = init,
      .update = update,
      .window_title = "GST",
  };
}
