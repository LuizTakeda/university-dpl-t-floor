#ifndef GAME_H
#define GAME_H

#define SCREEN_W 320
#define SCREEN_H 224

typedef enum
{
  GIS_NOTHING = 0,
  GIS_CLICK,
  GIS_HOLD,
  GIS_PRESS,
  GIS_RELEASED,
} GameInputStates;

typedef struct
{
  GameInputStates left;
  GameInputStates right;
  GameInputStates up;
  GameInputStates down;
  GameInputStates start;
  GameInputStates ok;
  GameInputStates mode;
} GameInputs;

typedef enum
{
  GSN_START = 0,
  GSN_GAME,
  GSN_PAUSE,
  GSN_GAME_OVER,
  GSN_FINISH,
  GSN_CREDITS,
} GameScreenName;

typedef enum
{
  PLAYER_STATE_RUNNING_LEFT = 0,
  PLAYER_STATE_STOPED_LEFT,
  PLAYER_STATE_TURING_LEFT,
  PLAYER_STATE_RUNNING_RIGHT,
  PLAYER_STATE_STOPED_RIGHT,
  PLAYER_STATE_TURING_RIGHT,
  PLAYER_STATE_CLIMBING_UP,
  PLAYER_STATE_CLIMBING_DOWN,
} GamePlayerStates;

typedef struct
{
  u16 left_x, right_x, middle_x;
  u16 bottom_y, top_y;
  GamePlayerStates state;
  bool intangible;
} GamePlayerInfo;

typedef enum{
  GAME_LEVEL_ONE = 1,
  GAME_LEVEL_TWO,
  GAME_LEVEL_THREE,
  GAME_LEVEL_FOUR,
  GAME_LEVEL_FIVE,
  GAME_LEVEL_SIX,
  GAME_LEVEL_SEVEN,
  GAME_LEVEL_EIGHT,
}GameLevel;

// ### START Globals ###
extern u16 _tile_index;
// ### END GLobals ###

// ### START Functions ###

/**
 * Realiza a inicialização do jogo
 */
void game_setup();

/**
 * Contem toda a logica do jogo
 */
void game_logic();

/**
 * Retorna true caso seja a primeira entrada na tela
 */
bool game_screen_is_first_entry();

/**
 * Define a tela atual
 */
void game_screen_set(const GameScreenName target_screen);

/**
 * Volta para uma tela sem reinicar o estado dela
 */
void game_screen_come_back(const GameScreenName target_screen);

/**
 * Tela inicial
 */
void game_screen_start(const GameInputs *inputs);

/**
 * Tela em jogo
 */
void game_screen_game(const GameInputs *inputs);

/**
 * Tela em pause
 */
void game_screen_pause(const GameInputs *inputs);

/**
 * Tela fim de jogo
 */
void game_screen_game_over(const GameInputs *inputs);

/**
 * Tela jogo concluído
 */
void game_screen_finish(const GameInputs *inputs);

/**
 * Tela de creditos
 */
void game_screen_credits(const GameInputs *inputs);

// ### END Functions ###

#endif