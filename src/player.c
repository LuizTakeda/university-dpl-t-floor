#include "player.h"
#include "resources.h"
#include "globals.h"

GameObject player;

u16 Player_init()
{
  return GameObject_init(&player, &spr_player, (SCREEN_W / 2) - (player.width / 2), (SCREEN_H / 2) - (player.height / 2));
}

void Player_get_input()
{
  u16 joy = JOY_readJoypad(JOY_1);

  player.speed_x = 0;

  if (player.y + 8 < SCREEN_H / 2)
    player.speed_y += FIX16(1);
  else
    player.speed_y = 0;

  if (joy & BUTTON_LEFT)
  {
    player.speed_x = -PLAYER_SPEED;
  }
  else if (joy & BUTTON_RIGHT)
  {
    player.speed_x = +FIX16(6);
  }

  if (joy & BUTTON_UP && player.speed_y == 0 && player.y + 8 > SCREEN_H / 2)
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
