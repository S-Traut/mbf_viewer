#include "sprite.h"

void mbfv_create_sprite(appdata *app, mbfv_sprite_desc_t sprite)
{
  const unsigned int offset = (gs_dyn_array_size(app->d_vbo) / 20) * 4;
  int texture_id = (int)sprite.texture_id;
  const gs_graphics_texture_desc_t texture = app->textures[texture_id];

  float w = (texture.width / 1920.0);
  float h = (texture.height / 1080.0);
  float posx = sprite.position.x / 960.0;
  float posy = sprite.position.y / 540.0;

  float v_data[] = {
      posx + -w, posy + -h, 0.0f, 0.0f, sprite.texture_id, // Top Left
      posx + w, posy + -h, 1.0f, 0.0f, sprite.texture_id,  // Top Right
      posx + -w, posy + h, 0.0f, 1.0f, sprite.texture_id,  // Bottom Left
      posx + w, posy + h, 1.0f, 1.0f, sprite.texture_id,   // Bottom Right
  };

  unsigned int indexes[] = {
      offset + 0,
      offset + 3,
      offset + 2,
      offset + 0,
      offset + 1,
      offset + 3,
  };

  for (int i = 0; i < 20; i++)
  {
    float value = v_data[i];
    gs_dyn_array_push(app->d_vbo, value);
  }

  for (int i = 0; i < 6; i++)
  {
    unsigned int value = indexes[i];
    gs_dyn_array_push(app->d_ibo, value);
  }
}