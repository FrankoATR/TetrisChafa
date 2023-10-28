#include <time.h>

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

void genereteFig() {
	srand(time(NULL));
	figures.push_back(new TetrisFigures(330, -72, rand() % 8, rand() % 5));
}

void Takeaobj(Player *obj, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento)
{
	int Mx = Evento.mouse.x;
	int My = Evento.mouse.y;

	if (obj->taked == false) {
		if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (Mx >= obj->x && Mx <= obj->x + obj->sprite_w && My >= obj->y && My <= obj->y + obj->sprite_h) {
				if (Evento.mouse.button & 1) {
					cout << "TOMADO" << endl;
					obj->x = Mx;
					obj->y = My;
					obj->ColliderPlayer->posX = Mx;
					obj->ColliderPlayer->posY = My;

					obj->taked = true;
				}
			}
		}
	}
	else {
		if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			obj->x = Mx;
			obj->y = My;
			obj->ColliderPlayer->posX = Mx;
			obj->ColliderPlayer->posY = My;
			bool isColling= false;
			for (list<Collider*>::iterator it = CollisionBlocks.begin(); it != CollisionBlocks.end(); it++) {
				if (obj->ColliderPlayer->Collision(*it)) {
					isColling = true;
				}
			}
			if (Evento.mouse.button & 1 && !isColling) {
				cout << "SOLTADO" << endl;
				obj->taked = false;
			}
		}
	}
}

int checkEachFile() {
	Collider tmpcheck(231, 159, 1, 1, 0, false);
	int fileCanDestroy = 0;
	int BONUS = 0;

	int FILESDESTROYED = 0;
	float EXTRABONUS = 1;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 10; j++) {
			//tmpcheck.DisplayFigureCollision();

			for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
				if (tmpcheck.Collision((*it)->ThisCollider) && !(*it)->taked) {
					fileCanDestroy++;
				}
			}



			tmpcheck.posX += 66;
		}
		if (fileCanDestroy == 10) {

			FILESDESTROYED++;

			tmpcheck.posX = 231;


			for (int j = 0; j < 10; j++) {

				for (list<TetrisBlocks*>::iterator it2 = Unique_blocks.begin(); it2 != Unique_blocks.end();) {
					if (tmpcheck.Collision((*it2)->ThisCollider)) {
						cout << "BLOCK DESTROYED" << endl;
						(*it2)->DestroyTetrisBlocks();
						it2 = Unique_blocks.erase(it2);
					}
					else {
						it2++;
					}
				}
				tmpcheck.posX += 66;

			}

			for (list<TetrisBlocks*>::iterator it3 = Unique_blocks.begin(); it3 != Unique_blocks.end(); it3++) {
				if ((*it3)->y <= tmpcheck.posY) {
					cout << "MOVERABAJO" << endl;
					(*it3)->mov();
				}
			}
		}

		fileCanDestroy = 0;
		tmpcheck.posX = 231;
		tmpcheck.posY += 66;

	}

		for (int i = 0; i < FILESDESTROYED-1 ; i++) {
			EXTRABONUS += 0.5;
		}


	BONUS = FILESDESTROYED * 500 * EXTRABONUS ;

	return BONUS;
}

void Takeaobj(TetrisFigures* obj, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento)
{
	int Mx = Evento.mouse.x;
	int My = Evento.mouse.y;

	if (obj->taked == false) {
		if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			for (list<TetrisBlocks*>::iterator it = obj->blocks.begin(); it != obj->blocks.end(); it++) {

				if (Mx >= (*it)->x && Mx <= (*it)->x + (*it)->sprite_w && My >= (*it)->y && My <= (*it)->y + (*it)->sprite_h) {
					if (Evento.mouse.button & 1) {
						cout << "TOMADO" << endl;

						obj->funcMov(obj->x - Mx, obj->y - My);

						obj->taked = true;
					}
				}

			}

		}
	}
	else {
		if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			obj->funcMov(obj->x - Mx, obj->y - My);

			bool isColling = false;

			for (list<TetrisBlocks*>::iterator it = obj->blocks.begin(); it != obj->blocks.end(); it++) {
				for (list<Collider*>::iterator it2 = CollisionBlocks.begin(); it2 != CollisionBlocks.end(); it2++) {
					if ((*it)->ThisCollider->Collision(*it2)) {
						isColling = true;
					}
				}
			}

			if (Evento.mouse.button & 1 && !isColling) {
				cout << "SOLTADO" << endl;
				obj->taked = false;
			}

		}
	}
}

void Takeaobj(TetrisBlocks* obj, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, int *B)
{
	int Mx = Evento.mouse.x;
	int My = Evento.mouse.y;

	if (obj->taked == false && *B >= 1000) {
		if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {


			if (Mx >= obj->x && Mx <= obj->x + obj->sprite_w && My >= obj->y && My <= obj->y + obj->sprite_h) {
				if (Evento.mouse.button & 1) {
					cout << "TOMADO" << endl;
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

			if (Mx>= 198 && Mx<= 857 && My >= 192 && My <= 653 ) {
				bool isColling = false;
				obj->x = Mx - Mx % 66;
				obj->y = My - 6 - My % 66;
				obj->ThisCollider->posX = Mx - Mx % 66;
				obj->ThisCollider->posY = My - 6 - My % 66;


				for (list<TetrisFigures*>::iterator it = figures.begin(); it != figures.end(); it++) {
					for (list<TetrisBlocks*>::iterator it2 = (*it)->blocks.begin(); it2 != (*it)->blocks.end(); it2++) {
						if (obj->ThisCollider->Collision((*it2)->ThisCollider)) {
							isColling = true;
						}
					}
				}

				if (Evento.mouse.button & 1 && !isColling) {
					cout << "SOLTADO" << endl;
					obj->taked = false;
				}

			}

		}
	}
}

bool checkGameOver() {
	for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
		if ((*it)->ThisCollider->posY <= 125) {
			return true;
		}

	}
	return false;

}