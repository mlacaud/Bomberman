#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>

// load game's sprites
void sprite_load();

SDL_Surface* sprite_get_player(way_t direction);
SDL_Surface* sprite_get_monster(way_t direction);
SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type);
SDL_Surface* sprite_get_tree();
SDL_Surface* sprite_get_box();
SDL_Surface* sprite_get_key();
SDL_Surface* sprite_get_stone();
SDL_Surface* sprite_get_door();
SDL_Surface* sprite_get_closed_door();
SDL_Surface* sprite_get_bomb(int state);

// Picture for the end
SDL_Surface* sprite_get_homer();
// Picture for the game over
SDL_Surface* sprite_get_over();


SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_banner_life();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_range();
SDL_Surface* sprite_get_banner_key();
SDL_Surface* sprite_get_banner_level();
SDL_Surface* sprite_get_goal();

#endif /* SPRITE_H_ */
