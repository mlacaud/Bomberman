#ifndef LEVEL_H_
#define LEVEL_H_

#include <map.h>

struct level;

// return level number num
struct level*  level_get_level(int num);
//Charger le nouveau niveau
struct level* load_level(int nb_map, int curr_map) ;

// return current map of the level
struct map* level_get_curr_map(struct level* level);

// return the number of the map
short level_get_nb_map(struct level* level);

// return map number num of the level
struct map*  level_get_map(struct level* level, int num);

// free the level
void  level_free(struct level* level);

// display the level
void  level_display(struct level* level);

// free level struct
void   level_free(struct level* level);

// Return the array of the maps
struct map ** level_get_array_map(struct level * level);

//Set the current map
void level_set_curr_map(struct level * level, short number_map);

//Return the number of the current map
short level_get_n_cur_map(struct level*level);

#endif /* LEVEL_H_ */
