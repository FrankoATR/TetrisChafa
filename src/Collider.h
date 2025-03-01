#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <list>

using namespace std;

class Collider
{
public:
	int posX;
	int posY;
	int dimX;
	int dimY;
	int typeCollision;
	bool active;

	Collider(int, int, int, int, int, bool);

	void DisplayFigureCollision();
	//bool SquaretoSquareCollision(Collider*);
	//bool Collision(Collider*);


};

#endif