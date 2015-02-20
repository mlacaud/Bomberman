#include <assert.h>
#include <time.h>
#include <SDL/SDL_timer.h>


#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>

struct bomb {
        int x, y, state;
        int start_timer;
};

struct bomb* bomb_init(struct player * player) {
	assert(player);
        struct bomb* bomb = malloc(sizeof(*bomb));
        if (!bomb)
                error("Memory error");
        bomb->x = player_get_x(player);
        bomb->y = player_get_y(player);
        bomb->start_timer=SDL_GetTicks();

        return bomb;
}

void bomb_update( struct bomb* bomb,struct game* game){
	assert(bomb);
	assert(game);

	bomb->state = (SDL_GetTicks() - bomb->start_timer)/1000;

}

void bomb_display(struct bomb* bomb) {

      if (bomb->state>= 0 && bomb->state <5){
    	  window_display_image(sprite_get_bomb(bomb->state), bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
      }

}

int bomb_get_x(struct bomb* bomb){
	assert(bomb);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb){
	assert(bomb);
	return bomb->y;
}

int bomb_get_state(struct bomb* bomb){
	assert(bomb);
	return bomb->state;
}

void bomb_set_timer(struct bomb* bomb,int state){
	bomb->start_timer=state;
}

void bomb_delay(struct bomb* bomb,int delay) {
	int new_time=0;
	new_time= bomb->start_timer + delay;
	bomb_set_timer(bomb,new_time);
}


