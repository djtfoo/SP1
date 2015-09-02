// This is the main file for the game logic and function
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "Framework\console.h"
#include "Framework\timer.h"
#include "menu.h"
#include "game.h"

using std::ofstream;
using std::string;
using std::vector;

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

bool showTeleporter = false;

// Game specific variables here
SGameChar   g_sChar;
vector<Enemy> enemyvec;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
double  g_dBounceTimeEnemy;
double	playTime;		//to record the gameplay time only
double	BufferTime;		//for splashscreen

//map rendering
char ** maze = 0;
int rows = 0;
int cols = 0;
GAMELEVELS levelCount;
bool levelClear;
int ItemCounter = 0;
int MaxItemCount = 0;

//Teleporter
ExitTeleporter Tel;

//high score name
char name[11];
char * pointer = 0;
int i = 0;

//PlaySound - victory screen
bool victoryplaymusic = true;

EKEYS prevKeyPressed = K_ESCAPE;
double bouncePrevKey = 0.0;

//pause menu
EGAMESTATES gamestate[] = {
    S_GAME,
    S_PAUSEONE,
    S_PAUSETWO,
};

//pause menu colours
WORD HighlightP = 0x3B;
WORD nonHighlightP = 0x0B;
WORD coloursPause[] = {HighlightP, nonHighlightP, nonHighlightP, nonHighlightP};
WORD *ptrPause = coloursPause;

WORD coloursPSound[] = {HighlightP, nonHighlightP};
WORD *ptrPSound = coloursPSound;

//changing character icon
CHAR charIcon = (char)1;
char arr[7] = {(char)1, (char)2, (char)3, (char)4, (char)5, (char)6, (char)7};
char *ptr = arr;
//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
	// Set precision for floating point output
	g_dElapsedTime = 0.0;
	g_dBounceTime = 0.0;
	g_dBounceTimeEnemy = 0.0;
	playTime = 0.0;
	BufferTime = 1.0;

    //reset the pause screen pointer selection to default
    *ptrPause = nonHighlightP;
    ptrPause = coloursPause;
    *ptrPause = HighlightP;

	// sets the initial state for the game
	levelCount = static_cast<GAMELEVELS>(1);
    levelClear = true;
	g_eGameState = S_SPLASHSCREEN;

    // sets the width, height and the font name to use in the console

    name[0] = '\0';
    pointer = name - 1;
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

void levelInit( void ) {

	if (levelClear && levelCount < MAX_LEVEL) {

        //resetting the player's coordinates to the default starting point (the top left corner)
		g_sChar.m_cLocation.X = 1;
		g_sChar.m_cLocation.Y = 2;

		levelClear = false; //to prevent levelInit() from running again while the player hasn't finished the level
		
        //clearing out the values that are different for each level
		maze = 0;
		MaxItemCount = 0;
		ItemCounter = 0;
        enemyvec.clear();

        //Render map based on the levels
		switch (static_cast<GAMELEVELS>(levelCount)) {
		case M1 : maze1(rows, cols);
            break;
		case M2 : maze2(rows, cols);
            break;
		case M3 : maze3(rows, cols);
            break;
		case M4 : maze4(rows, cols);
            break;
		case M5 : maze5(rows, cols);
            break;
		case M6 : maze6(rows, cols);
            break;
		}

		//Checking for max item count for each map 
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				if (maze[i][j] == '$') {    //go through the entire array, each time a '$' is found
					++MaxItemCount;     //increase the max item count by 1
				}
			}
		}

		//randomising of teleporters for each map
		randomiseTeleporters(rows, cols);
	}

}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);

    //for keying in name for high score
    g_abKeyPressed[K_A] = isKeyPressed(65);
    g_abKeyPressed[K_B] = isKeyPressed(66);
    g_abKeyPressed[K_C] = isKeyPressed(67);
    g_abKeyPressed[K_D] = isKeyPressed(68);
    g_abKeyPressed[K_E] = isKeyPressed(69);
    g_abKeyPressed[K_F] = isKeyPressed(70);
    g_abKeyPressed[K_G] = isKeyPressed(71);
    g_abKeyPressed[K_H] = isKeyPressed(72);
    g_abKeyPressed[K_I] = isKeyPressed(73);
    g_abKeyPressed[K_J] = isKeyPressed(74);
    g_abKeyPressed[K_K] = isKeyPressed(75);
    g_abKeyPressed[K_L] = isKeyPressed(76);
    g_abKeyPressed[K_M] = isKeyPressed(77);
    g_abKeyPressed[K_N] = isKeyPressed(78);
    g_abKeyPressed[K_O] = isKeyPressed(79);
    g_abKeyPressed[K_P] = isKeyPressed(80);
    g_abKeyPressed[K_Q] = isKeyPressed(81);
    g_abKeyPressed[K_R] = isKeyPressed(82);
    g_abKeyPressed[K_S] = isKeyPressed(83);
    g_abKeyPressed[K_T] = isKeyPressed(84);
    g_abKeyPressed[K_U] = isKeyPressed(85);
    g_abKeyPressed[K_V] = isKeyPressed(86);
    g_abKeyPressed[K_W] = isKeyPressed(87);
    g_abKeyPressed[K_X] = isKeyPressed(88);
    g_abKeyPressed[K_Y] = isKeyPressed(89);
    g_abKeyPressed[K_Z] = isKeyPressed(90);
    g_abKeyPressed[K_ENTER] = isKeyPressed(VK_RETURN);
    g_abKeyPressed[K_BACKSPACE] = isKeyPressed(VK_BACK);
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
	
	// get the delta time
	g_dElapsedTime += dt;
	g_dDeltaTime = dt;

	switch (g_eGameState)
	{
		case S_SPLASHSCREEN : splashScreenWait();
			break;

		case S_GAME : playTime += dt; gameplay();	// gameplay logic when we are in the game
            break;
        
        case S_DEATH : processDeath();
            break;

		case S_PAUSE : pauseGame();         // main stage of pause
			break;
        
        case S_PAUSEONE : pauseOne();       // sub stage of pause; game sounds
            break;

        case S_PAUSETWO : pauseTwo();       // sub stage of pause; character icon change
            break;

        case S_WIN : clearGame();           //the victory screen after the player completes the game
            break;
	}

}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render( void )
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen(); //Splashscreen shows for a few seconds with the level counter
            break;
        case S_GAME: renderGame();  //show out the game
            break;
        case S_DEATH : renderDeath();
            break;
		case S_PAUSE : renderPauseGame();   //show out the pause screen
			break;
        case S_PAUSEONE : renderPauseSound();   //show out the sub-menu of pause: sound selection
			break;
        case S_PAUSETWO : renderPauseChar();    //show out the sub-menu of pause: changing of character appearance
            break;
        case S_WIN : renderClearGame();     //show out the victory screen
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game

}

