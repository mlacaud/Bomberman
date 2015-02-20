#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>
#include <monster.h>

int doorx[7];
int doory[7];
char nextmap[7];
char closedoor[7];
char flag[2];

struct map {
	int width;
	int height;
	char* grid;
	struct list* list_bomb;
	struct list* list_monster;
};

#define CELL(i,j) (i +  map->width * j)

struct map* map_new(int width, int height) {
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL)
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			map->grid[CELL(i,j)] = CELL_EMPTY;

	map->list_monster = list_init();
	map->list_bomb = list_init();
	return map;
}

int map_is_inside(struct map* map, int x, int y) {
	assert(map);
	if (x < 0 || x > map->width - 1 || y < 0 || y > map->height - 1)
		return 0;
	else
		return 1;
}

char * map_get_grid(struct map* map) {
	return map->grid;
}
void map_free(struct map* map) {
	if (map == NULL)
		return;
	free(map->grid);
	free(map->list_bomb);
	free(map->list_monster);
	free(map);
}

int map_get_width(struct map* map) {
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map) {
	assert(map);
	return map->height;
}

struct list* map_get_list_bomb(struct map* map) {
	return map->list_bomb;
}
struct list* map_get_list_monster(struct map* map) {
	return map->list_monster;
}

void map_init_list_bomb(struct map* map) {
	map->list_bomb=list_init();
}

void map_init_list_monster(struct map* map) {
	 map->list_monster=list_init();
}

enum cell_type map_get_cell_type(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 15;
}

enum cell_type map_get_compose_type(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 255;
}

enum cell_type map_get_door_type(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] >> 7) & 1;
}

enum bonus_type map_get_bonus_type(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] >> 4) & 15;
}

enum cell_type map_get_map(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] >> 4) & 7;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type) {
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, char type) {
	// bonus is encoded with the 4 most significant bits
	switch (type >> 4) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int y, char type) {
	switch (type >> 4) { // sub-types are encoded with the 4 most significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}

void display_door(struct map* map, int x, int y, char type){

	if (type <0)
		window_display_image(sprite_get_closed_door(), x, y);
	else
		window_display_image(sprite_get_door(), x, y);

}

void map_display(struct map* map) {
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {
			x = i * SIZE_BLOC;
			y = j * SIZE_BLOC;

			char type = map->grid[CELL(i,j)];

			switch (type & 15) { // type is encoded with 4 bits, 15 (1111) is a mask to keep the four less significant bits
			case CELL_SCENERY:
				display_scenery(map, x, y, type);
				break;
			case CELL_CASE:
				window_display_image(sprite_get_box(), x, y);
				break;
			case CELL_BONUS:
				display_bonus(map, x, y, type);
				break;
			case CELL_KEY:
				window_display_image(sprite_get_banner_key(), x, y);
				break;
			case CELL_DOOR:
				display_door(map,x, y, type);
				break;

			case CELL_FLAG:
				window_display_image(sprite_get_banner_level(), x, y);
				break;

			case CELL_GOAL:
				window_display_image(sprite_get_goal(),x,y);
				break;
			}
		}

	}

}

struct map* map_get_default(void) {
	struct map* map = map_new(MAP_WIDTH, MAP_HEIGHT);

	char themap[MAP_WIDTH * MAP_HEIGHT] = {CELL_EMPTY, CELL_MONSTER,
			CELL_BONUS_LIFE, CELL_BONUS_RANGE_INC, CELL_BONUS_BOMB_DEC,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_MONSTER, CELL_STONE, CELL_STONE, CELL_STONE,
			CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE,
			CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE_LIFE,
			CELL_STONE, CELL_KEY, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_STONE, CELL_CASE_MONSTER, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE,
			CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,
			CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_TREE, CELL_CASE_BOMBINC, CELL_TREE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_FLAG, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_CASE_BOMBINC, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
			CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,
			CELL_CASE_LIFE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, 24};

	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
}

int bomb_explode_aux(struct bomb* bomb, struct map * map, int x, int y) {
	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY: //Pierre + arbres
		return 0;
		break;

	case CELL_CASE:
		return 2;
		break;

	case CELL_BONUS:
		return 1;
		break;

	case CELL_GOAL:
		return 0;
		break;

	case CELL_MONSTER:
		return 4;
		break;

	case CELL_PLAYER:
		return 6;
		break;

	case CELL_DOOR:
		return 0;
		break;

	default:
		break;
	}
	return 1;
}

void map_add_bomb(struct bomb* bomb, struct map* map) {
	assert(bomb);
	assert(map);
	list_add(map->list_bomb, bomb);
}

void map_add_monster(struct monster* monster, struct map* map) {
	assert(monster);
	assert(map);
	list_add(map->list_monster, monster);
}
void bonus_from_map(struct map* map) {

	assert(map);
	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_BONUS) {
				display_bonus(map, i, j, map_get_cell_type(map, i, j));
			}
		}
	}
}

void door_from_map(struct map* map) {
	assert(map);

	int i, j;
	int k = 0;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_DOOR) {
				doorx[k] = i;
				doory[k] = j;
				nextmap[k] = map_get_map(map, i, j);
				closedoor[k] = map_get_door_type(map, i, j);
				k++;
			}
		}
	}

}
void flag_from_map(struct map* map) {
	assert(map);
flag[0]=30;
flag[1]=30;
	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_FLAG) {
				flag[0] = i;
				flag[1] = j;
			}

		}
	}

}

void move_case_into_bonus(struct map* map, int x, int y) {
	char type = map_get_bonus_type(map, x, y);
	if (type == BONUS_MONSTER) {
		map_add_monster(monster_init(x, y), map);
		map_set_cell_type(map, x, y, type << 4 | CELL_MONSTER);
	} else
		map_set_cell_type(map, x, y, type << 4 | CELL_BONUS);
}

