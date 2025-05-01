#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"
#include "../inputs/inputs.h"

void game_screen_credits(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);

    VDP_drawText("Creditos para TAKEDA :)", 1, 1);
  }

  if (game_inputs_click(inputs->ok))
  {
    game_screen_set(GSN_START);
    return;
  }
}