#include "player.h"
#include "resources.h"
#include "globals.h"

#include "game/inputs/inputs.h"

GameObject player;

u16 Player_init()
{
  return GameObject_init(&player, &spr_player, (SCREEN_W / 2) - (player.width / 2), (SCREEN_H / 2) - (player.height / 2));
}

void Player_get_input()
{
  GameInputs inputs = game_inputs_read();

  player.speed_x = 0;

  if (player.y + 8 < SCREEN_H / 2)
    player.speed_y += FIX16(1);
  else
    player.speed_y = 0;

  if (game_inputs_hold(inputs.left))
  {
    SPR_setAnim(player.sprite, 0);
    player.speed_x = -PLAYER_SPEED;
  }
  else if (game_inputs_hold(inputs.right))
  {
    SPR_setAnim(player.sprite, 1);
    player.speed_x = +FIX16(6);
  }

  if (game_inputs_hold(inputs.up) && player.speed_y == 0 && player.y + 8 > SCREEN_H / 2)
  {
    player.speed_y = -PLAYER_SPEED;
  }
}

void Player_update()
{
  Player_get_input();

  player.x += fix16ToInt(player.speed_x);
  player.y += fix16ToInt(player.speed_y);

  GameObject_clamp_screen(&player);

  SPR_setPosition(player.sprite, player.x, player.y);
}
