#pragma once
#ifndef TETRISBLOCK_H
#define TETRISBLOCK_H

#include "Collider.cpp"

class TetrisBlock
{
	private:
	int x;
	int y;


	ALLEGRO_BITMAP* sprite;

	public:

	int sprite_h;
	int sprite_w;
	bool taked;
	bool die;
	int dieTime;

	TetrisBlock(int, int, ALLEGRO_BITMAP*);
	~TetrisBlock();

	void draw();
	void update();
	int get_x();
	int get_y();
	void set_x(int);
	void set_y(int);
	bool IsDeletable();
	void ChangeColor(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, ALLEGRO_TIMER*);
	void deleting(ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);
	bool hitBlock(TetrisBlock*);
	bool hitCollisionBlock(Collider*);
	void DisplayFigureCollision();
};

#endif
