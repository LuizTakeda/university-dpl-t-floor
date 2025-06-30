#ifndef _ITEM_H
#define _ITEM_H

#include "../game.h"

//************************************************************
// Struct
//************************************************************

typedef enum
{
  ITEM_EVENT_NONE = 0,
  ITEM_EVENT_LIFE,
} ItemsEvent;

//************************************************************
// Function Prototypes
//************************************************************

void items_setup();

void items_clean();

ItemsEvent items_logic(GamePlayerInfo *player_info);

#endif