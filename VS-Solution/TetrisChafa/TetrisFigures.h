#include "TetrisBlock.h"
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
	bool auxStatic = false;

	bool controlling;
	bool taked;

	//seleccionar Complete_Block
	int fig_id, fig_rotation;

	//Lista de Blocks
	list<TetrisBlocks*> blocks;
	list<TetrisBlocks*> ghostBlocks;

	bool keys[ALLEGRO_KEY_MAX] = { 0 };

	//funciones
	TetrisFigures(int, int, int, int, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*); //Constructor
	~TetrisFigures();

	bool Empty();

	void Display();
	template <class Complete_figure>
	void CreateFigure(Complete_figure, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);

	void CreateType(ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);
	void funcMov(int, int);
	void funcMovTo(int, int);

	template <class Complete_figure>
	void funcRot(Complete_figure);

	void update(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, ALLEGRO_TIMER*, ALLEGRO_TIMER*);
	void rotate(bool);

	void toDeep();
	void toDeepGhost();


private:
	template <class T>
	void forAllBlocksInFigure(void (*)());

};

list<TetrisFigures*> figures;

TetrisFigures::TetrisFigures(int x_, int y_, int id, int rotation, ALLEGRO_BITMAP* colorBlocks, ALLEGRO_BITMAP* colorGhost) {
	x = x_;
	y = y_;
	this->fig_id = id;
	this->fig_rotation = rotation % (figureModels[fig_id].cantitie);
	controlling = true;
	taked = false;
	StaticTime = 0;
	CreateType(colorBlocks, colorGhost);

}

void TetrisFigures::Display() {
	if (blocks.empty() == false) {
		for (auto b : ghostBlocks)
			b->Display();
		for (auto b : blocks)
			b->Display();

	}
}

template <class Complete_figure>
void TetrisFigures::CreateFigure(Complete_figure form, ALLEGRO_BITMAP* colorBlocks, ALLEGRO_BITMAP* colorGhost) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (form[i][j] == 1) {
				blocks.push_back(new TetrisBlocks(x + (j * 32), y + (i * 32), colorBlocks));
				ghostBlocks.push_back(new TetrisBlocks(x + (j * 32), y + (i * 32), colorGhost));
			}
		}
	}

}


void TetrisFigures::CreateType(ALLEGRO_BITMAP* colorBlocks, ALLEGRO_BITMAP* colorGhost) {
	if (blocks.empty()) {
		CreateFigure(figureModels[fig_id].model[fig_rotation], colorBlocks, colorGhost);
		ghostState.y = this->y;
	}
}


void TetrisFigures::funcMov(int x, int y) {

	auto ghost_ = ghostBlocks.begin();

	for (auto block : blocks) {
		block->y -= y;
		block->x -= x;
		block->ThisCollider->posX -= x;
		block->ThisCollider->posY -= y;

		(*ghost_)->x -= x;
		(*ghost_)->y = block->y;
		(*ghost_)->ThisCollider->posX -= x;
		(*ghost_)->ThisCollider->posY = block->y;

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



void TetrisFigures::update(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, ALLEGRO_TIMER* TimerVelocity, ALLEGRO_TIMER* TimerSecs) {
	if (!taked && controlling) {
		FigureState.x = this->x;
		FigureState.y = this->y;
		ghostState.y = this->y;


		auto ghost_ = ghostBlocks.begin();

		for (auto b : blocks) {
			(*ghost_)->x = b->x;
			(*ghost_)->y = b->y;
			(*ghost_)->ThisCollider->posX = b->x;
			(*ghost_)->ThisCollider->posY = b->y;

			ghost_++;
		}





		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			keys[Evento.keyboard.keycode] = true;

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

		if (Evento.keyboard.keycode == ALLEGRO_KEY_DOWN) {
			funcMov(0, -32);
			auxStatic = false;

		}



		if (Evento.type == ALLEGRO_EVENT_TIMER)
			if (Evento.timer.source == TimerVelocity) {
				funcMov(0, -32);
				auxStatic = false;
			}


		for (auto b : blocks) {
			for (auto cb : CollisionBlocks)
				if (b->ThisCollider->Collision(cb)) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if (cb->posY == 32 * 26) {
						auxStatic = true;
					}
				}
			for (auto ub : Unique_blocks) {
				if (b->ThisCollider->Collision(ub->ThisCollider)) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if (b->ThisCollider->posY != ub->ThisCollider->posY) {
						auxStatic = true;
					}
				}
			}
		}

/*
		forAllBlocksInFigure(
			funcMov(x - FigureState.x, y - FigureState.y),
			funcMov(x - FigureState.x, y - FigureState.y),
			funcMov(x - FigureState.x, y - FigureState.y)
		)
*/



		if (auxStatic) {
			if (Evento.type == ALLEGRO_EVENT_TIMER)
				if (Evento.timer.source == TimerSecs) {
					StaticTime++;
					if (StaticTime >= 2) {
						funcMov(0, -32);

						for (auto b : blocks) {
							for (auto cb : CollisionBlocks)
								if (b->ThisCollider->Collision(cb)) {
									funcMov(x - FigureState.x, y - FigureState.y);
									if (cb->posY == 32 * 26) {
										controlling = false;
									}
								}
							for (auto ub : Unique_blocks) {
								if (b->ThisCollider->Collision(ub->ThisCollider)) {
									funcMov(x - FigureState.x, y - FigureState.y);
									if (b->ThisCollider->posY != ub->ThisCollider->posY) {
										controlling = false;
									}
								}
							}
						}
						if (controlling) {
							StaticTime = 0;
						}
					}
				}
		}


		toDeepGhost();

	}
	else {
		for (auto b : blocks)
			Unique_blocks.push_back(b);
		blocks.clear();
	}
}


