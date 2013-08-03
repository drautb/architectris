/////////////////////////////////////////////
// Architectris! ////////////////////////////
// Tetris Clone By Ben Draut ////////////////
// Last Updated 11/11/05 ////////////////////
/////////////////////////////////////////////

//Include All our stuff
#include "Core_Global.h"

//use the standard namespace
using namespace std;

//Global Counter variables
//counter variables
static int force_down_counter = 10;
static int slide_counter = SLIDE_TIME;
static int n;
//vars for title color
int r = 130;
int g = 130;
int b = 130;

//bool variable to see if the game state occurred
BOOL game_happened = FALSE;

//CONSTRUCTORS
//Application Constructor
cApp::cApp()
{
	//setup window
	m_Style = WS_OVERLAPPEDWINDOW;
	m_XPos  = 0;
	m_YPos  = 0;
	m_Width = 300;
	m_Height= 400;
	//assign class and window name
	strcpy(m_Class, "NameClass");
	strcpy(m_Caption, "Architectris!");
	//load textures
}

//Square Class Constructor
cApp::cSquare::cSquare(int x, int y, enum BlockType type)
{
	//setup block
	m_CenterX = x; 
	m_CenterY = y; 
	m_BlockType = type;
}

//Block Class Constructor
cApp::cBlock::cBlock(int x, int y, enum BlockType type)
{
	//setup block
	m_CenterX = x;
	m_CenterY = y;
	m_Type = type;
	//set square pointers to null
	for (int i=0; i<4; i++)
	{
		m_Squares[i] = NULL;
	}
	//Setup the squares
	SetupSquares(x,y);
}

BOOL cApp::Init()
{
	//seed random generator
	srand(GetTickCount());

	//Graphics setup
	m_Graphics.Init();
	m_Graphics.SetMode(GethWnd(), TRUE, FALSE);
	ShowMouse(TRUE);

	//load HUD Texture
	HUDTex.Load(&m_Graphics, "Data/HUD.bmp", D3DCOLOR_RGBA(0,0,0,255));
	//load Title texture
	TitleTex.Load(&m_Graphics, "Data/Title.bmp", D3DCOLOR_RGBA(0,0,0,255));
	//load squares texture
	SquareTex.Load(&m_Graphics, "Data/Squares.bmp");
	//load scroll texture
	ScrollTex.Load(&m_Graphics, "Data/scroll.bmp", 0);
	//load backdrop splash
	SplashTex.Load(&m_Graphics, "Data/splash.bmp", 0);

	//init variables
	g_Lines = 0;
	g_Score = 0;
	g_Level = 1;
	g_FocusBlockSpeed = INITIAL_SPEED;

	//input init
	g_Input.Init(GethWnd(), GethInst());
	g_Keyboard.Create(&g_Input, KEYBOARD);
	g_Mouse.Create(&g_Input, MOUSE);

	//init sound
	g_Sound.Init(GethWnd());
	//load sounds
	g_Channel[0].Create(&g_Sound, &g_Data[0]);
	g_Channel[1].Create(&g_Sound, &g_Data[1]);
	g_Data[0].LoadWAV("Data/Hit.wav");
	g_Data[1].LoadWAV("Data/Music.wav");
	
	//init font
	m_Font.Create(&m_Graphics, "Arial", 10);
	m_Font2.Create(&m_Graphics, "Arial", 36);
	
	//play music
	g_Channel[1].Play(&g_Data[1], 100, 0);

	//State manager init
	m_StateManager.Push(Menu, this);
	
	return TRUE;
}