void gameplay( void )            // gameplay logic
{
    levelInit();
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.

    //check if each enemy is colliding with the player
    for (unsigned int i = 0; i < enemyvec.size(); ++i) {
            enemyCollisionWithPlayer(enemyvec[i]);
        }

    if (g_dBounceTimeEnemy < g_dElapsedTime) {
        for (unsigned int i = 0; i < enemyvec.size(); ++i) {
            moveEnemy(enemyvec[i]);
        }
        g_dBounceTimeEnemy = g_dElapsedTime + 0.3;
    }
}

//Movement for the character (UP,DOWN,LEFT,RIGHT) and check for collision with maze walls
void moveCharacter( void )
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime) {
        return;
    }

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
	int charY = g_sChar.m_cLocation.Y - 1;
	int charX = g_sChar.m_cLocation.X;

    //Character moving up
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
    {
        //Only move in that direction if its not a "wall"
		if (maze[charY-1][charX] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.Y--;
			bSomethingHappened = true;
		}
    }

    //Character moving left
    else if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        //Only move in that direction if its not a "wall"
		if (maze[charY][charX-1] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
		}
    }
    
    //Character moving down
    else if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Only move in that direction if its not a "wall"
		if (maze[charY+1][charX] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
		}
    }

    //Character moving right
    else if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Only move in that direction if its not a "wall"
		if (maze[charY][charX+1] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.X++;
			bSomethingHappened = true;
		}
    }

    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
        checkTrap();
		PickUpItems();
		exitLevel();
    }

}

void processUserInput( void )
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime) {
        return;
    }

    // pauses the game if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE]) {
        bSomethingHappened = true;
		g_eGameState = S_PAUSE;
	}

    //developer options (for debugging and demonstration)
    else if (g_abKeyPressed[K_R]) {     //to collect all the items
        bSomethingHappened = true;

        if (ItemCounter != MaxItemCount) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (maze[i][j] == '$') {
                        maze[i][j] = ' ';
                    }
                }
            }
        }

        ItemCounter = MaxItemCount;

    }
    
    else if (g_abKeyPressed[K_S]) {  //to skip and load the next level
        bSomethingHappened = true;

        if (levelCount == MAX_LEVEL-1) {
            g_eGameState = S_WIN;
		}
		else {
            levelClear = true;
            levelCount = static_cast<GAMELEVELS>(levelCount + 1);
        }
    }

    else if (g_abKeyPressed[K_T]) {  //to show the real teleporter
        bSomethingHappened = true;

        if (!showTeleporter) {
            showTeleporter = true;
        }
        else if (showTeleporter) {
            showTeleporter = false;
        }
    }

    if (bSomethingHappened) {
        g_dBounceTime = g_dElapsedTime + 0.5;
    }
}

//Glennda
void processPauseInput( void )
{
    if (g_dElapsedTime <= g_dBounceTime) {
        return;
    }

    if (g_abKeyPressed[K_UP]) {
        if (ptrPause != coloursPause) {
            *ptrPause = nonHighlightP;
            --ptrPause;
            *ptrPause = HighlightP;
            g_dBounceTime = g_dElapsedTime + 0.2;
        }
    }
    else if (g_abKeyPressed[K_DOWN]) {
        if (ptrPause != coloursPause + 3) {
            *ptrPause = nonHighlightP;
            ++ptrPause;
            *ptrPause = HighlightP;
            g_dBounceTime = g_dElapsedTime + 0.2;
        }
    }
    else if (g_abKeyPressed[K_ENTER]) {
        int offset = ptrPause - coloursPause;
        if (offset < 3) {
            g_eGameState = gamestate[offset];
        }
        else {  //offset == 3 is to quit game
            g_bQuitGame = true;     // exit game, return to Main Menu
        }
        g_dBounceTime = g_dElapsedTime + 0.3;
    }
    
}

void processPauseSound( void )
{
    if (g_dElapsedTime <= g_dBounceTime) {
        return;
    }
    
    if (g_abKeyPressed[K_UP]) {
        if (ptrPSound != coloursPSound) {
            *ptrPSound = nonHighlightP;
            --ptrPSound;
            *ptrPSound = HighlightP;
            g_dBounceTime = g_dElapsedTime + 0.2;
        }
    }
    else if (g_abKeyPressed[K_DOWN]) {
        if (ptrPSound == coloursPSound) {
            *ptrPSound = nonHighlightP;
            ++ptrPSound;
            *ptrPSound = HighlightP;
            g_dBounceTime = g_dElapsedTime + 0.2;
        }
    }
    else if (g_abKeyPressed[K_ENTER]) {
        g_eGameState = S_PAUSE;
        if (ptrPSound == coloursPSound) { //player wants sound to be on
            if (!playmusic) {
			    PlaySound(TEXT("gamemusic.wav"), NULL, SND_LOOP | SND_ASYNC);
		    }
		    playmusic = true;   //boolean to keep playing the music
        }
        else {  //player wants sound to be off
            PlaySound(NULL,NULL,0);
		    playmusic = false;  //boolean to stop playing music even after going back to main page and in game
            *ptrPSound = nonHighlightP;
            ptrPSound = coloursPSound;
            *ptrPSound = HighlightP;
        }
        g_dBounceTime = g_dElapsedTime + 0.3;
    }

}

