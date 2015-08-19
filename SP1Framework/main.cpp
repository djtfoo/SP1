// This is the main file to hold everything together

#include "Framework\timer.h"
#include "Framework\console.h"
#include "game.h"
#include "menu.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

CStopWatch g_Timer;                            // Timer function to keep track of time and the frame rate
bool g_bQuitGame = false;                    // Set to true if you want to quit the game
const unsigned char gc_ucFPS = 100;                // FPS of this game
const unsigned int gc_uFrameTime = 1000 / gc_ucFPS;    // time for each frame

//main loop declaration
void mainLoop( void );

//Console object
Console g_Console(80, 25, "SP1 Framework");

// TODO:
// Bug in waitUntil. it waits for the time from getElapsedTime to waitUntil, but should be insignificant.

// main function - starting function
// You should not be modifying this unless you know what you are doing
int main( void )
{
    init();      // initialize your variables
    gameLoop();  // main loop
    shutdown();  // do clean up, if any. free memory.
    
    return 0;
}

void gameLoop()
{
    Sequence seq = Menu;

    while (seq != Exit)
    {

		displayMenu(seq);

        switch(seq)
        {
            case Play : displayGame(); break;
            case Options : displayOptions(); break;
            case Exit : displayExit(); break;
        }

		if (seq != Menu && seq != Exit) {
			seq = Menu;
		}
    }
}

void userInput (Sequence &s) //If s is modified, seq is modified as well
{
    int input;
    cin >> input;
    //Cannot cin>>s cause it doesn't take in
    s = static_cast<Sequence>(input);

}

void displayMenu(Sequence &s)
{
	//use g_Console.writeToBuffer
    clearScreen();
	COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2;
    g_Console.writeToBuffer(c, "MENU", 0x03);
	c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press '1' to play Game", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press '4' to go Options", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
	g_Console.flushBufferToConsole();

	userInput(s);
	
}


void displayInstructions()
{

}

void displayHighscore()
{

}

void userInputOPT(SequenceOPT &s)
{
    int input1;
    cin >> input1;

    s = static_cast<SequenceOPT>(input1);
}

void displayOptions() {

	SequenceOPT s = OptionsMenu;

	while (s != Back) {

		//use g_Console.writeToBuffer
		clearScreen();

		COORD c = g_Console.getConsoleSize();
		c.Y /= 3;
		c.X = c.X / 2;
		g_Console.writeToBuffer(c, "OPTIONS", 0x03);
		c.Y += 1;
		c.X = g_Console.getConsoleSize().X / 2 - 20;
		g_Console.writeToBuffer(c, "Press '1' for Sound", 0x09);
		c.Y += 1;
		c.X = g_Console.getConsoleSize().X / 2 - 9;
		g_Console.writeToBuffer(c, "Press '2' to go back to Main Menu", 0x09);
		g_Console.flushBufferToConsole();

		userInputOPT(s);

		switch(s)
		{
			case Sound : displaySound(); break;
			case Back : s = Back; break;
		}

		if (s != OptionsMenu && s != Back) {
			s = OptionsMenu;
		}

	}

}

void displaySound(){

	clearScreen();
	COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2;
    g_Console.writeToBuffer(c, "EDIT SOUND HERE", 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press '1' to switch on sound", 0x09);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press '2' to switch off sound", 0x09);
	g_Console.flushBufferToConsole();

	//userInputSOUND();
	int i;
	cin >> i;

}



void displayExit()
{

}

//--------------------------------------------------------------
// Purpose  : This main loop calls functions to get input, 
//            update and render the game at a specific frame rate
//            You should not be modifying this unless you know what you are doing.
// Input    : void
// Output   : void
//--------------------------------------------------------------
void displayGame( void )
{
    g_Timer.startTimer();    // Start timer to calculate how long it takes to render this frame
    while (!g_bQuitGame)      // run this loop until user wants to quit 
    {        
        getInput();                         // get keyboard input
        update(g_Timer.getElapsedTime());   // update the game
        render();                           // render the graphics output to screen
        g_Timer.waitUntil(gc_uFrameTime);   // Frame rate limiter. Limits each frame to a specified time in ms.      
    }    
}