BOOL cApp::Shutdown()
{
	if (game_happened == TRUE)
	{
		//delete blocks
		cSquare **temp_array_1 = g_FocusBlock->GetSquares();
		cSquare **temp_array_2 = g_NextBlock->GetSquares();
		//delete the temporary arrays of squares
		for (int i=0; i<4; i++)
		{
			delete temp_array_1[i];
			delete temp_array_2[i];
		} //end for 
		//delete the old squares in the game area
		for (int i=0; i<g_OldSquares.size(); i++)
		{
			delete g_OldSquares[i];
		} //end for 
		g_FocusBlock = NULL;
		g_NextBlock = NULL;
		delete g_FocusBlock;
		delete g_NextBlock;
	}
	//graphical shutdown
	SquareTex.Free();
	HUDTex.Free();
	TitleTex.Free();
	ScrollTex.Free();
	SplashTex.Free();
	m_Font.Free();
	m_Font2.Free();
	m_Graphics.Shutdown();
	//input shutdown
	g_Keyboard.Free();
	g_Mouse.Free();
	g_Input.Shutdown();
	//sound shutdown
	for (int i=0; i<2; i++)
	{
		g_Channel[i].Stop();
		g_Channel[i].Free();
	}
	for (int i=0; i<2; i++)
	{
		g_Data[i].Free();
	}
	g_Sound.Shutdown();

	return TRUE;
}

BOOL cApp::Frame()
{
	//play music 
	//Check input 
	g_Keyboard.Read();
	g_Mouse.Read();
	
	// Begin the scene
	m_Graphics.ClearDisplay();
	
	//Process current State Function, 
	m_StateManager.Process(this);
	
	m_Graphics.Display();

	return TRUE;
}

//Menu State Function
void cApp::Menu(void *DataPtr, long Purpose)
{
	cApp *cc = (cApp*)DataPtr;

	//carry out State functions
	if (Purpose == INITPURPOSE)
	{
	
	}
	if (Purpose == FRAMEPURPOSE)
	{	
		//Check for input
		if (cc->g_Keyboard.GetKeyState(KEY_G))
			cc->m_StateManager.Push(Game, cc);
		if (cc->g_Keyboard.GetKeyState(KEY_Q) ||
			cc->g_Keyboard.GetKeyState(KEY_ESC))
			PostQuitMessage(0);

		//setup scroll
		n++;
		if (n > 400)
			n = -350;
		cc->ScrollTex.Blit(0, n);

		r = rand()%255;
		g = rand()%255;
		b = rand()%255;
		
		//START RENDERING
		if (cc->m_Graphics.BeginScene() == TRUE)
		{
			cc->TitleTex.Blit(0,0,0,0,0,0,1.0f,1.0f,D3DCOLOR_RGBA(r,g,b,255));
			//Draw font
			cc->m_Font2.Begin();
			{
				cc->m_Font2.Print("Start (G)ame", 50, 150);
				cc->m_Font2.Print("(Q)uit Game", 50, 200);
				cc->m_Font2.End();
			}
			//Draw Graphics
			cc->m_Graphics.EndScene();
		}
	}	
	if (Purpose == SHUTDOWNPURPOSE)
	{
		return;
	}
}

