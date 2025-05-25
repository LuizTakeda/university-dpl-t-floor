#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"

void game_screen_game_over(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);
    VDP_resetScreen();
    VDP_resetSprites();

    VDP_drawText("GAME OVER", 1, 1);

  }
  
  if(inputs->ok){
    game_screen_set(GSN_START);
  }
}