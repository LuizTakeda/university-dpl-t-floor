#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"
#include "game/globals.h"

void game_screen_finish(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);
    VDP_resetScreen();
    VDP_resetSprites();

    VDP_drawImageEx(BG_A, &img_you_won, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);

    VDP_drawText("Press A", 16, 19);
  }

  if (inputs->ok)
  {
    XGM2_playPCMEx(EFFECT_OPTION);
    game_screen_set(GSN_START);
  }
}