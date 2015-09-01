//This is the file for displaying the different game states
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "Framework\timer.h"
#include "Framework\console.h"
#include "game.h"
#include "menu.h"

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

//Storyline boolean
bool storyLine = true;

//Shania
void storyline( void )
{
	clearScreen();
    PlaySound(TEXT("egypt.wav"), NULL, SND_LOOP | SND_ASYNC); //Play the music
	COORD c = g_Console.getConsoleSize();

	c.X = g_Console.getConsoleSize().X / 16-6; //the x and y coords where the pharoah ascii art starts
	c.Y = g_Console.getConsoleSize().Y / 5;

	string pharaoh[10] = {   //string array
        "          .....",
        "         .\\:\\:/:/.",
        "        /=\\_.@._/=\\",
        "       /==/ _ _ \\==\\",
        "      /==(   -   )==\\",
        "      |===\\  =  /===|",
        "      \\====|-V-|====/",
        "       \\===|   |===/",
        "        |==|   |==|",
        "        '=.|   |.='"
    };
	
	for (int i = 0; i < 10; ++i, ++c.Y) {  //check through the string array and print out using writeToBuffer
        std::ostringstream ss;
        ss.str("");
        ss << pharaoh[i];
        g_Console.writeToBuffer(c, ss.str(), 0x0F);
    }

	c.X = 33; //the x and y coords where the storyline starts
	c.Y = 3;

	g_Console.writeToBuffer(c, "You are a greedy treasure hunter." , 0x0A); //use writetobuffer to print out the storyline
	c.Y += 2;
	c.X -= 6;
	g_Console.writeToBuffer(c, "One day, you heard about a Pyramid which has" , 0x0A);
	c.Y += 2;
	c.X -= 4;
	g_Console.writeToBuffer(c, "a lot of hidden treasures from the previous pharaoh." , 0x0A);
	c.Y += 2;
	c.X += 6;
    g_Console.writeToBuffer(c, "However, it is said to be well-guarded," , 0x0A);
	c.Y += 2;
	c.X -= 3;
	g_Console.writeToBuffer(c, "and no one has ever returned from the pyramid.", 0x0A);
	c.Y += 2;
	c.X += 1;
	g_Console.writeToBuffer(c, "Yet, you are unfazed, venturing towards the it.", 0x0A);
	c.Y += 2;
	c.X += 3;
	g_Console.writeToBuffer(c, "As you attempted to climb the pyramid,", 0x0A);
	c.Y += 2;
	c.X += 8;
	g_Console.writeToBuffer(c, "you fell into a maze.", 0x0A);
	c.Y += 4;
	c.X -= 9;
	g_Console.writeToBuffer(c, "Press ENTER to continue", 0x0B);
	g_Console.flushBufferToConsole();

	while(storyLine){   //boolean to make the screen stay til the player press enter

		userInput();   //get the player input

		if(menu_KeyPressed[K_ENT])
		{
			storyLine = false;  //go to the menu page
		}
	}
}

void userInput( void )
{
    menu_KeyPressed[K_1] = isKeyPressed(49);
    menu_KeyPressed[K_2] = isKeyPressed(50);
    menu_KeyPressed[K_3] = isKeyPressed(51);
    menu_KeyPressed[K_4] = isKeyPressed(52);
    menu_KeyPressed[K_ENT] = isKeyPressed(VK_RETURN);
    menu_KeyPressed[K_ESC] = isKeyPressed(VK_ESCAPE);
    
    addTime();
}

void addTime( void ) {

    ElapsedTime += g_Timer.getElapsedTime();

}

//for the switch sequence, 1 = player, 2 = instructions, 3 = highscore, 4 = options and 5 for exit
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

//Go back to main menu
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

//Detect the player's input to on and off the sounds in the game
void processInputSound(SequenceOPT &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }

    if(menu_KeyPressed[K_1])
	{
		if (!playmusic) {
			PlaySound(TEXT("menumusic.wav"), NULL, SND_LOOP | SND_ASYNC);
		}
        s = OptionsMenu;
        inputDetected = true;
		playmusic = true;  //boolean to keep playing the music
	}
	else if (menu_KeyPressed[K_2])
	{
		PlaySound(NULL,NULL,0);
        s = OptionsMenu;
        inputDetected = true;
		playmusic = false;  //boolean to stop playing music even after going back to main page and in game
	}

}

