
#ifndef MAP_H_
#define MAP_H_

struct bomb;
struct monster;


typedef enum cell_type {
	CELL_EMPTY=0,
	CELL_GOAL,  // 1
	CELL_SCENERY, // 2
	CELL_PLAYER, // 3
	CELL_CASE,   // 4
	CELL_BONUS, // 5
	CELL_MONSTER, // 6
	CELL_BOMB, // 7
	CELL_DOOR, // 8
	CELL_KEY, // 9
	CELL_CLOSED_DOOR, // 10
	CELL_FLAG, // 11
} cell_type_t;

typedef enum bonus_type {
	BONUS_BOMB_RANGE_INC=1,
	BONUS_BOMB_RANGE_DEC, // 2
	BONUS_BOMB_NB_INC, // 3
	BONUS_BOMB_NB_DEC, // 4
	BONUS_LIFE, // 5
	BONUS_MONSTER // 6
} bonus_type_t;

enum scenery_type {
	SCENERY_STONE, // 0
	SCENERY_TREE, // 1
};

typedef enum compose_type {
	CELL_STONE = (SCENERY_STONE << 4) | CELL_SCENERY, // 0000 0010 -> 2
	CELL_TREE = (SCENERY_TREE << 4) | CELL_SCENERY, // 0001 0010 -> 18
    CELL_CASE_RANGEINC = (BONUS_BOMB_RANGE_INC << 4) | CELL_CASE, // 0001 0100 -> 20
    CELL_CASE_RANGEDEC = (BONUS_BOMB_RANGE_DEC << 4) | CELL_CASE, // 0010 0100 -> 36
	CELL_CASE_BOMBINC = (BONUS_BOMB_NB_INC << 4) | CELL_CASE, // 0011 0100 -> 52
    CELL_CASE_BOMBDEC = (BONUS_BOMB_NB_DEC << 4) | CELL_CASE, // 0100 0100 -> 68
    CELL_CASE_LIFE = (BONUS_LIFE << 4) | CELL_CASE, // 0101 0100 -> 84
    CELL_CASE_MONSTER = (BONUS_MONSTER << 4) | CELL_CASE, // 0110 0100 -> 100
    CELL_BONUS_RANGE_INC = (BONUS_BOMB_RANGE_INC << 4) | CELL_BONUS, // 0001 0101 -> 21
    CELL_BONUS_RANGE_DEC = (BONUS_BOMB_RANGE_DEC << 4) | CELL_BONUS, // 0010 0101 -> 37
    CELL_BONUS_BOMB_INC = (BONUS_BOMB_NB_INC << 4) | CELL_BONUS, // 0011 0101 -> 53
    CELL_BONUS_BOMB_DEC = (BONUS_BOMB_NB_DEC << 4) | CELL_BONUS, // 0100 0101 -> 69
    CELL_BONUS_LIFE = (BONUS_LIFE << 4) | CELL_BONUS, // 0101 0101 -> 85
}compose_type_t;

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);

// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the bomb list of the map
struct list* map_get_list_bomb(struct map* map);

// Return the monster list of the map
struct list* map_get_list_monster(struct map* map);

// Initialize the bomb list of the map
void map_init_list_bomb(struct map* map);

// Initialize the monster list of the map
void map_init_list_monster(struct map* map);

// Return the type of a cell
cell_type_t map_get_cell_type(struct map* map, int x, int y);

//Return the type of the bonus
enum bonus_type map_get_bonus_type(struct map* map, int x, int y);

// Return the complete type of a composed cell
enum cell_type map_get_compose_type(struct map* map, int x, int y) ;
char * map_get_grid(struct map* map);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, cell_type_t type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default 12x12 static map
struct map* map_get_default();

// Display the map on the screen
void map_display(struct map* map);

//Manage the bomb explosion
int bomb_explode_aux(struct bomb* bomb, struct map * map, int x, int y);



//Add the bomb to the bomb list of the map
void map_add_bomb(struct bomb* bomb,struct map* map);

//Add the monster to the monster list of the map
void map_add_monster(struct monster* monster, struct map* map);

//Load the bonus position from the map
void bonus_from_map(struct map* map);

//Load the door position from the map
void door_from_map(struct map* map);

//Load the flag position from the map
void flag_from_map(struct map* map) ;

//Get the type of bonus in a case and apply the consequence
void move_case_into_bonus(struct map* map,int x, int y );


#endif /* MAP_H_ */
