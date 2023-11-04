/*
#ifndef GHOSTBLOCK
#define GHOSTBLOCK


#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include "Collisions.h"

class TetrisGhostBlock
{
	//Created by: Franco119 - Francisco Alonso Torres Rosa - SV
public:
	//variables
	int x;
	int y;

	//Area colision
	int sprite_h;
	int sprite_w;

	ALLEGRO_BITMAP* sprite;

	Collider* ThisCollider = new Collider(0, 0, 0, 0, 0, true);

	//funciones
	class TetrisGhostBlock(int, int); //Constructor
	void virtual DestroyTetrisBlocks();
	void Display();
	void mov();
	bool IsDeletable();
};

TetrisGhostBlock::TetrisGhostBlock(int x, int y) {
	this->x = x;
	this->y = y;
	sprite_h = 31;
	sprite_w = 31;
	this->sprite = blockColor.ghost;
}

void TetrisGhostBlock::Display() {
	al_draw_bitmap(sprite, x, y, NULL);
}

void TetrisGhostBlock::mov() {
	y += 32;
	ThisCollider->posY += 32;
}

bool TetrisGhostBlock::IsDeletable() {
	if (y > 25 * 32 - sprite_h) return true;
	else return false;
}


void TetrisGhostBlock::DestroyTetrisBlocks() {
	if (sprite != NULL) {
		al_destroy_bitmap(sprite);
		ThisCollider->DestroyCollider();
	}
	delete this;
}



#endif // !GHOSTBLOCK

*/