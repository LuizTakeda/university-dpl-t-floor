#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"

void game_screen_pause(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);
    VDP_drawImageEx(BG_A, &img_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);
  }
}