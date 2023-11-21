
class TetrisBlocks
{
public:
	int x;
	int y;

	int sprite_h;
	int sprite_w;
	bool taked;
	bool die;
	int dieTime;

	ALLEGRO_BITMAP* sprite;
	Collider* ThisCollider;

	TetrisBlocks(int, int, ALLEGRO_BITMAP*);
	~TetrisBlocks();

	void Display();
	void mov();
	bool IsDeletable();
	void ChangeColor(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, ALLEGRO_TIMER*);
	void deleting(ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);

};

list<TetrisBlocks*> Unique_blocks;

TetrisBlocks::TetrisBlocks(int x, int y, ALLEGRO_BITMAP* sprite) {
	this->x = x;
	this->y = y;
	this->sprite_h = 32;
	this->sprite_w = 32;
	this->taked = false;
	this->sprite = sprite;

	this->die = false;
	dieTime = 15;

	ThisCollider = new Collider(x, y, sprite_h, sprite_w, 0, true);

}

void TetrisBlocks::Display() {
	if(sprite)
		al_draw_bitmap(sprite, x, y, NULL);
}

void TetrisBlocks::mov() {
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


void TetrisBlocks::deleting(ALLEGRO_BITMAP* sprite1, ALLEGRO_BITMAP* sprite2) {
	if (dieTime > 7) this->sprite = sprite1;
	else this->sprite = sprite2;

	if (dieTime <= 0) dieTime = 0;
	else dieTime--;
}


TetrisBlocks::~TetrisBlocks(){
	delete ThisCollider;
}

