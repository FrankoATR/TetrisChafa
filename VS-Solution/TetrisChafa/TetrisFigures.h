#include "TetrisBlocks.h"
#include "FigureModels.h"

struct STATEFig {
	int x, y;
}FigureState, ghostState;

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
	int fig_id, fig_rotation;

	//Lista de Blocks
	list<TetrisBlocks*> blocks;
	list<TetrisBlocks*> ghostBlocks;

	bool keys[ALLEGRO_KEY_MAX] = { 0 };

	//funciones
	TetrisFigures(int, int, int, int); //Constructor
	void virtual DestroyTetrisFigures();
	bool Empty();

	void Display();
	template <class Complete_figure>
	void CreateFigure(Complete_figure);

	void CreateType();
	void funcMov(int, int);
	void funcMovTo(int, int);

	template <class Complete_figure>
	void funcRot(Complete_figure);

	void mov(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, int, int);
	void rotate(bool);

	void toDeep();
	void toDeepGhost();


private:
	void forAllBlocksInFigure();

};

list<TetrisFigures*> figures;

TetrisFigures::TetrisFigures(int x_, int y_, int id, int rotation) {
	x = x_;
	y = y_;
	this->fig_id = id;
	this->fig_rotation = rotation % (figureModels[fig_id].cantitie);
	controlling = true;
	taked = false;
	StaticTime = 0;
	CreateType();

}

void TetrisFigures::Display() {
	if (blocks.empty() == false) {
		for (auto it = ghostBlocks.begin(); it != ghostBlocks.end(); it++) {
			(*it)->Display();
		}
		for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end();) {

			(*it)->Display();
			it++;
		}


	}
	else {
		cout << "Vacio" << fig_id << " " << fig_rotation << endl;
	}
}

template <class Complete_figure>
void TetrisFigures::CreateFigure(Complete_figure form) {
	int auxColor = rand() % 4;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (form[i][j] == 1) {
				blocks.push_back(new TetrisBlocks(x + (j * 32), y + (i * 32), auxColor));
				ghostBlocks.push_back(new TetrisBlocks(x + (j * 32), y + (i * 32), 5));
			}
		}
	}

}


void TetrisFigures::CreateType() {
	if (blocks.empty()) {
		CreateFigure(figureModels[fig_id].model[fig_rotation]);
		ghostState.y = this->y;

	}
	else {
		cout << " Ya Creada" << fig_id << " " << fig_rotation << endl;
	}

}


void TetrisFigures::funcMov(int x, int y) {

	auto ghost_ = ghostBlocks.begin();

	for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end(); it++) {
		TetrisBlocks* A = *it;
		A->y -= y;
		A->x -= x;
		A->ThisCollider->posX -= x;
		A->ThisCollider->posY -= y;

		(*ghost_)->x -= x;
		(*ghost_)->y = A->y;
		(*ghost_)->ThisCollider->posX -= x;
		(*ghost_)->ThisCollider->posY = A->y;

		ghost_++;
	}
	this->y -= y;
	this->x -= x;



}


void TetrisFigures::funcMovTo(int x, int y) {
	auto block_ = blocks.begin();
	auto ghost_ = ghostBlocks.begin();

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (figureModels[fig_id].model[fig_rotation][i][j] == 1) {
				(*block_)->y = y + i * 32;
				(*block_)->x = x + j * 32;
				(*block_)->ThisCollider->posX = x + j * 32;
				(*block_)->ThisCollider->posY = y + i * 32;

				(*ghost_)->x = x + j * 32;
				(*ghost_)->y = (*block_)->y;
				(*ghost_)->ThisCollider->posX = x + j * 32;
				(*ghost_)->ThisCollider->posY = (*block_)->y;

				ghost_++;
				block_++;
			}
		}
	}


	this->y = y;
	this->x = x;



}



void TetrisFigures::mov(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, int Timer, int velocityLevel) {
	if (!taked && controlling) {
		FigureState.x = this->x;
		FigureState.y = this->y;

		ghostState.y = this->y;


		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			keys[Evento.keyboard.keycode] = true;
			/*
			switch (Evento.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					funcMov(0, 66);
					break;

				case ALLEGRO_KEY_DOWN:
					funcMov(0, -32);
					break;
				case ALLEGRO_KEY_LEFT:
					funcMov(32, 0);
					break;
				case ALLEGRO_KEY_RIGHT:
					funcMov(-32, 0);
					break;
				case ALLEGRO_KEY_X:
					rotate(true);
					break;
				case ALLEGRO_KEY_Z:
					rotate(false);
					break;
				case ALLEGRO_KEY_SPACE:
					toDeep();
					break;

				default:
					break;
			}
			*/

			if (Evento.keyboard.keycode == ALLEGRO_KEY_LEFT)
				funcMov(32, 0);
			else if (Evento.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				funcMov(-32, 0);
			if (Evento.keyboard.keycode == ALLEGRO_KEY_X)
				rotate(true);
			else if (Evento.keyboard.keycode == ALLEGRO_KEY_Z)
				rotate(false);
			if (Evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
				toDeep();

		}
		if (Evento.type == ALLEGRO_EVENT_KEY_UP)
			keys[Evento.keyboard.keycode] = false;



		if (Evento.keyboard.keycode == ALLEGRO_KEY_DOWN)
			funcMov(0, -32);



		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Timer % (59 - (velocityLevel-1) * 6)   == 0) {
				//cout << " Valor de x,y:" << this->x << "," << this->y << endl;
				funcMov(0, -32);
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

						if ((*it2)->posY == 24 * 32) { // CONDICION PARA DETENERLOS HASTA ABAJO

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


				//(*it)->ChangeColor(event_queue, Evento, segundoTimer);
				it++;
			}

		}

		toDeepGhost();

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
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (form[i][j] == 1) {
				if ((it) != blocks.end()) {
					TetrisBlocks* F = *it;
					(F)->x = x + (j * 32);
					(F)->y = y + (i * 32);
					(F)->ThisCollider->posX = x + (j * 32);
					(F)->ThisCollider->posY = y + (i * 32);
					it++;
				}
			}
		}
	}
	
	auto it2 = ghostBlocks.begin();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (form[i][j] == 1) {
				if ((it2) != ghostBlocks.end()) {
					TetrisBlocks* F = *it2;
					(F)->x = x + (j * 32);
					(F)->y = ghostState.y + (i * 32);
					(F)->ThisCollider->posX = x + (j * 32);
					(F)->ThisCollider->posY = ghostState.y + (i * 32);
					it2++;
				}
			}
		}
	}
	
}