//GAME FUNCTION
void cApp::Game(void *DataPtr, long Purpose)
{
	cApp *cc = (cApp*)DataPtr;

	if (Purpose == INITPURPOSE)
	{
		//intit the focus blocks
		cc->g_FocusBlock = new cBlock(BLOCK_START_X, BLOCK_START_Y, 
			                          (BlockType)(rand()%7));
		cc->g_NextBlock = new cBlock(NEXT_BLOCK_CIRCLE_X,
									 NEXT_BLOCK_CIRCLE_Y,
									 (BlockType)(rand()%7));
		//say the game happened
		game_happened = TRUE;
	} 
	if (Purpose == FRAMEPURPOSE)
	{
		//Handle input here
		if (cc->g_Keyboard.GetKeyState(KEY_ESC))
		{
			cc->g_Keyboard.SetLock(KEY_ESC);
			cc->m_StateManager.Pop(cc);
		}
		if (cc->g_Keyboard.GetKeyState(KEY_I))
		{
			cc->g_Keyboard.SetLock(KEY_I);
			//rotate block
			// Check collisions before rotating //
			if (!cc->CheckRotationCollision(cc->g_FocusBlock))
			{
				cc->g_FocusBlock->Rotate();
			}
		}
		if (cc->g_Keyboard.GetKeyState(KEY_K))
		{
			cc->g_Keyboard.SetLock(KEY_K);
			//check for collisions
			if (!cc->CheckWallCollision(cc->g_FocusBlock, DOWN) &&
				!cc->CheckEntityCollision(cc->g_FocusBlock, DOWN))
			{
				cc->g_FocusBlock->Move(DOWN);
			}
		}
		if (cc->g_Keyboard.GetKeyState(KEY_J))
		{
			cc->g_Keyboard.SetLock(KEY_J);
			//check for collisions
			if (!cc->CheckWallCollision(cc->g_FocusBlock, LEFT) &&
				!cc->CheckEntityCollision(cc->g_FocusBlock, LEFT))
			{
				cc->g_FocusBlock->Move(LEFT);
			}
		}
		if (cc->g_Keyboard.GetKeyState(KEY_L))
		{
			cc->g_Keyboard.SetLock(KEY_L);
			//check for collisions
			if (!cc->CheckWallCollision(cc->g_FocusBlock, RIGHT) &&
				!cc->CheckEntityCollision(cc->g_FocusBlock, RIGHT))
			{
				cc->g_FocusBlock->Move(RIGHT);
			}
		}

		//increment counters
		force_down_counter++;
		if (force_down_counter >= cc->g_FocusBlockSpeed)
		{
			//always check for collisions before moving
			if (!cc->CheckWallCollision(cc->g_FocusBlock, DOWN) &&
				!cc->CheckEntityCollision(cc->g_FocusBlock, DOWN))
			{
				cc->g_FocusBlock->Move(DOWN); //Move focus block
				force_down_counter = 0; //reset our counter
			} //end if 
		}//end if 
		
		//check to see if focus block has hit something below it
		if (cc->CheckWallCollision(cc->g_FocusBlock, DOWN) ||
			cc->CheckEntityCollision(cc->g_FocusBlock, DOWN))
		{
			slide_counter--;
		} //end if
		//if there isn't a collision, reset slide counter,
		//in case the player moves out of a collision
		else
		{
			slide_counter = SLIDE_TIME;
		}//end if else
		
		//if the counter hits 0, reset it and change the focus block
		if (slide_counter == 0)
		{
			slide_counter = SLIDE_TIME;
			cc->g_Channel[0].Play(&cc->g_Data[0]);
			cc->HandleBottomCollision();
		} //end if 

		//START RENDERING
		if (cc->m_Graphics.BeginScene())
		{
			//Render stuff
			//Main HUD
			cc->SplashTex.Blit(0,0);
			cc->HUDTex.Blit(0, 0);
			//Focus block and next block
			cc->g_FocusBlock->Draw(cc);
			cc->g_NextBlock->Draw(cc);
			//Old Squares
			for (int i=0; i<cc->g_OldSquares.size(); i++)
			{
				cc->g_OldSquares[i]->Draw(cc);
			} //end for
			//Render The text
			if (cc->m_Font.Begin())
			{
				//convert values to strings
				char temp[256];
				itoa(cc->g_Score, temp, 10);
				cc->m_Font.Print(temp, SCORE_RECT_X+35, SCORE_RECT_Y, 0, 0,
								  D3DCOLOR_RGBA(0,0,0,255));
				itoa(((cc->g_Level*6300)-cc->g_Score), temp, 10);
				cc->m_Font.Print(temp, NEEDED_SCORE_RECT_X+75, NEEDED_SCORE_RECT_Y,
								 0, 0, D3DCOLOR_RGBA(0,0,0,255));
				itoa(cc->g_Level, temp, 10);
				cc->m_Font.Print(temp, LEVEL_RECT_X+35, LEVEL_RECT_Y, 0, 0,
								 D3DCOLOR_RGBA(0,0,0,255));
				itoa(cc->g_Lines, temp, 10);
				cc->m_Font.Print(temp, LINES_RECT_X+35, LINES_RECT_Y, 0,0,
								 D3DCOLOR_RGBA(0,0,0,255));

				cc->m_Font.Print("Score: ", SCORE_RECT_X, SCORE_RECT_Y, 0, 0,
								 D3DCOLOR_RGBA(0,0,0,255));
				cc->m_Font.Print("Needed Score: ", NEEDED_SCORE_RECT_X,
								 NEEDED_SCORE_RECT_Y, 0, 0, D3DCOLOR_RGBA(0,0,0,255));
				cc->m_Font.Print("Level: ", LEVEL_RECT_X, LEVEL_RECT_Y, 0, 0,
								 D3DCOLOR_RGBA(0,0,0,255));
				cc->m_Font.Print("Lines: ", LINES_RECT_X, LINES_RECT_Y, 0, 0,
								 D3DCOLOR_RGBA(0,0,0,255));
				cc->m_Font.End();
			} //end if
			//end scene
			cc->m_Graphics.EndScene();
		}
	}
	if (Purpose == SHUTDOWNPURPOSE)
	{
		
	}
}