void processPauseChar(char * arr)
{
    if (g_dElapsedTime <= g_dBounceTime) {
        return;
    }

    if (g_abKeyPressed[K_RIGHT]) {
        g_dBounceTime = g_dElapsedTime + 0.2;
        if (ptr < arr+6) {      // ptr not at position 6
            ++ptr;      // right shift of pointer
        }
        charIcon = *ptr;
    }

    if (g_abKeyPressed[K_LEFT]) {
        g_dBounceTime = g_dElapsedTime + 0.2;
        if (ptr > arr) {       // ptr not pointing at position 0 of arr
            --ptr;      // left shift of pointer
        }
        charIcon = *ptr; // Selected Icon is where pointer is pointing at
    }

    else if (g_abKeyPressed[K_ENTER]) {
        g_dBounceTime = g_dElapsedTime + 0.1;
        g_eGameState = S_PAUSE;
    }
}

//Pause Gamestate_Main
void pauseGame( void ) {
	processPauseInput();
}

//Pause Gamestate_Sub
void pauseOne( void )
{
    processPauseSound();
}

void pauseTwo( void )
{
    processPauseChar(arr);
}


//Render text for main pause page and sub pause pages
void renderPauseGame( void ) {

	clearScreen();
	COORD c = g_Console.getConsoleSize();
	c.Y /= 4;
    c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 23;
	g_Console.writeToBuffer(c, "RETURN TO GAME", coloursPause[0]);
	++c.Y;
	g_Console.writeToBuffer(c, "SOUND", coloursPause[1]);
    ++c.Y;
	g_Console.writeToBuffer(c, "CHANGE CHARACTER ICON", coloursPause[2]);
    ++c.Y;
	g_Console.writeToBuffer(c, "RETURN TO MAIN MENU", coloursPause[3]);
}

void renderPauseSound( void ) {
    clearScreen();
	COORD c = g_Console.getConsoleSize();
	c.Y /= 4;
    c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 23;
    g_Console.writeToBuffer(c, "ON", coloursPSound[0]);
    ++c.Y;
	g_Console.writeToBuffer(c, "OFF", coloursPSound[1]);
}

void renderPauseChar( void ) {
    clearScreen();
	COORD c = g_Console.getConsoleSize();
	c.Y /= 4;
    c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 30;
    g_Console.writeToBuffer(c, "Press Right arrow key or Left arrow key to switch Player Icon", 0x0B);
    c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2;
    g_Console.writeToBuffer(c, charIcon, 0x0A);
    c.Y += 5;
	c.X = g_Console.getConsoleSize().X / 2 - 12;
    g_Console.writeToBuffer(c, "Press 'Enter' to return", 0x0B);
}

void clearScreen( void )
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x0F);
}

void splashScreenWait( void ) {

	if (g_dElapsedTime > BufferTime) {
		g_eGameState = S_GAME;
	}

}

void renderSplashScreen( void )  // renders the splash screen
{

	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = c.X / 2 - 3;
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(3);
	
    if (g_dElapsedTime < BufferTime - 1.0) {
        c.Y += 1;
	    c.X = g_Console.getConsoleSize().X / 2 - 20;
        ss.str ("");
        ss << "Level Clear!";
        g_Console.writeToBuffer(c, ss.str(), 0x0B);
        ++c.Y;
        ss.str("");
        ss << "Proceeding on to the next level";
        g_Console.writeToBuffer(c, ss.str(), 0x0B);
    }

    else
    {
        ss.str("");
	    ss << "Level " << levelCount;
	    g_Console.writeToBuffer(c, ss.str(), 0x0B);
    }
    

    

}

void clearGame( void ) {

    if (playmusic && victoryplaymusic) {
        PlaySound(TEXT("victory.wav"), NULL, SND_ASYNC);
        victoryplaymusic = false;
    }
    processNameInput(name);
}

//Render text for winning and get name for highscore
void renderClearGame( void ) {
    clearScreen();
    renderText();
    renderNameInput(name);
}

void renderDeath(void) 
{
    string scream[10] = {   //string array
    "   .----------.",
    "  /  .-.  .-.  \\",
    " /   | |  | |   \\",
    " \\   `-'  `-'  _/",
    " /\\     .--.  / |",
    " \\ |   /  /  / /",
    " / |  `--' /\\ \\",
    "  /`-------'  \\ \\" 
    };
    COORD c = g_Console.getConsoleSize();

    c.Y /= 2;
    c.X /=2;
    for (int i = 0; i < 10; ++i, ++c.Y) {  //check through the string array and print out using writeToBuffer
        std::ostringstream ss;
        ss.str("");
        ss << scream[i];
        g_Console.writeToBuffer(c, ss.str(), 0x0F);
    }

    std::ostringstream ss;
	c.X = g_Console.getConsoleSize().X/2-5;
	c.Y = g_Console.getConsoleSize().Y /3;
    ss.str("");
	ss << "You died!";
    g_Console.writeToBuffer(c, ss.str(), 0x0B);
    ss.str("");
    c.X -= 3;
    c.Y ++;
    ss << "Press ENTER to restart";
	g_Console.writeToBuffer(c, ss.str(), 0x0B);
}

void processDeath(void)
{
    if (g_abKeyPressed[K_ENTER])
    {
       levelClear = true;
       g_eGameState = S_GAME;
    }
};

