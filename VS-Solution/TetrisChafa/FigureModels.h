struct ARROW
{
	//Created by: Franco119 - Francisco Alonso Torres Rosa - SV
	int Down[3][3] = {
		{0,0,0},
		{1,1,1},
		{0,1,0} };
	int Right[3][3] = {
		{1,0,0},
		{1,1,0},
		{1,0,0} };
	int Up[3][3] = {
		{0,0,0},
		{0,1,0},
		{1,1,1} };
	int Left[3][3] = {
		{0,1,0},
		{1,1,0},
		{0,1,0} };
	int id = 4;
};

struct CROSS
{
	int Down[3][3] = {
		{0,0,0},
		{1,0,1},
		{0,1,0} };
	int Right[3][3] = {
		{0,1,0},
		{0,0,1},
		{0,1,0} };
	int Up[3][3] = {
		{0,1,0},
		{1,0,1},
		{0,0,0} };
	int Left[3][3] = {
		{0,1,0},
		{1,0,0},
		{0,1,0} };
	int id = 4;
};

struct CUBE {
	int Cube_V[3][3] = {
		{0,0,0},
		{1,0,1},
		{1,0,1} };
	int Cube_H[3][3] = {
		{1,1,0},
		{0,0,0},
		{1,1,0} };
	int id = 2;
};

struct SCALRIGTH {
	int Up[3][3] = {
		{0,1,0},
		{1,1,0},
		{1,0,0} };
	int Down[3][3] = {
		{0,0,0},
		{1,1,0},
		{0,1,1} };
	int id = 2;
};

struct SCALEFT {
	int Up[3][3] = {
		{1,0,0},
		{1,1,0},
		{0,1,0} };
	int Down[3][3] = {
		{0,0,0},
		{0,1,1},
		{1,1,0} };
	int id = 2;
};

struct LINE {
	int Vertical[3][3] = {
		{0,1,0},
		{0,1,0},
		{0,1,0} };
	int Horizontal[3][3] = {
		{0,0,0},
		{1,1,1},
		{0,0,0} };
	int id = 2;
};

struct LRIGHT {
	int Up[3][3] = {
		{0,1,1},
		{0,1,0},
		{0,1,0} };
	int Right[3][3] = {
		{0,0,0},
		{1,1,1},
		{0,0,1} };	
	int Down[3][3] = {
		{0,1,0},
		{0,1,0},
		{1,1,0} };
	int Left[3][3] = {
		{1,0,0},
		{1,1,1},
		{0,0,0} };
	int id = 4;
};

struct LLEFT {
	int Up[3][3] = {
		{1,1,0},
		{0,1,0},
		{0,1,0} };
	int Right[3][3] = {
		{0,0,1},
		{1,1,1},
		{0,0,0} };
	int Down[3][3] = {
		{0,1,0},
		{0,1,0},
		{0,1,1} };
	int Left[3][3] = {
		{0,0,0},
		{1,1,1},
		{1,0,0} };
	int id = 4;
};

struct Complete_figure {
	ARROW figArrow;
	CUBE figCube;
	LINE figLine;
	SCALEFT figScaleft;
	SCALRIGTH figScalrigth;
	LRIGHT figLRight;
	LLEFT figLLeft;
	CROSS figCross;

};