//User interface for main menu
//Writetobuffer to buffer the text and after wards flush to show the text in the screen
//Wei Min
void displayMenu( void )
{
	if(playmusic && menuplaymusic)
	{
		PlaySound(TEXT("menumusic.wav"), NULL, SND_LOOP | SND_ASYNC);
        menuplaymusic = false;
	}
    clearScreen();
	COORD c = g_Console.getConsoleSize();

    c.X /= 5;
    c.X += 2;
    c.Y /= 8;
    for (int i = 0; i < 40; ++i) {
		++c.X;
		g_Console.writeToBuffer(c, "=", 0x0D);
    }
    c.Y += 2;
    //SHOW GAME NAME
    c.X = g_Console.getConsoleSize().X / 2-5;
    g_Console.writeToBuffer(c, "ESCAPEE", 0x0A);
    c.X = g_Console.getConsoleSize().X / 4;
    --c.X;
	c.Y += 2;
    for (int i = 0; i < 38; ++i) {
		++c.X;
		g_Console.writeToBuffer(c, "=", 0x0D);
    }
    ++c.X;
    c.Y /= 4;
    c.Y += 2;
    for (int i = 0; i < 15; ++i)
    {
        ++c.Y;
        g_Console.writeToBuffer(c, "-", 0x0D);
    }
    c.Y = 0;
    c.Y /= 8;
    c.Y += 3;
    c.X /= 2;
    c.X -= 10;
    for (int i = 0; i < 15; ++i)
    {
        ++c.Y;
        g_Console.writeToBuffer(c, "-", 0x0D);
    }

    ++c.Y;
    c.X = 0;
    c.X = g_Console.getConsoleSize().X / 4;
    c.X -= 2;
        for (int i = 0; i < 40; ++i) {
		++c.X;
		g_Console.writeToBuffer(c, "=", 0x0D);
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
    g_Console.writeToBuffer(c, "Press '1' to play Game", 0x0A);
    c.Y += 1;

	c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press '2' for Instructions", 0x0A);
    c.Y += 1;

	c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press '3' for High Score", 0x0A);
	c.Y += 1;

    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press '4' to go Options", 0x0A);
    c.Y += 1;

    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x0A);
	g_Console.flushBufferToConsole();
	
}

//displaying the hows and what to do
//Glennda
void displayInstructions( void )
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
	c.X = g_Console.getConsoleSize().X / 2 - 22;
	g_Console.writeToBuffer(c, "Arrow keys are for movement.", 0x0D);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 22;
	g_Console.writeToBuffer(c, "Step over the objects in the maze to collect them.", 0x0D);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 22;
	g_Console.writeToBuffer(c, "Collecting objects along the way adds on to the Item Counter.", 0x0D);
    c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 22;
	g_Console.writeToBuffer(c, "Once all objects are collected, the exit is unlocked.", 0x0D);
    c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 22;
	g_Console.writeToBuffer(c, "Find the correct portal that leads to the exit.", 0x0D);
	c.Y += 4;
	c.X = g_Console.getConsoleSize().X / 3+4;
	g_Console.writeToBuffer(c, "Press ENTER to return", 0x0B);
	g_Console.flushBufferToConsole();

}

void displayHighscore( void )
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

void toCpp( void )
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

//Shania
//To get the user input and use switch sequence to check which one they inputed
void options( void ) {

    SequenceOPT s = OptionsMenu;
    ElapsedTime = 0.0;

    while (seq != Menu)
    {
        
        switch(s)
        {
            case OptionsMenu : displayOptions();  //This is to show Options
                while (!inputDetected) { //detect player input
                    userInput();
                    processInputOptions(s); 
                }
                inputDetected = false;  //while loop still run
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

//Shania
//Display the options to adjust sound or go back inside Options
void displayOptions( void ) {

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

//Shania
//Display the options to on or off the sound in the game
void displaySound( void ) {
    
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

void displayExit( void )
{
    clearScreen();
    
	/*Beep (1480,200);  
    Beep (1568,200);      
    Beep (1568,200);     
    Beep (1568,200);
	Beep (739.99,200);
    Beep (783.99,200);
    Beep (783.99,200);
    Beep (783.99,200);*/

    COORD c = g_Console.getConsoleSize();

	//Display exit text
	c.X = 12;
	c.Y = 2;

	g_Console.writeToBuffer(c,"~~YOU RIDE ON YOUR CAMEL ALL THE WAY HOME, GOOD BYE!~~", 0x0B);

	//Display ascii camel
	c.X = 3;
	c.Y = 5;

	g_Console.writeToBuffer(c,"                                                  =--_                ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                                   .-\"\"\"\"\"\"-.     |* _)               ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                                  /          \\   /  /                 ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                                 /            \\_/  /                  ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"     _                          /|                /                   ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c," _-'\"/\\                        / |    ____    _.-\"            _       ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"'   (  '-_            _       (   \\  |\\  /\\  ||           .-'\".\".     ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"     '.   `'-._   .-'\"/'.      \"   | |/ /  | |/        _-\"   (   '-_  ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"       '.      _-\"   (   '-_       \\ | /   \\ |     _.-'       )     \"-", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"     _.'   _.-'       )     \"-._    ||\\\\   |\\\\  '\"'        .-'        ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"   '               .-'          `'  || \\\\  ||))                       ", 0x0B);
	c.Y += 1;
    g_Console.writeToBuffer(c," _  ___  _ ____________ _____  ___ _|\\ _|\\_|\\\\/ _______________  ___  ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                 c  c  \" c C \"\"C  \" \"\"  \"\" \"\"                         ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"             c       C                                                ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"        C        C                                                    " , 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"             C                                                        ", 0x0B);

    
	g_Console.flushBufferToConsole();
    
	exitGame = true;
    Sleep(5000);
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
		PlaySound(TEXT("gamemusic.wav"), NULL, SND_LOOP | SND_ASYNC);
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