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

  enemy_horizontal_shooter_setup();

  enemy_jumper_setup();
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

  enemy_ball_projectile_clean();

  enemy_vertical_shooter_clean();

  enemy_horizontal_shooter_clean();

  enemy_jumper_clean();
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
  case GAME_LEVEL_FOUR:
  {
  }

  case GAME_LEVEL_FIVE:
  {
  }

  case GAME_LEVEL_SIX:
  {
  }

  case GAME_LEVEL_SEVEN:
  {
    enemy_jumper_spawn(_game_level);

    EnemiesEvents jumper_event = enemy_jumper_logic(player_info);

    enemies_event.enemies_dead += jumper_event.enemies_dead;
    enemies_event.player_hit |= jumper_event.player_hit;
  }

  case GAME_LEVEL_EIGHT:
  {
    enemy_vertical_shooter_spawn(_game_level);

    EnemiesEvents horizontal_shooter_event = enemy_vertical_shooter_logic(player_info);

    enemies_event.enemies_dead += horizontal_shooter_event.enemies_dead;
    enemies_event.player_hit |= horizontal_shooter_event.player_hit;
  }

  case GAME_LEVEL_THREE:
  {
    enemy_horizontal_shooter_spawn(_game_level);

    EnemiesEvents vertical_shooter_event = enemy_horizontal_shooter_logic(player_info);

    enemies_event.enemies_dead += vertical_shooter_event.enemies_dead;
    enemies_event.player_hit |= vertical_shooter_event.player_hit;

    EnemiesEvents ball_projectile_event = enemy_ball_projectile_logic(player_info);

    enemies_event.player_hit |= ball_projectile_event.player_hit;
  }

  case GAME_LEVEL_TWO:
  {
    enemy_bat_spawn(_game_level);

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }

  case GAME_LEVEL_ONE:
  {
    enemy_slime_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;
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
    if (player_info->left_x >= enemy->hit_box_left_x && player_info->left_x <= enemy->hit_box_right_x)
    {
      if ((enemy->hit_box_top_y >= player_info->top_y && enemy->hit_box_top_y <= player_info->bottom_y) ||
          (enemy->hit_box_bottom_y >= player_info->top_y && enemy->hit_box_bottom_y <= player_info->bottom_y))
      {
        return ENEMY_PLAYER_HIT_RIGHT;
      }
    }
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT)
  {
    if (player_info->right_x >= enemy->hit_box_left_x && player_info->right_x <= enemy->hit_box_right_x)
    {
      if ((enemy->hit_box_top_y >= player_info->top_y && enemy->hit_box_top_y <= player_info->bottom_y) ||
          (enemy->hit_box_bottom_y >= player_info->top_y && enemy->hit_box_bottom_y <= player_info->bottom_y))
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

  if ((player_info->top_y >= enemy->hit_box_top_y && player_info->top_y <= enemy->hit_box_bottom_y) || (player_info->bottom_y >= enemy->hit_box_top_y && player_info->bottom_y <= enemy->hit_box_bottom_y))
  {
    if ((player_info->left_x >= enemy->hit_box_left_x && player_info->left_x <= enemy->hit_box_right_x) || (player_info->right_x >= enemy->hit_box_left_x && player_info->right_x <= enemy->hit_box_right_x))
    {
      return true;
    }
  }

  return false;
}