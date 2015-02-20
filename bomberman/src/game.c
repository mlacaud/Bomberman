#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>
#include <list.h>
#include <map.h>
int doorx[7];
int doory[7];
char nextmap[7];
char closedoor[7];
char flag[2];

struct game {
	struct level* curr_level; // current level
	int n_curr_level;
	struct player* player;

};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->curr_level = level_get_level(0); // get maps of the first level
	game->n_curr_level = 0;

	game->player = player_init(4, 1, 2, 0);
	player_from_map(game->player, level_get_map(game->curr_level, 0)); // get x,y of the player on the first map


	monster_from_map(level_get_map(game->curr_level, 0));
doorx[1]=player_get_x(game->player);
doory[1]=player_get_y(game->player);
nextmap[1]=0;
closedoor[1]=0;
	door_from_map(level_get_map(game->curr_level, 0));
	flag_from_map(level_get_map(game->curr_level, 0));
	map_new(12, 12);

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	level_free(game->curr_level);
}

int game_set_n_curr_level(struct game* game, int next_level) {
	game->n_curr_level = next_level;
	return game->n_curr_level;
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct level* game_get_curr_level(struct game* game) {
	return game->curr_level;
}

int game_get_n_curr_level(struct game* game) {
	return game->n_curr_level;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = level_get_curr_map(game_get_curr_level(game));

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 10 * SIZE_BLOC) / 8;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_range(game_get_player(game))), x, y);

	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_key(game_get_player(game))), x, y);

	x = 5 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_banner_level(), x, y);

	x = 5 * white_bloc + 9 * SIZE_BLOC;
	window_display_image(sprite_get_number(game_get_n_curr_level(game)), x, y);
}

void game_display(struct game* game) {
	assert(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));
	window_clear();

	game_banner_display(game);
	level_display(game_get_curr_level(game));
	player_display(game->player);

	struct list* list_monster = map_get_list_monster(map);
	struct list_element * monster = list_get_first(list_monster);

	//boucle d'application de monster_display
	list_monster = map_get_list_monster(map);
	monster = list_get_first(list_monster);

	while (monster != NULL) {
		struct monster * monster1 = list_get_value(monster);
		monster_display(monster1);
		monster = list_get_next(monster);
	}

	//boucle d'application de bomb_update
	struct list* list_bomb = map_get_list_bomb(map);
	struct list_element * bomb = list_get_first(list_bomb);

	while (bomb != NULL) {
		struct bomb * bomb1 = list_get_value(bomb);
		bomb_update(bomb1, game);
		bomb = list_get_next(bomb);
	}

	//boucle d'application de bomb_display
	list_bomb = map_get_list_bomb(map);
	bomb = list_get_first(list_bomb);

	while (bomb != NULL) {
		struct bomb * bomb1 = list_get_value(bomb);
		bomb_display(bomb1);
		bomb = list_get_next(bomb);
	}
	// boucle d'affichage de l'explosion
	list_bomb = map_get_list_bomb(map);
	bomb = list_get_first(list_bomb);

	while (bomb != NULL) {
		struct bomb * bomb1 = list_get_value(bomb);
		if (bomb_get_state(bomb1) > 5 && bomb_get_state(bomb1) < 7) {
			game_bomb_explode_sprite(bomb1, game);
		}
		if (bomb_get_state(bomb1) > 7) {
			list_remove(map_get_list_bomb(map), bomb1);
		}
		bomb = list_get_next(bomb);
	}
	window_refresh();

}