//Pause Function
void cApp::Pause(void *DataPtr, long Purpose)
{
	cApp *cc = (cApp*)DataPtr;

	if (Purpose == INITPURPOSE)
	{

	}
	if (Purpose == FRAMEPURPOSE)
	{

	}
	if (Purpose == SHUTDOWNPURPOSE)
	{

	}
}

//Win Function
void cApp::Win(void *DataPtr, long Purpose)
{
	cApp *cc = (cApp*)DataPtr;

	if (Purpose == INITPURPOSE)
	{

	}
	if (Purpose == FRAMEPURPOSE)
	{
		if (cc->g_Keyboard.GetKeyState(KEY_ESC))
			PostQuitMessage(0);
		//Render Stuff
		if (cc->m_Graphics.BeginScene())
		{
			//Begin font rendering
			if (cc->m_Font2.Begin())
			{
				cc->m_Font2.Print("YOU WIN!", 20, 180);
				cc->m_Font2.Print("Esc to Exit", 20, 210);
				cc->m_Font2.End();
			} //end if 
			cc->m_Graphics.EndScene();
		} //end if
			
	}
	if (Purpose == SHUTDOWNPURPOSE)
	{

	}
}

//Lose Funtion
void cApp::Lose(void *DataPtr, long Purpose)
{
	cApp *cc = (cApp*)DataPtr;

	if (Purpose == INITPURPOSE)
	{
		
	}
	if (Purpose == FRAMEPURPOSE)
	{
		if (cc->g_Keyboard.GetKeyState(KEY_ESC))
			PostQuitMessage(0);
		if (cc->m_Graphics.BeginScene())
		{
			cc->m_Font2.Begin();
			cc->m_Font2.Print("YOU FAILED", 40, 140);
			cc->m_Font2.Print("Esc to Exit", 40, 200);
			cc->m_Font2.End();
			cc->m_Graphics.EndScene();
		}
	}
	if (Purpose == SHUTDOWNPURPOSE)
	{

	}
}

//Winmain entry point
int WINAPI WinMain(HINSTANCE hInst,
				   HINSTANCE hPrev,
				   LPSTR szCmdLine,
				   int nCmdShow)
{
	cApp App;
	return App.Run();
}

