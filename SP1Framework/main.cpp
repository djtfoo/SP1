// This is the main file to hold everything together
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "Framework\timer.h"
#include "Framework\console.h"
#include "game.h"
#include "menu.h"

using std::vector;
using std::string;
using std::ifstream;

//main loop declaration
void mainLoop( void );

//storyline
void storyline();

//defining the game state
Sequence seq = Menu;
bool exitGame = false;

//Console object
Console g_Console(80, 25, "Escapee");

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
	storyline();
	//g_Console.setConsoleFont(0, 25, L"Raster");
    gameLoop();  // main loop
    shutdown();  // do clean up, if any. free memory.
    
    return 0;
}

//looping for switch to check if the player is not inputing 'ESC' for exit
//Display according to what the user inputs with the case switching
//WeiMin, input edited by Jing Ting
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
            case Play : displayGame();
                seq = Menu;
                ElapsedTime = 0.0;
                break;
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
            case Options : options();
                ElapsedTime = 0.0;
                break;
            case Exit : displayExit();
                break;
        }
    }
}