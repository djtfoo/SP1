//This is the file for displaying the different game states

#include "Framework\timer.h"
#include "Framework\console.h"
#include "game.h"
#include "menu.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::ifstream;

//detecting key input
bool menu_KeyPressed[K_COUNT];

//timer and framerate
CStopWatch g_Timer;                            // Timer function to keep track of time and the frame rate
bool g_bQuitGame = false;                    // Set to true if you want to quit the game
const unsigned char gc_ucFPS = 100;                // FPS of this game
const unsigned int gc_uFrameTime = 1000 / gc_ucFPS;    // time for each frame

//PlaySound
bool playmusic = true;      //check if user wants music or not
bool menuplaymusic = true;  //while user remains in the menus, don't refresh the main menu music

void userInput() //If s is modified, seq is modified as well
{
    menu_KeyPressed[K_1] = isKeyPressed(49);
    menu_KeyPressed[K_2] = isKeyPressed(50);
    menu_KeyPressed[K_3] = isKeyPressed(51);
    menu_KeyPressed[K_4] = isKeyPressed(52);
    menu_KeyPressed[K_ENT] = isKeyPressed(VK_RETURN);
    menu_KeyPressed[K_ESC] = isKeyPressed(VK_ESCAPE);
    
    addTime();
}

void addTime() {

    ElapsedTime += g_Timer.getElapsedTime();

}

void processInputMenu(Sequence &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }
    if (menu_KeyPressed[K_1]) {
        s = Play;
        inputDetected = true;
    }
    else if (menu_KeyPressed[K_2]) {
        s = Instructions;
        inputDetected = true;
    }
    else if (menu_KeyPressed[K_3]) {
        s = HighScore;
        inputDetected = true;
    }
    else if (menu_KeyPressed[K_4]) {
        s = Options;
        inputDetected = true;
    }
    else if (menu_KeyPressed[K_ESC]) {
        s = Exit;
        inputDetected = true;
    }

}

void processInputBack(Sequence &s) {

    if (menu_KeyPressed[K_ENT]) {
        s = Menu;
        inputDetected = true;
    }

}

void processInputOptions(SequenceOPT &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }

    if(menu_KeyPressed[K_1])
	{
		s = Sound;
        inputDetected = true;
	}
	else if (menu_KeyPressed[K_ENT])
	{
		s = Back;
        inputDetected = true;
	}
    
}

void processInputSound(SequenceOPT &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }

    if(menu_KeyPressed[K_1])
	{
		if (!playmusic) {
			PlaySound( "menumusic.wav", NULL, SND_LOOP | SND_ASYNC);
		}
        s = OptionsMenu;
        inputDetected = true;
		playmusic = true;
	}
	else if (menu_KeyPressed[K_2])
	{
		PlaySound(NULL,NULL,0);
        s = OptionsMenu;
        inputDetected = true;
		playmusic = false;
	}

}

void displayMenu()
{
	if(playmusic && menuplaymusic)
	{
		PlaySound( "menumusic.wav", NULL, SND_LOOP | SND_ASYNC);
        menuplaymusic = false;
	}
    clearScreen();
	COORD c = g_Console.getConsoleSize();

    //GAME NAME
    c.X /= 5;
    c.X += 1;
    c.Y /= 8;
     for (int i = 20; i < 62; ++i) {
		c.X++;
		g_Console.writeToBuffer(c, "=", 0x06);
		if (i == 62) {
			break;
		}
    }
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2-5;
    g_Console.writeToBuffer(c, "ESCAPEE", 0x0A);
    c.X = g_Console.getConsoleSize().X / 4;
	c.Y += 2;
    for (int i = 20; i < 58; ++i) {
		c.X++;
		g_Console.writeToBuffer(c, "=", 0x06);
		if (i == 58) {
			break;
		}
    }
    c.Y /= 4;
    c.Y += 2;
    for (int i = 0; i < 15; ++i)
    {
        c.Y++;
        g_Console.writeToBuffer(c, "-", 0x06);
        if (i == 15)
        {
            break;
        }

    }
    c.Y = 0;
    c.Y /= 8;
    c.Y += 3;
    c.X /= 2;
    c.X -= 10;
    for (int i = 0; i < 15; ++i)
    {
        c.Y++;
        g_Console.writeToBuffer(c, "-", 0x06);
        if (i == 15)
        {
            break;
        }
    }

    c.Y ++;
    c.X = 0;
    c.X = g_Console.getConsoleSize().X / 4;
    c.X -= 2;
        for (int i = 20; i < 60; ++i) {
		c.X++;
		g_Console.writeToBuffer(c, "=", 0x06);
		if (i == 60) {
			break;
		}
    }

    c.Y = 0;
    c.Y /= 2;
    c.Y += 8;
    c.X = g_Console.getConsoleSize().X / 2;
    c.X -= 4;
    c.Y += 2;
    g_Console.writeToBuffer (c,"MENU", 0x0B);
  
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press '1' to play Game", 0x0D);
    c.Y += 1;

	c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press '2' for Instructions", 0x0D);
    c.Y += 1;

	c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press '3' for High Score", 0x0D);
	c.Y += 1;

    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press '4' to go Options", 0x0D);
    c.Y += 1;

    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x0D);
	g_Console.flushBufferToConsole();
	
}

