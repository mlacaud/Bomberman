#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>

struct monster ;

struct monster* monster_init(int x, int y);

//Return the monster position
int monster_get_x(struct monster* monster) ;
int monster_get_y(struct monster* monster) ;


//Return the bomb timer
int monster_get_start_timer(struct monster* monster);

//Set the bomb timer
void monster_set_start_timer(struct monster* monster, int timer);

//Set the current way of the monster
void monster_set_current_way(struct monster* monster, enum way way) ;


//Load the monster position
void monster_from_map(struct map* map);

//Control the movement of the monster according its environment
int monster_move_aux(struct monster* monster, struct map* map, int x, int y) ;

//Move the monster according to the current direction
int monster_move(struct monster * monster, struct map* map) ;

//Update
void monster_update(struct monster* monster,struct game* game);

//Display
void monster_display(struct monster* monster) ;


#endif
