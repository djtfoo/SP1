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

enum SequenceOPT{
	OptionsMenu,
	Sound,			//1
	Back,			//2
	MAX_SEQUENCEOPT,
};

void gameLoop();
void displayMenu();
void displayGame();
void displayInstructions();
void displayOptions();
void displayHighscore();
void displayExit();

//options
void options();
void displayOptions();
void displaySound();

//highscore
struct highScore
{
	double time;
	std::string name;
};
void toCpp();

//declaring game state
extern Sequence seq;
extern bool exitGame;

//PlaySound
extern bool playmusic;

//user input
void userInput();   //to get input
void addTime();     //this is to set a buffer between detection of input for different menu screens, or else the previous input will be read by the next userInput()
extern bool inputDetected;  //when a key input has been detected, this bool changes to break out of the loop and move on to the next game state
extern double BounceTime;   //this time is set to prevent the previous input from being read by the next userInput()
extern double ElapsedTime;  //time that has passed from starting of detecting user input for that current game state

//processing user input
void processInputMenu(Sequence &s); //to process the input when in main menu
void processInputBack(Sequence &s); //to process the input when in instructions and high score, to return to main menu
void processInputOptions(SequenceOPT &s); //to process the input when in options
void processInputSound(SequenceOPT &s); //to process the input when in sound options