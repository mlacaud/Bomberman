#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <level.h>

struct game;
struct bomb;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current level
struct level* game_get_curr_level(struct game* game);

//Return the number of the current level
int game_get_n_curr_level(struct game* game);

// Display the game of the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

//Apply the change if the player is in contact with a door.
void action_door(struct game* game, int doorx, int doory, char nextmap, char doorclose);

//Manage the bomb explosion
void game_bomb_explode(struct bomb * bomb, struct game* game);
void game_bomb_explode_sprite(struct bomb * bomb, struct game* game);

//Manage the collisions between monsters/players
void monster_attack_player(struct monster* monster, struct player* player);

// Put the game into break
void game_pause(struct game * game, int time);

//Set the next map
void map_next(struct game* game, char nextmap) ;

//Set the new level
void level_next(struct game* game);

//Apply the action if the player if he is in contact with a flag
void action_flag(struct game* game, int flagx, int flagy);

//Load a map
struct map* get_map(int n_curr_level, int next_map);

//Save the current game
int game_save(struct game * game);

//Load a saved game.
void load_game_saved(struct game* game);


#endif /* GAME_H_ */
