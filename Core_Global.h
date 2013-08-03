/**************************************************
Core_Global.h
GameCore Component
by Jim Adams (Jan 2004)
**************************************************/

#ifndef _CORE_GLOBAL_H_
#define _CORE_GLOBAL_H_

#define DIRECTINPUT_VERSION 0x0800

// Windows includes
#include <windows.h>

// Standard ANSI-C includes
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <time.h>
#include <math.h>

// DirectX includes
#include "d3d9.h"
#include "d3dx9.h"
#include "dmusici.h"
#include "dsound.h"
#include "dplay8.h"
#include "dpaddr.h"
#include "dinput.h"
#include "dshow.h"

// Core includes
#include "Core_System.h"
#include "Core_Graphics.h"
#include "Core_Input.h"
#include "Core_Sound.h"

//Defines
#define GAME_AREA_LEFT      53   //area coordinates of playing field
#define GAME_AREA_RIGHT     251
#define GAME_AREA_BOTTOM    298
#define NUM_LEVELS		    5    //values for points and level advancements
#define PTS_PER_LINE        525
#define PTS_PER_LEVEL       6300
#define INITIAL_SPEED	    60   //intial speed at which focus block falls
#define SPEED_CHANGE	    10   //speed is reduced by this per level
#define SLIDE_TIME		    40   //Time to slide a block at the bottom
#define SQUARES_PER_ROW     10   //number of squares in a row
#define SQUARE_MEDIAN	    10   //dist from center of square to edges
//Locations of objects in the bitmap
#define BLOCK_START_X	    151
#define BLOCK_START_Y	    39
#define LEVEL_RECT_X	    42
#define LEVEL_RECT_Y	    320
#define SCORE_RECT_X	    42
#define SCORE_RECT_Y	    340
#define NEEDED_SCORE_RECT_X 42
#define NEEDED_SCORE_RECT_Y 360
#define NEXT_BLOCK_CIRCLE_X 210
#define NEXT_BLOCK_CIRCLE_Y 348
#define LEVEL_X				0
#define LEVEL_Y				0
#define LINES_RECT_X		42
#define LINES_RECT_Y		378
//location of colored squares in bitmap
#define RED_SQUARE_X		0
#define RED_SQUARE_Y		0
#define PURPLE_SQUARE_X	    20
#define PURPLE_SQUARE_Y		0
#define GREY_SQUARE_X		40
#define GREY_SQUARE_Y		0
#define BLUE_SQUARE_X		60
#define BLUE_SQUARE_Y		0
#define GREEN_SQUARE_X	    80
#define GREEN_SQUARE_Y		0
#define BLACK_SQUARE_X		100
#define BLACK_SQUARE_Y		0
#define YELLOW_SQUARE_X		120
#define YELLOW_SQUARE_Y		0

//Enumerations for direction and block type
enum BlockType
{
	SQUARE_BLOCK,
	T_BLOCK,
	L_BLOCK,
	BACKWARDS_L_BLOCK,
	STRAIGHT_BLOCK,
	S_BLOCK,
	BACKWARDS_S_BLOCK
};

enum Direction
{
	LEFT,
	RIGHT,
	DOWN
};

//Derived cApp class
class cApp : public cApplication
{
private:
	//state manager object
	cStateManager m_StateManager;
	//state function prototypes
	static void Menu(void *, long);
	static void Game(void *, long);
	static void Pause(void *, long);
	static void Win(void *, long);
	static void Lose(void *, long);

	//core graphics object
	cGraphics       m_Graphics;
	//core input object and device(s)
	cInput  g_Input;
	cInputDevice g_Keyboard;
	cInputDevice g_Mouse;
	//core sound objects
	cSound g_Sound;
	
	
	//font object
	cFont m_Font;
	cFont m_Font2;

public:
	cApp(); //constructor

	//classes
	class cSquare;
	class cBlock;

	BOOL Init();     //all three are overloaded
	BOOL Shutdown();
	BOOL Frame();

	//Collision Detection Functions, The wall and entity functions are 
	//overloaded to make one for blocks and squares
	BOOL CheckEntityCollision(cSquare *square, Direction dir);
	BOOL CheckWallCollision(cSquare *square, Direction dir);
	BOOL CheckEntityCollision(cBlock *block, Direction dir);
	BOOL CheckWallCollision(cBlock *block, Direction dir);
	BOOL CheckRotationCollision(cBlock *block);

	//functions to check for win/lose situations
	void CheckWin();
	BOOL CheckLose();
	
	//functions to see if Blocks have hit bottom, and if a line should be cleared
	void HandleBottomCollision();
	void ChangeFocusBlock();
	int CheckCompletedLines();

