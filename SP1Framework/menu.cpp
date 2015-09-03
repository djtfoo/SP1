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

//menu colours
WORD Highlight = 0x3A;      //the selected option in the menu
WORD nonHighlight = 0x0A;   //non-selected options

//main menu
WORD coloursMenu[] = {Highlight, nonHighlight, nonHighlight, nonHighlight, nonHighlight};   //start with the first option highlighted
WORD *ptrMenu = coloursMenu;    //to point at the coloursMenu[] array
WORD mColor[] =
    {
        0x02,
        0x03,
        0x05,
        0x08,
        0x09,
        0x0A,
        0x0B,
        0x0C,
        0x0D,
        0x0E
    };

Sequence menu[] = {
    Play,
    Instructions,
    HighScore,
    Options,
    Exit
};

//options menu
WORD coloursOptions[] = {Highlight, nonHighlight, nonHighlight};  //start with the first option highlighted
WORD *ptrOPT = coloursOptions;  //to point at the coloursOptions[] array

SequenceOPT option[] = {
    Sound,
    Character,
    Back,
};

//options sub-menu: Sound
WORD coloursSound[] = {Highlight, nonHighlight};    //start with the first option highlighted
WORD *ptrSound = coloursSound;  //to point at the coloursSound[] array

//Shania
//To print out line by line the storyline before the game starts
void storyline( void )
{
    clearScreen();
    //Play the music
    PlaySound(TEXT("egypt.wav"), NULL, SND_LOOP | SND_ASYNC);
	COORD c = g_Console.getConsoleSize();

    //the x and y coords where the pharoah ascii art starts
	c.X = g_Console.getConsoleSize().X / 16-8; 
	c.Y = g_Console.getConsoleSize().Y / 4;

	string pharaoh[10] = {   //string array
        "          .......",
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
	
    //check through the string array and print out using writeToBuffer
	for (int i = 0; i < 10; ++i, ++c.Y) {  
        std::ostringstream ss;
		ss.str("");
        ss << pharaoh[i];
        g_Console.writeToBuffer(c, ss.str(), 0x0E);
    }
    
	c.X = 19;
	c.Y = 3;

    string storyline[9] = {
        "           You are a greedy treasure hunter.",
		"Through your adventures, you heard rumours of a pyramid",
		"       which houses a lot of hidden treasures.",
		"         However, no one has ever been known",
		"              to return from exploring it.",
		"  Yet, you are unfazed, and ventured to the pyramid.",
		"             As you explored the pyramid,",
		"    you activated a trap door and fell into a maze."
	};

	//check through the string array and print out using writeToBuffer
    for (int i = 0; i < 9; ++i, c.Y += 2) {  
		std::ostringstream ss;
		ss.str("");
		ss << storyline[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0A);
		g_Console.flushBufferToConsole();	
			
		userInput();   //get the player input
	
		if(menu_KeyPressed[MK_SPACE]){  //if space detected the text will print out faster
			Sleep(300);
		}
		else{
			Sleep(1500); //text will print out slower
		}
	}

    c.X = 30;
    c.Y = 21;

    int IElapsedTime;
    WORD menuColor;

    //boolean to make the screen stay til the player press enter
	while(storyLine){   
		userInput();   //get the player input

        IElapsedTime = ElapsedTime;
        menuColor = mColor[IElapsedTime % 10];

	    g_Console.writeToBuffer(c, "Press ENTER to continue", menuColor);
	    g_Console.flushBufferToConsole();

        if(menu_KeyPressed[MK_ENT]){
            storyLine = false;  //go to the menu page
            //reset the time elapsed since the program starts to detect key input
            ElapsedTime = 0.0;  
            //reset the bounce time to 0.3 to prevent possibility of enter being detected again in main menu
            BounceTime = 0.3;   
        }
    }
}

void userInput() //If s is modified, seq is modified as well
{
    menu_KeyPressed[MK_UP] = isKeyPressed(VK_UP);
    menu_KeyPressed[MK_DOWN] = isKeyPressed(VK_DOWN);
    menu_KeyPressed[MK_RIGHT] = isKeyPressed(VK_RIGHT);
    menu_KeyPressed[MK_LEFT] = isKeyPressed(VK_LEFT);
    menu_KeyPressed[MK_ENT] = isKeyPressed(VK_RETURN);
    menu_KeyPressed[MK_SPACE] = isKeyPressed(VK_SPACE);
    
    addTime();
}

void addTime( void ) {

    ElapsedTime += g_Timer.getElapsedTime();

}

void processInputMenu(Sequence &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }

    if (menu_KeyPressed[MK_UP]) {
        if (ptrMenu != coloursMenu) {
            *ptrMenu = nonHighlight;
            --ptrMenu;
            *ptrMenu = Highlight;
            BounceTime = 0.2;
        }
        inputDetected = true;
    }
    else if (menu_KeyPressed[MK_DOWN]) {
        if (ptrMenu != coloursMenu+static_cast<int>(MAX_SEQUENCE)-2) {
            *ptrMenu = nonHighlight;
            ++ptrMenu;
            *ptrMenu = Highlight;
            BounceTime = 0.2;
        }
        inputDetected = true;
    }
    else if (menu_KeyPressed[MK_ENT]) {
        int offset = ptrMenu - coloursMenu;
        s = menu[offset];
        inputDetected = true;
        BounceTime = 0.3;
    }
}