void TetrisFigures::rotate(bool clockwise) {
	FigureState.x = this->x;
	FigureState.y = this->y;
	if (blocks.empty() == false) {

		if (clockwise) {
			++fig_rotation;
			fig_rotation %= figureModels[fig_id].cantitie;
		}

		else {
			--fig_rotation;
			if (fig_rotation < 0)
				fig_rotation = figureModels[fig_id].cantitie - 1;
			else
				fig_rotation %= figureModels[fig_id].cantitie;

		}

		//cout << fig_rotation << endl;

		funcRot(figureModels[fig_id].model[fig_rotation]);

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
		rotate(clockwise);
	}

}

bool TetrisFigures::Empty() {
	if (!blocks.empty()) {

		return false;
	}
	else return true;
}

void TetrisFigures::DestroyTetrisFigures() {
	for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end();) {
		(*it)->DestroyTetrisBlocks();
		it = blocks.erase(it);
	}
	for (auto it = ghostBlocks.begin(); it != ghostBlocks.end();) {
		(*it)->DestroyTetrisBlocks();
		it = blocks.erase(it);
	}

	delete (this);
}

void TetrisFigures::forAllBlocksInFigure() {
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

					if ((*it2)->posY == 24 * 32) { // CONDICION PARA DETENERLOS HASTA ABAJO

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



		}

	}

}

void TetrisFigures::toDeep() {
	bool Indeep = false;
	while (!Indeep) {
		FigureState.x = this->x;
		FigureState.y = this->y;
		funcMov(0, -32);
		for (list<TetrisBlocks*>::iterator it = blocks.begin(); it != blocks.end();) {
			for (list<Collider*>::iterator it2 = CollisionBlocks.begin(); it2 != CollisionBlocks.end(); it2++) {
				if ((*it)->ThisCollider->Collision((*it2))) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if ((*it2)->posY == 24 * 32) { // CONDICION PARA DETENERLOS HASTA ABAJO
						Indeep = true;
						controlling = false;
					}
				}
			}
			for (list<TetrisBlocks*>::iterator it2 = Unique_blocks.begin(); it2 != Unique_blocks.end(); it2++) {
				if ((*it)->ThisCollider->Collision((*it2)->ThisCollider)) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if ((*it)->ThisCollider->posY != (*it2)->ThisCollider->posY) {
						Indeep = true;
						controlling = false;

					}

				}
			}


			it++;


		}

	}
}




void TetrisFigures::toDeepGhost() {




	bool Indeep = false;

	while (!Indeep) {


		for (auto it = ghostBlocks.begin(); it != ghostBlocks.end(); it++) {
			(*it)->mov();
		}

		for (auto it = ghostBlocks.begin(); it != ghostBlocks.end();) {

			for (auto it2 = CollisionBlocks.begin(); it2 != CollisionBlocks.end(); it2++) {
				if ((*it)->ThisCollider->Collision((*it2))) {

					for (auto it3 = ghostBlocks.begin(); it3 != ghostBlocks.end(); it3++) {
						(*it3)->y -= 32;
						(*it3)->ThisCollider->posY -= 32;

					}

					if ((*it2)->posY == 24 * 32) { // CONDICION PARA DETENERLOS HASTA ABAJO
						Indeep = true;
					}

				}

			}

			for (auto it2 = Unique_blocks.begin(); it2 != Unique_blocks.end(); it2++) {
				if ((*it)->ThisCollider->Collision((*it2)->ThisCollider)) {

					for (auto it3 = ghostBlocks.begin(); it3 != ghostBlocks.end(); it3++) {
						(*it3)->y -= 32;
						(*it3)->ThisCollider->posY -= 32;
					}

					if ((*it)->ThisCollider->posY != (*it2)->ThisCollider->posY) {
						Indeep = true;
					}

				}

			}

			it++;

		}

	}



}