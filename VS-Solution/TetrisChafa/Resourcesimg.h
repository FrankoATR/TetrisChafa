
struct Unit_Block {
	ALLEGRO_BITMAP* green = al_load_bitmap("images/blocks/green.png");
	ALLEGRO_BITMAP* blue = al_load_bitmap("images/blocks/blue.png");
	ALLEGRO_BITMAP* red = al_load_bitmap("images/blocks/red.png");
	ALLEGRO_BITMAP* black = al_load_bitmap("images/blocks/black.png");
	int identifier = 0;
};

struct Tetris_game {
	ALLEGRO_BITMAP* background1 = al_load_bitmap("images/backgrounds/BackgroundTetris.png");
	ALLEGRO_BITMAP* background2 = al_load_bitmap("images/backgrounds/background3.jpg");
	ALLEGRO_BITMAP* iconGame = al_load_bitmap("images/icon/icon.png");

};

