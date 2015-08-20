// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using std::string;

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

//Teleporters
Teleporters a_Tel;
Teleporters b_Tel;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once

//map rendering
//is there something we can do to not be declaring the variables globally?
char ** maze = 0;
int rows = 0;
int cols = 0;
int levelCount = 1;
bool levelClear = true;
int ItemCounter = 0;
int MaxItemCount = 0;

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
	
	levelCount = 1;
	// Set precision for floating point output
	g_dElapsedTime = 0.0;
	g_dBounceTime = 0.0;

	// sets the initial state for the game
	g_eGameState = S_SPLASHSCREEN;

	g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

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

		g_sChar.m_cLocation.X = 1;
		g_sChar.m_cLocation.Y = 2;

		levelClear = false;
		
		maze = 0;

		MaxItemCount = 0;
		ItemCounter = 0;

		a_Tel.a_Loc.X = 3;
		a_Tel.a_Loc.Y = 2;
		a_Tel.b_Loc.X = 16;
		a_Tel.b_Loc.Y = 10;

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

	}

	else if (levelClear && levelCount > 6) {
		g_bQuitGame = true;
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
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
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
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 1.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

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

	if (bSomethingHappened) {
		checkTeleport(g_sChar.m_cLocation);
		PickUpItems(g_sChar.m_cLocation);
		exitLevel(g_sChar.m_cLocation);
	}

    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE]) {
        g_bQuitGame = true;
		levelClear = true;
	}
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderSplashScreen()  // renders the splash screen
{
	//we can use this to say "LEVEL 1", etc
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 1 second", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
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
				g_Console.writeToBuffer(c, 178, 0x0F);
			}
		}
	}

}

void maze1(int& rows, int& cols) {

	string map1[21] = {
		"##################",
		"# #@             #",
		"# ### ######## # #",
		"#    $#  #   # # #",
		"### ### ## # # # #",
		"#   #    # # # # #",
		"# ###### # #@# # #",
		"#     #@   ### ###",
		"##### ######     #",
		"# # #*# *  # #####",
		"# # ###    #     #",
		"#   #@#    ##### #",
		"# ### #    #     #",
		"#     #   @# # # #",
		"# ### ######## # #",
		"# # # #@#   ## # #",
		"### # # # # ## # #",
		"#@# #   # #    # #",
		"# # ##### # ## # #",
		"#         # #  #@#",
		"##################" };

	rows = 21;
	maze = new char * [rows];
	cols = 18;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map1[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

}

void maze2(int& rows, int& cols) {

	string map2[13] = {
		"########################",
		"#     $#       #       #",
		"#### # # ##### # ##### #",
		"#    #    ## # #     # #",
		"# ########## # ##### # #",
		"#     @#     #     # # #",
		"#### ############# # # #",
		"#@   #@    *     #*# #@#",
		"########################",
		"#     @#    #@         #",
		"#  #####  ###########  #",
		"#     @#               #",
		"########################" };

	rows = 13;
	maze = new char * [rows];
	cols = 24;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map2[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

}

void maze3(int& rows, int& cols) {

	string map3[15] = {
		"###########################",
		"#         #       #       #",
		"######### # ############# #",
		"#*       $#       #       #",
		"##### ############# #######",
		"# # # #   #   #   #       #",
		"# # # # #   #   # #########",
		"# # # ############# # #   #",
		"# # # #             #   # #",
		"# # # #             ##### #",
		"# # # #              # #  #",
		"# # # ############## # # ##",
		"# # # #    *   #   # # #  #",
		"# #   #        # # #   ####",
		"###########################" };

	rows = 15;
	maze = new char * [rows];
	cols = 27;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map3[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

}

void maze4(int& rows, int& cols) {

	string map4[16] = {
		"#############################",
		"#   #         #       #     #",
		"# # # # ### ###### #    #   #",
		"# #   #          # # ## #####",
		"# ##### # # ###### # #   #  #",
		"# #     # #        ### # #  #",
		"# # ################ ### #  #",
		"# #     #          #     #  #",
		"# ##### # *        # # # #  #",
		"# #   # ############ # ###  #",
		"# ### # #            #      #",
		"#     # # ####### #### ###  #",
		"#  ###### #    #       # #  #",
		"## #      ###  # ####### # ##",
		"#$  *#         # #          #",
		"#############################" };

	rows = 16;
	maze = new char * [rows];
	cols = 29;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map4[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

}

void maze5(int& rows, int& cols) {

	string map5[14] = {
		"#######################",
		"#   #         #       #",
		"# #  $##  ### ##### # #",
		"# ####    # #$    # # #",
		"#    #### # #### *    #",
		"# # #   # #      # # ##",
		"# # # ############ #  #",
		"# # # #     *    # ####",
		"###   ############ #  #",
		"# #####            #  #",
		"#     ######  # ####  #",
		"##### #   #   #       #",
		"#       #   # #   #   #",
		"#######################" };

	rows = 14;
	maze = new char * [rows];
	cols = 23;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map5[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

}

void maze6(int& rows, int& cols) {

	string map6[22] = {
		"##############################",
		"#  @  #   #     #    #       #",
		"# # ### # # ### # ## # #######",
		"# # #   #   #   #  # # #     #",
		"# ### ####### ###### # # ### #",
		"# #   #   #   #   ## # # # # #",
		"# # ### # ##### # #  # # #   #",
		"# #     #       #    #   # # #",
		"# ### ###################### #",
		"# @   #       * @    #   # # #",
		"#######              ##### # #",
		"#   # #        $     #     # #",
		"### # ########################",
		"#     #      #       #     # #",
		"############################ #",
		"#             ##   #   #   # #",
		"# # ## ########  #   #   #   #",
		"# # #  #    # # ##############",
		"#   ####### #                #",
		"#####     # ################ #",
		"#     # #   #                #",
		"##############################" };

	rows = 22;
	maze = new char * [rows];
	cols = 30;

	for (int i = 0; i < rows; ++i) {
		maze[i] = new char[cols];
		string temp = map6[i];
		for (int j = 0; j < cols; ++j) {
			maze[i][j] = temp[j];
		}
	}

}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
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

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);

	// displays the item counter
	ss.str("");
	ss << "$: " << ItemCounter << "/" << MaxItemCount;
	c.X = 15;
	c.Y = 0;
	g_Console.writeToBuffer(c, ss.str(), 0x59);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void checkTeleport(COORD c) {

	int Y = g_sChar.m_cLocation.Y - 1;
	int X = g_sChar.m_cLocation.X;

	 if (maze[Y][X] == '@')
        {
             if(g_sChar.m_cLocation.X == a_Tel.a_Loc.X  && g_sChar.m_cLocation.Y == a_Tel.a_Loc.Y)
                {
                     g_sChar.m_cLocation.X =a_Tel.b_Loc.X;
                     g_sChar.m_cLocation.Y =a_Tel.b_Loc.Y;
                }
        
            else if(g_sChar.m_cLocation.X == a_Tel.b_Loc.X  && g_sChar.m_cLocation.Y == a_Tel.b_Loc.Y)
                {
                  g_sChar.m_cLocation.X = a_Tel.a_Loc.X;
                 g_sChar.m_cLocation.Y = a_Tel.a_Loc.Y;
                }
         }

}

void PickUpItems(COORD c)
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

void exitLevel(COORD c) {

	int charY = g_sChar.m_cLocation.Y - 1;
	int charX = g_sChar.m_cLocation.X;

	if(maze[charY][charX] == '*' && ItemCounter == MaxItemCount)
	{
		//Beep (1440,30)
		++levelCount;
		levelClear = true;
	}

}