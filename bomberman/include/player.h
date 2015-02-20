#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number,int range, int life,int key);
void player_free(struct player* player);

//Return, Increase, Decrease the current life of the player
int player_get_life(struct player* player);
void player_inc_life(struct player* player);
void player_dec_life(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

//Return the number of key of the player
int player_get_key(struct player* player);

//Return, Increase, Decrease the current range of the bomb
int player_get_range(struct player* player);
void player_inc_range(struct player* player);
void player_dec_range(struct player* player);
void player_inc_key(struct player* player);
void player_dec_key(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player* player, way_t way);
void player_set_xy(struct player* player, int x,int y);

// Set, Increase, Decrease the number of bomb that player can put
int player_get_nb_bomb(struct player* player);
void player_inc_nb_bomb(struct player* player);
void player_dec_nb_bomb(struct player* player);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

//Apply the effect of a bonus on the player
void player_apply_bonus(struct player * player, struct map* map);

#endif /* PLAYER_H_ */
