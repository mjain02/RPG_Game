#include "speech.h"
#include "map.h"
#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 * Use this file to draw speech bubbles, look at the uLCD libraries for printing
 * text to the uLCD (see graphics.cpp note for link)
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
 void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();


static void draw_speech_bubble()
{
    //add a rectangle to the bottom of the screen 
    uLCD.filled_rectangle(0, 90, 127, 127, BLACK); 
}

static void erase_speech_bubble()
{
     uLCD.filled_rectangle(0, 90, 127, 127, BLACK); //clear the text box to display the next lines of speech
}

 void draw_speech_line(const char* line, int which)
{
    if (which == 0) {
        uLCD.locate(1, 12); //set cursor position
    } else {
        uLCD.locate(1, 14); //cursor position
    }
    uLCD.printf(line); //puts the string at the cursor position
}

static void speech_bubble_wait()
{
    while(1) {
    GameInputs next = read_inputs();
    if (next.b1 == 1)
        break;
    }
}

void speech(char* line1, char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}

void long_speech(char* lines[], int n)
{
    for(int i = 0; i < (sizeof lines); ++i) {
        int line;
        if(i%2==0) line = 0;
        else line = 1;
        draw_speech_bubble();
        draw_speech_line(lines[i], line);
        speech_bubble_wait();
    }
}
