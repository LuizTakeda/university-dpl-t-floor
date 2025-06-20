#ifndef ENEMIES_INTERNALS_H
#define ENEMIES_INTERNALS_H

#include <genesis.h>

#include "enemies.h"

typedef enum
{
  ENEMY_STATE_SPAWNING = 0,
  ENEMY_RUNNING_RIGHT,
  ENEMY_RUNNING_LEFT,
  ENEMY_MOVING,
  ENEMY_SHOOTING,
  ENEMY_IDLE,
  ENEMY_DYING,
} EnemyState;

typedef enum
{
  ENEMY_PLAYER_HIT_NO = 0,
  ENEMY_PLAYER_HIT_LEFT,
  ENEMY_PLAYER_HIT_RIGHT,
} EnemyPlayerHit;

typedef struct
{
  Sprite *_sprite;
  fix16 x, y;
  fix16 velocity_x, velocity_y;
  u16 hit_box_left_x, hit_box_right_x, hit_box_top_y, hit_box_bottom_y;
  bool dead;
  EnemyState last_state;
  EnemyState state;
  u32 data;
} Enemy;

EnemyPlayerHit did_player_hit_enemy(const Enemy *enemy, const GamePlayerInfo *player_info);

bool did_enemy_hit_player(const Enemy *enemy, const GamePlayerInfo *player_info);

// ### START SLIME ###

void enemy_slime_setup();

void enemy_slime_clean();

bool enemy_slime_spawn(GameLevel game_level);

EnemiesEvents enemy_slime_logic(const GamePlayerInfo *player_info);

// ### END SLIME ###

// ### START BAT ###

void enemy_bat_setup();

void enemy_bat_clean();

bool enemy_bat_spawn(GameLevel game_level);

EnemiesEvents enemy_bat_logic(const GamePlayerInfo *player_info);

// ### END BAT ###

#endif