void processInputBack(Sequence &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }

    if (menu_KeyPressed[MK_ENT]) {
        s = Menu;
        inputDetected = true;
    }

}

void processInputOptions(SequenceOPT &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }

    if (menu_KeyPressed[MK_UP]) {
        if (ptrOPT != coloursOptions) {
            *ptrOPT = nonHighlight;
            --ptrOPT;
            *ptrOPT = Highlight;
            BounceTime = 0.2;
        }
        inputDetected = true;
    }
    else if (menu_KeyPressed[MK_DOWN]) {
        if (ptrOPT != coloursOptions+static_cast<int>(MAX_SEQUENCEOPT)-2) {
            *ptrOPT = nonHighlight;
            ++ptrOPT;
            *ptrOPT = Highlight;
            BounceTime = 0.2;
        }
        inputDetected = true;
    }
    else if (menu_KeyPressed[MK_ENT]) {
        int offset = ptrOPT - coloursOptions;
        s = option[offset];
        inputDetected = true;
        *ptrOPT = nonHighlight;
        ptrOPT = coloursOptions;
        *ptrOPT = Highlight;
        BounceTime = 0.3;
    }
    
}

//Detect the player's input to on and off the sounds in the game
void processInputSound(SequenceOPT &s) {

    if (ElapsedTime <= BounceTime) {
        return;
    }

    if (menu_KeyPressed[MK_UP]) {
        if (ptrSound != coloursSound) {
            *ptrSound = nonHighlight;
            --ptrSound;
            *ptrSound = Highlight;
            BounceTime = 0.2;
        }
        inputDetected = true;
    }
    else if (menu_KeyPressed[MK_DOWN]) {
        if (ptrSound == coloursSound) {
            *ptrSound = nonHighlight;
            ++ptrSound;
            *ptrSound = Highlight;
            BounceTime = 0.2;
        }
        inputDetected = true;
    }
    else if (menu_KeyPressed[MK_ENT]) {
        inputDetected = true;
        s = OptionsMenu;
        BounceTime = 0.3;
        if (ptrSound == coloursSound) { //player wants sound to be on
            if (!playmusic) {
			    PlaySound(TEXT("menumusic.wav"), NULL, SND_LOOP | SND_ASYNC);
		    }
		    playmusic = true;   //boolean to keep playing the music
        }
        else {  //player wants sound to be off
            PlaySound(NULL,NULL,0);
		    playmusic = false;  //boolean to stop playing music even after going back to main page and in game
            *ptrSound = nonHighlight;
            ptrSound = coloursSound;
            *ptrSound = Highlight;
        }
    }

}

