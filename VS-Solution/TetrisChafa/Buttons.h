class Buttons {
	//Created by: Franco119 - Francisco Alonso Torres Rosa - SV
	int x, y;
	int sprite_w, sprite_h;
	ALLEGRO_BITMAP* sprite_0;
	ALLEGRO_BITMAP* sprite_1;
	float proportion_W;
	float proportion_H;
	bool Press_Range;
	bool Press_Button;

	public:
		Buttons(int, int, int, int, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);
		~Buttons();
		void Display(ALLEGRO_EVENT_QUEUE* ,ALLEGRO_EVENT, float, float);
		bool Pressed();
		void update(float);
};

Buttons::Buttons(int x, int y, int sprite_w, int sprite_h, ALLEGRO_BITMAP* sprite_0, ALLEGRO_BITMAP* sprite_1) {
	this->x = x;
	this->y = y;
	this->sprite_w = sprite_w;
	this->sprite_h = sprite_h;
	this->sprite_0 = sprite_0;
	this->sprite_1 = sprite_1;
	Press_Range = false;
	Press_Button = false;

}

void Buttons::update(float relativePos) {
	this->x -= relativePos;
}

Buttons::~Buttons() {
}

void Buttons::Display(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, float proportion_W, float proportion_H) {
	if (Press_Range) al_draw_bitmap(sprite_1, x, y, NULL);

	else al_draw_bitmap(sprite_0, x, y, NULL);


	if (Evento.type == ALLEGRO_EVENT_MOUSE_AXES || Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		int Mx = Evento.mouse.x/ proportion_W;
		int My = Evento.mouse.y/ proportion_H;
		if (Mx >= x && Mx <= x + sprite_w && My >= y && My <= y + sprite_h) {
			al_draw_bitmap(sprite_1, x, y, NULL);
			Press_Range = true;
			if (Evento.mouse.button & 1) Press_Button = true;
			else Press_Button = false;
		}
		else { 
			al_draw_bitmap(sprite_0, x, y, NULL);
			Press_Range = false;
		}
	}
	else {
		Press_Button = false;
	}
}

bool Buttons::Pressed() {
	if (Press_Button) {
		return true;
	}
	else return false;
}