short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: // keyboard : esc
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
				if (player_get_nb_bomb(player) > 0) {
					map_add_bomb(bomb_init(player), map);
					player_dec_nb_bomb(player);
				}
				break;
				//Permet de mettre en pause le jeu.
			case SDLK_p:
				game_pause(game, SDL_GetTicks());
				break;
				// Permet de mettre en pause le jeu !
			case SDLK_s:
				game_save(game);
				break;
				// Permet de charger une partie sauvergardée
			case SDLK_l:
				load_game_saved(game);
				break;
			case SDLK_f:
				game_finish();
				break;

			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {
	assert(game);

	if (input_keyboard(game) == 1)
		return 1; // exit game
	if (player_get_life(game->player) == 0){
		game_over();

	}



	struct map* map = level_get_curr_map(game_get_curr_level(game));

	struct list* list_bomb = map_get_list_bomb(map);
	struct list_element * bomb = list_get_first(list_bomb);

	struct list* list_monster = map_get_list_monster(map);
	struct list_element * monster = list_get_first(list_monster);

	int j = 0;
	while (monster != NULL) {
		struct monster * monster1 = list_get_value(monster);
		int monster_timer = SDL_GetTicks() - monster_get_start_timer(monster1);

		if (monster_timer >= 1000) {
			srand(SDL_GetTicks() + j);
			int i = rand() % 5;
			j++;
			switch (i) {
			case 0:
				break;
			case 1:
				monster_set_current_way(monster1, NORTH);
				monster_move(monster1, map);
				monster_set_start_timer(monster1, SDL_GetTicks());
				break;
			case 2:
				monster_set_current_way(monster1, SOUTH);
				monster_move(monster1, map);
				monster_set_start_timer(monster1, SDL_GetTicks());
				break;
			case 3:
				monster_set_current_way(monster1, EAST);
				monster_move(monster1, map);
				monster_set_start_timer(monster1, SDL_GetTicks());
				break;
			case 4:
				monster_set_current_way(monster1, WEST);
				monster_move(monster1, map);
				monster_set_start_timer(monster1, SDL_GetTicks());
				break;
			}
			monster_timer = 0;
		}
		monster = list_get_next(monster);
	}
	//collision entre monstre et player
	list_monster = map_get_list_monster(map);
	monster = list_get_first(list_monster);
	while (monster != NULL) {
		struct monster * monster1 = list_get_value(monster);
		monster_attack_player(monster1, game->player);
		monster = list_get_next(monster);
	}

	//Lance l'explosion d'une bombe
	while (bomb != NULL) {
		struct bomb * bomb1 = list_get_value(bomb);
		if (bomb_get_state(bomb1) > 3 && bomb_get_state(bomb1) < 5) {
			game_bomb_explode(bomb1, game);
			bomb_set_timer(bomb1, SDL_GetTicks() - 6000); //On fait un "saut dans le temps" pour gérer l'affichage en ne déclenchant l'explosion qu'une seule fois.
		}

		bomb = list_get_next(bomb);

	}
	for (int i = 0; i < 7; i++) {
		action_door(game, doorx[i], doory[i], nextmap[i],
				closedoor[i]);
	}
	action_flag(game, flag[0], flag[1]);
	return 0;
}

void action_door(struct game* game, int doorx, int doory,
		char nextmap, char doorclose) {
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	int x = player_get_x(player);
	int y = player_get_y(player);

	if (x == doorx && y == doory) {
		// La porte est ouverte
		if (doorclose == 0) {
			map_next(game, nextmap);
		}
		if (doorclose == 1 && player_get_key(player) == 0) {
					player_from_map(player, map);
				}
		//La porte est fermée et le player a une clé
		if (doorclose == 1 && player_get_key(player) > 0) {
			player_dec_key(player);
			unsigned char type = map_get_compose_type(map, x, y);
			map_set_cell_type(map, x, y, type & 127);
			map_next(game, nextmap);

		}

	}
}

void action_flag(struct game* game, int flagx, int flagy) {
	struct player* player = game_get_player(game);
	int x = player_get_x(player);
	int y = player_get_y(player);
	if (x == flagx && y == flagy) {
		level_next(game);
		map_next(game, 0);
	}
}