//Collision Function Definitions
//Check collision between square and oldsquares
BOOL cApp::CheckEntityCollision(cSquare *square, Direction dir)
{
	//width/height of a square
	int distance = SQUARE_MEDIAN*2;

	//center of given square
	int centerX = square->GetCenterX();
	int centerY = square->GetCenterY();

	//Determine location of square after moving
	switch (dir)
	{
	case DOWN:
		{
			centerY += distance;
		} break;
	case LEFT:
		{
			centerX -= distance;
		} break;
	case RIGHT:
		{
			centerX += distance;
		} break;
	} //end switch
	
	//iterate through the old squares vector checking for collisions
	for (int i=0; i<g_OldSquares.size(); i++)
	{
		if ((abs(centerX - g_OldSquares[i]->GetCenterX()) < distance) &&
			(abs(centerY - g_OldSquares[i]->GetCenterY()) < distance))
		{
			return TRUE;
		}
	}
	return FALSE;
} //end function

//check collision between block and old squares
BOOL cApp::CheckEntityCollision(cBlock *block, Direction dir)
{
	//get array of squares that make up block and array for old squares
	cSquare **temp_array = block->GetSquares();
	//call other entity collision function for each square in the block
	for (int i=0; i<4; i++)
	{
		if (CheckEntityCollision(temp_array[i], dir))
			return TRUE;
	}
	return FALSE;
} //End Function

//Function for collsions against walls
BOOL cApp::CheckWallCollision(cSquare *square, Direction dir)
{
	//get center of square
	int x = square->GetCenterX();
	int y = square->GetCenterY();
	
	//get location after movement and see if its out of bounds
	switch (dir)
	{
	case DOWN:
		{
			if ((y+(SQUARE_MEDIAN*2)) > GAME_AREA_BOTTOM)
				return TRUE;
			else
				return FALSE;
		} break;
	case LEFT:
		{
			if ((x-(SQUARE_MEDIAN*2)) < GAME_AREA_LEFT)
				return TRUE;
			else 
				return FALSE;
		} break;
	case RIGHT:
		{
			if ((x+(SQUARE_MEDIAN*2)) > GAME_AREA_RIGHT)
				return TRUE;
			else 
				return FALSE;
		} break;
	} //end switch
	
	return FALSE;
} //End function

//Wall Collisons for Blocks
BOOL cApp::CheckWallCollision(cBlock *block, Direction dir)
{
	//get array
	cSquare **temp_array = block->GetSquares();

	//Call Wall Collsion function for each square
	for (int i=0; i<4; i++)
	{
		if (CheckWallCollision(temp_array[i], dir))
			return TRUE;
	} // end for
	return FALSE;
} //end function

//Rotation Collsion function
BOOL cApp::CheckRotationCollision(cBlock *block)
{
	//get array
	int *temp_array = block->GetRotatedSquares();

	//dist between two touching squares
	int distance = SQUARE_MEDIAN*2;

	for (int i=0; i<4; i++)
	{
		//check to see if it goes out of bounds
		if ((temp_array[i*2] < GAME_AREA_LEFT) ||
			(temp_array[i*2] > GAME_AREA_RIGHT))
		{
			//delete temp array
			delete temp_array;
			return TRUE;
		}
		if (temp_array[i*2+1] > GAME_AREA_BOTTOM)
		{
			delete temp_array;
			return TRUE;
		}
		
		//check to see if block will hit any squares
		for (int index=0; index<g_OldSquares.size(); index++)
		{
			if ((abs(temp_array[i*2] - g_OldSquares[index]->GetCenterX()) < distance &&
				(abs(temp_array[i*2+1]-g_OldSquares[index]->GetCenterY()) < distance)))
			{
				delete temp_array;
				return TRUE;
			}
		} //end for 
	} //end for 
	//delete array
	delete temp_array;
	return FALSE;
}//end Function