//Shania
//=======
//Render the text for the win page
void renderText( void ) {

    COORD c = g_Console.getConsoleSize();

	string victory[21] = { //string array
		".____________________________________________________________________.",
		"|  _    _         _    _        _    _         _    _        _    _  |",
		"|   \\/}/     /}    \\/}/     /}   \\/}/     /}    \\/}/     /}   \\/}/   |",
		"|    |_    _/|\\_    |_    _/|\\_   |_    _/|\\_    |_    _/|\\_   |_    |",
		"|    | \\    / \\     | \\    / \\    | \\    / \\     | \\    / \\    | \\   |",
		"|vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv|",
		"|                                                                    |",
		"|                                                                    |",
		"|                                                                    |",
		"|                                                                    |",
		"|     _                                                        _     |",
		"|    /_\\                                                      /_\\    |",
		"|    =|=                                                      =|=    |",
		"|          .*.                                          .*.          |",
		"|         ;(;);________________________________________;(;);         |",
		"|         |;;;    _    _         _    _        _    _   ;;;|         |",
		"|         | ;/}    \\/}/     /}    \\/}/     /}   \\/}/    /; |         |",
		"|         |_/|\\_    |_    _/|\\_    |_    _/|\\_   |_   _/|\\_|         |",
		"|         | / \\     | \\    / \\     | \\    / \\    | \\   / \\ |         |",
		"|       __|vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv|__       |",
		"|______|______________________________________________________|______|"
	};

	c.X = 5;  //x and y coordinates to start the box
	c.Y = 1;
	for (int i = 0; i < 21; ++i, ++c.Y) {  //check through the string array and writeToBuffer to print out
		std::ostringstream ss;
		ss.str("");
		ss << victory[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
	}

	//display "YOU ESCAPED!"
	std::ostringstream ss;
    c.X = g_Console.getConsoleSize().X /2 - 5;
	c.Y = 9;

	g_Console.writeToBuffer(c, "YOU ESCAPED!", 0x0F);

   // displays the playtime
	
    ss.str("");
    ss << "Time: " << playTime << "secs";
    c.X = g_Console.getConsoleSize().X / 2 - 7;
    ++c.Y;
    g_Console.writeToBuffer(c, ss.str());

   // text to ask user to input name
    c.Y += 2;
	g_Console.writeToBuffer(c, "INPUT YOUR NAME: ", 0x0F);

    c.X = g_Console.getConsoleSize().X / 2;
    c.X -= 14;
    c.Y += 4;
}

//JingTing
void renderNameInput(char * name) {
    COORD c = g_Console.getConsoleSize();
    c.X = c.X / 2 - 5;
    c.Y /= 3;
    c.Y += 5;
    g_Console.writeToBuffer(c, "          ", 0x13);
    g_Console.writeToBuffer(c, name, 0x1A);
}

//JingTing
void processNameInput(char * name) {

    bool keySomething = false;
    bool backspace = false;
    if (g_dBounceTime > g_dElapsedTime) {
        return;
    }

    if (pointer != name+9) {
        if (g_abKeyPressed[K_A] && prevKeyPressed != K_A) {
            *(pointer+1) = 'A';
            keySomething = true;
            prevKeyPressed = K_A;
        }
        else if (g_abKeyPressed[K_B] && prevKeyPressed != K_B) {
            *(pointer+1) = 'B';
            keySomething = true;
            prevKeyPressed = K_B;
        }
        else if (g_abKeyPressed[K_C] && prevKeyPressed != K_C) {
            *(pointer+1) = 'C';
            keySomething = true;
            prevKeyPressed = K_C;
        }
        else if (g_abKeyPressed[K_D] && prevKeyPressed != K_D) {
            *(pointer+1) = 'D';
            keySomething = true;
            prevKeyPressed = K_D;
        }
        else if (g_abKeyPressed[K_E] && prevKeyPressed != K_E) {
            *(pointer+1) = 'E';
            keySomething = true;
            prevKeyPressed = K_E;
        }
        else if (g_abKeyPressed[K_F] && prevKeyPressed != K_F) {
            *(pointer+1) = 'F';
            keySomething = true;
            prevKeyPressed = K_F;
        }
        else if (g_abKeyPressed[K_G] && prevKeyPressed != K_G) {
            *(pointer+1) = 'G';
            keySomething = true;
            prevKeyPressed = K_G;
        }
        else if (g_abKeyPressed[K_H] && prevKeyPressed != K_H) {
            *(pointer+1) = 'H';
            keySomething = true;
            prevKeyPressed = K_H;
        }
        else if (g_abKeyPressed[K_I] && prevKeyPressed != K_I) {
            *(pointer+1) = 'I';
            keySomething = true;
            prevKeyPressed = K_I;
        }
        else if (g_abKeyPressed[K_J] && prevKeyPressed != K_J) {
            *(pointer+1) = 'J';
            keySomething = true;
            prevKeyPressed = K_J;
        }
        else if (g_abKeyPressed[K_K] && prevKeyPressed != K_K) {
            *(pointer+1) = 'K';
            keySomething = true;
            prevKeyPressed = K_K;
        }
        else if (g_abKeyPressed[K_L] && prevKeyPressed != K_L) {
            *(pointer+1) = 'L';
            keySomething = true;
            prevKeyPressed = K_L;
        }
        else if (g_abKeyPressed[K_M] && prevKeyPressed != K_M) {
            *(pointer+1) = 'M';
            keySomething = true;
            prevKeyPressed = K_M;
        }
        else if (g_abKeyPressed[K_N] && prevKeyPressed != K_N) {
            *(pointer+1) = 'N';
            keySomething = true;
            prevKeyPressed = K_N;
        }
        else if (g_abKeyPressed[K_O] && prevKeyPressed != K_O) {
            *(pointer+1) = 'O';
            keySomething = true;
            prevKeyPressed = K_O;
        }
        else if (g_abKeyPressed[K_P] && prevKeyPressed != K_P) {
            *(pointer+1) = 'P';
            keySomething = true;
            prevKeyPressed = K_P;
        }
        else if (g_abKeyPressed[K_Q] && prevKeyPressed != K_Q) {
            *(pointer+1) = 'Q';
            keySomething = true;
            prevKeyPressed = K_Q;
        }
        else if (g_abKeyPressed[K_R] && prevKeyPressed != K_R) {
            *(pointer+1) = 'R';
            keySomething = true;
            prevKeyPressed = K_R;
        }
        else if (g_abKeyPressed[K_S] && prevKeyPressed != K_S) {
            *(pointer+1) = 'S';
            keySomething = true;
            prevKeyPressed = K_S;
        }
        else if (g_abKeyPressed[K_T] && prevKeyPressed != K_T) {
            *(pointer+1) = 'T';
            keySomething = true;
            prevKeyPressed = K_T;
        }
        else if (g_abKeyPressed[K_U] && prevKeyPressed != K_U) {
            *(pointer+1) = 'U';
            keySomething = true;
            prevKeyPressed = K_U;
        }
        else if (g_abKeyPressed[K_V] && prevKeyPressed != K_V) {
            *(pointer+1) = 'V';
            keySomething = true;
            prevKeyPressed = K_V;
        }
        else if (g_abKeyPressed[K_W] && prevKeyPressed != K_W) {
            *(pointer+1) = 'W';
            keySomething = true;
            prevKeyPressed = K_W;
        }
        else if (g_abKeyPressed[K_X] && prevKeyPressed != K_X) {
            *(pointer+1) = 'X';
            keySomething = true;
            prevKeyPressed = K_X;
        }
        else if (g_abKeyPressed[K_Y] && prevKeyPressed != K_Y) {
            *(pointer+1) = 'Y';
            keySomething = true;
            prevKeyPressed = K_Y;
        }
        else if (g_abKeyPressed[K_Z] && prevKeyPressed != K_Z) {
            *(pointer+1) = 'Z';
            keySomething = true;
            prevKeyPressed = K_Z;
        }

    }

    if (g_abKeyPressed[K_BACKSPACE]) {
        *pointer = '\0';
        keySomething = true;
        backspace = true;
    }

    //saves the name into the text file if the player presses ENTER key
    if (g_abKeyPressed[K_ENTER]) {
        ofstream outData;

	    outData.open("Timings.txt", ofstream::app);

	    outData << std::endl << name << " " << playTime;

	    outData.close();

        g_bQuitGame = true;
	}

    if (keySomething)
    {
        g_dBounceTime = g_dElapsedTime + 0.05;
        if (!backspace && pointer < name+9) {
            ++pointer;
            bouncePrevKey = g_dElapsedTime + 0.2;
        }
        if (backspace && pointer >= name) {
            --pointer;
            g_dBounceTime = g_dElapsedTime + 0.125; // set the bounce time to some time in the future to prevent backspacing too much
        }
    }
    if (bouncePrevKey < g_dElapsedTime) {
        prevKeyPressed = K_ESCAPE;  //set the prevKeyPressed to a key value that will not be processed by this function
    }

}

void renderGame( void )
{
    renderCounters();   // counters will be rendered in gameplay() only (not in the splashscreen)
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderEnemy();
}

void renderMap( void )
{
    // Set up sample colours, and output shadings
	mapgenerator(rows, cols);

}

//After map is rendered in to the buffer, this is called out to show out the ascii codes for each of the items
//Eg walls, items, etc
void mapgenerator(int rows, int cols) {

	COORD c;

	for (int i = 0; i < rows; ++i) {
		c.Y = i+1;
		for (int j = 0; j < cols; ++j) {
			c.X = j;
			if (maze[i][j] == '@') {
				g_Console.writeToBuffer(c, '@', 0x0E);
			}
			else if (maze[i][j] == '$') {
				g_Console.writeToBuffer(c, '$', 0x07);
			}
			else if (maze[i][j] == '*') {
				g_Console.writeToBuffer(c, '*', 0x0D);
			}
			else if (maze[i][j] == '#') {
				g_Console.writeToBuffer(c, (char)178, 0x0B);
			}
		}
	}
    if (showTeleporter) {
        g_Console.writeToBuffer(Tel.own_Loc, '@', 0x0C);
        g_Console.writeToBuffer(Tel.warp_Loc, '@', 0x0C);
    }
}

void randomiseTeleporters(int rows, int cols) {

	COORD c;
	vector<Traps> trapvec;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (maze[i][j] == '@') {
				c.Y = i+1;
				c.X = j;
				if (c.X != Tel.own_Loc.X || c.Y != Tel.own_Loc.Y) {
					Traps tempTrap;
					tempTrap.Loc = c;
					trapvec.push_back(tempTrap);
				}
			}
		}
	}

	//assigning a random trap to be the teleporter
	int size = trapvec.size();
	srand((unsigned int)time(NULL));
	int teleport = rand() % size;
	Traps tempTel = trapvec.at(teleport);
	
	Tel.warp_Loc = tempTel.Loc;

}

