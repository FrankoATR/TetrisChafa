/*

	If you looking at this code, I want you to know that you are amazing!
	Enjoy this code maze XD
	
	This project was Created by: Francisco Alonso Torres Rosa - SV

*/

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

#include <string>
#include <Windows.h>
#include <list>

using namespace std;


#include "Collisions.h"
#include "Resourcesimg.h"
#include "Player.h"
#include "Buttons.h"
#include "TetrisFigures.h"
#include "SpecialFuncs.h"


#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


//Inicializadores
//DECLARACION DE TYPES PRINCIPALES
ALLEGRO_FONT* dirt_chunk;
ALLEGRO_FONT* coords;

ALLEGRO_TIMER* segundoTimer;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_DISPLAY* ventana;
ALLEGRO_EVENT Evento;
ALLEGRO_TIMER* FPS;


//DISPLAY DE VENTANA
int ancho = 1080;
int alto = 720;
int ancho_W = GetSystemMetrics(SM_CXSCREEN);
int alto_W = GetSystemMetrics(SM_CYSCREEN);



//VARIABLES PRINCIPALES
ALLEGRO_COLOR colorblanco = al_map_rgb(255, 255, 255);
ALLEGRO_COLOR colorazar = al_map_rgb(155, 55, 25);
int segundo = 1000;
int countFPS = 0;
int x = -1, y = -1;
int boolbuttons[] = { 0,0,0,0 };

//Funciones a llamar
int menu_game();
int game_in();
void timerfunc();
void genereteFig();

int main() {


	if (!al_init()) {
		al_show_native_message_box(NULL, "ERROR CRITICO", "ERROR: en 4 como tu vieja", "solo tenias que seguir el maldito tren CJ", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
		return -1;
	}

	//INIT DE METODOS
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();

	ventana = al_create_display(ancho, alto);
	al_set_window_title(ventana, "TetrisChafa");
	al_set_window_position(ventana, ancho_W / 2 - ancho / 2, alto_W / 2 - alto / 2);


	//TYPES PRINCIPALES
	segundoTimer = al_create_timer(1.0);
	FPS = al_create_timer(1.0 / 30);
	event_queue = al_create_event_queue();

	//REGISTRO DE EVENTOS
	al_register_event_source(event_queue, al_get_timer_event_source(segundoTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(FPS));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(ventana));

	//START DE TYPES
	al_start_timer(segundoTimer);
	al_start_timer(FPS);


	menu_game();
	//game_in();

	al_destroy_display(ventana);
	al_destroy_font(dirt_chunk);

	return 0;
}

int menu_game(){
	Tetris_game SysImg;
	al_set_display_icon(ventana, SysImg.iconGame);

	dirt_chunk = al_load_font("fonts/Minecraft.ttf", 70, 0);
	coords = al_load_font("fonts/Minecraft.ttf", 10, 0);
	/*
	ALLEGRO_BITMAP* menu_null = al_load_bitmap("images/menu_null.bmp");
	ALLEGRO_BITMAP* menu_opciones = al_load_bitmap("images/menu_1.bmp");
	ALLEGRO_BITMAP* menu_fornicar = al_load_bitmap("images/menu_2.bmp");
	ALLEGRO_BITMAP* menu_follar = al_load_bitmap("images/menu_3.bmp");
	ALLEGRO_BITMAP* menu_copular = al_load_bitmap("images/menu_4.bmp");
	ALLEGRO_BITMAP* menu_salir = al_load_bitmap("images/menu_5.bmp");
	*/
	Tetris_game Tetrismenu;

	int MaxPoints = 0;

	Buttons BOTON1(470, 450, 200, 100, al_load_bitmap("images/buttons/button_salir_false.png"), al_load_bitmap("images/buttons/button_salir_true.png"));
	Buttons BOTON2(250, 300, 200, 100, al_load_bitmap("images/buttons/button_false.png"), al_load_bitmap("images/buttons/button_true.png"));
	Buttons BOTON3(470, 300, 200, 100, al_load_bitmap("images/buttons/button_jugar_false.png"), al_load_bitmap("images/buttons/button_jugar_true.png"));

	while (true) {


		al_clear_to_color(al_map_rgb(255, 255, 255));

		al_wait_for_event(event_queue, &Evento);

		al_draw_bitmap(Tetrismenu.background1, 0, 0, NULL);

		al_draw_text(dirt_chunk, al_map_rgb( 255, 255, 255), 300, 50, NULL, ("TetrisChafa V1.1"));

		al_draw_text(dirt_chunk, al_map_rgb(255, 255, 255), 340, 150, NULL, ("Max score: " + to_string(MaxPoints)).c_str());
		//al_draw_text(dirt_chunk, al_map_rgb(255, 255, 255), 200, 600, NULL, ("Made by: Francisco Rosa"));

		BOTON1.Display(event_queue, Evento);
		//BOTON2->Display(event_queue, Evento);
		BOTON3.Display(event_queue, Evento);


		if (BOTON1.Pressed()) return 0;
		//if (BOTON2->Pressed()) al_set_display_icon(ventana, Tetrismenu.background1);

		if (BOTON3.Pressed()) {
			int tmpPoints = game_in();

			if (MaxPoints < tmpPoints) MaxPoints = tmpPoints;

		}

		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			return 0;
		}

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				return 0;
			}
		}



		al_flip_display();
	}
}

