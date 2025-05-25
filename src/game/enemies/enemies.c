#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "enemies.h"
#include "enemies_internals.h"

GameLevel _game_level;

/**
 *
 */
void enemies_setup(GameLevel level)
{
  _game_level = level;

  PAL_setPalette(PAL3, spr_enemy_01.palette->data, DMA);

  enemy_slime_setup();
}

/**
 *
 */
void enemies_clean()
{
}

/**
 *
 */
EnemiesEvents enemies_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event = {0};

  switch (_game_level)
  {
  case GAME_LEVEL_ONE:
  {
    enemy_slime_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);
  }
  break;
  case GAME_LEVEL_TWO:
    break;
  case GAME_LEVEL_THREE:
    break;
  case GAME_LEVEL_FOUR:
    break;
  case GAME_LEVEL_FIVE:
    break;
  case GAME_LEVEL_SIX:
    break;
  case GAME_LEVEL_SEVEN:
    break;
  case GAME_LEVEL_EIGHT:
    break;

  default:
    break;
  }

  return enemies_event;
}

/**
 *
 */
EnemyPlayerHit enemy_did_player_hit(const Enemy *enemy, const GamePlayerInfo *player_info)
{
  if (player_info->state == PLAYER_STATE_RUNNING_LEFT)
  {
    if (fix16ToInt(enemy->y) >= player_info->top_y && fix16ToInt(enemy->y) <= player_info->bottom_y)
    {
      if (player_info->left_x >= fix16ToInt(enemy->x) && player_info->left_x <= fix16ToInt(enemy->x) + 7)
      {
        return ENEMY_PLAYER_HIT_RIGHT;
      }
    }
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT)
  {
    if (fix16ToInt(enemy->y) >= player_info->top_y && fix16ToInt(enemy->y) <= player_info->bottom_y)
    {
      if (player_info->right_x >= fix16ToInt(enemy->x) && player_info->right_x <= fix16ToInt(enemy->x) + 7)
      {
        return ENEMY_PLAYER_HIT_LEFT;
      }
    }
  }

  return ENEMY_PLAYER_HIT_NO;
}
