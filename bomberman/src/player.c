#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <game.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <bomb.h>
#include <monster.h>
#include <level.h>

int player_of_steel = 0;

struct player {
	int x, y, range, life;
	enum way current_way;
	int nb_bomb;
	int life_timer;
	int key;
};

struct player* player_init(int bomb_number,int range,int life,int key) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");
player->x=0;
player->y=0;
	player->current_way = SOUTH;
	player->nb_bomb = bomb_number;
	player->range = range;
	player->life = life;
	player->life_timer = 2000;
	player->key = key;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}
int player_get_life(struct player* player) {
	assert(player != NULL);
	return player->life;
}
int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

int player_get_range(struct player* player) {
	assert(player != NULL);
	return player->range;
}

int player_get_key(struct player* player) {
	assert(player != NULL);
	return player->key;
}

void player_set_current_way(struct player* player, enum way way) {
	assert(player);
	player->current_way = way;
}

void player_set_xy(struct player* player, int x, int y) {
	player->x = x;
	player->y = y;
}
int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bomb;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb -= 1;
}

void player_inc_range(struct player* player) {
	assert(player);
	player->range += 1;
}

void player_dec_range(struct player* player) {
	assert(player);
	player->range -= 1;
}
void player_inc_key(struct player* player) {
	assert(player);
	player->key += 1;
}

void player_dec_key(struct player* player) {
	assert(player);
	player->key -= 1;
}
void player_inc_life(struct player* player) {
	assert(player);
	player->life += 1;
}

void player_dec_life(struct player* player) {
	assert(player);
	if (player_of_steel == 0) {
		player->life -= 1;
		player->life_timer = SDL_GetTicks();
	}
}

void player_from_map(struct player* player, struct map* map) { // savoir l'emplacement INITIAL du player sur la map
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				player->x = i;
				player->y = j;
			}
		}
	}
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

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
		return 3;
		break;

	case CELL_GOAL:
		game_finish();
		break;

	case CELL_MONSTER:

		break;

	case CELL_PLAYER:

		break;
	case CELL_KEY:
		return 5;
		break;

	case CELL_DOOR:
		return 4;
		break;
	case CELL_FLAG:

		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {

	int x = player->x;
	int y = player->y;
	int move = 0;
	int move_case = 0;
	int move_bonus = 0;
	int move_key = 0;


	switch (player->current_way) {
	case NORTH:

		if (player_move_aux(player, map, x, y - 1) == 1) {
			player->y--;
			move = 1;
		}
		if (player_move_aux(player, map, x, y - 1) == 2 && player_move_aux(
				player, map, x, y - 2) == 1) {
			player->y--;
			move_case = 1;

		}
		if (player_move_aux(player, map, x, y - 1) == 3) {
			player->y--;
			move_bonus = 1;

		}
		if (player_move_aux(player, map, x, y - 1) == 4) {
		 player->y--;
		 move=0;

		 }



		if (player_move_aux(player, map, x, y - 1) == 5) {
			player->y--;

			move_key = 1;

		}

		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1) == 1) {
			player->y++;
			move = 1;
		}
		if (player_move_aux(player, map, x, y + 1) == 2 && player_move_aux(
				player, map, x, y + 2) == 1) {
			player->y++;
			move_case = 2;
		}
		if (player_move_aux(player, map, x, y + 1) == 3) {
			player->y++;
			move_bonus = 1;

		}
		if (player_move_aux(player, map, x, y + 1) == 4) {
			 player->y++;
			 move=0;

			 }
		if (player_move_aux(player, map, x, y + 1) == 5) {
			player->y++;

			move_key = 1;

		}

		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y) == 1) {
			player->x--;
			move = 1;
		}
		if (player_move_aux(player, map, x - 1, y) == 2 && player_move_aux(
				player, map, x - 2, y) == 1) {
			player->x--;
			move_case = 3;
		}
		if (player_move_aux(player, map, x - 1, y) == 3) {
			player->x--;
			move_bonus = 1;

		}
		if (player_move_aux(player, map, x - 1, y) == 4) {
			 player->x--;
			 move=0;

			 }
		if (player_move_aux(player, map, x - 1, y) == 5) {
			player->x--;

			move_key = 1;

		}

		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y) == 1) {
			player->x++;
			move = 1;
		}
		if (player_move_aux(player, map, x + 1, y) == 2 && player_move_aux(
				player, map, x + 2, y) == 1) {
			player->x++;
			move_case = 4;
		}
		if (player_move_aux(player, map, x + 1, y) == 3) {
			player->x++;
			move_bonus = 1;

		}
		if (player_move_aux(player, map, x + 1, y) == 4) {
					 player->x++;
					 move=0;

					 }
		if (player_move_aux(player, map, x + 1, y) == 5) {
			player->x++;
			move_key = 1;
		}

		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	if (move_bonus) {
		player_apply_bonus(player, map);
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);

	}

	if (move_key) {
		player_inc_key(player);
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);

	}
	char type;

	if (move_case == 1) {

		type = map_get_compose_type(map, x, y - 1);
		map_set_cell_type(map, x, y - 2, type);
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, x, y - 1, CELL_EMPTY);
		map_set_cell_type(map, x, y - 1, CELL_PLAYER);
	}

	if (move_case == 2) {
		type = map_get_compose_type(map, x, y + 1);
		map_set_cell_type(map, x, y + 2, type);
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, x, y + 1, CELL_EMPTY);
		map_set_cell_type(map, x, y + 1, CELL_PLAYER);
	}

	if (move_case == 3) {
		type = map_get_compose_type(map, x - 1, y);
		map_set_cell_type(map, x - 2, y, type);
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, x - 1, y, CELL_EMPTY);
		map_set_cell_type(map, x - 1, y, CELL_PLAYER);
	}

	if (move_case == 4) {
		type = map_get_compose_type(map, x + 1, y);
		map_set_cell_type(map, x + 2, y, type);
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, x + 1, y, CELL_EMPTY);
		map_set_cell_type(map, x + 1, y, CELL_PLAYER);
	}

	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_way),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
	if (SDL_GetTicks() - player->life_timer < 2000) {
		player_of_steel = 1;
	}
	if (SDL_GetTicks() - player->life_timer >= 2000) {
		player_of_steel = 0;
	}
}

void player_apply_bonus(struct player * player, struct map* map) {
	char type = map_get_bonus_type(map, player->x, player->y);

	switch (type) {
	case BONUS_BOMB_RANGE_INC:
		player_inc_range(player);
		break;
	case BONUS_BOMB_RANGE_DEC:
		player_dec_range(player);
		break;
	case BONUS_BOMB_NB_INC:
		player_inc_nb_bomb(player);
		break;
	case BONUS_BOMB_NB_DEC:
		player_dec_nb_bomb(player);
		break;
	case BONUS_LIFE:
		player_inc_life(player);
		break;
	}

}

