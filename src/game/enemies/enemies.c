#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "enemies.h"

#define ENEMIES_MAX 60

// _sprites[_enemies_counter]->frameInd <- Indica qual frame atual da animação

typedef enum
{
  ENEMY_TYPE_SLIME = 0
} EnemyType;

typedef enum
{
  ENEMY_STATE_SPAWNING = 0,
  ENEMY_RUNNING_RIGHT,
  ENEMY_RUNNING_LEFT,
  ENEMY_SHOOTING,
  ENEMY_IDLE,
  ENEMY_DYING
} EnemyState;

typedef struct
{
  Sprite *_sprite;
  EnemyType type;
  fix16 x, y;
  fix16 velocity_x, velocity_y;
  bool dead;
  EnemyState last_state;
  EnemyState state;
} Enemy;

static Enemy _enemies[ENEMIES_MAX] = {{0}};
static fix16 _enemies_counter = 0;

static void create_enemy(EnemyType type);

static u16 enemy_create_slime();
static void enemy_logic_slime(Enemy *enemy, const GamePlayerInfo *player_info);

void enemies_setup()
{
  PAL_setPalette(PAL3, spr_enemy_01.palette->data, DMA);
}

u16 num = 0;

u16 counter = 0;

void enemies_logic(const GamePlayerInfo *player_info)
{
  counter++;
  if (num < ENEMIES_MAX && counter >= 50)
  {
    num++;
    create_enemy(ENEMY_TYPE_SLIME);
    counter = 0;
  }

  for (int i = 0; i < ENEMIES_MAX; i++)
  {
    if (_enemies[i].dead)
      continue;

    switch (_enemies[i].type)
    {
    case ENEMY_TYPE_SLIME:
      enemy_logic_slime(&_enemies[i], player_info);
      break;

    default:
      break;
    }
  }
}

static void create_enemy(EnemyType type)
{
  switch (type)
  {
  case ENEMY_TYPE_SLIME:
    enemy_create_slime();
    break;

  default:
    break;
  }
}

static u16 enemy_create_slime()
{
  _enemies[_enemies_counter].type = ENEMY_TYPE_SLIME;
  _enemies[_enemies_counter].x = FIX16(80 + (8 * (random() % 19)));
  _enemies[_enemies_counter].y = FIX16(168 - (32 * (random() % 4)));
  _enemies[_enemies_counter].velocity_y = 0;
  _enemies[_enemies_counter].velocity_x = FIX16(0.75);
  _enemies[_enemies_counter].dead = false;
  _enemies[_enemies_counter].state = ENEMY_STATE_SPAWNING;
  _enemies[_enemies_counter]._sprite =
      SPR_addSprite(
          &spr_enemy_01,
          fix16ToInt(_enemies[_enemies_counter].x), fix16ToInt(_enemies[_enemies_counter].y),
          TILE_ATTR_FULL(PAL3, 0, false, false, 1));

  _enemies_counter++;
}

typedef enum
{
  ENEMY_PLAYER_HIT_NO = 0,
  ENEMY_PLAYER_HIT_LEFT,
  ENEMY_PLAYER_HIT_RIGHT,
} EnemyPlayerHit;

static EnemyPlayerHit enemy_did_player_hit(const Enemy *enemy, const GamePlayerInfo *player_info)
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

static void enemy_set_state(Enemy *enemy, EnemyState state)
{
  enemy->last_state = enemy->state;
  enemy->state = state;
}

static void enemy_logic_slime(Enemy *slime, const GamePlayerInfo *player_info)
{
  switch (slime->state)
  {
  case ENEMY_STATE_SPAWNING:
    SPR_setAnim(slime->_sprite, 1);

    if (enemy_did_player_hit(slime, player_info))
    {
      enemy_set_state(slime, ENEMY_DYING);
      break;
    }

    if (slime->_sprite->frameInd == 4)
    {
      if (random() % 2)
      {
        slime->state = ENEMY_RUNNING_RIGHT;
        break;
      }

      slime->state = ENEMY_RUNNING_LEFT;
      break;
    }
    break;

  case ENEMY_RUNNING_RIGHT:
    SPR_setAnim(slime->_sprite, 2);

    if (enemy_did_player_hit(slime, player_info))
    {
      enemy_set_state(slime, ENEMY_DYING);
      break;
    }

    slime->x += slime->velocity_x;

    if (slime->x > FIX16(224))
    {
      slime->x = FIX16(224);
      slime->state = ENEMY_RUNNING_LEFT;
      break;
    }
    break;

  case ENEMY_RUNNING_LEFT:
    SPR_setAnim(slime->_sprite, 3);

    if (enemy_did_player_hit(slime, player_info))
    {
      enemy_set_state(slime, ENEMY_DYING);
      break;
    }

    slime->x -= slime->velocity_x;

    if (slime->x < FIX16(80))
    {
      slime->x = FIX16(80);
      slime->state = ENEMY_RUNNING_RIGHT;
      break;
    }
    break;

  case ENEMY_SHOOTING:
    break;

  case ENEMY_IDLE:
    break;

  case ENEMY_DYING:
    SPR_setAnim(slime->_sprite, slime->last_state == ENEMY_RUNNING_RIGHT ? 5 : 4);

    if (slime->_sprite->frameInd >= 3)
    {
      SPR_releaseSprite(slime->_sprite);
      slime->dead = true;
      break;
    }

    break;

  default:
    break;
  }

  SPR_setPosition(slime->_sprite, fix16ToInt(slime->x), fix16ToInt(slime->y));
}