void maze1(int& rows, int& cols) {

	string map1[21] = {
        "################################",   // 3 Items, 5 teleports, 2 AI
        "#              #  #@           #",   
		"##### ######## ## ###########  #",
		"#     # @# $ # #  #            #",
        "#   ### ## # # # ## #### #######",
        "#        # # # #               #",
		"# ###### # # #   ######  ##### #",
        "#     #    # # ## #         $# #",
        "# ### ######      #  ######### #",
        "# # # # *  #### ###  #   #   # #",
        "# # ###    #           #   #   #",
        "#     #    ##### ###############",
        "# ##### @  #   #    #  $  #  #@#",
        "#     #    # # # #           # #",
        "# ### ######## # # ##### ##### #",
        "# # # #     ##   #    #      # #",
        "### # # # # ## # ########### # #",
        "#@# #   # #    #               #",
        "# # ##### # ## ###### ######## #",
        "#         # #@                 #",
        "################################" };

	rows = 21;
	maze = new char * [rows];
	cols = 32;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map1[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

	Tel.own_Loc.X = 8;
	Tel.own_Loc.Y = 13;

	//Shania
    //Enemy 1
    Enemy g_Enemy1;
	g_Enemy1.m_Enemy.X = 18;   //g_Enemy1's X coordinate
    g_Enemy1.m_Enemy.Y = 8;   //g_Enemy1's Y coordinate
    g_Enemy1.d = right;       //g_Enemy1's direction
    enemyvec.push_back(g_Enemy1);  //Store the X & Y coordinates and direction of g_Enemy1 into enemyvec

    //Enemy2
    Enemy g_Enemy2;
    g_Enemy2.m_Enemy.X = 27;
    g_Enemy2.m_Enemy.Y = 14;
    g_Enemy2.d = left;
    enemyvec.push_back(g_Enemy2);
}

void maze2(int& rows, int& cols) {

	string map2[21] = {
        "################################",    // 5 Items, 5 teleports, 3 AI
		"# #                  #    #    #",
		"# # ### ### # ###### # #  # #  #",
		"# # #     # #  #  #  #@#  # # $#",
        "#     # # # ## #  # #### ## ####",
		"# ##### # #$#     # #        #@#",
        "# #     # #### # ## # ###### # #",
        "# # ##### #    #        #    # #",
		"# #    @# ## # ######## #  # # #",
		"# #######    #        # #  # # #",
		"#     #    # ########## #### # #",
        "#####   #### #*     #          #",
		"# $ # #      #     @######## # #",
		"## ## ############### #  # # # #",
        "#   #   #   #   #     #  # # # #",
		"# # ### ### # # #  ####  # # # #",
        "# # @ # #@    # #        #   #$#",
        "# ##### ####### #  ## #  # # ###",
        "#  #  #  #         #  #### #   #",
        "#          # # #   #   $   #   #",
        "################################" };

	rows = 21;
	maze = new char * [rows];
	cols = 32;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map2[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

	Tel.own_Loc.X = 19;
	Tel.own_Loc.Y = 13;

    
	//Shania
    //Enemy 1
    Enemy g_Enemy1;
    g_Enemy1.m_Enemy.X = 9;
    g_Enemy1.m_Enemy.Y = 2;
    g_Enemy1.d = right;
    enemyvec.push_back(g_Enemy1);

    //Enemy2
    Enemy g_Enemy2;
    g_Enemy2.m_Enemy.X = 30;
    g_Enemy2.m_Enemy.Y = 15;
    g_Enemy2.d = up;
    enemyvec.push_back(g_Enemy2);

	//Enemy3
	Enemy g_Enemy3;
	g_Enemy3.m_Enemy.X = 2;
	g_Enemy3.m_Enemy.Y = 20;
	g_Enemy3.d = right;
	enemyvec.push_back(g_Enemy3);


}

void maze3(int& rows, int& cols) {

	string map3[21] = {
        "################################",    // 8 Items, 8 teleports, 3 AI
        "# #  #  @  #               #   #",
        "# #  # ### ###   # ######### # #",
        "# #          # # # #@        # #",
        "# ## ####### # # # ####### # # #",
        "#      #     #$# #       # # # #",
		"## ##  # ### ### # ####### ### #",
        "#   #@   # #  @# # #$      #   #",
        "##  ###### # ### # ####### #   #",
        "#   #  #   #     #         # # #",
        "#   # ## #$############### #$#@#",
        "# #@#    ###@ #@         # #####",
        "# ###  ##     #         *#   #$#",
        "# $#    # #   ############## # #",
        "# ##### ###   #  #      #    # #",
		"#           ###    #  #   #    #",
        "## ### ###  #$############## # #",
        "#  #   #      #   #   #   #    #",
        "#  #   #  ##### # # # # # ### ##",
		"#$   # #        #   #   #     @#",
        "################################" };

	rows = 21;
	maze = new char * [rows];
	cols = 32;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map3[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

	Tel.own_Loc.X = 15;
	Tel.own_Loc.Y = 12;


	//Shania
    //Enemy 1
    Enemy g_Enemy1;
    g_Enemy1.m_Enemy.X = 13;
    g_Enemy1.m_Enemy.Y = 13;
    g_Enemy1.d = right;
    enemyvec.push_back(g_Enemy1);
    
    //Enemy 2
    Enemy g_Enemy2;
    g_Enemy2.m_Enemy.X = 30;
    g_Enemy2.m_Enemy.Y = 9;
    g_Enemy2.d = right;
    enemyvec.push_back(g_Enemy2);

	//Enemy 3
	Enemy g_Enemy3;
	g_Enemy3.m_Enemy.X = 1;
	g_Enemy3.m_Enemy.Y = 10;
	g_Enemy3.d = up;
	enemyvec.push_back(g_Enemy3);
}

void maze4(int& rows, int& cols) {

	string map4[24] = {
        "###################################",   // 10 Items, 10 teleportss, 4 AI
        "#     #@   #          #           #",
        "##### #### #  ####### #   ######  #",
        "#     #    #  #$      #   #    #  #",
        "# #####  # ## ##### ### ### #  #  #",
        "#@    #  #        #     #   #  #$ #",
        "##### ## ########### #### #########",
        "#    $#          @#  #     #    @ #",
        "# ##### ## ####   #     #### ######",
        "#       #    #    #### ##@   #   $#",
        "#### #### ###### ##               #",
		"#$ #     @#    # #  $# # # ########",
        "#  #### ### #### # ### # # #$  #  #",
        "#           #      #     # # # #  #",
        "## #################### ##   #    #",
        "#   #     #          *#    ###### #",
        "# #   #   #@          # ###     # #",
        "######### ############# #   ##### #",
        "#       #$              #         #",
        "# ### # #### # ##### ###### #######",
        "# #@ $#      # #      @#     #$   #",
        "# ############ ## ### ##  # ##### #",
        "#     @#       #  #       #@      #",
        "###################################" };

	rows = 24;
	maze = new char * [rows];
	cols = 35;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map4[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

	Tel.own_Loc.X = 11;
	Tel.own_Loc.Y = 17;

	//Shania
    //Enemy 1
    Enemy g_Enemy1;
    g_Enemy1.m_Enemy.X = 18;
    g_Enemy1.m_Enemy.Y = 16;
    g_Enemy1.d = right;
    enemyvec.push_back(g_Enemy1);

    //Enemy2
    Enemy g_Enemy2;
    g_Enemy2.m_Enemy.X = 26;
    g_Enemy2.m_Enemy.Y = 15;
    g_Enemy2.d = up;
    enemyvec.push_back(g_Enemy2);

	//Enemy3
	Enemy g_Enemy3;
	g_Enemy3.m_Enemy.X = 24;
	g_Enemy3.m_Enemy.Y = 2;
	g_Enemy3.d = right ;
	enemyvec.push_back(g_Enemy3);

	//Enemy4
	Enemy g_Enemy4;
	g_Enemy4.m_Enemy.X = 25;
	g_Enemy4.m_Enemy.Y = 11;
	g_Enemy4.d = right ;
	enemyvec.push_back(g_Enemy4);

}

void maze5(int& rows, int& cols) {

	string map5[24] = {
        "########################################",   // 10 Items, 15 teleports, 5 AI
        "#      #         #          #      #@  #",
        "# #### ##### # # # ######## ### # #### #",
        "#    #   #@  # #   # @ #        # #    #",
        "######## ##### # ###   # ######## #### #",
        "#   @  #       # #       #      # #    #",
        "# #### # ######### ############ # #  # #",
        "#    #   #@     $# #          # # #  #$#",
        "#### ###### ###### # ######   # #   ####",
        "#    #@  #         # #$       # # ##@  #", 
        "# ##### ###### ##### ########   #  #   #",
        "# #      #$  # #@           #  ### ## ##",
        "# ## # #####   ###########$ #   $# ## ##",
        "#    # #     ###        *#### #### ## ##",
        "###### ## ####$#@        #@           ##",
        "#    # #@    # ########### #############",
        "#      #### ## ###       #            @#",
        "# ######    #      ##### ###### ###### #",
        "# #@        ###### #$  # #@            #",
        "# #########        ### # # #############",
        "#        $# # ######   #$#      # @    #",
        "# ######### # #    #   ######## ###### #",
        "#     @     #                          #",
        "########################################" };

	rows = 24;
	maze = new char * [rows];
	cols = 40;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map5[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

	Tel.own_Loc.X = 16;
	Tel.own_Loc.Y = 15;

	//Shania
    //Enemy 1
    Enemy g_Enemy1;
    g_Enemy1.m_Enemy.X = 25;
    g_Enemy1.m_Enemy.Y = 15;
    g_Enemy1.d = right;
    enemyvec.push_back(g_Enemy1);

    //Enemy2
    Enemy g_Enemy2;
    g_Enemy2.m_Enemy.X = 11;
    g_Enemy2.m_Enemy.Y = 20;
    g_Enemy2.d = up;
    enemyvec.push_back(g_Enemy2);

	//Enemy3
	Enemy g_Enemy3;
    g_Enemy3.m_Enemy.X = 3;
    g_Enemy3.m_Enemy.Y = 12;
    g_Enemy3.d = right;
    enemyvec.push_back(g_Enemy3);

	//Enemy4
    Enemy g_Enemy4;
    g_Enemy4.m_Enemy.X = 14;
    g_Enemy4.m_Enemy.Y = 23;
    g_Enemy4.d = right;
    enemyvec.push_back(g_Enemy4);

	//Enemy5
    Enemy g_Enemy5;
    g_Enemy5.m_Enemy.X = 30;
    g_Enemy5.m_Enemy.Y = 23;
    g_Enemy5.d = right;
    enemyvec.push_back(g_Enemy5);
}

void maze6(int& rows, int& cols) {
    
	string map6[23] = {
        "#######################################",  //  10 Items, 15 teleports, 2AI
        "#     #   #  @  #    #       $# #   #@#",
        "# # ### # # ### # ## # #### ### # # # #",
        "# #@#   #   #   #  #   #      #       #",
        "# ### ####### ###### # # ### ######## #",
        "# #   # $ #@  #   ##   # # #@   #$#   #",
        "# # ### # ##### # #  # ### ### ## #   #",
        "# #     #       #    #  $#   #  #     #",
        "# ### ################# ## # ## ##### #",
        "#     #       * @    #@    #    #  $# #",
		"##### #              ### ### # ## ### #",
        "#@  # #        $     #    $# #        #",
        "#   # ################ ##### #### #####",
		"#     #$   # #         # @ # #        #",
		"## #######   # ####### # # ########## #",
		"#          # @   #   # # # # #    #@  #",
        "# # ## ## #####      #   #        # ###",
        "# # #@ #    #@# ################# #   #",
        "#   #### ##       @                 ###",
        "#####    $# ###### ########### ###### #",
		"# @ #   ### #   #   #  # $ #   #  @ # #",
		"#     # #     #     #    #   #   ##   #",
		"#######################################" };

	rows = 23;
	maze = new char * [rows];
	cols = 39;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map6[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

	Tel.own_Loc.X = 16;
	Tel.own_Loc.Y = 10;

	//Shania
    //Enemy1
    Enemy g_Enemy1;
	g_Enemy1.m_Enemy.X = 25;
    g_Enemy1.m_Enemy.Y = 19;
    g_Enemy1.d = right;
    enemyvec.push_back(g_Enemy1);
    
    //Enemy2
    Enemy g_Enemy2;
    g_Enemy2.m_Enemy.X = 5;
    g_Enemy2.m_Enemy.Y = 10;
    g_Enemy2.d = up;
    enemyvec.push_back(g_Enemy2);

	//Enemy3
    Enemy g_Enemy3;
	g_Enemy3.m_Enemy.X = 8;
    g_Enemy3.m_Enemy.Y = 11;
    g_Enemy3.d = right;
    enemyvec.push_back(g_Enemy3);

	//Enemy4
    Enemy g_Enemy4;
	g_Enemy4.m_Enemy.X = 3;
    g_Enemy4.m_Enemy.Y = 13;
    g_Enemy4.d = right;
    enemyvec.push_back(g_Enemy4);

	//Enemy5
    Enemy g_Enemy5;
	g_Enemy5.m_Enemy.X = 36;
    g_Enemy5.m_Enemy.Y = 4;
    g_Enemy5.d = right;
    enemyvec.push_back(g_Enemy5);

	//Enemy6
    Enemy g_Enemy6;
	g_Enemy6.m_Enemy.X = 32;
    g_Enemy6.m_Enemy.Y = 12;
    g_Enemy6.d = right;
    enemyvec.push_back(g_Enemy6);

	//Enemy7
    Enemy g_Enemy7;
	g_Enemy7.m_Enemy.X = 11;
    g_Enemy7.m_Enemy.Y = 15;
    g_Enemy7.d = right;
    enemyvec.push_back(g_Enemy7);

	//Enemy8
    Enemy g_Enemy8;
	g_Enemy8.m_Enemy.X = 13;
    g_Enemy8.m_Enemy.Y = 12;
    g_Enemy8.d = up;
    enemyvec.push_back(g_Enemy8);
	 
	//Enemy9
    Enemy g_Enemy9;
    g_Enemy9.m_Enemy.X = 7;
    g_Enemy9.m_Enemy.Y = 21;
    g_Enemy9.d = up;
    enemyvec.push_back(g_Enemy9);

    //Enemy10
    Enemy g_Enemy10;
    g_Enemy10.m_Enemy.X =22;
    g_Enemy10.m_Enemy.Y = 8;
    g_Enemy10.d = up;
    enemyvec.push_back(g_Enemy10);

}

void renderCharacter( void )
{
    // Draw the location of the character
    g_Console.writeToBuffer(g_sChar.m_cLocation, charIcon, 0x0A);
}
//JingTing + Shania
void renderEnemy( void )
{
    for (unsigned int i = 0; i < enemyvec.size(); ++i) {  // Use for loop is to check through the enemyvec for each level.
        Enemy tempEnemy = enemyvec[i];
        COORD tempE = tempEnemy.m_Enemy;            
        g_Console.writeToBuffer(tempE, 'X', 0x0C);   
    }
}
//Shania
void moveEnemy(Enemy& g_Enemy)
{

    //Get the enemy coordinates
    int eX = g_Enemy.m_Enemy.X;
    int eY = g_Enemy.m_Enemy.Y-1;
	
    //Move the enemy
	//If enemy next X position is not a wall
    if (g_Enemy.d == right && maze[eY][eX+1] != '#') //if not a wall, and direction is right, move right
    {
        g_Enemy.m_Enemy.X++;     //enemy's x-axis increase by 1
    }

    else if(g_Enemy.d == left && maze[eY][eX-1] != '#') //if not a wall, and direction is left, move left
    {
        g_Enemy.m_Enemy.X--;     //enemy's x-axis decrease by 1
    }
    else if (g_Enemy.d == up && maze[eY-1][eX] != '#') //if not a wall, and direction is up, move up
    {
        g_Enemy.m_Enemy.Y--;      //enemy's y-axis decrease by 1
    }
	else if(g_Enemy.d == down && maze[eY+1][eX] != '#') //if not a wall, and direction is down, move down
    {
        g_Enemy.m_Enemy.Y++;     //enemy's y-axis increase by 1
    }

	//change direction when a wall is detected
    else if (g_Enemy.d == right && maze[eY][eX+1] == '#')
	{
		g_Enemy.m_Enemy.X--;
		g_Enemy.d = left;
	}
	else if (g_Enemy.d == left && maze[eY][eX-1] == '#')
	{
		g_Enemy.m_Enemy.X++;
		g_Enemy.d = right;
	}
    else if (g_Enemy.d == down && maze[eY+1][eX] == '#')
	{
		g_Enemy.m_Enemy.Y--;
		g_Enemy.d = up;
	}
	else if (g_Enemy.d == up && maze[eY-1][eX] == '#')
	{
		g_Enemy.m_Enemy.Y++;
		g_Enemy.d = down;
	}

}
//WeiMin
void enemyCollisionWithPlayer(Enemy g_Enemy) {

	//If character touches the enemy, spawn the character back to starting location
    if (g_sChar.m_cLocation.X == g_Enemy.m_Enemy.X && g_sChar.m_cLocation.Y == g_Enemy.m_Enemy.Y)
    {
        g_sChar.m_cLocation.X = 1;
        g_sChar.m_cLocation.Y = 2;

        g_eGameState = S_DEATH;
    }
}

void renderFramerate( void )
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());
}

//Render in game counters
//WeiMin
void renderCounters( void ) {

    COORD c;    
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c,"ESCAPEE");

	std::ostringstream ss;

     // displays the level count
    ss.str("");
    ss << "Level: " << levelCount;
    c.X = 50;
    c.Y = 6;
    g_Console.writeToBuffer(c, ss.str());

    // displays the item counter
	ss.str("");
	ss << "$: " << ItemCounter << "/" << MaxItemCount;
    c.Y = 8;
	g_Console.writeToBuffer(c, ss.str());

    
	// displays the playtime
    ss.str("");
    ss << "Timer: " << playTime << "secs";
    c.Y = 10;
    g_Console.writeToBuffer(c, ss.str());

    //show if the exit is locked
    if (ItemCounter < MaxItemCount)
    {
        c.X = 40;
        c.Y = 12;
        g_Console.writeToBuffer(c,"You have not collected enough items");
        c.X = 50;
        c.Y = 14;
        g_Console.writeToBuffer(c,"Exit is locked");
    }
    else
    {
        c.X = 45;
        c.Y = 12;
        g_Console.writeToBuffer(c,"Exit is unlocked");
    }
    c.Y+=2;
    g_Console.writeToBuffer(c,"Press ESC to pause");
}

void renderToScreen( void )
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void checkTrap( void ) {

	int Y = g_sChar.m_cLocation.Y - 1;
	int X = g_sChar.m_cLocation.X;

     //Checking if player is on the teleporter at the exit point (the locations of exit teleporters are saved at each map)
	 if (maze[Y][X] == '@') {
		 if (g_sChar.m_cLocation.X == Tel.own_Loc.X && g_sChar.m_cLocation.Y == Tel.own_Loc.Y) {
			 g_sChar.m_cLocation = Tel.warp_Loc;
             g_dBounceTime = g_dElapsedTime + 0.3;
		 }
         //Checking if player is on the teleporter at the "random assigned" teleporters
         //Move player to the exit point
		 else if (g_sChar.m_cLocation.X == Tel.warp_Loc.X && g_sChar.m_cLocation.Y == Tel.warp_Loc.Y) {
			 g_sChar.m_cLocation = Tel.own_Loc;
             g_dBounceTime = g_dElapsedTime + 0.3;
		 }
		 else {
			 //it's a trap, delete the '@' sign and make the player immobile for 1 second with the player bounce time
			 maze[Y][X] = ' ';
             g_dBounceTime = g_dElapsedTime + 1;
		 }
	 }

}
//Shania
void PickUpItems( void )
{
	int charY = g_sChar.m_cLocation.Y - 1; //call the coordinates of the player
	int charX = g_sChar.m_cLocation.X;

	if(maze[charY][charX] == '$')  //Check if the coordinates the character is on has $ 
	{
		//Beep (1440,30)
		ItemCounter++;             //Add 1 to the ItemCounter
		maze[charY][charX] = ' ';  //Change the $ to a space
	}

}

void exitLevel( void ) {

	int charY = g_sChar.m_cLocation.Y - 1;
	int charX = g_sChar.m_cLocation.X;

	if(maze[charY][charX] == '*' && ItemCounter == MaxItemCount)
	{
		//Beep (1440,30)
		if (levelCount == MAX_LEVEL-1) {
            g_eGameState = S_WIN;
		}
		else {
			levelCount = static_cast<GAMELEVELS>(levelCount + 1);
            levelClear = true;
		    g_eGameState = S_SPLASHSCREEN;
            BufferTime = g_dElapsedTime + 3.0;
        }
        for (int i = 0; i < rows; ++i) {
            delete[] maze[i];
        }
        showTeleporter = false;
	}

}