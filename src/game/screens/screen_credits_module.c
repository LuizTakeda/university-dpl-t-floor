#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"

void game_screen_credits(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_drawImageEx(BG_A, &img_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);
  }
}