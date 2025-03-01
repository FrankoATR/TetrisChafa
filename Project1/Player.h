struct STATE {
	int x, y;
}PlayerState;

class Player
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

	Collider* ColliderPlayer = new Collider(100, 100, 150, 150, 0, true);

	ALLEGRO_BITMAP* sprite;

	//funciones
	Player(int, int, int, int, ALLEGRO_BITMAP*); //Constructor

	void mov(ALLEGRO_EVENT_QUEUE* , ALLEGRO_EVENT);
};

Player::Player(int x , int y, int sprite_h, int sprite_w, ALLEGRO_BITMAP* sprite) {
	this->x = x;
	this->y = y;
	this->sprite = sprite;
	this->sprite_h = sprite_h;
	this->sprite_w = sprite_w;
	ColliderPlayer->posX = x;
	ColliderPlayer->posY = y;
	ColliderPlayer->dimX = sprite_h;
	ColliderPlayer->dimY = sprite_w;
	ColliderPlayer->active = true;
	taked = false;
}

void Player::mov(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento) {
	PlayerState.x = x;
	PlayerState.y = y;
	if (!taked) {

	if (Evento.type == ALLEGRO_EVENT_KEY_CHAR) {
		if (Evento.keyboard.keycode == ALLEGRO_KEY_W) {
			y -= 10;
			ColliderPlayer->posY -= 10;
		}
		if (Evento.keyboard.keycode == ALLEGRO_KEY_S) {
			y += 10;
			ColliderPlayer->posY += 10;
		}
		if (Evento.keyboard.keycode == ALLEGRO_KEY_A) {
			x -= 10;
			ColliderPlayer->posX -= 10;
		}
		if (Evento.keyboard.keycode == ALLEGRO_KEY_D) {
			x += 10;
			ColliderPlayer->posX += 10;
		}

	}
	

	if (x < 0) x = 0;
	if (x > 1080 - sprite_w) x = 1080 - sprite_w;
	if (y < 0) y = 0;
	if (y > 720 - sprite_h) y = 720 - sprite_h;


		for (list<Collider*>::iterator it = CollisionBlocks.begin(); it != CollisionBlocks.end(); it++) {
			if (ColliderPlayer->Collision((*it))) {
				cout << "collision" << endl;
				x = PlayerState.x;
				y = PlayerState.y;
				ColliderPlayer->posX = PlayerState.x;
				ColliderPlayer->posY = PlayerState.y;
			}
		}
	}
	al_draw_bitmap(sprite, x, y, NULL);
	ColliderPlayer->DisplayFigureCollision();

}