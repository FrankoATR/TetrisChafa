#include "TetrisFigure.h"

struct STATEFig {
	int x, y;
}FigureState, ghostState;

//list<TetrisBlock*> Unique_blocks; // pasar por referencia
//list<Collider*> CollisionBlocks; // pasar por referencia

TetrisFigure::TetrisFigure(int x_, int y_, int id, int rotation, ALLEGRO_BITMAP* colorBlocks, ALLEGRO_BITMAP* colorGhost) {
	x = x_;
	y = y_;
	this->fig_id = id;
	this->fig_rotation = rotation % (figureModels[fig_id].cantitie);
	controlling = true;
	taked = false;
	StaticTime = 0;
	CreateType(colorBlocks, colorGhost);

}

void TetrisFigure::Display() {
	if (blocks.empty() == false) {
		for (auto b : ghostBlocks)
			b->draw();
		for (auto b : blocks)
			b->draw();

	}
}

template <class Complete_figure>
void TetrisFigure::CreateFigure(Complete_figure form, ALLEGRO_BITMAP* colorBlocks, ALLEGRO_BITMAP* colorGhost) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (form[i][j] == 1) {
				blocks.push_back(new TetrisBlock(x + (j * 32), y + (i * 32), colorBlocks));
				ghostBlocks.push_back(new TetrisBlock(x + (j * 32), y + (i * 32), colorGhost));
			}
		}
	}

}


void TetrisFigure::CreateType(ALLEGRO_BITMAP* colorBlocks, ALLEGRO_BITMAP* colorGhost) {
	if (blocks.empty()) {
		CreateFigure(figureModels[fig_id].model[fig_rotation], colorBlocks, colorGhost);
		ghostState.y = this->y;
	}
}


void TetrisFigure::funcMov(int x, int y) {

	auto ghost_ = ghostBlocks.begin();

	for (auto block : blocks) {
		block->set_y(block->get_y() - y);
		block->set_x(block->get_x() - x);

		(*ghost_)->set_x((*ghost_)->get_x() - x);
		(*ghost_)->set_y( block->get_y());


		ghost_++;
	}

	this->y -= y;
	this->x -= x;

}


void TetrisFigure::funcMovTo(int x, int y) {
	auto block_ = blocks.begin();
	auto ghost_ = ghostBlocks.begin();

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (figureModels[fig_id].model[fig_rotation][i][j] == 1) {
				(*block_)->set_y(y + i * 32);
				(*block_)->set_x(x + j * 32);

				(*ghost_)->set_x(x + j * 32);
				(*ghost_)->set_y((*block_)->get_y());

				ghost_++;
				block_++;
			}
		}
	}
	this->y = y;
	this->x = x;
}



