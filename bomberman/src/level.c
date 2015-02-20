#include <assert.h>
#include <stdlib.h>
#include <stddef.h> // NULL (macos)
#include <level.h>
#include <game.h>
#include <map.h>
#include <bomb.h>

struct level {
	struct map** maps; // array of the level's maps
	short nb_maps; // nb maps of the level
	short cur_map; // the current map
};

struct level* level_get_level(int num) {
	struct level* level = malloc(sizeof(*level));
	switch (num) {
	case 0:
		level->nb_maps = 2;
		level->cur_map = 0;
		level->maps = malloc(sizeof(*level->maps));
		level->maps[0] = get_map(num, 0);
		level->maps[1] = get_map(num, 1);
		return level;
		break;
	case 1:
		level->nb_maps = 2;
		level->cur_map = 0; //indice du tableau maps donc la map courante est ds maps[0]
		level->maps = malloc(sizeof(*level->maps));
		level->maps[0] = get_map(num, 0);
		level->maps[1] = get_map(num, 1);
		return level;
		break;
	case 2:
		level->nb_maps = 3;
		level->cur_map = 0; //indice du tableau maps donc la map courante est ds maps[0]
		level->maps = malloc(sizeof(*level->maps));
		level->maps[0] = get_map(num, 0);
		level->maps[1] = get_map(num, 1);
		level->maps[2]=get_map(num,2);
		return level;
		break;
	}
	return NULL;
}






// Charger le nouveau niveau

struct level* load_level(int nb_map, int curr_map) {
	int num=50;
	struct level* level = malloc(sizeof(*level));
	level->nb_maps = nb_map;
	level->cur_map = curr_map;
	level->maps = malloc(sizeof(*level->maps));

	for(int i=0;i<nb_map;i++){
	level->maps[i] = get_map(num, i);

	}
	return level;
}






struct map* level_get_curr_map(struct level* level) {
	return level->maps[level->cur_map];
}


short level_get_nb_map(struct level* level) {
	return level->nb_maps;
}

struct map* level_get_map(struct level* level, int num) {
	assert(num <= level->nb_maps);
	return level->maps[num];
}

void level_free(struct level* level) {
	for (int i = 0; i < level->nb_maps; i++)
		map_free(level->maps[i]);

	free(level->maps);
}

void level_display(struct level* level) {
	map_display(level->maps[level->cur_map]);
}

struct map ** level_get_array_map(struct level * level) {
	return level->maps;
}

void level_set_curr_map(struct level * level, short number_map) {
	level->cur_map = number_map;

}
short level_get_n_cur_map(struct level*level) {
	return level->cur_map;
}