void displayInstructions()
{
	clearScreen();
	COORD c = g_Console.getConsoleSize();
	c.Y /= 4;
	c.X = g_Console.getConsoleSize().X / 2 - 6;
	g_Console.writeToBuffer(c, "INSTRUCTIONS", 0x0B);
	c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 23;
	g_Console.writeToBuffer(c, "Players' main objective is to escape the maze.", 0x0A);
	c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "1) Arrow keys are for movement", 0x0D);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "2) Collect all objects in the maze", 0x0D);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "3) Escape the room after collecting ALL objects", 0x0D);
	c.Y += 4;
	c.X = g_Console.getConsoleSize().X / 3+4;
	g_Console.writeToBuffer(c, "Press ENTER to return", 0x0B);
	g_Console.flushBufferToConsole();

}

void displayHighscore()
{
	clearScreen();
	toCpp();
	COORD c = g_Console.getConsoleSize();
	c.Y /= 10;
    c.X = g_Console.getConsoleSize().X / 2 - 7;
    g_Console.writeToBuffer(c, "HIGH SCORE", 0x0B);
	c.Y /= 3;
	c.X = 0;
	for (int i = 3; i < 80; ++i) {
		c.X++;
		g_Console.writeToBuffer(c, "_", 0x06);
		if (i == 80) {
			break;
		}
	}
	c.Y += 4;
	c.X = 0;
	for (int i = 3; i < 80; ++i) {
		c.X++;
		g_Console.writeToBuffer(c, "_", 0x06);
	}

	c.Y += 2;
	c.X = c.X / 2 - 10;
	for (int i = 1; i < 11; ++i) {
		c.Y++;
		std::ostringstream ss;
		ss.str("");
		ss << i;
		g_Console.writeToBuffer(c, ss.str(), 0x08);
	}

	c.Y += 4;
	c.X = g_Console.getConsoleSize().X / 2 - 10;
	g_Console.writeToBuffer(c, "Press ENTER to return", 0x0B);
	
	g_Console.flushBufferToConsole();
}

void toCpp()
{
	// timings copied to displayHighscore
	ifstream inData;
	string data;
	vector<highScore> highS;

	inData.open ("Timings.txt");

	while (!inData.eof()) {
		getline (inData, data);
		std::istringstream tempData(data);
		string tempName, tempTime;
		tempData >> tempName;
		tempData >> tempTime;

		highScore temp;
		temp.time = std::stod(tempTime);
		temp.name = tempName;
		highS.push_back(temp);
	}

	inData.close();

	for (unsigned int i = 0; i < highS.size(); ++i) {
		for (unsigned int j = 0; j < highS.size() - 1; ++j) {
			highScore current = highS[j];
			highScore next = highS[j+1];
			if (current.time > next.time) {
				highScore temp = current;
				highS[j] = next;
				highS[j+1] = temp;
			}
		}
	}

	COORD c = g_Console.getConsoleSize();
	c.Y = 7;
	c.X = g_Console.getConsoleSize().X / 2 - 10;
	std::ostringstream ss;

	for (unsigned int i = 0; i < highS.size(); ++i, ++c.Y) {
		highScore temp = highS[i];
		ss.str("");
		ss << temp.name << " " << temp.time;
		g_Console.writeToBuffer(c, ss.str(), 0xA1);
		if (i == 9) {
			break;
		}
	}

}

void options() {

    SequenceOPT s = OptionsMenu;
    ElapsedTime = 0.0;

    while (seq != Menu)
    {
        
        switch(s)
        {
            case OptionsMenu : displayOptions();
                while (!inputDetected) {
                    userInput();
                    processInputOptions(s);
                }
                inputDetected = false;
                break;
            case Sound : displaySound();
                ElapsedTime = 0.0;
                while (!inputDetected) {
                    userInput();
                    processInputSound(s);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            case Back : seq = Menu; break;
        }
    }

}

void displayOptions() {

    clearScreen();

    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 5;
    g_Console.writeToBuffer(c, "OPTIONS", 0x0B);
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 2 - 12;
    g_Console.writeToBuffer(c, "Press '1' for Sound", 0x0D);
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 2 -11;    
    g_Console.writeToBuffer(c, "Press ENTER to Return", 0x0B);
    g_Console.flushBufferToConsole();

}

void displaySound() {
    
    clearScreen();

    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "EDIT SOUND HERE", 0x0B);
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 2 - 15;
    g_Console.writeToBuffer(c, "Press '1' to switch on sound", 0x0D);
    c.Y += 2;
    g_Console.writeToBuffer(c, "Press '2' to switch off sound", 0x0D);
    g_Console.flushBufferToConsole();

}

void displayExit()
{
    clearScreen();

    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 3;
    g_Console.writeToBuffer(c, "HAVE A NICE DAY, BYE~~", 0x0B);
    g_Console.flushBufferToConsole();

    exitGame = true;
    Sleep(1500);
}

//--------------------------------------------------------------
// Purpose  : This main loop calls functions to get input, 
//            update and render the game at a specific frame rate
//            You should not be modifying this unless you know what you are doing.
// Input    : void
// Output   : void
//--------------------------------------------------------------
void displayGame( void )
{
	if(playmusic)
	{
		PlaySound( "gamemusic.wav", NULL, SND_LOOP | SND_ASYNC);
        menuplaymusic = true;
	}
    g_Timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	init();
	while (!g_bQuitGame)      // run this loop until user wants to quit 
    {
		getInput();                         // get keyboard input
		update(g_Timer.getElapsedTime());   // update the game
		render();                           // render the graphics output to screen
		g_Timer.waitUntil(gc_uFrameTime);   // Frame rate limiter. Limits each frame to a specified time in ms.
    }
    g_bQuitGame = false;		//to enable player to play game again
}