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

//looping for switch to check if the player is not inputing 'ESC' for exit
//Display according to what the user inputs with the case switching
void gameLoop()
{

    while (!exitGame)
    {
        switch(seq)
        {
            //Display the menu once game starts
            case Menu : displayMenu();
                while (!inputDetected) {
                    userInput();
                    processInputMenu(seq);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            //Display the game once player presses 1
            case Play : displayGame(); seq = Menu; ElapsedTime = 0.0; break;
            //Display instructions page once player presses 2
			case Instructions : displayInstructions();
                while (!inputDetected) {
                    userInput();
                    processInputBack(seq);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            //Display high score page once player presses 3
			case HighScore : displayHighscore();
                while (!inputDetected) {
                    userInput();
                    processInputBack(seq);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            //Display options page once player presses 4
            case Options : options(); break;
            //Exit when player presses 5
            case Exit : displayExit(); break;
        }
    }
}