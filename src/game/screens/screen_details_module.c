#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "game/globals.h"
#include "../game.h"
#include "../inputs/inputs.h"

static Sprite *_sprite;

void game_screen_details(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);
    SPR_reset();

    VDP_drawImageEx(BG_A, &img_details, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);

    VDP_drawText("Turn < >", 11, 9);

    VDP_drawText("Climb ^ v", 11, 12);

    VDP_drawText("Do not touch", 11, 15);

    PAL_setPalette(PAL3, spr_enemy_05.palette->data, DMA);
    _sprite = SPR_addSprite(
        &spr_enemy_05,
        8 * 24, 15 * 8,
        TILE_ATTR(PAL3, 0, false, false));

    SPR_setAnim(_sprite, 1);

    VDP_drawText("Press A to exit", 11, 20);
  }

  if (game_inputs_click(inputs->ok))
  {
    XGM2_playPCMEx(EFFECT_OPTION);
    SPR_releaseSprite(_sprite);
    game_screen_set(GSN_START);
    return;
  }
}