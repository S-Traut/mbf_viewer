#ifndef MBFV_SPRITE
#define MBFV_SPRITE

#include <gs/gs.h>
#include "data.h"

typedef struct mbfv_sprite_desc_t
{
  float texture_id;
  gs_vec2 position;
  float opacity;
} mbfv_sprite_desc_t;

void mbfv_create_sprite(appdata *app, mbfv_sprite_desc_t sprite);

#endif