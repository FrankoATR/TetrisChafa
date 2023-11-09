struct STATEBlock{
	int x, y;
}BlockState;

class TetrisBlocks
{
public:
	int x;
	int y;

	int sprite_h;
	int sprite_w;
	bool taked;

	ALLEGRO_BITMAP* sprite;
	Collider* ThisCollider = new Collider(0, 0, 0, 0, 0, true);

	TetrisBlocks(int, int, ALLEGRO_BITMAP*);
	void virtual DestroyTetrisBlocks();
	void Display();
	void mov();
	bool IsDeletable();
	void ChangeColor(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, ALLEGRO_TIMER*);

};

list<TetrisBlocks*> Unique_blocks;

TetrisBlocks::TetrisBlocks(int x, int y, ALLEGRO_BITMAP* sprite) {
	this->x = x;
	this->y = y;
	this->sprite_h = 31;
	this->sprite_w = 31;
	this->taked = false;
	this->sprite = sprite;

	ThisCollider->posX = x;
	ThisCollider->posY = y;
	ThisCollider->dimX = sprite_h;
	ThisCollider->dimY = sprite_w;
	ThisCollider->active = true;
}

void TetrisBlocks::Display() {
	if(sprite)
		al_draw_bitmap(sprite, x, y, NULL);
}

void TetrisBlocks::mov() {
	BlockState.y = y;
	y += 32;
	ThisCollider->posY += 32;

}

bool TetrisBlocks::IsDeletable() {
	if (y > 27*32 - sprite_h) return true;
	else return false;
}

void TetrisBlocks::ChangeColor(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, ALLEGRO_TIMER* segundoTimer) {
	if (Evento.type == ALLEGRO_EVENT_TIMER) {
		if (Evento.timer.source == segundoTimer) {
			//spriteId = ((++spriteId) % 4);
		}
	}
}


void TetrisBlocks::DestroyTetrisBlocks() {
	ThisCollider->DestroyCollider();
	delete this;
}