	//pointer to bitmaps
	cTexture HUDTex;
	cTexture SquareTex;
	cTexture TitleTex;
	cTexture ScrollTex;
	cTexture SplashTex;
	
	//sound
	cSoundData g_Data[2];
	cSoundChannel g_Channel[2];
	
	//pointers to focus block and next focus block
	cBlock *g_FocusBlock;
	cBlock *g_NextBlock;

	//vector array for all squares at the bottom
	std::vector<cSquare*> g_OldSquares; 
	//other vars
	int g_Lines;
	int g_Score;
	int g_Level;
	int g_FocusBlockSpeed;
};

//Square and block classes
class cApp::cSquare
{
private:
	//location of center of square
	int m_CenterX;
	int m_CenterY;
	//Type of block
	BlockType m_BlockType;

public:
	//Default constructor
	cSquare()
	{}
	//Main constructor takes location, type of block, and pointer to bitmap
	cSquare(int x, int y, BlockType type);
	
	//Draw Function
	void Draw(void *DataPtr)
	{
		cApp *cc = (cApp*)DataPtr;

		RECT source = {0,0,0,0};
		
		//switch stmt to determine location of square within bitmap
		switch (m_BlockType)
		{
		case SQUARE_BLOCK:
			{
				RECT temp = {RED_SQUARE_X, RED_SQUARE_Y, 
					SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
				source = temp;				  
			} break;
		case T_BLOCK:
			{
				RECT temp = {PURPLE_SQUARE_X, PURPLE_SQUARE_Y, 
					SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
				source = temp;
			} break;
		case L_BLOCK:
			{
				RECT temp = {GREY_SQUARE_X, GREY_SQUARE_Y, 
					SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
				source = temp;
			} break;
		case BACKWARDS_L_BLOCK:
			{
				RECT temp = {BLUE_SQUARE_X, BLUE_SQUARE_Y, 
					SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
				source = temp;
			} break;
		case STRAIGHT_BLOCK:
			{
				RECT temp = {GREEN_SQUARE_X, GREEN_SQUARE_Y, 
					SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
				source = temp;
			} break;
		case S_BLOCK:
			{
				RECT temp = {BLACK_SQUARE_X, BLACK_SQUARE_Y, 
					SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
				source = temp;
			} break;
		case BACKWARDS_S_BLOCK:
			{
				RECT temp = {YELLOW_SQUARE_X, YELLOW_SQUARE_Y, 
					SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
				source = temp;
			} break;
		}
		//DRAW IT
		cc->SquareTex.Blit(m_CenterX-SQUARE_MEDIAN, m_CenterY-SQUARE_MEDIAN,
			source.left, source.top, SQUARE_MEDIAN*2, SQUARE_MEDIAN*2);
	} //END DRAW FUNCTION

	//Move Function
	void Move(Direction dir)
	{
		switch (dir)
		{
		case LEFT:
			{
				m_CenterX-=SQUARE_MEDIAN*2;
			} break;
		case RIGHT:
			{
				m_CenterX+=SQUARE_MEDIAN*2;
			} break;
		case DOWN:
			{
				m_CenterY+=SQUARE_MEDIAN*2;
			} break;
		}
	} //End Function

	//Accessing and changing methods
	//accessors
	int GetCenterX() {return m_CenterX;}
	int GetCenterY() {return m_CenterY;}
	//mutators
	void SetCenterX(int x) {m_CenterX = x;}
	void SetCenterY(int y) {m_CenterY = y;}
	//End cSquare Definition and functions
};

//cBlock class
class cApp::cBlock
{
private:
	//fake center of block to rotate around
	int m_CenterX;
	int m_CenterY;

	//Type of Block
	BlockType m_Type;

	//array of pointers to the 4 squares that make a block
	cSquare *m_Squares[4];

public:
	//constructor sets block location, and calls setupsquares 
	cBlock(int x, int y, BlockType type);

	//SetupSquares function sets up blocks according to type and location
	void SetupSquares(int x, int y)
	{
		//set location
		m_CenterX = x;
		m_CenterY = y;
		
		//Make sure current squares are deleted
		for (int i=0; i<4; i++)
		{
			if (m_Squares[i])
				delete m_Squares[i];
		}

		//lengthy switch statement to setup block shape
		switch (m_Type)
		{
		case SQUARE_BLOCK:
			{
				//[0][1]
				//[2][3]
				m_Squares[0] = new cSquare(x-SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[1] = new cSquare(x-SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[2] = new cSquare(x+SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[3] = new cSquare(x+SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
			} break;
		case T_BLOCK:
			{
				//   [0]
				//[1][2][3]
				m_Squares[0] = new cSquare(x+SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[1] = new cSquare(x+SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[2] = new cSquare(x-SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[3] = new cSquare(x+(SQUARE_MEDIAN*3), y+SQUARE_MEDIAN,
										   m_Type);
			} break;
		case L_BLOCK:
			{
				//[0]
				//[1]
				//[2][3]
				m_Squares[0] = new cSquare(x-SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[1] = new cSquare(x-SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[2] = new cSquare(x-SQUARE_MEDIAN, y+(SQUARE_MEDIAN*3),
										   m_Type);
				m_Squares[3] = new cSquare(x+SQUARE_MEDIAN, y+(SQUARE_MEDIAN*3),
										   m_Type);
			} break;
		case BACKWARDS_L_BLOCK:
			{
				//   [0]
				//   [1]
				//[3][2]
				m_Squares[0] = new cSquare(x+SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[1] = new cSquare(x+SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[2] = new cSquare(x+SQUARE_MEDIAN, y+(SQUARE_MEDIAN*3),
										   m_Type);
				m_Squares[3] = new cSquare(x-SQUARE_MEDIAN, y+(SQUARE_MEDIAN*3),
										   m_Type);
			} break;
		case STRAIGHT_BLOCK:
			{
				//[0]
				//[1]
				//[2]
				//[3]
				m_Squares[0] = new cSquare(x+SQUARE_MEDIAN, y-(SQUARE_MEDIAN*3),
										   m_Type);
				m_Squares[1] = new cSquare(x+SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[2] = new cSquare(x+SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[3] = new cSquare(x+SQUARE_MEDIAN, y+(SQUARE_MEDIAN*3),
										   m_Type);
			} break;
		case S_BLOCK:
			{
				//   [1][0]
				//[3][2]
				m_Squares[0] = new cSquare(x+(SQUARE_MEDIAN*3), y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[1] = new cSquare(x+SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[2] = new cSquare(x+SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[3] = new cSquare(x-SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
			} break;
		case BACKWARDS_S_BLOCK:
			{
				//[0][1]
				//   [2][3]
				m_Squares[0] = new cSquare(x-SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[1] = new cSquare(x+SQUARE_MEDIAN, y-SQUARE_MEDIAN,
										   m_Type);
				m_Squares[2] = new cSquare(x+SQUARE_MEDIAN, y+SQUARE_MEDIAN,
										   m_Type);
				m_Squares[3] = new cSquare(x+(SQUARE_MEDIAN*3), y+SQUARE_MEDIAN,
										   m_Type);
			} break;
		}
	}//End setup function

	//Draw Function
	void Draw(void *DataPtr)
	{
		cApp *cc = (cApp*)DataPtr;
		for(int i=0; i<4; i++)
		{
			m_Squares[i]->Draw(cc);
		}
	}//end draw function

	//Move Function changes block center and moves each square
	void Move(Direction dir)
	{
		switch (dir)
		{
		case LEFT:
			{
				m_CenterX -= SQUARE_MEDIAN*2;
			} break;
		case RIGHT:
			{
				m_CenterX += SQUARE_MEDIAN*2;
			} break;
		case DOWN:
			{
				m_CenterY += SQUARE_MEDIAN*2;
			} break;
		} //end switch
		//move individual squares
		for (int i=0; i<4; i++)
		{
			m_Squares[i]->Move(dir);
		} //end for
	} //end move function

	//Rotate Function
	void Rotate()
	{
		//two sets of temporary variables for rotation
		int x1, y1, x2, y2;

		for (int i=0; i<4; i++)
		{
			//Get Current Center
			x1 = m_Squares[i]->GetCenterX();
			y1 = m_Squares[i]->GetCenterY();

			//Position square at origin
			x1 -= m_CenterX;
			y1 -= m_CenterY;

			//Do actual rotation
			x2 = -y1;
			y2 = x1;

			//Move Square back to original location
			x2 += m_CenterX;
			y2 += m_CenterY;

			//Set Squares location to our temps
			m_Squares[i]->SetCenterX(x2);
			m_Squares[i]->SetCenterY(y2);
		}//end for
	} //End function rotate

	//returns array of squares' location after rotation
	int *GetRotatedSquares()
	{
		int *temp_array = new int[8];
		int x1, y1, x2, y2;

		for (int i=0; i<4; i++)
		{
			x1 = m_Squares[i]->GetCenterX();
			y1 = m_Squares[i]->GetCenterY();

			x1 -= m_CenterX;
			y1 -= m_CenterY;

			x2 = -y1;
			y2 = x1;

			x2 += m_CenterX;
			y2 += m_CenterY;

			//instead of setting the squares, store values in the array
			temp_array[i*2] = x2;
			temp_array[i*2+1]=y2;
		}//end for 
		//return the array
		return temp_array;
	} //end function;

	//accessor method to return a pointer to our array of pointers
	cSquare **GetSquares()
	{
		return m_Squares;
	} //end function
}; // End cBlock class

#endif
