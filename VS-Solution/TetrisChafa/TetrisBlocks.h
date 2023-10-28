struct STATEBlock{
	int x, y;
}BlockState;

class TetrisBlocks
{
	//Created by: Franco119 - Francisco Alonso Torres Rosa - SV
public:
	//variables
	int x;
	int y;

	//Area colision	
	int sprite_h;
	int sprite_w;
	bool taked;

	//seleccionar Complete_Block
	int figureid;

	ALLEGRO_BITMAP* sprite1;
	ALLEGRO_BITMAP* sprite2;
	ALLEGRO_BITMAP* sprite3;
	ALLEGRO_BITMAP* sprite4;

	Unit_Block BLOCK;

	Collider* ThisCollider = new Collider(0, 0, 0, 0, 0, true);

	//funciones
	TetrisBlocks(int, int, int); //Constructor
	void virtual DestroyTetrisBlocks();
	void Display();
	void mov();
	bool IsDeletable();
	void ChangeColor(ALLEGRO_EVENT_QUEUE*, ALLEGRO_EVENT, ALLEGRO_TIMER*);

};

list<TetrisBlocks*> Unique_blocks;

TetrisBlocks::TetrisBlocks(int x, int y, int figureid) {
	this->x = x;
	this->y = y;
	this->figureid = figureid;
	sprite_h = 65;
	sprite_w = 65;
	this->sprite1 = BLOCK.green;
	this->sprite2 = BLOCK.blue;
	this->sprite3 = BLOCK.red;
	this->sprite4 = BLOCK.black;

	taked = false;

	ThisCollider->posX = x;
	ThisCollider->posY = y;
	ThisCollider->dimX = sprite_h;
	ThisCollider->dimY = sprite_w;
	ThisCollider->active = true;

	//CollisionBlocks.push_back(ThisCollider);
}

void TetrisBlocks::Display() {
	if (figureid == 0) al_draw_bitmap(sprite1,x,y,NULL);
	if (figureid == 1) al_draw_bitmap(sprite2, x, y, NULL);
	if (figureid == 2) al_draw_bitmap(sprite3, x, y, NULL);
	if (figureid == 3) al_draw_bitmap(sprite4, x, y, NULL);
}

void TetrisBlocks::mov() {

	BlockState.y = y;

	y += 66;
	ThisCollider->posY += 66;

}

bool TetrisBlocks::IsDeletable() {
	if (y > 720 - sprite_h) return true;
	else return false;
}

void TetrisBlocks::ChangeColor(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, ALLEGRO_TIMER* segundoTimer) {
	if (Evento.type == ALLEGRO_EVENT_TIMER) {
		if (Evento.timer.source == segundoTimer) {
			figureid = ((++figureid) % 4);
		}
	}
}


void TetrisBlocks::DestroyTetrisBlocks() {
	if (sprite1 != NULL) {
		al_destroy_bitmap(sprite1);
		al_destroy_bitmap(sprite2);
		al_destroy_bitmap(sprite3);
		al_destroy_bitmap(sprite4);
		ThisCollider->DestroyCollider();
	}
	delete this;
}

