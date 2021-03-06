#include <SDL/SDL_image.h> // IMG_Load
#include <assert.h>

#include <constant.h>
#include <misc.h>

#include <sprite.h>

SDL_Surface* window;

void window_create(int width, int height) {
	assert(width > 0 && height > 0);

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_WM_SetCaption(WINDOW_NAME, NULL );
	window = SDL_SetVideoMode(width, height, 0, // If bits-per-pixel is 0, it is treated as the current display bits per pixel.
			SDL_HWSURFACE);

	if (window == NULL ) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void window_free() {
	assert(window);
	SDL_FreeSurface(window);
}

void window_display_image(SDL_Surface* sprite, int x, int y) {
	assert(window);
	assert(sprite);

	SDL_Rect place;
	place.x = x;
	place.y = y;

	SDL_BlitSurface(sprite, NULL, window, &place);
}

void window_clear() {
	assert(window);
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
}

void window_refresh() {
	assert(window);
	SDL_Flip(window);
}

void game_finish() {
	window_clear();

	int time = SDL_GetTicks();
	while (SDL_GetTicks() - time < 5000){
		window_display_image(sprite_get_homer(),70,75);
		window_refresh();

	}
	exit(0);
}
void game_over() {
	window_clear();

	int time = SDL_GetTicks();
	while (SDL_GetTicks() - time < 4000){
		window_display_image(sprite_get_over(),30,65);
		window_refresh();

	}
	exit(0);
}