//PauseGame Character settings duplicated and changed to fit OptionsMenu requirements
//Glennda
void processInputChar(SequenceOPT &s)
{
    if (ElapsedTime <= BounceTime) {
        return;
    }

    if (menu_KeyPressed[MK_RIGHT]) {
        inputDetected = true;
        BounceTime = 0.2;
        if (ptr < arr+6) {      // ptr not at position 6
            ++ptr;      // right shift of pointer
        }
        charIcon = *ptr;
    }

    if (menu_KeyPressed[MK_LEFT]) {
        inputDetected = true;
        BounceTime = 0.2;
        if (ptr > arr) {       // ptr not pointing at position 0 of arr
            --ptr;      // left shift of pointer
        }
        charIcon = *ptr; // Selected Icon is where pointer is pointing at
    }

    if (menu_KeyPressed[MK_UP]) {
        inputDetected = true;
        BounceTime = 0.2;
        if (rtp < rra+5) {      // rtp not at position 5
            ++rtp;      // up shift of pointer
        }
        charClr = *rtp;
    }

    if (menu_KeyPressed[MK_DOWN]) {
        inputDetected = true;
        BounceTime = 0.2;
        if (rtp > rra) {      // rtp not pointing at position 0 of rra
            --rtp;      // down shift of pointer
        }
        charClr = *rtp; // Selected Color is where pointer is pointing at
    }

    else if (menu_KeyPressed[MK_ENT]) {
        inputDetected = true;
        s = OptionsMenu;
        BounceTime = 0.3;
    }
}