void TetrisFigure::update(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, ALLEGRO_TIMER* TimerVelocity, ALLEGRO_TIMER* TimerSecs, ALLEGRO_TIMER* TimerKeys, list<TetrisBlock*> *Unique_blocks, list<Collider*> *CollisionBlocks) {
	if (!taked && controlling) {
		FigureState.x = this->x;
		FigureState.y = this->y;
		ghostState.y = this->y;


		auto ghost_ = ghostBlocks.begin();

		for (auto b : blocks) {
			(*ghost_)->set_x(b->get_x());
			(*ghost_)->set_y(b->get_y());


			ghost_++;
		}


		al_get_keyboard_state(&KeyboardState);





		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			keys[Evento.keyboard.keycode] = true;


			if (Evento.keyboard.keycode == ALLEGRO_KEY_X)
				rotate(true, Unique_blocks, CollisionBlocks);
			else if (Evento.keyboard.keycode == ALLEGRO_KEY_Z)
				rotate(false, Unique_blocks, CollisionBlocks);

			if (Evento.keyboard.keycode == ALLEGRO_KEY_SPACE)
				toDeep(Unique_blocks, CollisionBlocks);

		}

		if (Evento.type == ALLEGRO_EVENT_KEY_UP)
			keys[Evento.keyboard.keycode] = false;





		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == TimerKeys) {
				if (al_key_down(&KeyboardState, ALLEGRO_KEY_LEFT))
					funcMov(32, 0);
				if (al_key_down(&KeyboardState, ALLEGRO_KEY_RIGHT))
					funcMov(-32, 0);

				if (al_key_down(&KeyboardState, ALLEGRO_KEY_DOWN)) {
					funcMov(0, -32);
					auxStatic = false;

				}
			}
			if (Evento.timer.source == TimerVelocity) {
				funcMov(0, -32);
				auxStatic = false;
			}
		}



		for (auto b : blocks) {
			for (auto cb : *CollisionBlocks)
				if (b->hitCollisionBlock(cb)) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if (cb->posY == 32 * 26) {
						auxStatic = true;
					}
				}
			for (auto ub : *Unique_blocks) {
				if (b->hitBlock(ub)) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if (b->get_y() != ub->get_y()) {
						auxStatic = true;
					}
				}
			}
		}



		if (auxStatic) {
			if (Evento.type == ALLEGRO_EVENT_TIMER)
				if (Evento.timer.source == TimerSecs) {
					StaticTime++;
					if (StaticTime >= 2) {
						funcMov(0, -32);

						for (auto b : blocks) {
							for (auto cb : *CollisionBlocks)
								if (b->hitCollisionBlock(cb)) {
									funcMov(x - FigureState.x, y - FigureState.y);
									if (cb->posY == 32 * 26) {
										controlling = false;
									}
								}
							for (auto ub : *Unique_blocks) {
								if (b->hitBlock(ub)) {
									funcMov(x - FigureState.x, y - FigureState.y);
									if (b->get_y() != ub->get_y()) {
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


		toDeepGhost(Unique_blocks, CollisionBlocks);

	}
	else { //change items from blocks to unique_blocks and clear blocks
		for (auto b : blocks)
			Unique_blocks->push_back(b);
		blocks.clear();
	}
}


template <class Complete_figure>
void TetrisFigure::funcRot(Complete_figure form) {
	auto it = blocks.begin();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (form[i][j] == 1) {
				if ((it) != blocks.end()) {
					TetrisBlock* F = *it;
					(F)->set_x(x + (j * 32));
					(F)->set_y(y + (i * 32));
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
					TetrisBlock* F = *it2;
					(F)->set_x(x + (j * 32));
					(F)->set_y(ghostState.y + (i * 32));
					it2++;
				}
			}
		}
	}

}

void TetrisFigure::rotate(bool clockwise, list<TetrisBlock*> *Unique_blocks, list<Collider*> *CollisionBlocks) {
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
		for (auto cb : *CollisionBlocks)
			if (b->hitCollisionBlock(cb))
				auxstate = true;
		for (auto ub : *Unique_blocks)
			if (b->hitBlock(ub))
				auxstate = true;
	}


	if (auxstate == true) {
		bool auxColling = false;
		funcMov(32, 0);
		for (auto b : blocks) {

			for (auto cb : *CollisionBlocks)
				if (b->hitCollisionBlock(cb))
					auxColling = true;
			for (auto ub : *Unique_blocks)
				if (b->hitBlock(ub))
					auxColling = true;
		}
		if (auxColling) {
			funcMov(-64, 0);
			bool auxColling2 = false;
			for (auto b : blocks) {

				for (auto cb : *CollisionBlocks)
					if (b->hitCollisionBlock(cb))
						auxColling2 = true;
				for (auto ub : *Unique_blocks)
					if (b->hitBlock(ub))
						auxColling2 = true;
			}
			if (auxColling2) {
				funcMov(32, 0);
				rotate(clockwise, Unique_blocks, CollisionBlocks);
			}

		}
	}

}

bool TetrisFigure::Empty() {
	return blocks.empty();
}



void TetrisFigure::toDeep(list<TetrisBlock*> *Unique_blocks, list<Collider*> *CollisionBlocks) {
	bool Indeep = false;
	while (!Indeep) {
		FigureState.x = this->x;
		FigureState.y = this->y;
		funcMov(0, -32);

		for (auto b : blocks) {
			for (auto cb : *CollisionBlocks)
				if (b->hitCollisionBlock(cb)) {
					funcMov(x - FigureState.x, y - FigureState.y);
					if (cb->posY == 32 * 26) {
						Indeep = true;
						controlling = false;
					}
				}
			for (auto ub : *Unique_blocks) {
				if (b->hitBlock(ub)) {
					funcMov(x - FigureState.x, y - FigureState.y);

					if (b->get_y() != ub->get_y()) {
						Indeep = true;
						controlling = false;
					}
				}
			}
		}


	}
}

void TetrisFigure::toDeepGhost(list<TetrisBlock*> *Unique_blocks, list<Collider*> *CollisionBlocks) {
	bool Indeep = false;

	while (!Indeep) {

		for (auto gb : ghostBlocks)
			gb->update();

		for (auto gb : ghostBlocks) {
			for (auto cb : *CollisionBlocks)
				if (gb->hitCollisionBlock(cb)) {
					for (auto gb2 : ghostBlocks) {
						gb2->set_y(gb2->get_y()-32);
					}
					if (cb->posY == 32 * 26)
						Indeep = true;
				}
			for (auto b : *Unique_blocks) {
				if (gb->hitBlock(b)) {
					for (auto gb2 : ghostBlocks) {
						gb2->set_y(gb2->get_y() - 32);
					}
					if (gb->get_y() != b->get_y())
						Indeep = true;
				}
			}
		}

	}

	//Fix de Ghost when take a block
	auto ghost_ = ghostBlocks.begin();
	for (auto b : blocks) {
		if ((*ghost_)->get_y() < b->get_y())
			for (auto gb : ghostBlocks)
				gb->update();

		ghost_++;
	}

}

TetrisFigure::~TetrisFigure() {
	for (auto b : blocks) {
		delete b;
	}
	for (auto gb : ghostBlocks) {
		delete gb;
	}
}


/*
template <class T>
void TetrisFigure::forAllBlocksInFigure(void (*WhenColling)()) {


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
*/