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

  enemy_bat_setup();

  enemy_ball_projectile_setup();

  enemy_vertical_shooter_setup();
}

void enemies_next_level(GameLevel level)
{
  _game_level = level;
}

/**
 *
 */
void enemies_clean()
{
  enemy_slime_clean();

  enemy_bat_clean();
}

/**
 *
 */
EnemiesEvents enemies_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  switch (_game_level)
  {
  case GAME_LEVEL_ONE:
  {
    enemy_slime_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;
  }
  break;

  case GAME_LEVEL_TWO:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }
  break;

  case GAME_LEVEL_THREE:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    enemy_vertical_shooter_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;

    EnemiesEvents vertical_shooter_event = enemy_vertical_shooter_logic(player_info);

    enemies_event.enemies_dead += vertical_shooter_event.enemies_dead;
    enemies_event.player_hit |= vertical_shooter_event.player_hit;

    EnemiesEvents ball_projectile_event = enemy_ball_projectile_logic(player_info);

    enemies_event.enemies_dead += ball_projectile_event.enemies_dead;
    enemies_event.player_hit |= ball_projectile_event.player_hit;
  }
  break;

  case GAME_LEVEL_FOUR:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }
  break;

  case GAME_LEVEL_FIVE:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }
  break;

  case GAME_LEVEL_SIX:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }
  break;

  case GAME_LEVEL_SEVEN:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }
  break;

  case GAME_LEVEL_EIGHT:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }
  break;

  default:
    break;
  }

  return enemies_event;
}

/**
 *
 */
EnemyPlayerHit did_player_hit_enemy(const Enemy *enemy, const GamePlayerInfo *player_info)
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

/**
 *
 */
bool did_enemy_hit_player(const Enemy *enemy, const GamePlayerInfo *player_info)
{
  if (player_info->intangible)
  {
    return false;
  }

  if (player_info->state == PLAYER_STATE_RUNNING_LEFT && (enemy->hit_box_left_x <= player_info->middle_x || enemy->hit_box_right_x <= player_info->middle_x || enemy->velocity_x > FIX16(0)))
  {
    return false;
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT && (enemy->hit_box_left_x > player_info->middle_x || enemy->hit_box_right_x > player_info->middle_x || enemy->velocity_x < FIX16(0)))
  {
    return false;
  }

  if ((enemy->hit_box_top_y >= player_info->top_y && enemy->hit_box_top_y <= player_info->bottom_y) || (enemy->hit_box_bottom_y >= player_info->top_y && enemy->hit_box_bottom_y <= player_info->bottom_y))
  {
    if ((enemy->hit_box_left_x >= player_info->left_x && enemy->hit_box_left_x <= player_info->right_x) || (enemy->hit_box_right_x >= player_info->left_x && enemy->hit_box_right_x <= player_info->right_x))
    {
      return true;
    }
  }

  return false;
}