void game_bomb_explode(struct bomb * bomb, struct game* game) {
	struct player * player = game->player;
	struct map * map = level_get_curr_map(game_get_curr_level(game));
	struct list* list_monster = map_get_list_monster(map);
	struct list_element * monster = list_get_first(list_monster);
	int a = 0;
	int x = bomb_get_x(bomb);
	int y = bomb_get_y(bomb);
	//ouest
	for (int r = 0; r <= player_get_range(player); r++) { // Pierre arbre
		if (bomb_explode_aux(bomb, map, x + r, y) == 0) {
			break;
		}
		if (bomb_explode_aux(bomb, map, x + r, y) == 1) { // Bonus
			a = x + r;
			map_set_cell_type(map, a, y, CELL_EMPTY);
		}
		if (bomb_explode_aux(bomb, map, x + r, y) == 4) { // Monstres
			a = x + r;

			//boucle de suppression du bon monstre
			list_monster = map_get_list_monster(map);
			monster = list_get_first(list_monster);

			while (monster != NULL) {
				struct monster * monster1 = list_get_value(monster);
				if (monster_get_x(monster1) == a && monster_get_y(monster1)
						== y) {
					list_remove(list_monster, monster1);
				}
				monster = list_get_next(monster);
			}

			map_set_cell_type(map, a, y, CELL_EMPTY);
		}
		if (bomb_explode_aux(bomb, map, x + r, y) == 2) { // Caisses
			a = x + r;

			move_case_into_bonus(map, a, y);
		}
		if (bomb_explode_aux(bomb, map, x + r, y) == 6) { // Player
			a = x + r;

			player_dec_life(game->player);
		}

	}

	//est
	for (int r = 0; r <= player_get_range(player); r++) {
		if (bomb_explode_aux(bomb, map, x - r, y) == 0) {
			break;
		}
		if (bomb_explode_aux(bomb, map, x - r, y) == 1) {
			a = x - r;
			map_set_cell_type(map, a, y, CELL_EMPTY);
		}
		if (bomb_explode_aux(bomb, map, x - r, y) == 4) {
			a = x - r;

			//boucle de suppression du bon monstre
			list_monster = map_get_list_monster(map);
			monster = list_get_first(list_monster);

			while (monster != NULL) {
				struct monster * monster1 = list_get_value(monster);
				if (monster_get_x(monster1) == a && monster_get_y(monster1)
						== y) {
					list_remove(list_monster, monster1);
				}
				monster = list_get_next(monster);
			}

			map_set_cell_type(map, a, y, CELL_EMPTY);
		}
		if (bomb_explode_aux(bomb, map, x - r, y) == 2) {
			a = x - r;

			move_case_into_bonus(map, a, y);
		}
		if (r > 0) {
			if (bomb_explode_aux(bomb, map, x - r, y) == 6) {
				a = x - r;

				player_dec_life(game->player);
			}
		}

	}

	//sud
	for (int r = 0; r <= player_get_range(player); r++) {
		if (bomb_explode_aux(bomb, map, x, y + r) == 0) {
			break;
		}
		if (bomb_explode_aux(bomb, map, x, y + r) == 1) {
			a = y + r;
			map_set_cell_type(map, x, a, CELL_EMPTY);
		}
		if (bomb_explode_aux(bomb, map, x, y + r) == 4) {
			a = y + r;

			//boucle de suppression du bon monstre
			list_monster = map_get_list_monster(map);
			monster = list_get_first(list_monster);

			while (monster != NULL) {
				struct monster * monster1 = list_get_value(monster);
				if (monster_get_x(monster1) == x && monster_get_y(monster1)
						== a) {
					list_remove(list_monster, monster1);
				}
				monster = list_get_next(monster);
			}

			map_set_cell_type(map, a, y, CELL_EMPTY);
		}

		if (bomb_explode_aux(bomb, map, x, y + r) == 2) {
			a = y + r;

			move_case_into_bonus(map, x, a);
		}
		if (r > 0) {
			if (bomb_explode_aux(bomb, map, x, y + r) == 6) {
				a = y + r;

				player_dec_life(game->player);
			}
		}

	}

	//nord
	for (int r = 0; r <= player_get_range(player); r++) {
		if (bomb_explode_aux(bomb, map, x, y - r) == 0) {
			break;
		}
		if (bomb_explode_aux(bomb, map, x, y - r) == 1) {
			a = y - r;
			map_set_cell_type(map, x, a, CELL_EMPTY);
		}
		if (bomb_explode_aux(bomb, map, x, y - r) == 4) {
			a = y - r;

			//boucle de suppression du bon monstre
			list_monster = map_get_list_monster(map);
			monster = list_get_first(list_monster);

			while (monster != NULL) {
				struct monster * monster1 = list_get_value(monster);
				if (monster_get_x(monster1) == x && monster_get_y(monster1)
						== a) {
					list_remove(list_monster, monster1);
				}
				monster = list_get_next(monster);
			}

			map_set_cell_type(map, a, y, CELL_EMPTY);
		}

		if (bomb_explode_aux(bomb, map, x, y - r) == 2) {
			a = y - r;

			move_case_into_bonus(map, x, a);
		}
		if (r > 0) {
			if (bomb_explode_aux(bomb, map, x, y - r) == 6) {
				a = y - r;

				player_dec_life(game->player);
			}
		}

	}
}
void game_bomb_explode_sprite(struct bomb * bomb, struct game* game) {
	struct player * player = game->player;
	struct map * map = level_get_curr_map(game_get_curr_level(game));
	;
	int a = 0;
	int x = bomb_get_x(bomb);
	int y = bomb_get_y(bomb);

	//ouest
	for (int r = 0; r <= player_get_range(player); r++) {
		if (!bomb_explode_aux(bomb, map, x + r, y)) {
			break;
		}
		// cas vide
		if (bomb_explode_aux(bomb, map, x + r, y)) {
			a = x + r;
			window_display_image(sprite_get_bomb(4), a * SIZE_BLOC,
					y * SIZE_BLOC);
		}

	}

	//est
	for (int r = 0; r <= player_get_range(player); r++) {
		if (!bomb_explode_aux(bomb, map, x - r, y)) {
			break;
		}
		if (bomb_explode_aux(bomb, map, x - r, y)) {
			a = x - r;
			window_display_image(sprite_get_bomb(4), a * SIZE_BLOC,
					y * SIZE_BLOC);
		}

	}
	//sud
	for (int r = 0; r <= player_get_range(player); r++) {
		if (!bomb_explode_aux(bomb, map, x, y + r)) {
			break;
		}
		if (bomb_explode_aux(bomb, map, x, y + r)) {
			a = y + r;
			window_display_image(sprite_get_bomb(4), x * SIZE_BLOC,
					a * SIZE_BLOC);
		}

	}

	//nord
	for (int r = 0; r <= player_get_range(player); r++) {
		if (!bomb_explode_aux(bomb, map, x, y - r)) {
			break;
		}
		if (bomb_explode_aux(bomb, map, x, y - r)) {
			a = y - r;
			window_display_image(sprite_get_bomb(4), x * SIZE_BLOC,
					a * SIZE_BLOC);
		}

	}
}

