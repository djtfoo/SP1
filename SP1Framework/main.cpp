// This is the main file to hold everything together

#include "Framework\timer.h"
#include "Framework\console.h"
#include "game.h"
#include "menu.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::ifstream;

//main loop declaration
void mainLoop( void );

//defining the game state
Sequence seq = Menu;
bool exitGame = false;

//Console object
Console g_Console(80, 25, "SP1 Framework");

//user input variables
bool inputDetected = false;
double BounceTime = 0.3;
double ElapsedTime = 0.0;

// TODO:
// Bug in waitUntil. it waits for the time from getElapsedTime to waitUntil, but should be insignificant.

// main function - starting function
// You should not be modifying this unless you know what you are doing
int main( void )
{
	//g_Console.setConsoleFont(0, 25, L"Raster");
    gameLoop();  // main loop
    shutdown();  // do clean up, if any. free memory.
    
    return 0;
}

void gameLoop()
{

    while (!exitGame)
    {
        switch(seq)
        {
            case Menu : displayMenu();
                while (!inputDetected) {
                    userInput();
                    processInputMenu(seq);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            case Play : displayGame(); seq = Menu; ElapsedTime = 0.0; break;
			case Instructions : displayInstructions();
                while (!inputDetected) {
                    userInput();
                    processInputBack(seq);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
			case HighScore : displayHighscore();
                while (!inputDetected) {
                    userInput();
                    processInputBack(seq);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            case Options : options(); break;
            case Exit : displayExit(); break;
        }
    }
}