#include "TetrisBlocks.h"
#include "FigureModels.h"

struct STATEFig {
	int x, y;
}FigureState;

class TetrisFigures
{
	//Created by: Franco119 - Francisco Alonso Torres Rosa - SV
public:
	//variables
	int x;
	int y;

	float StaticTime;

	bool controlling;
	bool taked;

	//seleccionar Complete_Block
	Complete_figure FIGURE;
	int figuretype, figureid;

	//Lista de Blocks
	list<TetrisBlocks*> blocks;

	//funciones
	TetrisFigures(int, int, int, int); //Constructor
	void virtual DestroyTetrisFigures();
	bool Empty();

	void Display();
	template <class Complete_figure>
	void CreateFigure(Complete_figure);

	void CreateType();
	void funcMov(int, int);

	template <class Complete_figure>
	void funcRot(Complete_figure);

	void mov(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, ALLEGRO_TIMER*);
	void rotate();

};

list<TetrisFigures*> figures;

TetrisFigures::TetrisFigures(int x_, int y_, int figuretype, int figureid) {
	x = x_;
	y = y_;
	this->figuretype = figuretype;
	this->figureid = figureid;
	controlling = true;
	taked = false;
	StaticTime = 0;
	CreateType();

}

void TetrisFigures::Display() {
	if (blocks.empty() == false) {
		for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end();) {
				(*it)->Display();
				it++;
		}
	}
	else {
		cout << "Vacio" << figuretype << " " << figureid << endl;
	}
}

template <class Complete_figure>
void TetrisFigures::CreateFigure(Complete_figure form) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (form[i][j] == 1)
				blocks.push_back(new TetrisBlocks(x + (j * 66), y + (i * 66), figureid));
		}
	}

}


void TetrisFigures::CreateType() {
	if (blocks.empty()) {
		if (figuretype == 0) {
			if (figureid == 0) CreateFigure(FIGURE.figArrow.Right);
			if (figureid == 1) CreateFigure(FIGURE.figArrow.Down);
			if (figureid == 2) CreateFigure(FIGURE.figArrow.Left);
			if (figureid == 3) CreateFigure(FIGURE.figArrow.Up);
		}
		if (figuretype == 1) {
			if (figureid == 0) CreateFigure(FIGURE.figLLeft.Right);
			if (figureid == 1) CreateFigure(FIGURE.figLLeft.Down);
			if (figureid == 2) CreateFigure(FIGURE.figLLeft.Left);
			if (figureid == 3) CreateFigure(FIGURE.figLLeft.Up);
		}
		if (figuretype == 2) {
			if (figureid == 0) CreateFigure(FIGURE.figLRight.Right);
			if (figureid == 1) CreateFigure(FIGURE.figLRight.Down);
			if (figureid == 2) CreateFigure(FIGURE.figLRight.Left);
			if (figureid == 3) CreateFigure(FIGURE.figLRight.Up);
		}
		if (figuretype == 3) {
			figureid = figureid % 2;
			if (figureid == 0) CreateFigure(FIGURE.figLine.Vertical);
			if (figureid == 1) CreateFigure(FIGURE.figLine.Horizontal);
		}
		if (figuretype == 4) {
			figureid = figureid % 2;
			if (figureid == 0) CreateFigure(FIGURE.figCube.Cube_H);
			if (figureid == 1) CreateFigure(FIGURE.figCube.Cube_V);
		}
		if (figuretype == 5) {
			figureid = figureid % 2;
			if (figureid == 0) CreateFigure(FIGURE.figScaleft.Up);
			if (figureid == 1) CreateFigure(FIGURE.figScaleft.Down);
		}
		if (figuretype == 6) {
			figureid = figureid % 2;
			if (figureid == 0) CreateFigure(FIGURE.figScalrigth.Up);
			if (figureid == 1) CreateFigure(FIGURE.figScalrigth.Down);
		}

		if (figuretype == 7) {
			if (figureid == 0) CreateFigure(FIGURE.figCross.Right);
			if (figureid == 1) CreateFigure(FIGURE.figCross.Down);
			if (figureid == 2) CreateFigure(FIGURE.figCross.Left);
			if (figureid == 3) CreateFigure(FIGURE.figCross.Up);
		}


	}
	else {
		cout << " Ya Creada" << figuretype << " " << figureid << endl;
	}

}


