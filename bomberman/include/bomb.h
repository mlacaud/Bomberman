#ifndef _BOMB_H_
#define _BOMB_H_

#include <player.h>
#include <map.h>
#include <list.h>

int bomb_exist;

struct bomb;
struct game;

struct list_bomb;

struct list_bomb * list_bomb_init();

struct bomb* bomb_init();

//Display of the bomb
void bomb_display(struct bomb* bomb);

//Set the bomb position
int bomb_get_y(struct bomb* bomb);
int bomb_get_x(struct bomb* bomb);

// Return the bomb state
int bomb_get_state(struct bomb* bomb);

//Set the bomb time :
void bomb_set_timer(struct bomb* bomb,int state);

//Update
void bomb_update( struct bomb* bomb,struct game* game);

//Put the bombs into a break.
void bomb_delay(struct bomb* bomb, int delay);

#endif
