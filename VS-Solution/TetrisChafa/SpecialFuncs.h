list<TetrisFigures*> queue_figures;

float RandomFunc(int limit_A, int limit_B) {
	return (limit_A + rand() % limit_B);
}


int checkEachFile(ALLEGRO_BITMAP* sprite1, ALLEGRO_BITMAP* sprite2) {
	int countBlockFile = 0;
	int countFiles = 0;
	float combo = 1;

	list<TetrisBlocks*> lineBlocks_list;

	for (int i = 0 + 6; i < 20 + 6; i++) {
		for (auto it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
			auto block = *it;

			if (block->y == i * 32 and !block->taked) {
				countBlockFile++;
				lineBlocks_list.push_back(block);
			}

		}
		if (countBlockFile == 10) {
			bool MoveAbove = false;
			for (auto it = lineBlocks_list.begin(); it != lineBlocks_list.end(); it++) {
				auto blockToDelete = *it;

				blockToDelete->die = true;
				blockToDelete->deleting(sprite1, sprite2);
				if (blockToDelete->dieTime == 0) {
					Unique_blocks.remove(blockToDelete);
					delete blockToDelete;
					MoveAbove = true;

				}


			}
			if (MoveAbove) {
				for (auto blockToMove : Unique_blocks) {
					if (blockToMove->y < i * 32)
						blockToMove->mov();
				}
				countFiles++;

			}

		}

		countBlockFile = 0;
		lineBlocks_list.clear();
	}


	for (int i = 0; i < countFiles-1; i++)
		combo += 0.5;

	return countFiles * 200 * combo;
}




void Takeaobj(TetrisBlocks* obj, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, int *B, bool globalTaked, float proportion_W, float proportion_H){
	int Mx = Evento.mouse.x / proportion_W;
	int My = Evento.mouse.y / proportion_H;

	if (obj->taked == false and !obj->die and !globalTaked && *B >= 1000) {
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
					for (auto fig : figures)
						for (auto block : fig->blocks)
							if (obj->ThisCollider->Collision(block->ThisCollider))
								isColling = true;

					for (auto block : Unique_blocks)
						if (obj->ThisCollider->Collision(block->ThisCollider) and block != obj)
							isColling = true;


					if (!isColling)
						obj->taked = false;
				}
			}
		}
	}
}

bool checkGameOver() {
	for (auto block : Unique_blocks)
		if (block->y < 32 * 6)
			return true;
	return false;
}