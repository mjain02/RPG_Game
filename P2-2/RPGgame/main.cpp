// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
//int update(MapItem* item, int dir);
void draw_game (int init);
void init_maps(int map);
static void init_map1();
static void init_map2();
int main ();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;  //key of the player
    int found_key; //0 for no, 1 for yes
    int fought_monster; //0 for no, 1 for yes - collect key after fighting monster - button triggered
    int choose_spell; //0 for no, 1 for yes
    int strength;
    // You can add other properties for the player here
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define OMNI_BUTTON 7
#define GO_UP_FAST 8
#define GO_RIGHT_FAST 9
#define GO_DOWN_FAST 10
#define GO_LEFT_FAST 11
bool omni = false;
int monster_position = 13;
bool movement = true; //true - down, false- up
int boss_lives = 2;

int get_action(GameInputs inputs)
{
    if (inputs.b1 == 1) //button 1 is pressed
        return ACTION_BUTTON;
    if (inputs.b2 == 1) //omni mode button
        return OMNI_BUTTON;
    if (inputs.b3 == 1) //menu
        return MENU_BUTTON;
    if (inputs.ax > 0.1) 
        return GO_RIGHT;
    if (inputs.ay > 0.1)
        return GO_UP;
    if (inputs.ax < -0.1)
        return GO_LEFT;
    if (inputs.ay < -0.1)
        return GO_DOWN;  
    if (inputs.ax > 0.4) 
        return GO_RIGHT_FAST;
    if (inputs.ay > 0.4)
        return GO_UP_FAST;
    if (inputs.ax < -0.4)
        return GO_LEFT_FAST;
    if (inputs.ay < -0.4)
        return GO_DOWN_FAST;  
    return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2

#define UP 3
#define DOWN 4
#define LEFT 5
#define RIGHT 6
#define UP_FAST 7
#define DOWN_FAST 8
#define LEFT_FAST 9
#define RIGHT_FAST 10
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    void beep();
    switch(action)
    {
        int update(MapItem* item, int dir);
        MapItem* item;
        case GO_UP:   
            item = get_north(Player.px, Player.py);
            return update(item, UP);
        case GO_LEFT:
            item = get_west(Player.px, Player.py);
            return update(item, LEFT);            
        case GO_DOWN:   
            item = get_south(Player.px, Player.py);
            return update(item, DOWN);
        case GO_RIGHT: 
            item = get_east(Player.px, Player.py);
            return update(item, RIGHT);
        case GO_UP_FAST:
            item = get_north(Player.px - 1, Player.py);
            return update(item, UP_FAST);
        case GO_DOWN_FAST:
            item = get_south(Player.px + 1, Player.py);
            return update(item, DOWN_FAST);
        case GO_LEFT_FAST:
            item = get_west(Player.px, Player.py - 1);
            return update(item, LEFT_FAST);
        case GO_RIGHT_FAST:
            item = get_east(Player.px, Player.py + 1);
            return update(item, RIGHT_FAST);
        case ACTION_BUTTON: //button 1
            if (Player.choose_spell == 1) { //fire
                beep();
                add_fire();
                wait_ms(200);
                if (boss_lives == 2) {
                     Player.fought_monster = 0; //still alive 
                     boss_lives--;
                }else {
                    Player.fought_monster = 1;
                    add_nothing(20, 17);
                    uLCD.cls();
                }
            } else if (Player.choose_spell == 2) {//water
                beep();
                add_water();
                wait_ms(200);
                if (boss_lives == 2) {
                     Player.fought_monster = 0; //still alive 
                     boss_lives--;
                }else {
                    Player.fought_monster = 1;
                    add_nothing(20, 17);
                    uLCD.cls();
                }
            }
            break;
        case MENU_BUTTON: //button 3
            //clear screen
            //write menu
            //build map based on choices
            
            break;
        case OMNI_BUTTON: //button 2
            omni = !omni;
            if (omni) {
            uLCD.locate(0,0);
            uLCD.printf("Omnipotent mode");
            wait_ms(1500);
            uLCD.cls(); }
            break;
        default:       
            
            break;
    }
  /**  if(get_map() == 1) {
        pc.printf("add monster\n");
        for(int i = 15; i <=20;) {
            draw_nothing(20, i-1);
            pc.printf("erase og char\n");
            add_monster(20, i);
            pc.printf("monster added\n");
            if(i==20) {
                pc.printf("go back up\n");
                draw_nothing(20,i);
            i=14; }
        }
    }*/
    return FULL_DRAW;
}

