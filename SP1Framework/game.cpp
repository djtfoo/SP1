// This is the main file for the game logic and function

#include "game.h"
#include "Framework\console.h"
#include "Framework\timer.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

using std::ofstream;
using std::string;
using std::vector;

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
vector<Enemy> enemyvec;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
double  g_dBounceTimeEnemy;
double	playTime;		//to record the gameplay time only
double	BufferTime;		//for splashscreen

//map rendering
//is there something we can do to not be declaring the variables globally?
char ** maze = 0;
int rows = 0;
int cols = 0;
int levelCount;
bool levelClear;
int ItemCounter = 0;
int MaxItemCount = 0;

//Teleporter
ExitTeleporter Tel;

//high score name
char name[11];
char * pointer = 0;
int i = 0;

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
	BufferTime = 3.0;

	// sets the initial state for the game
	levelCount = 1;
    levelClear = true;
	g_eGameState = S_SPLASHSCREEN;

	g_sChar.m_bActive = true;
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

void levelInit() {

	if (levelClear && levelCount <= 6) {

        //resetting the player's coordinates to the default starting point (the top left corner)
		g_sChar.m_cLocation.X = 1;
		g_sChar.m_cLocation.Y = 2;

		levelClear = false; //to prevent levelInit() from running again while the player hasn't finished the level
		
        //clearing out the values that are different for each level
		maze = 0;
		MaxItemCount = 0;
		ItemCounter = 0;
        enemyvec.clear();

		/*switch (static_cast<GAMELEVELS>(levelCount)) {
		case M1 : maze1(rows, cols); break;
		case M2 : maze2(rows, cols); break;
		case M3 : maze3(rows, cols); break;
		case M4 : maze4(rows, cols); break;
		case M5 : maze5(rows, cols); break;
		case M6 : maze6(rows, cols); break;
		case MAX_LEVEL : g_bQuitGame = true; levelClear = false; break;
		}
		*/

		if (levelCount == 1) {
			maze1(rows, cols);
		}

		else if (levelCount == 2) {
			maze2(rows, cols);
		}
		else if (levelCount == 3) {
			maze3(rows, cols);
		}

		else if (levelCount == 4) {
			maze4(rows, cols);
		}

		else if (levelCount == 5) {
			maze5(rows, cols);
		}

		else if (levelCount == 6) {
			maze6(rows, cols);
		}

		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				if (maze[i][j] == '$') {
					++MaxItemCount;
				}
			}
		}

		//randomising of teleporters here
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
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);

	//for pause screen
	g_abKeyPressed[K_ONE] = isKeyPressed(49);
	g_abKeyPressed[K_TWO] = isKeyPressed(50);

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

		case S_PAUSE : pauseGame(); 
			break;

        case S_WIN : clearGame();
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
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
        case S_GAME: renderGame();
            break;
        case S_WIN : renderClearGame();
            break;
		case S_PAUSE : renderPauseGame();
			break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game

}

