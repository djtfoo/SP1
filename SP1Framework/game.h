#ifndef _GAME_H
#define _GAME_H

//MR SIM
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
    //for gameplay_pause
	K_ONE,
	K_TWO,
    K_THREE,
    K_FOUR,
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
    S_DEATH,
    S_PAUSE,
    S_PAUSEONE,
    S_PAUSETWO,
    S_COUNT
};

//JingTing
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

// Enumeration to store the directions of the game enemy
enum Direction {
    left = 1,
    right,
    down,
    up,
    MAX_DIRECTION
};

// Struct for the game enemy - wei min
struct Enemy
{
    COORD m_Enemy;  
    Direction d;
};

struct Traps
{   
	COORD Loc;
};

//Store co-ordinates - wei min
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


void splashScreenWait( void );    // waits for time to pass in splash screen
void gameplay( void );            // gameplay logic
void moveCharacter( void );       // moves the character, collision detection, physics, etc
void processUserInput( void );	// checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen( void );         // clears the current screen and draw from scratch 
void renderSplashScreen( void );  // renders the splash screen
void renderGame( void );          // renders the game stuff
void renderMap( void );           // renders the map to the buffer first
void renderCharacter( void );     // renders the character into the buffer
void renderEnemy( void );
void renderFramerate( void );     // renders debug information, frame rate, elapsed time, etc
void renderCounters( void );      // renders item counter, level count. for gameplay() only
void renderToScreen( void );      // dump the contents of the buffer to the screen, one frame worth of game

//Enemy
void moveEnemy(Enemy& g_Enemy); //shania
void enemyCollisionWithPlayer(Enemy g_Enemy); //wei min

//generating map - JingTing
void levelInit( void );
void maze1(int& rows, int& cols);
void maze2(int& rows, int& cols);
void maze3(int& rows, int& cols);
void maze4(int& rows, int& cols);
void maze5(int& rows, int& cols);
void maze6(int& rows, int& cols);
void mapgenerator(int rows, int cols);
void randomiseTeleporters(int rows, int cols);

//checking things on map
void checkTrap( void );
void activateTrap( void );
void PickUpItems( void );
void exitLevel( void );

//finish the game - JingTing
void clearGame( void );
void processNameInput(char * name);
void renderClearGame( void );
void renderText( void );
void renderNameInput(char * name);

//WeiMin
void renderDeath(void);
void processDeath(void);

//pause the game - Glennda
void pauseGame( void );
void pauseOne( void );
void pauseTwo( void );
void renderPauseGame( void );
void renderPauseSound( void );
void renderPauseChar( void );
void processPauseInput( void );
void processPauseSound( void );
void processPauseChar(char * arr, char * rra);



#endif // _GAME_H