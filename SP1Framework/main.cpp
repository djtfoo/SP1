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

        switch(seq)
        {
			case Menu : displayMenu(seq); break;
            case Play : displayGame(); break;
            case Options : displayOptions(); break;
            case Exit : displayExit(); break;
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
	COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
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

void displayOptions() {
	//use g_Console.writeToBuffer
	cout << "Options:" << endl;
	for (SequenceOPT s = Sound; s != MAX_SEQUENCE; s = static_cast<SequenceOPT>(s+1)) 
	{ 
		cout << "Option " << s << " " << option[static_cast<int>(s)-1] << endl;
	}
}

void displaySound(){
	cout << "Adjust your sound here:" << endl;
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
