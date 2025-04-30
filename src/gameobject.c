#include "gameobject.h"
#include "globals.h"

u16 GameObject_init(
    GameObject *const obj,
    const SpriteDefinition *const sprite,
    s16 x, s16 y)
{
  obj->x = x;
  obj->y = y;
  obj->speed_x = 0;
  obj->speed_y = 0;
  obj->flip = 0;
  // Carregando paleta no sistema
  PAL_setPalette(PAL1, sprite->palette->data, DMA);

  // Criando sprite e usando paleta
  obj->sprite = SPR_addSprite(sprite, x, y, TILE_ATTR_FULL(PAL1, false, false, false, 1));
  obj->width = obj->sprite->definition->w;
  obj->height = obj->sprite->definition->h;

  return obj->sprite->definition->maxNumTile;
}

void GameObject_clamp_screen(GameObject *obj)
{
  if (obj->x < 0)
  {
    obj->x = 0;
  }
  else if (obj->x > SCREEN_W - obj->width)
  {
    obj->x = SCREEN_W - obj->width;
  }

  if (obj->y < 0)
  {
    obj->y = 0;
  }
  else if (obj->y > SCREEN_H - obj->width)
  {
    obj->y = SCREEN_W - obj->width;
  }
}

void GameObject_wrap_screen(GameObject *obj)
{
}