void TetrisFigures::funcMov(int x, int y) {
	for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end(); it++) {
		TetrisBlocks* A = *it;
		A->y -= y;
		A->x -= x;
		A->ThisCollider->posX -= x;
		A->ThisCollider->posY -= y;
	}
	this->y -= y;
	this->x -= x;
}


void TetrisFigures::mov(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, ALLEGRO_TIMER* segundoTimer) {
	if (!taked && controlling) {
		FigureState.x = this->x;
		FigureState.y = this->y;
		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {

			switch (Evento.keyboard.keycode) {
			/*
			case ALLEGRO_KEY_UP:
				funcMov(0, 66);
				break;
			*/
			case ALLEGRO_KEY_DOWN:
				funcMov(0, -66);
				break;
			case ALLEGRO_KEY_LEFT:
				funcMov(66, 0);
				break;
			case ALLEGRO_KEY_RIGHT:
				funcMov(-66, 0);
				break;
			case ALLEGRO_KEY_SPACE:
				rotate();
				break;

			default:
				break;

			}

		}
		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == segundoTimer) {
				cout << " Valor de x,y:" << this->x << "," << this->y << endl;
				funcMov(0, -66);
			}
		}


		for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end();) {
			if ((*it)->IsDeletable()) {
				cout << "mueltoF" << endl;
				//CollisionBlocks.remove((*it)->ThisCollider);
				(*it)->DestroyTetrisBlocks();
				//delete (*it);
				it = blocks.erase(it);
			}
			else {


				for (list<Collider*>::iterator it2 = CollisionBlocks.begin(); it2 != CollisionBlocks.end(); it2++) {


					if ((*it)->ThisCollider->Collision((*it2))) {
						cout << "CAMBIO STATE" << endl;
						funcMov(x - FigureState.x, y - FigureState.y);

						if ((*it2)->posY == 654) {

							StaticTime++;
							cout << "Static: " << StaticTime << endl;

							if (StaticTime >= 2) {
								controlling = false;
								cout << "QUIETOOOO" << endl;
								//DestroyTetrisFigures();

							}

						}
					}
				}
				for (list<TetrisBlocks*>::iterator it2 = Unique_blocks.begin(); it2 != Unique_blocks.end(); it2++) {


					if ((*it)->ThisCollider->Collision((*it2)->ThisCollider)) {

						cout << "CAMBIO STATE" << endl;
						funcMov(x - FigureState.x, y - FigureState.y);

						if ((*it)->ThisCollider->posY != (*it2)->ThisCollider->posY) {

							StaticTime++;
							cout << "Static: " << StaticTime << endl;

							if (StaticTime >= 2) {
								controlling = false;
								cout << "QUIETOOOO" << endl;
								//DestroyTetrisFigures();

							}
						}

					}
				}


				(*it)->ChangeColor(event_queue, Evento, segundoTimer);
				it++;
			}

		}

	}
	else {

		for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end(); ) {
			//(*it)->ChangeColor(event_queue, Evento, segundoTimer);

			Unique_blocks.push_back((*it));
			//(*it)->DestroyTetrisBlocks();
			//delete (*it);
			it = blocks.erase(it);

		}

		

	}
}


template <class Complete_figure>
void TetrisFigures::funcRot(Complete_figure form) {
	list<TetrisBlocks*>::iterator it = blocks.begin();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (form[i][j] == 1) {
				if ((it) != blocks.end()) {
					TetrisBlocks* F = *it;
					(F)->x = x + (j * 66);
					(F)->y = y + (i * 66);
					(F)->ThisCollider->posX = x + (j * 66);
					(F)->ThisCollider->posY = y + (i * 66);
					it++;
				}
			}
		}
	}
}

