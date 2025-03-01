#include "TetrisBlock.h"


TetrisBlock::TetrisBlock(int x, int y, ALLEGRO_BITMAP* sprite) {
	this->x = x;
	this->y = y;
	this->sprite_h = 32 - 1;
	this->sprite_w = 32 - 1;
	this->taked = false;
	this->sprite = sprite;

	this->die = false;
	this->dieTime = 15;
}

void TetrisBlock::draw() {
	if (sprite)
		al_draw_bitmap(sprite, x, y, NULL);
}

void TetrisBlock::update() {
	y += 32;
}

bool TetrisBlock::IsDeletable() {
	if (y > 27 * 32 - sprite_h) return true;
	else return false;
}

void TetrisBlock::ChangeColor(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, ALLEGRO_TIMER* segundoTimer) {
	if (Evento.type == ALLEGRO_EVENT_TIMER) {
		if (Evento.timer.source == segundoTimer) {
			//spriteId = ((++spriteId) % 4);
		}
	}
}


void TetrisBlock::deleting(ALLEGRO_BITMAP* sprite1, ALLEGRO_BITMAP* sprite2) {
	if (dieTime > 7) this->sprite = sprite1;
	else this->sprite = sprite2;
	if (dieTime <= 0) dieTime = 0;
	else dieTime--;
}

bool TetrisBlock::hitBlock(TetrisBlock* block){
		return ( (x <= block->x + block->sprite_w) && (x + sprite_w >= block->x) &&
			(y <= block->y + block->sprite_h) && (y + sprite_h >= block->y) );
}

bool TetrisBlock::hitCollisionBlock(Collider* block) {
	return ((x <= block->posX + block->dimX) && (x + sprite_w >= block->posX) &&
		(y <= block->posY + block->dimY) && (y + sprite_h >= block->posY));
}

int TetrisBlock::get_x() {
	return x;
}

int TetrisBlock::get_y() {
	return y;
}

void TetrisBlock::set_x(int x) {
	this->x = x;
}

void TetrisBlock::set_y(int y) {
	this->y = y;
}

void TetrisBlock::DisplayFigureCollision() {
	al_draw_rectangle(x, y, x + sprite_w, y + sprite_h, al_map_rgb(255, 0, 0), 2);
}

TetrisBlock::~TetrisBlock() {

}