//User interface for main menu
//Writetobuffer to buffer the text and after wards flush to show the text in the screen
//WeiMin
void displayMenu( void )
{
	if(playmusic && menuplaymusic)
	{
		PlaySound(TEXT("menumusic.wav"), NULL, SND_LOOP | SND_ASYNC);
        menuplaymusic = false;
	}
    clearScreen();
	COORD c = g_Console.getConsoleSize();

    c.X = 18;
    c.Y /= 8;
    for (int i = 0; i < 43; ++i) {
		++c.X;
		g_Console.writeToBuffer(c, "=", 0x0C);
    }
    c.Y += 2;
    //SHOW GAME NAME
    c.X = g_Console.getConsoleSize().X / 2-2;
    g_Console.writeToBuffer(c, "ESCAPEE", 0x0B);
    c.X = 18;
	c.Y += 2;
    for (int i = 0; i < 43; ++i) {
		++c.X;
		g_Console.writeToBuffer(c, "=", 0x0C);
    }
    c.X = 19;
    c.Y = 3;
    for (int i = 0; i < 15; ++i)
    {
        ++c.Y;
        g_Console.writeToBuffer(c, "-", 0x0C);
    }
    c.X = 61;
    c.Y = 3;
    for (int i = 0; i < 15; ++i)
    {
        ++c.Y;
        g_Console.writeToBuffer(c, "-", 0x0C);
    }

    ++c.Y;
    c.X = 18;
        for (int i = 0; i < 43; ++i) {
		++c.X;
		g_Console.writeToBuffer(c, "=", 0x0C);
    }

    c.Y = 9;
    c.X = g_Console.getConsoleSize().X / 2-1;
    g_Console.writeToBuffer (c,"MENU", 0x0B);
  
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, "PLAY", coloursMenu[0]);
    c.Y += 1;

	c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, "INSTRUCTIONS", coloursMenu[1]);
    c.Y += 1;

	c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, "HIGH SCORE", coloursMenu[2]);
	c.Y += 1;

    c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, "OPTIONS", coloursMenu[3]);
    c.Y += 1;

    c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, "QUIT", coloursMenu[4]);

    string MenuLeft[14] = {
        "          ,.====.  ",           
        "        .-`\"/c)}}},",      
        "    .-'-6>   {{{{{'",       
        "   t         |}}}}}",           
        "    \\__.___.'{{{{{{",             
        "        `[__/}}}}}}",          
        "         {{{.'     ",          
        "         }}/       ",              
        "         {{|       ",                   
        "         }}|       ",                   
        "  .------{{{\\    , ",              
        " //.--------'    ; ",             
        "((///            \\ ",            
        " (((--' `''-------'" 
    };

        c.X = 0;  //x and y coordinates to start the box
	    c.Y = 6;
        
        for(int i = 0; i < 14; ++i, ++c.Y){  //check through the string array and writeToBuffer to print out
        std::ostringstream ss;
		ss.str("");
		ss << MenuLeft[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
        }

     string MenuRight[14] = {
        " .====.,",
        ",{{{(c\\\"`-.",
        "}}}}   <9-'-.",
        "{{{{{|        y",
        "}}}}}}'.___.__/",
        "{{{{{{\\__]`",
        "   '.}}}",
        "       \\{{",
        "       |}}",
        "       |{{",
        " ,    /}}}------.",
        " ;   '---------.\\\\",
        "/             \\\\\\)",
        "'-------''` '--))"
    };

        c.X = 62;  //x and y coordinates to start the box
	    c.Y = 6;
        
        for(int i = 0; i < 14; ++i, ++c.Y){  //check through the string array and writeToBuffer to print out
        std::ostringstream ss;
		ss.str("");
		ss << MenuRight[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
        }

        g_Console.flushBufferToConsole();
}

//displaying the hows and what to do
//Glennda
void displayInstructions( void )
{
	clearScreen();
	COORD c = g_Console.getConsoleSize();

    string instruction[21] = {
        "|vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv|",
        "",
        "   _                                                                     _",
        "  /_\\                                                                   /_\\",
        "  =|=                                                                   =|=",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "L                                                                            J",
        "LL                                                                          JJ",
        "LLL                                                                        JJJ",
        "LLLL                                                                      JJJJ",
        "LLLLL                                                                    JJJJJ",
    };

    c.X = 1;
	c.Y = 1;
	for (int i = 0; i < 21; ++i, ++c.Y) {
		std::ostringstream ss;
		ss.str("");
		ss << instruction[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
	}

	c.Y /= 4;
	c.X = g_Console.getConsoleSize().X / 2 - 6;
	g_Console.writeToBuffer(c, "INSTRUCTIONS", 0x0B);
	c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 23;
	g_Console.writeToBuffer(c, "Players' main objective is to escape the maze.", 0x0A);
	c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 26;
	g_Console.writeToBuffer(c, "Use arrow keys to move", 0x0D);

    //visual display of arrow keys
    c.X += 24;
    g_Console.writeToBuffer(c, 27, 0x0E);
    c.X += 1;
    g_Console.writeToBuffer(c, 18, 0x0E);
    c.X += 1;
    g_Console.writeToBuffer(c, 26, 0x0E);

    c.X = g_Console.getConsoleSize().X / 2 - 26;
	c.Y += 1;
	g_Console.writeToBuffer(c, "Step over objects   in the maze to collect them", 0x0D);

    //visual display of object
    c.X += 18;
    g_Console.writeToBuffer(c, '$', 0x0F);

    c.X = g_Console.getConsoleSize().X / 2 - 26;
	c.Y += 1;
	g_Console.writeToBuffer(c, "Collecting objects   along the way adds to the Item Counter", 0x0D);

    //visual display of object
    c.X += 19;
    g_Console.writeToBuffer(c, '$', 0x0F);

    c.X = g_Console.getConsoleSize().X / 2 - 26;
    c.Y += 1;
	g_Console.writeToBuffer(c, "Once all objects   are collected, the exit is unlocked", 0x0D);
    
    //visual display of object
    c.X += 17;
    g_Console.writeToBuffer(c, '$', 0x0F);

    c.X = g_Console.getConsoleSize().X / 2 - 26;
    c.Y += 1;
	g_Console.writeToBuffer(c, "Find the correct portal   that leads to the exit  ", 0x0D);

    //visual display of portal
    c.X += 24;
    g_Console.writeToBuffer(c, '@', 0x0E);

    //visual display of exit
    c.X += 25;
    g_Console.writeToBuffer(c, '*', 0x0B);

    c.X = g_Console.getConsoleSize().X / 2 - 26;
    c.Y += 1;
	g_Console.writeToBuffer(c, "Careful, fake portals   will stun you for 1.5 seconds!", 0x0D);

    //visual display of portal
    c.X += 22;
    g_Console.writeToBuffer(c, '@', 0x0E);

	c.Y += 4;
	c.X = g_Console.getConsoleSize().X / 3 + 4;
	g_Console.writeToBuffer(c, "Press ENTER to return", 0x0B);
	g_Console.flushBufferToConsole();

}

void displayHighscore( void )
{
	clearScreen();
	COORD c = g_Console.getConsoleSize();

    string highscore[21] = {
        " ",
        " ",
        " ",
        " ",
        " ",
        " ",
        " ",
        "    _                                                                  _",
        "   //  _                                                              //  _",
        "  ((   ))                                                            ((   ))",
        "   \\\\ //                                                              \\\\ //",
        "    ---                                                                ---",
        "  -------                                                            -------",
        "    ||                                                                 ||",
        "    ||                                                                 ||",
        "",
        "",
        "",
        "",
        "",
        "",
    };

    c.X = 1;
	c.Y = 1;
	for (int i = 0; i < 21; ++i, ++c.Y) {
		std::ostringstream ss;
		ss.str("");
		ss << highscore[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
	}

	c.Y = 3;
    c.X = g_Console.getConsoleSize().X / 2 - 7;
    g_Console.writeToBuffer(c, "HIGH SCORE", 0x0B);
	c.Y = 1;
	c.X = 0;
	for (int i= 3; i < 80; ++i) {
		c.X++;
		g_Console.writeToBuffer(c, "_", 0x0E);
		if (i == 80) {
			break;
		}
	}
	c.Y += 4;
	c.X = 0;
	for (int i = 3; i < 80; ++i) {
		c.X++;
		g_Console.writeToBuffer(c, "_", 0x0E);
	}

	c.Y += 2;
	c.X = c.X / 2 - 10;
	for (int i = 1; i < 11; ++i) {
		c.Y++;
		std::ostringstream ss;
		ss.str("");
		ss << i;
		g_Console.writeToBuffer(c, ss.str(), 0x0F);
	}

	c.Y += 4;
	c.X = g_Console.getConsoleSize().X / 2 - 10;
	g_Console.writeToBuffer(c, "Press ENTER to return", 0x0B);

    toCpp();
	
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
		temp.time = std::stod(tempTime);        // gets time from dt in gameplay
		temp.name = tempName;       // gets name through user input
		highS.push_back(temp);      // push in both temp.name and temp.time into .txt file
	}

	inData.close();

    // arranging data in .txt file in ascending order
    highScore current;
    highScore next;
    highScore temp;
	for (unsigned int i = 0; i < highS.size(); ++i) {
		for (unsigned int j = 0; j < highS.size() - 1; ++j) {
			current = highS[j];
			next = highS[j+1];
			if (current.time > next.time) {
				temp = current;
				highS[j] = next;
				highS[j+1] = temp;
			}
		}
	}

    // fishing out data that is within the top 10 (shortest time)
	COORD c = g_Console.getConsoleSize();
	c.Y = 8;
	c.X = g_Console.getConsoleSize().X / 2 - 10;
	std::ostringstream ss;

	for (unsigned int i = 0; i < highS.size(); ++i, ++c.Y) {
		highScore temp = highS[i];
		ss.str("");
		ss << temp.name << " " << temp.time;
		g_Console.writeToBuffer(c, ss.str(), 0x07);
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
                ElapsedTime = 0.0;
                break;
            case Sound : displaySound();
                while (!inputDetected) {
                    userInput();
                    processInputSound(s);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            case Character : displayChar();
                while (!inputDetected) {
                    userInput();
                    processInputChar(s);
                }
                inputDetected = false;
                ElapsedTime = 0.0;
                break;
            case Back : seq = Menu;
                break;
        }
    }

}

//Shania
//Display the options to adjust sound or go back inside Options
void displayOptions( void ) {

    clearScreen();
    COORD c = g_Console.getConsoleSize();

    c.Y /= 3;
    c.X = c.X / 2 - 3;
    g_Console.writeToBuffer(c, "OPTIONS", 0x0B);
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "SOUND", coloursOptions[0]);
    ++c.Y;
    g_Console.writeToBuffer(c, "CHARACTER SETTINGS", coloursOptions[1]);
    ++c.Y;
    g_Console.writeToBuffer(c, "BACK", coloursOptions[2]);
   
    string OptionsLeft[17] = {
        "][][][][][]",   
        "|^^^^^|",                       
        "|^^^^^|",   
        "|^^^^^|  .::Z::.",     
        "|^^^^^|  :'^ ^':",      
        "|^^^^^|  ::)^(::",     
        "|^^^^^| /''\\ /''\\",     
        "|^^^^^| | / @ \\ |",    
        "|^^^^^| ||);;;(||",    
        "|^^^^^__|||   |||__",    
        "|^^^^^|'-'|   |`-'",    
        "|^^^^^|   | | |",    
        "|^^^^^|   | | |",   
        "|^^^^^|   | | |",   
        "|^^^^^|   |/|\\|",    
        "|^^^^^|    |||",    
        "][][][][][]" 
    };
    
    c.X = 8;  //x and y coordinates to start
	c.Y = 3;
        
    for(int i = 0; i < 17; ++i, ++c.Y){  //check through the string array and writeToBuffer to print out
        std::ostringstream ss;
		ss.str("");
		ss << OptionsLeft[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
    }

    string OptionsRight[17] = {
        "       [][][][][][",
        "           |^^^^^|",
        "  |\\_/|    |^^^^^|",
        "  | | |    |^^^^^|",
        "  (=|=)    |^^^^^|",
        "  |\\^/|    |^^^^^|",
        "  | ' |    |^^^^^|",
        "  )\\ /(    |^^^^^|",
        " /  |  \\   |^^^^^|",
        " |  @  |   |^^^^^|",
        " |  |  |   |^^^^^|",
        " | / \\ |   |^^^^^|",
        " | | | |   |^^^^^|",
        " | | | |   |^^^^^|",
        " |_|_|_|   |^^^^^|",
        "(_______)  |^^^^^|",
        "      []][][][][]["
    };

    c.X = 52;  //x and y coordinates to start
	c.Y = 3;
        
    for(int i = 0; i < 17; ++i, ++c.Y){  //check through the string array and writeToBuffer to print out
        std::ostringstream ss;
		ss.str("");
		ss << OptionsRight[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
    }

    g_Console.flushBufferToConsole();
}

//Shania
//Display the options to on or off the sound in the game
void displaySound( void ) {
    
    clearScreen();

    COORD c = g_Console.getConsoleSize();
    c.Y = 10;
    c.X = 53;
    g_Console.writeToBuffer(c, "EDIT SOUND HERE", 0x0B);
    c.Y = 12;
    c.X = 53;
    g_Console.writeToBuffer(c, "ON", coloursSound[0]);
    ++c.Y;
    g_Console.writeToBuffer(c, "OFF", coloursSound[1]);

    string Sound[21] = {
        "    _________________________________",
        "    \\____________________________( ( )",
        "      \\___________________________( (|    _____",
        "        \\________________________( ( |   /VVVVO\\_____",
        "          \\________________________( /  |VVVVVV/     \\",
        "            \\_____________________( /   |VVVVV(_-----\\|",
        "              \\__________________( /    |VVVVVV|",
        "               _\\_______________( /     /VVVVVV|",  
        "              _\\________________( \\    /VVVVVVV|",   
        "       _______\\__________________( \\  /VVVVVVVV|",  
        "    __/      \\___________________(  \\/VVVVVVVVV|",
        "   /=|                                VVVVVVVVV|",
        "  // |       |                      \\VVVVVVVV|V|",
        " //  |       |_______________________|VVVVVVV|/",
        "||    \\     /                         \\VVVVVV|",
        "||    /   /                             \\VVVV\\",
        "||   /   /                                \\VVV\\",
        " \\\\  \\   \\                                 \\VVV\\",
        "  V    \\  \\                                  \\==\\",
        "  V     \\  \\__                                \\==\\__",
        "         (____)>                             ( ---- )"
    };


    c.X = 1;  //x and y coordinates to start
	c.Y = 1;
        
    for(int i = 0; i < 21; ++i, ++c.Y){  //check through the string array and writeToBuffer to print out
        std::ostringstream ss;
		ss.str("");
		ss << Sound[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
    }

    g_Console.flushBufferToConsole();
}

//Glennda
void displayChar( void )
{
    clearScreen();
    COORD c = g_Console.getConsoleSize();
    c.Y /= 4;
    c.X = 3;
    g_Console.writeToBuffer(c, "Switch Player Icon: Use Right key or Left key", 0x0B);
    c.Y+= 2;
    c.X = 3;
    g_Console.writeToBuffer(c, "Switch Player Color: Use Up key or Down key", 0x0B);
    c.Y += 3;
    c.X = 25;
    g_Console.writeToBuffer(c, charIcon, charClr);
    c.Y += 5;
    c.X = 13;
    g_Console.writeToBuffer(c, "Press Enter to return", 0x0B);

    //Shania
    string CharSet[19] = {
        "     _.mmmmmmmmm._",
        "   dMMMY'~~~~~`YMMMb",
        " dMMMY'         `YMMMb",
        "dMMMY'           `YMMMb",
        "CMMM(             )MMMD",
        "YMMMb.           .dMMMY",
         " YMMMb.         .dMMMY",
        "  `YMMMboo...oodMMMY'",
        ".    `\"#MMMMMMM#\"'    .",
        "Mb       `MMM'       dM",
        "MMMM.   .dMMMb.   .dMMM",
        "MMMMMMMMMMMMMMMMMMMMMMM",
        "MMMMMMMMMMMMMMMMMMMMMMM",
        "MMMM'   `YMMMY'   `YMMM",
        "MM'      )MMM(      `MM",
        "'       .MMMMM.       `",
        "        dMMMMMb",
        "       dMMMMMMMb",
        "        \"\"\"\"\"\"\"   "                     
    };

    c.X = 50;  //x and y coordinates to start
	c.Y = 3;
        
    for(int i = 0; i < 19; ++i, ++c.Y){  //check through the string array and writeToBuffer to print out
        std::ostringstream ss;
		ss.str("");
		ss << CharSet[i];
		g_Console.writeToBuffer(c, ss.str(), 0x0E);
    }

    g_Console.flushBufferToConsole();
}

void displayExit( void )
{
    clearScreen();

    COORD c = g_Console.getConsoleSize();

	//Display exit text
	c.X = 12;
	c.Y = 2;

	g_Console.writeToBuffer(c,"~~YOU RIDE ON YOUR CAMEL ALL THE WAY HOME, GOOD BYE!~~", 0x0F);

	//Display ascii camel
	c.X = 3;
	c.Y = 5;

	g_Console.writeToBuffer(c,"                                                  =--_                ", 0x0C);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                                   .-\"\"\"\"\"\"-.     |* _)               ", 0x0C);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                                  /          \\   /  /                 ", 0x0E);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                                 /            \\_/  /                  ", 0x0E);
	c.Y += 1;
	g_Console.writeToBuffer(c,"     _                          /|                /                   ", 0x0A);
	c.Y += 1;
	g_Console.writeToBuffer(c," _-'\"/\\                        / |    ____    _.-\"            _       ", 0x0A);
	c.Y += 1;
	g_Console.writeToBuffer(c,"'   (  '-_            _       (   \\  |\\  /\\  ||           .-'\".\".     ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"     '.   `'-._   .-'\"/'.      \"   | |/ /  | |/        _-\"   (   '-_  ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c,"       '.      _-\"   (   '-_       \\ | /   \\ |     _.-'       )     \"-", 0x09);
	c.Y += 1;
	g_Console.writeToBuffer(c,"     _.'   _.-'       )     \"-._    ||\\\\   |\\\\  '\"'        .-'        ", 0x09);
	c.Y += 1;
	g_Console.writeToBuffer(c,"   '               .-'          `'  || \\\\  ||))                       ", 0x0D);
	c.Y += 1;
    g_Console.writeToBuffer(c," _  ___  _ ____________ _____  ___ _|\\ _|\\_|\\\\/ _______________  ___  ", 0x0D);
	c.Y += 1;
	g_Console.writeToBuffer(c,"                 c  c  \" c C \"\"C  \" \"\"  \"\" \"\"                         ", 0x07);
	c.Y += 1;
	g_Console.writeToBuffer(c,"             c       C                                                ", 0x07);
	c.Y += 1;
	g_Console.writeToBuffer(c,"        C        C                                                    " , 0x07);
	c.Y += 1;
	g_Console.writeToBuffer(c,"             C                                                        ", 0x07);

    
	g_Console.flushBufferToConsole();
    
	exitGame = true;
    Sleep(6000);
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