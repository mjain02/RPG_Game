#include "map.h"
#include "hash_table.h"
#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 * In this file you need to define how the map will be structured. IE how will
 * you put values into the map, pull them from the map. Remember a "Map" variable
 * is a hashtable plus two ints (see below) 
 * You will have more than one map variable, one will be the main map with it's own hashtable.
 * Then you'll have a second map with another hashtable
 * You should store objects into the hashtable with different properties (spells
 * etc)
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map[2];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int x, int y) {
    return map_width()*x + y;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned hash(unsigned key)
{
    unsigned hash = key%10;
    return hash;
}

void maps_init()
{
    Map* map1 = &map[0];
    map1->items = createHashTable(hash, 10);
    map1->w = 50;
    map1->h = 50;
    Map* map2 = &map[1];
    map2->items = createHashTable(hash, 10);
    map2->w = 30;
    map2->h = 30;
}

Map* get_active_map()
{
    // select the map from the array
    return &(map[active_map]);
}

Map* set_active_map(int m)
{
    active_map = m;
    return &(map[active_map]);
}

void print_map() //debugging
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P', 'R', 'D', 'K', 'M', 'E', 'W', 'F'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return map[active_map].w;
}

int map_height()
{
    return map[active_map].h;
}

int map_area()
{
    return (map_width()*map_height());
}

MapItem* get_north(int x, int y)
{
    if (y-1 != -1) {
        unsigned int key = XY_KEY(x,y-1);
        MapItem* item = (MapItem*) getItem(map[active_map].items, key);
        return item;
    } else {
        return NULL;
    }
}

MapItem* get_south(int x, int y)
{
    if (y+1 != map_height()) {
        unsigned int key = XY_KEY(x,y+1);
        MapItem* item = (MapItem*) getItem(map[active_map].items, key);
        return item;
    } else {
        return NULL;
    }
}

MapItem* get_east(int x, int y)
{
    if (x+1 != map_width()) {
        unsigned int key = XY_KEY(x+1,y);
        MapItem* item = (MapItem*) getItem(map[active_map].items, key);
        return item;
    } else {
        return NULL;
    }
}

MapItem* get_west(int x, int y)
{
    if (x-1 != -1) {
        unsigned int key = XY_KEY(x-1,y);
        MapItem* item = (MapItem*) getItem(map[active_map].items, key);
        return item;
    } else {
        return NULL;
    }
}

MapItem* get_here(int x, int y) //get item at position x,y
{
    unsigned int key = XY_KEY(x,y);
    MapItem* here = (MapItem*)getItem(map[active_map].items, key);
    return here;
}


void map_erase(int x, int y) //remove item at position x,y - no return
{
    unsigned int key = XY_KEY(x,y);
    deleteItem(map[active_map].items, key);
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        pc.printf("malloc\n");
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        pc.printf("malloc done\n");
        pc.printf("%p\n", w1);
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        ;
        
        pc.printf("set items\n");
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        pc.printf("direction set\n");
        void* val = insertItem(get_active_map()->items, key, w1);
        pc.printf("inserted\n");
        if (val) free(val); // If something is already there, free it
        pc.printf("frred\n");
    }
    pc.printf("drawn walls\n");
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_nothing(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NOTHING;
    w1->draw = draw_nothing;
    w1->walkable = true;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_rock(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ROCK;
    w1->draw = draw_rock;
    w1->walkable = false;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = true;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

int get_map() {
 return active_map;    
}

void add_key(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = true;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_monster(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = MONSTER;
    w1->draw = draw_monster;
    w1->walkable = false;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_exit(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = EXIT;
    w1->draw = draw_exit;
    w1->walkable = true;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void  add_waterspell(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = WATER;
    w1->draw = draw_water;
    w1->walkable = true;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void  add_firespell(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = FIRE;
    w1->draw = draw_fire;
    w1->walkable = true;
    
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}