void monster_attack_player(struct monster* monster, struct player* player) {
	int x = monster_get_x(monster);
	int y = monster_get_y(monster);
	int x1 = player_get_x(player);
	int y1 = player_get_y(player);
	if (x == x1 && y == y1) {
		player_dec_life(player);
	}
}

void game_pause(struct game * game, int time) {
	struct map* map = level_get_curr_map(game_get_curr_level(game));
	struct list* list_bomb = map_get_list_bomb(map);
	struct list_element * bomb = list_get_first(list_bomb);

	SDL_Event event;
	int resume = 0;
	int delay;

	while (!resume) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p)
				resume++;
		}
	}

	delay = SDL_GetTicks() - time;

	while (bomb != NULL) {
		struct bomb * bomb1 = list_get_value(bomb);
		bomb_delay(bomb1, delay);
		bomb = list_get_next(bomb);
	}

}

struct map* get_map(int n_curr_level, int next_map) {
	//char dest [256]="data/map_0_1";
	char buf1[12];
	char path[] = "data/map_";
	snprintf(buf1, sizeof(buf1), "%d", n_curr_level);
	snprintf(buf1, sizeof(buf1), "%s%d", path, n_curr_level);
	strcat(buf1, "_");
	char dest[20];
	snprintf(dest, sizeof(dest), "%d", next_map);
	snprintf(dest, sizeof(dest), "%s%d", buf1, next_map);
	//dest = "data/map_(n_curr_level)_(next_map)"


	FILE* file = fopen(dest, "r"); // ouvrir en lecture


	int x = 12;
	int y = 12;
	int i = 0;
	int j = 0;
	int read;
	struct map* map = map_new(x, y);

	char* map1 = malloc(x * y); // we create a table of (width * height) cases
	// double loop of reading
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			fscanf(file, "%d ", &read);
			map1[i * x + j] = (char) read;

		}
		// all the int x read // on est arrivés a la fin de la ligne on veut sauter une ligne
		fscanf(file, "\n");
	}
	map_init_list_bomb(map);
	map_init_list_monster(map);
	char* grid = map_get_grid(map);
	for (int i = 0; i < x * y; i++) {
		grid[i] = map1[i];



	}
	fclose(file);

	return map;
}