int update(MapItem* item, int dir) {
    if (item == NULL) {
        switch(dir) {
            case UP: Player.y = Player.y - 1; break;
            case DOWN: Player.y = Player.py + 1; break;
            case LEFT: Player.x = Player.px - 1; break;
            case RIGHT: Player.x = Player.x + 1; break;
            case UP_FAST: add_rock(Player.px, Player.py); Player.y = Player.py - 2; break;
            case DOWN_FAST: add_rock(Player.px, Player.py); Player.y = Player.py + 2; break;
            case LEFT_FAST: add_rock(Player.px, Player.py); Player.x = Player.px - 2; break;
            case RIGHT_FAST: add_rock(Player.px, Player.py); Player.x = Player.x + 2; break;
        }
    } else if (item->type == DOOR) {
        //go through door
        //set active map to 1   
        if(get_map() == 0) {
            set_active_map(1);
            init_maps(1); 
            Player.x = 10;
            Player.y = 10;
            speech("Cast spells to", "fight the monster");
            wait_ms(1000);
            speech("Find the key.", "Find exit.");
            wait_ms(1500);
            draw_game(true);
            }  else {
            set_active_map(0);
            init_maps(0); 
            Player.x = 20;
            Player.y = 20;
            draw_game(true);
            }
               //draw new map
    } else if (item->type == EXIT && Player.found_key == 1) {
        return GAME_OVER;
    } else if (item->type == FIRE) {
        speech("You have chosen ", "the fire spell");
        Player.choose_spell = 1;
    } else if (item->type == WATER) {
        speech("You have chosen ", "the water spell");
        Player.choose_spell = 2;
    } else if (item->type == WALL && !omni) {
        Player.strength = Player.strength - 50;
    } else if (item->type == MONSTER && Player.fought_monster == 0) {
        Player.strength = Player.strength - 100;
    } else if (item->walkable || omni) {
        Player.x = Player.px + 1; //y stays the same
    }
    MapItem* ifKey = get_here(Player.x, Player.y);
    if(ifKey->type == KEY){
        if(Player.fought_monster == 1) {
           Player.found_key = 1;
           speech("Found the key","");
        }
    }
    
    return FULL_DRAW;
}

/**void move_monster(int pos) {
     draw_nothing(20, pos-1);
     draw_nothing(20, pos+1);
     add_monster(20, pos);
} */

void beep() {
    speaker.period(1.0/500.0); // 500hz period  
    speaker =0.25; //50% duty cycle - max volume
    wait(0.5);
    speaker=0.0; // turn off audio
}
/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    } 

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}

void init_maps(int map)
{
    if (map == 0) {
        init_map1();
    } else if (map == 1) {
        init_map2();
    }
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
static void init_map1()
{
    Map* map = set_active_map(0); //set the active map to map 1
    // "Random" plants    
    for(int i = map_width() + 5; i < map_area()-500; i += 45)
    {
        add_plant(i % map_width() + (i % 3), i / map_width());
    }
    
    //draw door
    add_door(25, 25);
    add_door(25, 26);
    add_door(26, 26);
    add_door(26, 25);
    
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    
    add_exit(30, 30);
    
    pc.printf("Walls done!\r\n");

    print_map(); //debugging
}


static void init_map2() 
{
    Map* map = set_active_map(1); //set the active map to map 2
    /**for(int i = map_width(); i < map_area(); i = i + 30) {
            add_rock(i % map_width() + (i % 15), i / map_height() + (i%5));
        
    }*/
    
    /**for(int i = 0; i < map_width(); i++) {
        for(int j = 0; i < map_height(); j++) {
            if (i%6 == 0) {
                add_rock(i, j);
            }
        }
    }*/
    add_rock(5,5);
    add_rock(10, 10);
    add_rock(5, 15);
    add_rock(10, 5);
    add_rock(20, 15);
    add_rock(15, 20);
    add_rock(24,18);
    
    add_door(25, 25);
    add_door(25, 26);
    add_door(26, 25);
    add_door(26, 26);
    
    add_waterspell(13, 15);
    add_firespell(13, 20);
    
    add_key(23, 18);
    if (Player.fought_monster == 0)
    add_monster(20, 17);
       
}
/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    // Initialize the maps
    while(1) {
        uLCD.locate(5, 8); //center of the screen
        uLCD.printf("START GAME");
        GameInputs next = read_inputs();
        if (next.b1 == 1) {
            beep();
            break;
        }
    }
    maps_init();
    init_maps(0); //draw first map
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.strength = 1000;
    // Initial drawing
    draw_game(true);
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actually do the game update:
        // 1. Read inputs  
        GameInputs in = read_inputs();      
        // 2. Determine action (get_action) 
        int action = get_action(in);       
        // 3. Update game (update_game)
        int state = update_game(action);
        // 3b. Check for game over       
        // 4. Draw frame (draw_game)
        
        if (state == GAME_OVER){
            uLCD.cls(); //clear screen
            uLCD.locate(5, 8); //center of the screen
            uLCD.printf("GAME OVER.");
            uLCD.locate(6, 10);
            uLCD.printf("You win!");
            break;
        } else if (Player.strength < 0) {
            uLCD.cls(); //clear screen
            uLCD.locate(5, 8); //center of the screen
            uLCD.printf("GAME OVER.");
            uLCD.locate(6, 10);
            uLCD.printf("You lose");
            break;
        }
        draw_game(true);
        
        /**if(get_map() == 1) {
            draw_nothing(20, monster_position); //clear the current tile position
            if(movement){ //down
                monster_position += 2;
            } else {
                monster_position -= 2; //up
            }
            if (monster_position >= 23 || monster_position <= 13) { //change direction
             movement = !movement;
            }
            void move_monster(int a);
            move_monster(monster_position);    
        } */
        uLCD.locate(0,15); 
        uLCD.printf("Position: (%d, %d)", Player.x, Player.y);
        uLCD.locate(0, 0);
        uLCD.printf("Strength: %d", Player.strength);
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}
