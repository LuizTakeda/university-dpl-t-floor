#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <genesis.h>
#include <sprite_eng.h>

typedef struct
{
  Sprite *sprite;
  s16 x, y;
  f16 speed_x, speed_y;
  u8 height, width;
  u8 flip;
} GameObject;

u16 GameObject_init(
    GameObject *const obj,
    const SpriteDefinition *const sprite,
    s16 x, s16 y);

void GameObject_clamp_screen(GameObject *obj);

void GameObject_wrap_screen(GameObject *obj);

#endif