void level_next(struct game* game) {
	game->n_curr_level++;
	level_free(game->curr_level);
	game->curr_level = (level_get_level(game->n_curr_level));
}

void map_next(struct game* game, char doordirection1){

	struct map* map = level_get_curr_map(game_get_curr_level(game));
	struct player* player = game_get_player(game);
	struct level* level = game_get_curr_level(game);

	// on supprime la liste des bombes et celle des monsters
	struct list * listbomb = map_get_list_bomb(map);
	struct list * listmonster = map_get_list_monster(map);
	struct list_element * monster = list_get_first(listmonster);
	struct list_element * bomb = list_get_first(listbomb);

	while (monster != NULL) {
		struct monster * monster1 = list_get_value(monster);
		list_remove(listmonster, monster1);
		monster = list_get_next(monster);
	}

	while (bomb != NULL) {
		struct bomb * bomb1 = list_get_value(bomb);
		list_remove(listbomb, bomb1);
		bomb = list_get_next(bomb);
	}

	level_set_curr_map(level, doordirection1);
	for (int i = 0; i < 7; i++) {

	}

	map = level_get_curr_map(game_get_curr_level(game));
	for (int i = 0; i < 7; i++) {
		doorx[i] = 30;
		doory[i] = 30;
		nextmap[i] = 30;
		closedoor[i] = 30;
	}
	door_from_map(map);

	player_from_map(player, map);

	//liste de monstres a jour
	monster_from_map(map);
	flag_from_map(map);
}

int game_save(struct game * game) {
	FILE * file = NULL;
	FILE * file0= NULL;
	FILE * file1= NULL;
	FILE * file2= NULL;
	int i, j;
	int x = 12;
	int y = 12;
	int nb_map = level_get_nb_map(game_get_curr_level(game));

	struct player * player = game_get_player(game);

	file = fopen("data/savegame.txt", "w"); // Open write mode
	file0 = fopen("data/map_50_0", "w");
	file1 = fopen("data/map_50_1", "w");
	file2 = fopen("data/map_50_2", "w");

	if (file == NULL) {
		printf("Error file not created");
		return 0;
	}

	fprintf(file, "%d ", player_get_nb_bomb(player));
	fprintf(file, "%d ", player_get_range(player));
	fprintf(file, "%d ", player_get_life(player));
	fprintf(file, "%d ", player_get_key(player));
	fprintf(file, "%d ", game_get_n_curr_level(game));
	fprintf(file,"%d ",  level_get_n_cur_map(game_get_curr_level(game)));
	fprintf(file,"%d ",  level_get_nb_map(game_get_curr_level(game)));

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			fprintf(file0, "%d ",map_get_compose_type(level_get_map(game_get_curr_level(game),0), j, i));
		}
		fprintf(file0, "\n");
	}
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			fprintf(file1, "%d ",map_get_compose_type(level_get_map(game_get_curr_level(game),1), j, i));
		}
		fprintf(file1, "\n");
	}

	if(nb_map>2){
	for (i = 0; i < x; i++) {
			for (j = 0; j < y; j++) {

				fprintf(file2, "%d ",map_get_compose_type(level_get_map(game_get_curr_level(game),2), j, i));
			}
			fprintf(file2, "\n");
		}
	}

	fclose(file);
	fclose(file0);
	fclose(file1);
	fclose(file2);
	return 1;
}

void load_game_saved(struct game* game) {
	FILE * file = NULL;
	int nb_bomb, range, life,key;
	int n_curr_level,nb_map,curr_map;


	file = fopen("data/savegame.txt", "r"); // Open read mode

	fscanf(file, "%d ", &nb_bomb);
	fscanf(file, "%d ", &range);
	fscanf(file, "%d ", &life);
	fscanf(file, "%d ", &key);
	fscanf(file, "%d ", &n_curr_level);
	fscanf(file, "%d ", &curr_map);
	fscanf(file, "%d ", &nb_map);
	printf("\n%d", n_curr_level);

	level_free(game->curr_level);
	game->curr_level = load_level(nb_map,curr_map);
	game->n_curr_level = n_curr_level;
	player_free(game->player);
	game->player = player_init(nb_bomb, range, life, key);

	map_next(game, curr_map);

}