template <class Complete_figure>
void TetrisFigures::funcRot(Complete_figure form) {
	auto it = blocks.begin();
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
		funcRot(figureModels[fig_id].model[fig_rotation]);

	}

	bool auxstate = false;

	for (auto b : blocks) {
		for (auto cb : CollisionBlocks)
			if (b->ThisCollider->Collision(cb))
				auxstate = true;
		for (auto ub : Unique_blocks)
			if (b->ThisCollider->Collision(ub->ThisCollider))
				auxstate = true;
	}


	if (auxstate == true) {
		bool auxColling = false;
		funcMov(32, 0);
		for (auto b : blocks) {

			for (auto cb : CollisionBlocks)
				if (b->ThisCollider->Collision(cb))
					auxColling = true;
			for (auto ub : Unique_blocks)
				if (b->ThisCollider->Collision(ub->ThisCollider))
					auxColling = true;
		}
		if (auxColling) {
			funcMov(-64, 0);
			bool auxColling2 = false;
			for (auto b : blocks) {

				for (auto cb : CollisionBlocks)
					if (b->ThisCollider->Collision(cb))
						auxColling2 = true;
				for (auto ub : Unique_blocks)
					if (b->ThisCollider->Collision(ub->ThisCollider))
						auxColling2 = true;
			}
			if (auxColling2) {
				funcMov(32, 0);
				rotate(clockwise);
			}

		}
	}

}

bool TetrisFigures::Empty() {
	return blocks.empty();
}



void TetrisFigures::toDeep() {
	bool Indeep = false;
	while (!Indeep) {
		FigureState.x = this->x;
		FigureState.y = this->y;
		funcMov(0, -32);

		for (auto b : blocks) {
			for (auto cb : CollisionBlocks)
				if (b->ThisCollider->Collision(cb)) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if (cb->posY == 32 * 26) {
						Indeep = true;
						controlling = false;
					}
				}
			for (auto ub : Unique_blocks) {
				if (b->ThisCollider->Collision(ub->ThisCollider)) {
					funcMov(x - FigureState.x, y - FigureState.y);

					if (b->ThisCollider->posY != ub->ThisCollider->posY) {
						Indeep = true;
						controlling = false;
					}
				}
			}
		}


	}
}

void TetrisFigures::toDeepGhost() {
	bool Indeep = false;

	while (!Indeep) {

		for (auto gb : ghostBlocks)
			gb->mov();

		for (auto gb : ghostBlocks) {
			for (auto cb : CollisionBlocks)
				if (gb->ThisCollider->Collision(cb)) {
					for (auto gb2 : ghostBlocks) {
						gb2->y -= 32;
						gb2->ThisCollider->posY -= 32;
					}
					if (cb->posY == 32 * 26)
						Indeep = true;
				}
			for (auto b : Unique_blocks) {
				if (gb->ThisCollider->Collision(b->ThisCollider)) {
					for (auto gb2 : ghostBlocks) {
						gb2->y -= 32;
						gb2->ThisCollider->posY -= 32;
					}
					if (gb->ThisCollider->posY != b->ThisCollider->posY)
						Indeep = true;
				}
			}
		}

	}

	//Fix de Ghost when take a block
	auto ghost_ = ghostBlocks.begin();
	for (auto b : blocks) {
		if ((*ghost_)->y < b->y)
			for (auto gb : ghostBlocks)
				gb->mov();

		ghost_++;
	}
		
}

TetrisFigures::~TetrisFigures() {
	for (auto b : blocks) {
		delete b;
	}
	for (auto gb : ghostBlocks) {
		delete gb;
	}
}



template <class T>
void TetrisFigures::forAllBlocksInFigure(void (*WhenColling)()) {


	for (auto b : blocks) {
		for (auto cb : CollisionBlocks)
			if (b->ThisCollider->Collision(cb)) {
				WhenColling();
				if (cb->posY == 32 * 26) {
				}
			}
		for (auto ub : Unique_blocks) {
			if (b->ThisCollider->Collision(ub->ThisCollider)) {
				if (b->ThisCollider->posY != ub->ThisCollider->posY) {
					
				}
			}
		}
	}
}