///////////////////////////////////////////////////////////////
//Core_Global.cpp
//file for function definitions of cSquare and cBlock classes
/////////////////////////////////////////////////////////////////

//include
#include "Core_Global.h"

//Draw Function
void Draw()
{
	RECT source;
	
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
	SquareTex.Blit(m_CenterX-SQUARE_MEDIAN, m_CenterY-SQUARE_MEDIAN,
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
			m_Centery+=SQUARE_MEDIAN*2;
		} break;
	}
} //End Function
