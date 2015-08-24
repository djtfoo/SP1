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
void displayMenu(Sequence &s);
void userInput(Sequence &s);
void displayGame();
void displayInstructions();
void displayOptions();
void displayHighscore();
void displayExit();
int input;

//options
void displayOptions();
void displaySound();

struct highScore
{
	double time;
	std::string name;
};

//highscore
void toCpp();