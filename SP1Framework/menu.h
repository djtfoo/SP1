#include "Framework\timer.h"
#include "Framework\console.h"

enum Sequence 
{
    Menu,
    Play,             //1
    Instructions,     //2
    HighScore,        //3
    Options,          //4
    Exit,             //5
    MAX_SEQUENCE      //Last (6)
};

enum MENUKEYS
{
    //for gameplay
    K_1,
    K_2,
    K_3,
    K_4,
    K_ENT,
    K_ESC,
    K_MAX,
};

const char* menu[] = 
{
   "Play",
   "Instructions",
   "High Score",
   "Options",
   "Exit"
};

enum SequenceOPT{
	OptionsMenu,
	Sound,			//1
	Back,			//2
	MAX_SEQUENCEOPT,
};

const char* option[] = 
{
   "Sound",
   "Back",
};

void gameLoop();
void displayMenu();
void displayGame();
void displayInstructions();
void displayOptions();
void displayHighscore();
void displayExit();

//options
void displayOptions();
void displaySound();

//highscore
struct highScore
{
	double time;
	std::string name;
};
void toCpp();

//change this
void userInputSOUND();

//user input
void userInput(); //to get input
void processInputMenu(Sequence &s); //to process the input when in main menu
void processInputOptions(SequenceOPT &s); //to process the input when in options
void processInputSound(); //to process the input when in sound options
void processInputBack(Sequence &s); //to process the input when in instructions and high score, to return to main menu