//More Functions For Game Processing
void cApp::HandleBottomCollision()
{
	if (CheckLose() == FALSE)
	{
		ChangeFocusBlock();
	
		//check for completed lines
		int num_lines = CheckCompletedLines();
	
		if (num_lines > 0)
		{
			//increase players score according to lines completed
			g_Score += PTS_PER_LINE*num_lines;
			g_Lines += num_lines;
			
			//see if player achieved a new level
			if (g_Score >= g_Level * PTS_PER_LEVEL)
			{
				g_Level++;
				CheckWin();
				g_FocusBlockSpeed -= SPEED_CHANGE;
			} //end if
		} //end if 
	}
	else
	{
		m_StateManager.PopAll();
		//delete old squares
		for (int i=0; i<g_OldSquares.size(); i++)
		{
			delete g_OldSquares[i];
		}
		g_OldSquares.clear();
		//push the lose state
		m_StateManager.Push(Lose);
	}
} //end function

//function to change focus block
void cApp::ChangeFocusBlock()
{
	//get array of pointers to focus block squares
	cSquare **square_array = g_FocusBlock->GetSquares();
	
	//add focus block squares to old squares
	for (int i=0; i<4; i++)
	{
		g_OldSquares.push_back(square_array[i]);
	} //end for 

	//delete current focus block and setup the new one
	delete g_FocusBlock;
	g_FocusBlock = g_NextBlock;
	g_FocusBlock->SetupSquares(BLOCK_START_X, BLOCK_START_Y);
	g_NextBlock = new cBlock(NEXT_BLOCK_CIRCLE_X, NEXT_BLOCK_CIRCLE_Y, 
							 (BlockType)(rand()%7));
} //end function

//Function to see if a line is completed
int cApp::CheckCompletedLines()
{
	//amount of squares in each row
	int squares_per_row[13];

	//set all indices to 0
	for (int index=0; index<13; index++)
		squares_per_row[index] = 0;

	//locational variables
	int row_size = SQUARE_MEDIAN*2;
	int bottom = GAME_AREA_BOTTOM - SQUARE_MEDIAN;
	int top = bottom - (12*row_size);
	
	//number of lines cleared and a multipurpose variable
	int num_lines = 0;
	int row = 0;
	
	//check for full lines
	for (int i=0; i<g_OldSquares.size(); i++)
	{
		//get the row the current square is in
		row = (g_OldSquares[i]->GetCenterY() - top) / row_size;
		//increment the row counter
		squares_per_row[row]++;
	} //end for

	//erase full lines
	for (int line=0; line<13; line++)
	{
		//check for completed lines
		if (squares_per_row[line] == SQUARES_PER_ROW)
		{
			//keep track of completed lines
			num_lines++;
			//remove squares in current line
			for (int index=0; index<g_OldSquares.size(); index++)
			{
				if (((g_OldSquares[index]->GetCenterY() - top) / row_size) == line)
				{
					//delete the square
					delete g_OldSquares[index];
					//remove it from vector
					g_OldSquares.erase(g_OldSquares.begin() + index);
					//move all the squares forward in the vector to fill in the gap
					index--;
				} //end if
			} //end for
		}// end if 
	}// end for 

	//move squares above cleared line down
	for (int index=0; index<g_OldSquares.size(); index++)
	{
		for (int line=0; line<13; line++)
		{
			//see if row was filled
			if (squares_per_row[line] == SQUARES_PER_ROW)
			{
				//If so, get location of row in game area
				row = (g_OldSquares[index]->GetCenterY() - top) / row_size;
				//now move squares above row down
				if (row < line)
				{
					g_OldSquares[index]->Move(DOWN);
				} //end if
			} //end if 
		} //end for
	} // end for 
	return num_lines;
} // end function

//check win function
void cApp::CheckWin()
{
	//if current level is greater than number of levels, then they won
	if (g_Level > NUM_LEVELS)
	{
		//push win state
		m_StateManager.Push(Win);
	} //end if 
} //end function

//check loss functon
BOOL cApp::CheckLose()
{
	for (int i=0; i<g_OldSquares.size(); i++)
	{
		if (g_OldSquares[i]->GetCenterY() < 39)
		{
			return TRUE;
		}
	}
	return FALSE;
} //end function


