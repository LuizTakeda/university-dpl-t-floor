#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "game/globals.h"
#include "../game.h"
#include "../inputs/inputs.h"

void game_screen_credits(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);

    VDP_drawImageEx(BG_A, &img_credits, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);

    VDP_drawText("Dev/Art", 11, 9);
    VDP_drawText(" -Takeda", 11, 10);

    VDP_drawText("Music", 11, 12);
    VDP_drawText(" -Safety Stoat", 11, 13);
    VDP_drawText("  Studios", 11, 14);

    VDP_drawText("Sound Effect", 11, 16);
    VDP_drawText(" -DSherbert", 11, 17);
    VDP_drawText(" -Dask", 11, 18);

    VDP_drawText("Press A to exit", 11, 20);
  }

  if (game_inputs_click(inputs->ok))
  {
    XGM2_playPCMEx(EFFECT_OPTION);
    game_screen_set(GSN_START);
    return;
  }
}