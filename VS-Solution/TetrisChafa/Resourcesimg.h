
struct Unit_Block {
	ALLEGRO_BITMAP* green = al_load_bitmap("images/blocks/green.png");
	ALLEGRO_BITMAP* blue = al_load_bitmap("images/blocks/blue.png");
	ALLEGRO_BITMAP* red = al_load_bitmap("images/blocks/red.png");
	ALLEGRO_BITMAP* black = al_load_bitmap("images/blocks/black.png");
	ALLEGRO_BITMAP* white1 = al_load_bitmap("images/blocks/white1.png");
	ALLEGRO_BITMAP* white2 = al_load_bitmap("images/blocks/white2.png");
	ALLEGRO_BITMAP* ghost = al_load_bitmap("images/blocks/ghost.png");
	int identifier = 0;
} blockColor;

struct Tetris_game {
	ALLEGRO_BITMAP* menu_sprite = al_load_bitmap("images/backgrounds/menu.png");
	ALLEGRO_BITMAP* bg_sprite = al_load_bitmap("images/backgrounds/bg.png");
	ALLEGRO_BITMAP* iconGame = al_load_bitmap("images/icon/icon.png");

};

