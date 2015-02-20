#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <bomb.h>
#include <monster.h>

struct monster {
	int x, y;
	enum way current_way;
	int start_timer;
};

struct monster* monster_init(int x, int y) {
			struct monster* monster = malloc(sizeof(*monster));
			if (!monster)
					error("Memory error");
			monster->x = x;
			monster->y = y;
			monster->current_way = SOUTH;
			monster->start_timer=SDL_GetTicks();

			return monster;
	}


int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum way way) {
	assert(monster);
	monster->current_way = way;
}

int monster_get_start_timer(struct monster* monster){
	assert(monster);
	return monster->start_timer;
}
void monster_set_start_timer(struct monster* monster, int timer){
	assert(monster);
monster->start_timer=timer;
}

void monster_from_map(struct map* map) {
   assert(map);
	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
				map_add_monster(monster_init(i,j),map);
				}
		}
	}
}

int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY: //Pierre + arbres
		return 0;
		break;

	case CELL_CASE:
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_GOAL:
		return 0;
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_PLAYER:
		return 1;
		break;

	case CELL_DOOR:
		return 0;
		break;
	case CELL_FLAG:
		return 0;
		break;

	default:
		break;
	}

	// Monster has moved
	return 1;
}

int monster_move(struct monster * monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;
	switch (monster->current_way) {
	case NORTH:

		if (monster_move_aux(monster, map, x, y - 1) == 1) {
			monster->y--;
			move = 1;
		}

		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1) == 1) {
			monster->y++;
			move = 1;
		}

		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y) == 1) {
			monster->x--;
			move = 1;
		}

		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y) == 1) {
			monster->x++;
			move = 1;
		}

		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
	}

	return move;
}

void monster_display(struct monster* monster) {
	assert(monster);
    window_display_image(sprite_get_monster(monster->current_way),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}