int game_in() {
	dirt_chunk = al_load_font("fonts/Minecraft.ttf", 50, 0);
	coords = al_load_font("fonts/Minecraft.ttf", 15, 0);

	ALLEGRO_BITMAP* background1 = al_load_bitmap("images/backgrounds/BackgroundTetris.png");
	Tetris_game Tetrisimg;

	//Buttons* BOTON4 = new Buttons(800, 100, 200, 100, al_load_bitmap("images/buttons/button_false.png"), al_load_bitmap("images/buttons/button_true.png"));
	//Buttons* BOTON5 = new Buttons(800, 250, 200, 100, al_load_bitmap("images/buttons/button_false.png"), al_load_bitmap("images/buttons/button_true.png"));
	//Buttons* BOTON6 = new Buttons(800, 400, 200, 100, al_load_bitmap("images/buttons/button_false.png"), al_load_bitmap("images/buttons/button_true.png"));

	for (int i = 0; i < 11; i++) {
		CollisionBlocks.push_back(new Collider(132, -72 + 66 * i, 65, 65, 0, true));
		CollisionBlocks.push_back(new Collider(858, -72 + 66 * i, 65, 65, 0, true));
	}
	for (int i = 0; i < 17; i++) {
		CollisionBlocks.push_back(new Collider(0 + 66 * i, 654, 65, 65, 0, true));
	}

	int BONUSPOINTS = 0;
	bool MouseIn = false;

	//Player *PLAYER1 = new Player(500, 500, 100, 100, al_load_bitmap("images/sprites/player1copy.png"));


	//figures.push_back(new TetrisFigures(198, 192, 3, 0));

	bool CollidersTetrisON = false;
	bool CollidersBackgroundON = false;

	while (true) {

		al_wait_for_event(event_queue, &Evento);

		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == segundoTimer) {
				segundo--;
				cout << "Bloques Unicos: " << Unique_blocks.size() << endl;
				cout << "Figuras: " << figures.size() << endl;
				cout << "Total Colliders: " << CollisionBlocks.size() << endl;

				//timerfunc();
			}
		}

		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			return 0;
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap(Tetrisimg.background2 , 0, 0, 0);

		//PLAYER1->mov(event_queue, Evento);

		//BOTON4->Display(event_queue, Evento);
		//BOTON5->Display(event_queue, Evento);
		//BOTON6->Display(event_queue, Evento);

		//if (BOTON5->Pressed()) genereteFig();

		if (!Unique_blocks.empty()) {
			for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
				(*it)->Display();
				//(*it)->ChangeColor(event_queue, Evento, segundoTimer);
				Takeaobj(*it, event_queue, Evento, &BONUSPOINTS);
			
			}	
		}



		if (!figures.empty()) {
			for (list<TetrisFigures*>::iterator it = figures.begin(); it != figures.end();) {
				if (!(*it)->Empty()) {
					(*it)->Display();
					(*it)->mov(event_queue, Evento, segundoTimer);
				
					//Takeaobj((*it), event_queue, Evento);

					it++;

				}
				else {
					cout << "muelto del todo" << endl;
					(*it)->DestroyTetrisFigures();
					delete (*it);
					it = figures.erase(it);
				}
			}
		}
		else {
			genereteFig();
		}
		//al_draw_text(dirt_chunk, al_map_rgb(255, 255, 255), 550, 650, NULL, ("Made by: Francisco Rosa"));

		/*
		// ******************** MOSTRAR COLLIDERS ********************
		if (CollidersTetrisON) {
			for (list<TetrisFigures*>::iterator it = figures.begin(); it != figures.end(); it++) {
				for (list<TetrisBlocks*>::iterator it2 = (*it)->blocks.begin(); it2 != (*it)->blocks.end(); it2++) {
					(*it2)->ThisCollider->DisplayFigureCollision();
					al_draw_text(coords, al_map_rgb(255, 255, 255), (*it2)->ThisCollider->posX + 5, (*it2)->ThisCollider->posY + 5, NULL, ("(" + to_string((*it2)->ThisCollider->posX) + "," + to_string((*it2)->ThisCollider->posY) + ")").c_str());
					al_draw_text(coords, al_map_rgb(255, 255, 255), (*it2)->x + 5, (*it2)->y + 25, NULL, ("(" + to_string((*it2)->x) + "," + to_string((*it2)->y) + ")").c_str());
				}
			}

			for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
				(*it)->ThisCollider->DisplayFigureCollision();
			}

		}
		if (BOTON4->Pressed()) {
			if (!CollidersTetrisON) CollidersTetrisON = true;
			else CollidersTetrisON = false;
		}
		if (CollidersBackgroundON) {
			for (list<Collider*>::iterator it = CollisionBlocks.begin(); it != CollisionBlocks.end(); it++) {
				(*it)->DisplayFigureCollision();
				al_draw_text(coords, al_map_rgb(255, 255, 255), (*it)->posX + 5, (*it)->posY + 5, NULL, ("(" + to_string((*it)->posX) + "," + to_string((*it)->posY) + ")").c_str());
				al_draw_text(coords, al_map_rgb(255, 255, 255), (*it)->posX + 5, (*it)->posY + 25, NULL, ("(" + to_string((*it)->posX+ (*it)->dimX) + "," + to_string((*it)->posY + (*it)->dimY) + ")").c_str());

			}
		}
		if (BOTON6->Pressed()) {
			if (!CollidersBackgroundON) CollidersBackgroundON = true;
			else CollidersBackgroundON = false;
		}
		// ******************** ********************* ********************
		*/


		//Takeaobj(PLAYER1, event_queue, Evento);

		BONUSPOINTS += checkEachFile();
		if (checkGameOver()) {
			cout << "GAME OVER" << endl;
			for (list<TetrisBlocks*>::iterator it2 = Unique_blocks.begin(); it2 != Unique_blocks.end();) {
				cout << "ALL BLOCK DESTROYED" << endl;
				(*it2)->DestroyTetrisBlocks();
				it2 = Unique_blocks.erase(it2);
			}
			for (list<Collider*>::iterator it = CollisionBlocks.begin(); it != CollisionBlocks.end();) {
				cout << "ALL COLLIDER DESTROYED" << endl;
				(*it)->DestroyCollider();
				it = CollisionBlocks.erase(it);
			}
			al_destroy_bitmap(Tetrisimg.background2);
			al_destroy_bitmap(background1);
			
			return BONUSPOINTS;
		}

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				return 0;
			}
		}


		//al_draw_text(dirt_chunk, al_map_rgb(255, 255, 255), 400, 50, NULL, ("Contador: " + to_string(segundo / 60) + ":" + to_string(segundo % 60)).c_str());
		al_draw_text(dirt_chunk, al_map_rgb(255, 255, 255), 50, 30, NULL, ("Score: " + to_string(BONUSPOINTS)).c_str() );
		al_draw_text(dirt_chunk, al_map_rgb(255, 255, 255), 550, 30, NULL, ("Move 1 block for 1000"));


		al_flip_display();
	}
}

void timerfunc() {
	cout << "Minutos: " << segundo / 60 << ", " << "segundos: " << segundo % 60 << endl;
}