void gameplay()            // gameplay logic
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

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime) {
        return;
    }

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
	int charY = g_sChar.m_cLocation.Y - 1;
	int charX = g_sChar.m_cLocation.X;

    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
    {
		if (maze[charY-1][charX] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.Y--;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
		if (maze[charY][charX-1] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
		if (maze[charY+1][charX] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
		if (maze[charY][charX+1] != '#') {
			//Beep(1440, 30);
			g_sChar.m_cLocation.X++;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_SPACE])
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;
        bSomethingHappened = true;
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

void processUserInput()
{
    // pauses the game if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE]) {
		g_eGameState = S_PAUSE;
	}
}

void processPauseInput()
{
	if (g_abKeyPressed[K_ONE]) {
		g_eGameState = S_GAME;
	}

	if (g_abKeyPressed[K_TWO]) {
		g_bQuitGame = true;
	}
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void splashScreenWait() {

	if (g_dElapsedTime > BufferTime) {
		g_eGameState = S_GAME;
	}

}

void renderSplashScreen()  // renders the splash screen
{

	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = c.X / 2 - 9;
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(3);
	ss.str("");
	ss << "Level " << levelCount;
	g_Console.writeToBuffer(c, ss.str(), 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	//g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 9;
	g_Console.writeToBuffer(c, "Press 'Esc' to pause", 0x09);

}

//Jing Ting
void clearGame() {

    processNameInput(name);

}

void renderClearGame() {
    clearScreen();
    renderText();
	renderPlaytime();
    renderNameInput(name);
}

void pauseGame() {
	processPauseInput();
}

void renderPauseGame() {

	clearScreen();
	COORD c = g_Console.getConsoleSize();
	c.X /= 2;
	c.Y /= 3;
	g_Console.writeToBuffer(c, "Press '2' to quit game", 0x03);
	c.X /= 2;
	c.Y /= 3;
	g_Console.writeToBuffer(c, "Press '1' to return", 0x03);
}

void renderText() {
    COORD c = g_Console.getConsoleSize();
    c.X /= 2;
    c.Y /= 3;
    g_Console.writeToBuffer(c, "YOU WIN!", 0x03);
	c.Y++;
	g_Console.writeToBuffer(c, "Input your name: ", 0x03);
}

void renderNameInput(char * name) {
    COORD c = g_Console.getConsoleSize();
    c.X /= 2;
    c.Y /= 3;
    c.Y += 2;
    g_Console.writeToBuffer(c, name, 0x03);
}

void processNameInput(char * name) {

    bool keySomething = false;
    bool backspace = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

    if (pointer != name+9) {
        if (g_abKeyPressed[K_A]) {
            *(pointer+1) = 'A';
            keySomething = true;
        }
        if (g_abKeyPressed[K_B]) {
            *(pointer+1) = 'B';
            keySomething = true;
        }
        if (g_abKeyPressed[K_C]) {
            *(pointer+1) = 'C';
            keySomething = true;
        }
        if (g_abKeyPressed[K_D]) {
            *(pointer+1) = 'D';
            keySomething = true;
        }
        if (g_abKeyPressed[K_E]) {
            *(pointer+1) = 'E';
            keySomething = true;
        }
        if (g_abKeyPressed[K_F]) {
            *(pointer+1) = 'F';
            keySomething = true;
        }
        if (g_abKeyPressed[K_G]) {
            *(pointer+1) = 'G';
            keySomething = true;
        }
        if (g_abKeyPressed[K_H]) {
            *(pointer+1) = 'H';
            keySomething = true;
        }
        if (g_abKeyPressed[K_I]) {
            *(pointer+1) = 'I';
            keySomething = true;
        }
        if (g_abKeyPressed[K_J]) {
            *(pointer+1) = 'J';
            keySomething = true;
        }
        if (g_abKeyPressed[K_K]) {
            *(pointer+1) = 'K';
            keySomething = true;
        }
        if (g_abKeyPressed[K_L]) {
            *(pointer+1) = 'L';
            keySomething = true;
        }
        if (g_abKeyPressed[K_M]) {
            *(pointer+1) = 'M';
            keySomething = true;
        }
        if (g_abKeyPressed[K_N]) {
            *(pointer+1) = 'N';
            keySomething = true;
        }
        if (g_abKeyPressed[K_O]) {
            *(pointer+1) = 'O';
            keySomething = true;
        }
        if (g_abKeyPressed[K_P]) {
            *(pointer+1) = 'P';
            keySomething = true;
        }
        if (g_abKeyPressed[K_Q]) {
            *(pointer+1) = 'Q';
            keySomething = true;
        }
        if (g_abKeyPressed[K_R]) {
            *(pointer+1) = 'R';
            keySomething = true;
        }
        if (g_abKeyPressed[K_S]) {
            *(pointer+1) = 'S';
            keySomething = true;
        }
        if (g_abKeyPressed[K_T]) {
            *(pointer+1) = 'T';
            keySomething = true;
        }
        if (g_abKeyPressed[K_U]) {
            *(pointer+1) = 'U';
            keySomething = true;
        }
        if (g_abKeyPressed[K_V]) {
            *(pointer+1) = 'V';
            keySomething = true;
        }
        if (g_abKeyPressed[K_W]) {
            *(pointer+1) = 'W';
            keySomething = true;
        }
        if (g_abKeyPressed[K_X]) {
            *(pointer+1) = 'X';
            keySomething = true;
        }
        if (g_abKeyPressed[K_Y]) {
            *(pointer+1) = 'Y';
            keySomething = true;
        }
        if (g_abKeyPressed[K_Z]) {
            *(pointer+1) = 'Z';
            keySomething = true;
        }

    }

    if (g_abKeyPressed[K_BACKSPACE]) {
        *pointer = '\0';
        keySomething = true;
        backspace = true;
    }

    // saves the name if the player presses ENTER key
    if (g_abKeyPressed[K_ENTER]) {
        ofstream outData;

	    outData.open("Timings.txt", ofstream::app);

	    outData << std::endl << name << " " << playTime;

	    outData.close();

        g_bQuitGame = true;
	}

    if (keySomething)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
        if (!backspace && pointer < name+9) {
            ++pointer;
        }
        if (backspace && pointer >= name) {
            pointer--;
        }
    }

}

void renderGame()
{
    renderCounters();   // counters will be rendered in gameplay() only
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderEnemy();
}

void renderMap()
{
    // Set up sample colours, and output shadings

	mapgenerator(rows, cols);

}

void mapgenerator(int rows, int cols) {

	COORD c;

	for (int i = 0; i < rows; ++i) {
		c.Y = i+1;
		for (int j = 0; j < cols; ++j) {
			c.X = j;
			if (maze[i][j] == '@') {
				g_Console.writeToBuffer(c, '@', 0x1A);
			}
			else if (maze[i][j] == '$') {
				g_Console.writeToBuffer(c, '$', 0x1C);
			}
			else if (maze[i][j] == '*') {
				g_Console.writeToBuffer(c, '*', 0x1D);
			}
			else if (maze[i][j] == '#') {
				g_Console.writeToBuffer(c, (char)178, 0x1F);
			}
		}
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
	srand(time(NULL));
	int teleport = rand() % size;
	Traps tempTel = trapvec.at(teleport);
	
	Tel.warp_Loc = tempTel.Loc;

}

void maze1(int& rows, int& cols) {

	string map1[21] = {
        "################################",   // 3 Items, 5 teleports, 2 AI, 1 trap
        "#              #  #@           #",   
		"##### ######## ## ###########  #",
		"#     # @# $ # #  #            #",
        "#   ### ## # # # ## #### #######",
        "#        # # # #               #",
        "# ###### #  # #  ######  ##### #",
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

    //Enemy 1
    Enemy g_Enemy1;
	g_Enemy1.m_Enemy.X = 5;
    g_Enemy1.m_Enemy.Y = 2;
    g_Enemy1.d = right;
    enemyvec.push_back(g_Enemy1);

    //Enemy2
    Enemy g_Enemy2;
    g_Enemy2.m_Enemy.X = 16;
    g_Enemy2.m_Enemy.Y = 12;
    g_Enemy2.d = up;
    enemyvec.push_back(g_Enemy2);
}

void maze2(int& rows, int& cols) {

	string map2[21] = {
        "################################",    // 5 Items, 5 teleports, 3 AI, 2 traps 
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
        "################################",    // 8 Items, 8 teleports, 3 AI, 3 traps
        "# #  #     #               #   #",
        "# #  # ### ###   # ######### # #",
        "# #          # # # #@        # #",
        "# ## ####### # # # ####### # # #",
        "#      #     #$# #       # # # #",
		"## ##  # ### ### # ####### ### #",
        "#  $#    # #  @# # #$      #   #",
        "##  ###### # ### # ####### #   #",
        "#   # @#   #     #         # # #",
        "#   # ## # ############### #$#@#",
        "# #@#    ###@ #@         # #####",
        "# ###  ##     #         *#   #$#",
        "#  #    #$#   ############## # #",
        "# ##### ###   #  #      #    # #",
		"#           ###    #  #   #    #",
        "## ### ###  #$############## # #",
        "#  #   #      #   # @ #   #    #",
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
	g_Enemy3.m_Enemy.X = 3;
	g_Enemy3.m_Enemy.Y = 10;
	g_Enemy3.d = up;
	enemyvec.push_back(g_Enemy3);
}

void maze4(int& rows, int& cols) {

	string map4[24] = {
        "###################################",   // 10 Items, 10 teleportss, 4 AI, 5 traps
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
        "########################################",   // 10 Items, 15 teleports, 5 AI, 8 traps
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
    g_Enemy3.m_Enemy.X = 31;
    g_Enemy3.m_Enemy.Y = 11;
    g_Enemy3.d = up;
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
        "#######################################",  //  10 Items, 15 teleports, 2AI, 15traps
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

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x1C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x1A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
}

void renderEnemy()
{
    WORD enemyColor = 0x1C;
    for (unsigned int i = 0; i < enemyvec.size(); ++i) {
        Enemy tempEnemy = enemyvec[i];
        COORD tempE = tempEnemy.m_Enemy;
        g_Console.writeToBuffer(tempE, (char)88, enemyColor);
    }
}

//Shania + Jing Ting
void moveEnemy(Enemy& g_Enemy)
{

    //Get the enemy coordinates
    int eX = g_Enemy.m_Enemy.X;
    int eY = g_Enemy.m_Enemy.Y-1;
	
    //Move the enemy
	//If enemy next X position is not a wall
    if (g_Enemy.d == right && maze[eY][eX+1] != '#') //move right
    {
        g_Enemy.m_Enemy.X++;
    }

    else if(g_Enemy.d == left && maze[eY][eX-1] != '#') //move left
    {
        g_Enemy.m_Enemy.X--;
    }
    else if (g_Enemy.d == up && maze[eY-1][eX] != '#') //move up
    {
        g_Enemy.m_Enemy.Y--;
    }
	else if(g_Enemy.d == down && maze[eY+1][eX] != '#') //move down
    {
        g_Enemy.m_Enemy.Y++;
    }

	//change direction
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

void enemyCollisionWithPlayer(Enemy g_Enemy) {

	//If character touches the enemy, spawn the character back to starting location
    if (g_sChar.m_cLocation.X == g_Enemy.m_Enemy.X && g_sChar.m_cLocation.Y == g_Enemy.m_Enemy.Y)
    {
        g_sChar.m_cLocation.X = 1;
        g_sChar.m_cLocation.Y = 2;
    }

}

void renderFramerate()
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

void renderCounters() {

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
}

void renderPlaytime() {

	COORD c;
	std::ostringstream ss;
    // displays the playtime
    ss << "Time: " << playTime << "secs";
    c.X = 50;
    c.Y = 6;
    g_Console.writeToBuffer(c, ss.str());

}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void checkTrap() {

	int Y = g_sChar.m_cLocation.Y - 1;
	int X = g_sChar.m_cLocation.X;

	 if (maze[Y][X] == '@') {
		 if (g_sChar.m_cLocation.X == Tel.own_Loc.X && g_sChar.m_cLocation.Y == Tel.own_Loc.Y) {
			 g_sChar.m_cLocation = Tel.warp_Loc;
             g_dBounceTime = g_dElapsedTime + 0.3;
		 }
		 else if (g_sChar.m_cLocation.X == Tel.warp_Loc.X && g_sChar.m_cLocation.Y == Tel.warp_Loc.Y) {
			 g_sChar.m_cLocation = Tel.own_Loc;
             g_dBounceTime = g_dElapsedTime + 0.3;
		 }
		 else {
			 //it's a trap
			 maze[Y][X] = ' ';
             g_dBounceTime = g_dElapsedTime + 1;
		 }
	 }

}

//Shania
void PickUpItems()
{
	int charY = g_sChar.m_cLocation.Y - 1;
	int charX = g_sChar.m_cLocation.X;

	if(maze[charY][charX] == '$')
	{
		//Beep (1440,30)
		ItemCounter++;
		maze[charY][charX] = ' ';
	}

}

//Jing Ting
void exitLevel() {

	int charY = g_sChar.m_cLocation.Y - 1;
	int charX = g_sChar.m_cLocation.X;

	if(maze[charY][charX] == '*' && ItemCounter == MaxItemCount)
	{
		//Beep (1440,30)
		if (levelCount == 6) {
            g_eGameState = S_WIN;
		}
		else {
			++levelCount;
            levelClear = true;
		    g_eGameState = S_SPLASHSCREEN;
            BufferTime = g_dElapsedTime + 3.0;
        }
	}

}