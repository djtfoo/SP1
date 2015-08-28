#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include "Framework\console.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;


// Enumeration to store the control keys that your game will have
enum EKEYS
{
    //for gameplay
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE,
	K_ONE,
	K_TWO,
    //for keying in name for highscore
    K_A,
    K_B,
    K_C,
    K_D,
    K_E,
    K_F,
    K_G,
    K_H,
    K_I,
    K_J,
    K_K,
    K_L,
    K_M,
    K_N,
    K_O,
    K_P,
    K_Q,
    K_R,
    K_S,
    K_T,
    K_U,
    K_V,
    K_W,
    K_X,
    K_Y,
    K_Z,
    K_ENTER,
    K_BACKSPACE,
    K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_GAME,
    S_WIN,
	S_PAUSE,
    S_COUNT
};

enum GAMELEVELS
{
	M1 = 1,
	M2,
	M3,
	M4,
	M5,
	M6,
	MAX_LEVEL
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
};

enum Direction {
    left = 1,
    right,
    down,
    up,
    MAX_DIRECTION
};

struct Enemy
{
    COORD m_Enemy;
    Direction d;
};

struct Traps
{   
	COORD Loc;
};

struct ExitTeleporter
{
	COORD own_Loc;
	COORD warp_Loc;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

//PlaySound
extern bool playmusic;

void splashScreenWait();    // waits for time to pass in splash screen
void gameplay();            // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();	// checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderEnemy();
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderCounters();      // renders item counter, level count. for gameplay() only
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game

//Enemy
void moveEnemy(Enemy& g_Enemy);
void enemyCollisionWithPlayer(Enemy g_Enemy);

//generating map - Jing Ting
void levelInit();
void maze1(int& rows, int& cols);
void maze2(int& rows, int& cols);
void maze3(int& rows, int& cols);
void maze4(int& rows, int& cols);
void maze5(int& rows, int& cols);
void maze6(int& rows, int& cols);
void mapgenerator(int rows, int cols);
void randomiseTeleporters(int rows, int cols);

//checking things on map
void checkTrap();
void PickUpItems();
void exitLevel();

//finish the game - Jing Ting
void clearGame();
void processNameInput(char * name);
void renderClearGame();
void renderText();
void renderNameInput(char * name);

//pause the game - Glennda
void pauseGame();
void renderPauseGame();
void processPauseInput();

#endif // _GAME_H