void TetrisFigures::rotate() {
	FigureState.x = this->x;
	FigureState.y = this->y;
	if (blocks.empty() == false) {
		if (figuretype == 0) {
			if (figureid == 0) funcRot(FIGURE.figArrow.Down);
			if (figureid == 1) funcRot(FIGURE.figArrow.Left);
			if (figureid == 2) funcRot(FIGURE.figArrow.Up);
			if (figureid == 3) funcRot(FIGURE.figArrow.Right);
			figureid = (++figureid) % FIGURE.figArrow.id;
		}
		if (figuretype == 1) {
			if (figureid == 0) funcRot(FIGURE.figLLeft.Down);
			if (figureid == 1) funcRot(FIGURE.figLLeft.Left);
			if (figureid == 2) funcRot(FIGURE.figLLeft.Up);
			if (figureid == 3) funcRot(FIGURE.figLLeft.Right);
			figureid = (++figureid) % FIGURE.figLLeft.id;
		}
		if (figuretype == 2) {
			if (figureid == 0) funcRot(FIGURE.figLRight.Down);
			if (figureid == 1) funcRot(FIGURE.figLRight.Left);
			if (figureid == 2) funcRot(FIGURE.figLRight.Up);
			if (figureid == 3) funcRot(FIGURE.figLRight.Right);
			figureid = (++figureid) % FIGURE.figLRight.id;
		}
		if (figuretype == 3) {
			if (figureid == 0) funcRot(FIGURE.figLine.Horizontal);
			if (figureid == 1) funcRot(FIGURE.figLine.Vertical);
			figureid = (++figureid) % FIGURE.figLine.id;
		}
		if (figuretype == 4) {
			if (figureid == 0) funcRot(FIGURE.figCube.Cube_H);
			if (figureid == 1) funcRot(FIGURE.figCube.Cube_V);
			figureid = (++figureid) % FIGURE.figCube.id;

		}
		if (figuretype == 5) {
			if (figureid == 0) funcRot(FIGURE.figScaleft.Down);
			if (figureid == 1) funcRot(FIGURE.figScaleft.Up);
			figureid = (++figureid) % FIGURE.figScaleft.id;
		}
		if (figuretype == 6) {
			if (figureid == 0) funcRot(FIGURE.figScalrigth.Down);
			if (figureid == 1) funcRot(FIGURE.figScalrigth.Up);
			figureid = (++figureid) % FIGURE.figScalrigth.id;
		}
		if (figuretype == 7) {
			if (figureid == 0) funcRot(FIGURE.figCross.Down);
			if (figureid == 1) funcRot(FIGURE.figCross.Left);
			if (figureid == 2) funcRot(FIGURE.figCross.Up);
			if (figureid == 3) funcRot(FIGURE.figCross.Right);
			figureid = (++figureid) % FIGURE.figCross.id;
		}
	}
	bool auxstate = false;

	for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end(); it++) {

		for (list<Collider*>::iterator it2 = CollisionBlocks.begin(); it2 != CollisionBlocks.end(); it2++) {

			if ((*it)->ThisCollider->Collision((*it2))) {

				auxstate = true;

			}

		}

		for (list<TetrisBlocks*>::iterator it3 = Unique_blocks.begin(); it3 != Unique_blocks.end(); it3++) {
		
			if ((*it)->ThisCollider->Collision((*it3)->ThisCollider)) {

				auxstate = true;

			}

		}

	}
	if (auxstate == true) {
		rotate();
	}

}

bool TetrisFigures::Empty() {
	if (!blocks.empty()) {

		return false;
	}
	else return true;
}

void TetrisFigures::DestroyTetrisFigures() {
	//delete this;
}