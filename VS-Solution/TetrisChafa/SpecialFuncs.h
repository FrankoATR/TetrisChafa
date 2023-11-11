#include <time.h>

list<TetrisFigures*> queue_figures;


struct Matrix_Dimensions {
	int files;
	int columns;
};

/*Matrix_Dimensions Get_MatrixDims(int M[][]) {
	int files = (sizeof(M) / sizeof(M[0]));
	int columns = (sizeof(M[0]) / sizeof(M[0][0]));
}
*/

float RandomFunc(int limit_A, int limit_B) {
	return (limit_A + rand() % limit_B);
}


int checkEachFile() {
	Collider tmpcheck(96+32/2, 192+64/2, 1, 1, 0, false);
	int fileCanDestroy = 0;
	int BONUS = 0;

	int FILESDESTROYED = 0;
	float EXTRABONUS = 1;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
				if (tmpcheck.Collision((*it)->ThisCollider) && !(*it)->taked) {
					fileCanDestroy++;
				}
			}
			tmpcheck.posX += 32;
		}
		if (fileCanDestroy == 10) {
			FILESDESTROYED++;
			tmpcheck.posX = 96 + 32 / 2;
			for (int j = 0; j < 10; j++) {

				for (list<TetrisBlocks*>::iterator it2 = Unique_blocks.begin(); it2 != Unique_blocks.end();) {
					if (tmpcheck.Collision((*it2)->ThisCollider)) {
						(*it2)->DestroyTetrisBlocks();
						it2 = Unique_blocks.erase(it2);
					}
					else
						it2++;
				}
				tmpcheck.posX += 32;
			}
			for (list<TetrisBlocks*>::iterator it3 = Unique_blocks.begin(); it3 != Unique_blocks.end(); it3++) {
				if ((*it3)->y <= tmpcheck.posY) {
					(*it3)->mov();
				}
			}
		}
		fileCanDestroy = 0;
		tmpcheck.posX = 96 + 32 / 2;
		tmpcheck.posY += 32;

	}
	for (int i = 0; i < FILESDESTROYED-1 ; i++)
		EXTRABONUS += 0.5;

	BONUS = FILESDESTROYED * 200 * EXTRABONUS ;
	return BONUS;
}


void newCheckEachFile() {
	int contAux = 0;
	list<TetrisBlocks*> lineBlocks_list;

	for (int i = 1; i < 10; i++) {
		for (auto it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
			if ((*it)->y == i*66) {
				contAux++;
				lineBlocks_list.push_back(*it);
			}
			if (contAux == 10) {

			}
		}
		contAux = 0;
		lineBlocks_list.clear();
	}

}

void Takeaobj(TetrisBlocks* obj, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, int *B, bool globalTaked){
	int Mx = Evento.mouse.x;
	int My = Evento.mouse.y;

	if (obj->taked == false and !globalTaked ) {
		if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (Mx >= obj->x && Mx <= obj->x + obj->sprite_w && My >= obj->y && My <= obj->y + obj->sprite_h) {
				if (Evento.mouse.button & 1) {
					obj->x = Mx;
					obj->y = My;
					obj->ThisCollider->posX = Mx;
					obj->ThisCollider->posY = My;
					obj->taked = true;
					*B -= 1000;
				}
			}
		}
	}
	else {
		if ((Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && obj->taked== true) {
			if (Mx>= 32*3 && Mx<= 32*13-1 && My >= 32*6 && My <= 32*26-1 ) {
				bool isColling = false;
				obj->x = Mx - Mx % 32;
				obj->y = My - My % 32;
				obj->ThisCollider->posX = Mx - Mx % 32;
				obj->ThisCollider->posY = My - My % 32;

				if (Evento.mouse.button & 1) {
					for (list<TetrisFigures*>::iterator it = figures.begin(); it != figures.end(); it++) {
						for (list<TetrisBlocks*>::iterator it2 = (*it)->blocks.begin(); it2 != (*it)->blocks.end(); it2++) {
							if (obj->ThisCollider->Collision((*it2)->ThisCollider)) {
								isColling = true;
							}
						}
					}
					for (auto it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
						if (obj->ThisCollider->Collision((*it)->ThisCollider) and (*it) != obj) {
							isColling = true;
						}

					}
					if (!isColling)
						obj->taked = false;
				}
			}
		}
	}
}

bool checkGameOver() {
	for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
		if ((*it)->ThisCollider->posY < 32 * 6) {
			return true;
		}

	}
	return false;
}