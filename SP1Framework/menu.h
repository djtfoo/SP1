#include "Framework\timer.h"
#include "Framework\console.h"

//WeiMin
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
    MK_UP,
    MK_DOWN,
    MK_RIGHT,
    MK_LEFT,
    MK_ENT,
    MK_SPACE,
    K_MAX
};

enum SequenceOPT {
	OptionsMenu,
	Sound,			//1
    Character,      //2
	Back,			//3
	MAX_SEQUENCEOPT,
};

//WeiMin
void gameLoop( void );
void displayMenu( void );
void displayGame( void );
void displayInstructions( void );
void displayOptions( void );
void displayHighscore( void );
void displayExit( void );

//options
void options( void );
void displayOptions( void );
void displaySound( void );
void displayChar( void );

//highscore
struct highScore
{
	double time;
	std::string name;
};
void toCpp( void );

//declaring game state
extern Sequence seq;
extern bool exitGame;

//user input
void userInput( void );   //to get input
void addTime( void );     //this is to set a buffer between detection of input for different menu screens, or else the previous input will be read by the next userInput()
extern bool inputDetected;  //when a key input has been detected, this bool changes to break out of the loop and move on to the next game state
extern double BounceTime;   //this time is set to prevent the previous input from being read by the next userInput()
extern double ElapsedTime;  //time that has passed from starting of detecting user input for that current game state

//processing user input
void processInputMenu(Sequence &s); //to process the input when in main menu
void processInputBack(Sequence &s); //to process the input when in instructions and high score, to return to main menu
void processInputOptions(SequenceOPT &s); //to process the input when in options
void processInputSound(SequenceOPT &s); //to process the input when in sound options
void processInputChar(SequenceOPT &s); //to process input when in character icon change options