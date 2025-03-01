#pragma once
#ifndef TETRISFIGURE_H
#define TETRISFIGURE_H

#include "TetrisBlock.cpp"
#include "FigureModels.cpp"

class TetrisFigure
{
	//Created by: Franco119 - Francisco Alonso Torres Rosa - SV
public:
	//variables
	int x;
	int y;

	ALLEGRO_KEYBOARD_STATE KeyboardState;
	float StaticTime;
	bool auxStatic = false;

	bool controlling;
	bool taked;

	//seleccionar Complete_Block
	int fig_id, fig_rotation;

	//Lista de Blocks
	list<TetrisBlock*> blocks;
	list<TetrisBlock*> ghostBlocks;

	bool keys[ALLEGRO_KEY_MAX] = { 0 };

	//funciones
	TetrisFigure(int, int, int, int, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*); //Constructor
	~TetrisFigure();

	bool Empty();

	void Display();
	template <class Complete_figure>
	void CreateFigure(Complete_figure, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);

	void CreateType(ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);
	void funcMov(int, int);
	void funcMovTo(int, int);

	template <class Complete_figure>
	void funcRot(Complete_figure);

	void update(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, ALLEGRO_TIMER*, ALLEGRO_TIMER*, ALLEGRO_TIMER*, list<TetrisBlock*> *, list<Collider*> *);
	void rotate(bool, list<TetrisBlock*> * , list<Collider*> *);

	void toDeep(list<TetrisBlock*> *, list<Collider*> *);
	void toDeepGhost(list<TetrisBlock*> *, list<Collider*> *);


//private:
	//template <class T>
	//void forAllBlocksInFigure(void (*)());

};



#endif