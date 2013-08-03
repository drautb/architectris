/////////////////////////////////////////////////////////////////
//Game.h  ///////////////////////////////////////////////////////
//Header file for all the game specific classes, defines, etc. //
/////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H

//Defines
#define GAME_AREA_LEFT      53   //area coordinates of playing field
#define GAME_AREA_RIGHT     251
#define GAME_AREA_BOTTOM    298
#define NUM_LEVELS		    10   //values for points and level advancements
#define PTS_PER_LINE        525
#define PTS_PER_LEVEL       6300
#define INITIAL_SPEED	    60   //intial speed at which focus block falls
#define SPEED_CHANGE	    10   //speed is reduced by this per level
#define SLIDE_TIME		    15   //Time to slide a block at the bottom
#define SQUARES_PER_ROW     10   //number of squares in a row
#define SQUARE_MEDIAN	    10   //dist from center of square to edges
//Locations of objects in the bitmap
#define BLOCK_START_X	    151
#define BLOCK_START_Y	    59
#define LEVEL_RECT_X	    42
#define LEVEL_RECT_Y	    320
#define SCORE_RECT_X	    42
#define SCORE_RECT_Y	    340
#define NEEDED_SCORE_RECT_X 42
#define NEEDED_SCORE_RECT_Y 360
#define NEXT_BLOCK_CIRCLE_X 214
#define NEXT_BLOCK_CIRCLE_Y 347
#define LEVEL_X				0
#define LEVEL_Y				0
//location of colored squares in bitmap
#define RED_SQUARE_X		299
#define RED_SQUARE_Y		0
#define PURPLE_SQUARE_X	    319
#define PURPLE_SQUARE_Y		0
#define GREY_SQUARE_X		339
#define GREY_SQUARE_Y		0
#define BLUE_SQUARE_X		359
#define BLUE_SQUARE_Y		0
#define GREEN_SQUARE_X	    379
#define GREEN_SQUARE_Y		0
#define BLACK_SQUARE_X		399
#define BLACK_SQUARE_Y		0
#define YELLOW_SQUARE_X		419
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

//Backbuffer surface
LPDIRECT3DSURFACE9 backbuffer;

//Square and block classes
class cSquare
{
private:
	//location of center of square
	int m_CenterX;
	int m_CenterY;
	//Type of block
	BlockType m_BlockType;
	//pointer to bitmap surface 
	LPDIRECT3DSURFACE9 m_Bitmap;

public:
	//Default constructor
	cSquare()
	{}
	//Main constructor takes location, type of block, and pointer to bitmap
	cSquare(int x, int y, LPDIRECT3DSURFACE9 *bitmap, BlockType type) : 
			m_CenterX(x), m_CenterY(y), m_Bitmap(bitmap), m_BlockType(type)
	{}
	//Draw Function
	void Draw(LPDIRECT3DSURFACE9 *backbuffer)
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
							 SQUARE_MEDIAN*2, SQUARE_MEDIAN*2}
				source = temp;
			} break;
		case L_BLOCK:
			{
				RECT temp = {GREY_SQUARE_X, GREY_SQUARE_Y,
							 SQUARE_MEDIAN*2, SQUARE_MEDIAN*2}
				source = temp;
			} break;
		case BACKWARDS_L_BLOCK:
			{
				RECT temp = {BLUE_SQUARE_X, BLUE_SQUARE_Y,
							 SQUARE_MEDIAN*2, SQUARE_MEDIAN*2}
				source = temp;
			} break;
		case STRAIGHT_BLOCK:
			{
				RECT temp = {GREEN_SQUARE_X, GREEN_SQUARE_Y,
							 SQUARE_MEDIAN*2, SQUARE_MEDIAN*2}
				source = temp;
			} break;
		case S_BLOCK:
			{
				RECT temp = {BLACK_SQUARE_X, BLACK_SQUARE_Y,
							 SQUARE_MEDIAN*2, SQUARE_MEDIAN*2}
					source = temp;
			} break;
		case BACKWARDS_S_BLOCK:
			{
				RECT temp = {YELLOW_SQUARE_X, YELLOW_SQUARE_Y,
							 SQUARE_MEDIAN*2, SQUARE_MEDIAN*2}
				source = temp;
			} break;
		}

		//Draw at squares current location, m_x and m_y are centers
		RECT dest = {m_CenterX - SQUARE_MEDIAN, m_CenterY - SQUARE_MEDIAN,
			         SQUARE_MEDIAN*2, SQUARE_MEDIAN*2};
		
		
};

#endif 
