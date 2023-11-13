/*
	If you looking at this code, I want you to know that you are amazing!
	Enjoy this code maze XD
	This project was Created by: Francisco Alonso Torres Rosa - SV
*/

#pragma region Includes
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <string>
#include <Windows.h>
#include <list>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#include <stdio.h>
#include <shlobj.h>
#include <objbase.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")

using namespace std;

#include "Collisions.h"
#include "Buttons.h"
#include "TetrisFigures.h"
#include "SpecialFuncs.h"
#pragma endregion

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

void printAUX(const void* aux) {
	const Collider* collider = static_cast<const Collider*>(aux);
	std::cout << collider->posX << std::endl;
}

template <class T> void DoALL(T dataList, void (*callback)(const void*)) {
	for (auto it = dataList.begin(); it != dataList.end();) {
		callback(&(*it));
		it = dataList.erase(it);
	}
}


#pragma region GlobalVars
//ALLEGRO VARS
ALLEGRO_FONT* globalFont;
ALLEGRO_TIMER* FPS;
ALLEGRO_TIMER* secsTimer;
ALLEGRO_TIMER* fallingTimer;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_DISPLAY* window;
ALLEGRO_EVENT Evento;
ALLEGRO_TRANSFORM tr;
ALLEGRO_SAMPLE* menu_song;
ALLEGRO_SAMPLE* game_song0;
ALLEGRO_SAMPLE* game_song1;
ALLEGRO_SAMPLE* game_song2;
ALLEGRO_SAMPLE_INSTANCE* menu_song_Instance;
ALLEGRO_SAMPLE_INSTANCE* game_song0_Instance;
ALLEGRO_SAMPLE_INSTANCE* game_song1_Instance;
ALLEGRO_SAMPLE_INSTANCE* game_song2_Instance;
ALLEGRO_BITMAP* menu_sprite;
ALLEGRO_BITMAP* bg_sprite;
ALLEGRO_BITMAP* iconGame;
ALLEGRO_COLOR colorblanco = al_map_rgb(255, 255, 255);
ALLEGRO_BITMAP* block_green;
ALLEGRO_BITMAP* block_blue;
ALLEGRO_BITMAP* block_red;
ALLEGRO_BITMAP* block_black;
ALLEGRO_BITMAP* block_white1;
ALLEGRO_BITMAP* block_white2;
ALLEGRO_BITMAP* block_ghost;
ALLEGRO_BITMAP* button_play_t;
ALLEGRO_BITMAP* button_play_f;
ALLEGRO_BITMAP* button_exit_t;
ALLEGRO_BITMAP* button_exit_f;
ALLEGRO_BITMAP* button_controls_t;
ALLEGRO_BITMAP* button_controls_f;
ALLEGRO_BITMAP* button_toFull_t;
ALLEGRO_BITMAP* button_toFull_f;
ALLEGRO_BITMAP* button_toWindow_t;
ALLEGRO_BITMAP* button_toWindow_f;
ALLEGRO_BITMAP* key_up;
ALLEGRO_BITMAP* key_down;
ALLEGRO_BITMAP* key_left;
ALLEGRO_BITMAP* key_right;
ALLEGRO_BITMAP* key_space;
ALLEGRO_BITMAP* key_z;
ALLEGRO_BITMAP* key_x;
ALLEGRO_BITMAP* key_leftClick;
ALLEGRO_BITMAP* key_esc;

ALLEGRO_BITMAP* colorBlocks[4];

struct Songs {
	ALLEGRO_SAMPLE_INSTANCE* song;
	int duration;
}songsList[3];

int MusicDuration;
int idMusic = 0;
float volumeIntermediate = 0.0;

//WINDOW VARS
int widthORG = 32 * 36;
int heightORG = 32 * 29;
int widtht_W = GetSystemMetrics(SM_CXSCREEN);
int height_W = GetSystemMetrics(SM_CYSCREEN);
bool isFullScreen;

float proportion_W = float(widtht_W) / float(widthORG);
float proportion_H = float(height_W) / float(heightORG);

//OTHER VARS
int segundo = 1000;
int countFPS = 0;
bool ButtonPressed = false;
bool blockTaked = false;
int auxMinCount = 60;
int velocityLevel = 1;
bool alreadyHold = false;
TetrisFigures* holdFigure = NULL;

#pragma endregion

#pragma region GlobalMethods
//Main Methods
int menu_game();
int game_in();
void menu_controls_in();
void timerfunc();
void initialize();
void endProgram();

//Other Methods
void displayInfoInGame(int);
void displayGridAndCoordinates();
void cleanAllInGame();
void displayAllColliders();
void drawText(int, int, string);
void genereteFig();

#pragma endregion

int main() {

	if (!al_init()) {
		al_show_native_message_box(NULL, "Error al intentar iniciar", "ERROR: initialize", "solo tenias que seguir el maldito tren CJ", NULL, ALLEGRO_MESSAGEBOX_YES_NO);
		return -1;
	}

	initialize();
	menu_game();
	endProgram();

	return 0;
}

int menu_game() {

	int MaxPoints = 0;
	bool inControls = false;
	int WaitToChangeScreen = 0;


	PWSTR path = NULL;
	HRESULT r;
	json j;
	r = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &path);
	if (path != NULL)
	{
		wstring savedGamesPath(path);
		CoTaskMemFree(path);
		wstring jsonFilePath = savedGamesPath + L"\\My Games\\TetrisChafa\\data.json";
		const wchar_t* jsonPath = jsonFilePath.c_str();
		ifstream f(jsonPath);
		if (f.is_open()) {
			j = json::parse(f);
			MaxPoints = j["MaxScore"];
		}
	}




	al_play_sample_instance(menu_song_Instance);

	al_set_timer_speed(fallingTimer, 0.2);



	Buttons playButton(450, 350, 200, 100, button_play_f, button_play_t);
	Buttons controlsButton(410, 500, 280, 100, button_controls_f, button_controls_t);
	Buttons exitButton(450, 650, 200, 100, button_exit_f, button_exit_t);
	Buttons toFullButton(32*34, 32*1, 50, 50, button_toFull_f, button_toFull_t);
	Buttons toWindowButton(32 * 34, 32 * 1, 50, 50, button_toWindow_f, button_toWindow_t);

	while (true) {

		al_clear_to_color(al_map_rgb(255, 255, 255));

		al_wait_for_event(event_queue, &Evento);

		al_draw_bitmap(menu_sprite, 0, 0, NULL);
		
		//Animation Menu

		for (auto it = figures.begin(); it != figures.end(); it++)
			(*it)->Display();

		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == secsTimer) {
				int xPosAux = (rand() % 32) * 32;

				figures.push_back(new TetrisFigures(xPosAux, 32 * -5, rand() % cantFig, rand() % 5, colorBlocks[rand() % 4], block_ghost));



			}
			if (Evento.timer.source == FPS) {


				for (auto it = figures.begin(); it != figures.end();) {
					if ((*it)->y <= 32 * 35) {
						(*it)->funcMov(0, -4);
						it++;
					}
					else {
						(*it)->DestroyTetrisFigures();
						it = figures.erase(it);
					}
				}

			}
			if (Evento.timer.source == secsTimer) {
				for (auto it = figures.begin(); it != figures.end();) {
					if ((*it)->y <= 32 * 35) {
						int auxRot = rand() % 4;
						if (auxRot == 0)
							(*it)->rotate(true);
						else if (auxRot == 1)
							(*it)->rotate(false);
						it++;
					}
					else {
						(*it)->DestroyTetrisFigures();
						it = figures.erase(it);
					}
				}

			}
		}



		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W* 1.5, proportion_H* 1.5);
		al_use_transform(&tr);
		drawText(32 * 5, 32 * 1, "TetrisChafa");


		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W * 0.4, proportion_H * 0.4);
		al_use_transform(&tr);
		drawText(32 * 1, 32 * 70, "v1.25.1");

		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W*0.7, proportion_H*0.7);
		al_use_transform(&tr);
		drawText(32 * 17, 32 * 8, "Max score: " + to_string(MaxPoints));

		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W, proportion_H);
		al_use_transform(&tr);

		playButton.Display(event_queue, Evento, proportion_W, proportion_H);
		controlsButton.Display(event_queue, Evento, proportion_W, proportion_H);
		exitButton.Display(event_queue, Evento, proportion_W, proportion_H);


		if (widtht_W == 1920 && height_W == 1080) {
			if (isFullScreen) {
				toWindowButton.Display(event_queue, Evento, proportion_W, proportion_H);
				if (toWindowButton.Pressed() && WaitToChangeScreen <= 1) {
					isFullScreen = !isFullScreen;
					proportion_W = 1.0;
					proportion_H = 1.0;
					al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, false);
					al_set_display_flag(window, ALLEGRO_WINDOWED, true);
					al_resize_display(window, widthORG, heightORG);
					al_set_window_position(window, widtht_W / 2 - widthORG / 2, (height_W / 2 - heightORG / 2));

					WaitToChangeScreen = 30;
				}
			}
			else {
				toFullButton.Display(event_queue, Evento, proportion_W, proportion_H);
				if (toFullButton.Pressed() && WaitToChangeScreen <= 1) {
					isFullScreen = !isFullScreen;
					proportion_W = float(widtht_W) / float(widthORG);
					proportion_H = float(height_W) / float(heightORG);
					al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, true);
					al_set_display_flag(window, ALLEGRO_WINDOWED, false);
					al_resize_display(window, widtht_W, height_W);

					WaitToChangeScreen = 30;

				}
			}

			if (WaitToChangeScreen > 0) WaitToChangeScreen--;
		}


		if (exitButton.Pressed()) return 0;

		if (playButton.Pressed()) {

			for (auto it = figures.begin(); it != figures.end();) {
				(*it)->DestroyTetrisFigures();
				it = figures.erase(it);
			}

			al_stop_sample_instance(menu_song_Instance);

			
			int tmpPoints = game_in();

			al_stop_sample_instance(songsList[idMusic].song);

			al_play_sample_instance(menu_song_Instance);

			al_set_timer_speed(fallingTimer, 0.2);

			if (MaxPoints < tmpPoints) { 
				MaxPoints = tmpPoints;

				r = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &path);
				if (path != NULL)
				{
					wstring savedGamesPath(path);
					CoTaskMemFree(path);
					wstring jsonFilePath = savedGamesPath + L"\\My Games\\TetrisChafa\\data.json";
					const wchar_t* jsonPath = jsonFilePath.c_str();
					if (CreateDirectory(jsonFilePath.substr(0, jsonFilePath.find_last_of(L"\\")).c_str(), NULL) ||
						ERROR_ALREADY_EXISTS == GetLastError()) {
						ifstream f(jsonPath);
						j["MaxScore"] = MaxPoints;
						ofstream o(jsonPath);
						o << setw(4) << j << endl;
					}
					else {
						ifstream f(jsonPath);
						j["MaxScore"] = MaxPoints;
						ofstream o(jsonPath);
						o << setw(4) << j << endl;
					}
				}



			};
		}

		if (controlsButton.Pressed()) {
			menu_controls_in();

		}

		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return 0;

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !ButtonPressed)
				return 0;


		ButtonPressed = false;
		al_flip_display();
	}
}

void menu_controls_in() {

	while (true) {

		al_wait_for_event(event_queue, &Evento);

		al_clear_to_color(al_map_rgb(255, 255, 255));

		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W, proportion_H);
		al_use_transform(&tr);
		al_draw_bitmap(menu_sprite, 0, 0, 0);

		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				ButtonPressed = true;
				break;
			}

		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W*2, proportion_H*2);
		al_use_transform(&tr);
		drawText(32 * 4, 32 * 0 + 16, "Controls");

		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W, proportion_H);
		al_use_transform(&tr);

		al_draw_bitmap(key_right, 32 * 2, 32 * 5, NULL);
		drawText(32 * 5, 32 * 5, "Move piece to rigth");

		al_draw_bitmap(key_left, 32 * 2, 32 * 7, NULL);
		drawText(32 * 5, 32 * 7, "Move piece to left");

		al_draw_bitmap(key_down, 32 * 2, 32 * 9, NULL);
		drawText(32 * 5, 32 * 9, "Fast fall");

		al_draw_bitmap(key_up, 32 * 2, 32 * 11, NULL);
		drawText(32 * 5, 32 * 11, "Hold current piece");

		al_draw_bitmap(key_z, 32 * 2, 32 * 13, NULL);
		drawText(32 * 5, 32 * 13, "Counterclockwise rotation");

		al_draw_bitmap(key_x, 32 * 2, 32 * 15, NULL);
		drawText(32 * 5, 32 * 15, "Clockwise rotation");

		al_draw_bitmap(key_space, 32 * 2, 32 * 17, NULL);
		drawText(32 * 9, 32 * 17, "Instant fall");

		al_draw_bitmap(key_leftClick, 32 * 2, 32 * 19, NULL);
		drawText(32 * 5, 32 * 19, "Take a block");


		al_draw_bitmap(key_esc, 32 * 2, 32 * 23, NULL);
		drawText(32 * 5, 32 * 23, "Exit");


		al_flip_display();

	}
}

int game_in() {

	int BONUSPOINTS = 0;
	bool CollidersTetrisON = false;
	bool CollidersBackgroundON = false;
	bool auxTaked = false;
	float auxVolumeCount = 0;
	velocityLevel = 1;
	auxMinCount = 60;

	idMusic = rand() % 3;
	MusicDuration = songsList[idMusic].duration;
	volumeIntermediate = 0.0;

	al_set_sample_instance_gain(songsList[idMusic].song, volumeIntermediate);
	al_play_sample_instance(songsList[idMusic].song);

	al_set_timer_speed(fallingTimer, 1.0);


	//Initialize Objects in game
	for (int i = 0; i < 29; i++) {
		CollisionBlocks.push_back(new Collider(32*2, 32*i, 31, 31, 0, true));
		CollisionBlocks.push_back(new Collider(32*1, 32*i, 31, 31, 0, true));
		CollisionBlocks.push_back(new Collider(32*0, 32*i, 31, 31, 0, true));


		CollisionBlocks.push_back(new Collider(32*13, 32*i, 31, 31, 0, true));
		CollisionBlocks.push_back(new Collider(32*14, 32*i, 31, 31, 0, true));
		CollisionBlocks.push_back(new Collider(32*15, 32*i, 31, 31, 0, true));
	}
	for (int i = 3; i < 13; i++) {
		CollisionBlocks.push_back(new Collider(32 * i, 32 * 26, 31, 31, 0, true));
		CollisionBlocks.push_back(new Collider(32 * i, 32 * 27, 31, 31, 0, true));
		CollisionBlocks.push_back(new Collider(32 * i, 32 * 28, 31, 31, 0, true));

	}

	for (int i = 0; i < 3; i++) {

		queue_figures.push_front(new TetrisFigures(32 * 18, 32 * 12 + (i * 6 * 32), rand() % cantFig, rand() % 5, colorBlocks[rand() % 4], block_ghost));

	}




	while (true) {

		al_wait_for_event(event_queue, &Evento);

		++countFPS %= 60;

		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == secsTimer) {
				segundo--;
				
				//cout << "Bloques Unicos: " << Unique_blocks.size() << endl;
				//cout << "Figuras: " << figures.size() << endl;
				//cout << "Total Colliders: " << CollisionBlocks.size() << endl;
				//timerfunc();

				//cout << "Duration: " << MusicDuration << endl;
				//cout << "MusicId " << idMusic << endl;

				if (auxMinCount < 1) {
					velocityLevel++;

					if (velocityLevel < 10) auxMinCount = 60;
					else auxMinCount = 120;

					if (velocityLevel <= 10) {
						al_set_timer_speed(fallingTimer, 1.0 - (velocityLevel - 1) * 0.1);
					}
					else if (velocityLevel >= 11) {
						velocityLevel = 11;
						al_set_timer_speed(fallingTimer, 0.05);
					}

				}
				else auxMinCount--;

				if (MusicDuration == 0) {
					al_stop_sample_instance(songsList[idMusic].song);
					idMusic++;
					if (idMusic >= 3) idMusic = 0;
					MusicDuration = songsList[idMusic].duration;
					al_play_sample_instance(songsList[idMusic].song);

				}
				else {
					MusicDuration--;
				}


			}
			if (Evento.timer.source == FPS) {
				if (MusicDuration <= 5) {
					auxVolumeCount -= 0.01;
					volumeIntermediate = exp(auxVolumeCount - 5);
					al_set_sample_instance_gain(songsList[idMusic].song, volumeIntermediate);

				}
				else {
					if (volumeIntermediate < 0.5) {
						auxVolumeCount += 0.01;
						volumeIntermediate = exp(auxVolumeCount - 5);
						al_set_sample_instance_gain(songsList[idMusic].song, volumeIntermediate);
					}
					else {
						volumeIntermediate = 0.5;
					}
				}
			}
			//cout << volumeIntermediate << endl;
		}



		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			cleanAllInGame();
			return 0;
		}

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (Evento.keyboard.keycode == ALLEGRO_KEY_UP and !alreadyHold ) {
				auto actual = figures.begin();
				if (!figures.empty() and (*actual)->blocks.size() != 0 ) {
					if (!holdFigure) {
						figures.erase(actual);
						holdFigure = (*actual);
					}
					else {
						auto aux = holdFigure;
						figures.erase(actual);
						holdFigure = (*actual);
						aux->funcMovTo(6 * 32, 1 * 32);
						aux->StaticTime = 0;
						aux->auxStatic = false;
						figures.push_back(aux);
					}
					holdFigure->funcMovTo(32 * 28, 32 * 10);
					alreadyHold = true;
				}
			}
		}

		al_clear_to_color(al_map_rgb(64, 4, 38));

		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W, proportion_H);
		al_use_transform(&tr);

		al_draw_bitmap(bg_sprite, 32*2, 32*2, 0);

		auxTaked = false;
		for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
			(*it)->Display();

			Takeaobj(*it, event_queue, Evento, &BONUSPOINTS, blockTaked, proportion_W, proportion_H);
			if ((*it)->taked) {
				blockTaked = true;
				auxTaked = true;
			}
		}	

		if (!auxTaked) {
			BONUSPOINTS += checkEachFile();
			blockTaked = false;
		}

		 
		if (!figures.empty()) {
			for (list<TetrisFigures*>::iterator it = figures.begin(); it != figures.end();) {
				if (!(*it)->Empty()) {
					(*it)->update(event_queue, Evento, fallingTimer, secsTimer);
					(*it)->Display();
					it++;
				}
				else {
					(*it)->DestroyTetrisFigures();
					it = figures.erase(it);
				}
			}
		}
		else {
			BONUSPOINTS += checkEachFile();
			genereteFig();
			alreadyHold = false;
		}
		

		for (auto it = queue_figures.begin(); it != queue_figures.end(); it++)
			(*it)->Display();

		if (holdFigure)
			holdFigure->Display();

		if (checkGameOver()) {
			cleanAllInGame();
			return BONUSPOINTS;
		}

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				/*Errors with memory
				DoALL(CollisionBlocks, [](const void* aux) {
					const Collider* collider = static_cast<const Collider*>(aux);
					collider->DestroyCollider();
					});
				*/
				cleanAllInGame();
				ButtonPressed = true;
				return BONUSPOINTS;
			}
		}

		displayInfoInGame(BONUSPOINTS);

		//displayGridAndCoordinates();
		//displayAllColliders();


		al_flip_display();
	}
}

void cleanAllInGame() {
	for (list<TetrisFigures*>::iterator it = figures.begin(); it != figures.end();) {
		(*it)->DestroyTetrisFigures();
		it = figures.erase(it);
	}

	for (list<TetrisBlocks*>::iterator it2 = Unique_blocks.begin(); it2 != Unique_blocks.end();) {
		(*it2)->DestroyTetrisBlocks();
		it2 = Unique_blocks.erase(it2);
	}
	for (list<Collider*>::iterator it = CollisionBlocks.begin(); it != CollisionBlocks.end();) {
		(*it)->DestroyCollider();
		it = CollisionBlocks.erase(it);
	}

	for (auto it = queue_figures.begin(); it != queue_figures.end();) {
		(*it)->DestroyTetrisFigures();
		it = queue_figures.erase(it);
	}

	if (holdFigure) {
		holdFigure->DestroyTetrisFigures();
		holdFigure = NULL;
	}
}



void displayInfoInGame(int BonusPoints) {



	al_identity_transform(&tr);
	al_scale_transform(&tr, proportion_W*0.7, proportion_H*0.7);
	al_use_transform(&tr);

	drawText(32 * 21, 32 * 2, "Move 1 block for 1000");
	drawText(32 * 21, 32 * 4, "Score: " + to_string(BonusPoints));
	drawText(32 * 21, 32 * 6, "Level: " + to_string(velocityLevel));
	if (velocityLevel <= 10)
		drawText(32 * 21, 32 * 8, "Next level in: " + to_string(auxMinCount));
	else
		drawText(32 * 21, 32 * 8, "Final level");
	drawText(32 * 21, 32 * 10, "Next figures ");
	al_draw_line(32 * 37, 32 * 11, 32 * 37, 32 * 37, al_map_rgb(255, 255, 255), 1.0f);
	drawText(32 * 41, 32 * 10, "Hold ");

	al_identity_transform(&tr);
	al_scale_transform(&tr, proportion_W, proportion_H);
	al_use_transform(&tr);
}

void displayGridAndCoordinates() {
	int block_pixels = 32;
	for (int i = 0; i <= widthORG / block_pixels; i++)
		al_draw_line(i * block_pixels, 0, i * block_pixels, heightORG, al_map_rgb(255, 255, 255), 1.0f);

	for (int j = 0; j <= heightORG / block_pixels; j++)
		al_draw_line(0, j * block_pixels, widthORG, j * block_pixels, al_map_rgb(255, 255, 255), 1.0f);


	al_identity_transform(&tr);
	al_scale_transform(&tr, proportion_W * 0.2, proportion_H * 0.2);
	al_use_transform(&tr);

	for (int i = 0; i <= widthORG / block_pixels; i++) {
		for (int j = 0; j <= heightORG / block_pixels; j++) {
			drawText(i * block_pixels * 5, (j * block_pixels * 5) + 20, to_string(i * block_pixels));
			drawText(i * block_pixels * 5, (j * block_pixels * 5) + 80, to_string(j * block_pixels));
		}
	}

	al_identity_transform(&tr);
	al_scale_transform(&tr, proportion_W, proportion_H);
	al_use_transform(&tr);
}


void displayAllColliders() {
	
	// ******************** MOSTRAR COLLIDERS ********************

	for (list<TetrisFigures*>::iterator it = figures.begin(); it != figures.end(); it++) {
		for (list<TetrisBlocks*>::iterator it2 = (*it)->blocks.begin(); it2 != (*it)->blocks.end(); it2++) {
			(*it2)->ThisCollider->DisplayFigureCollision();
			//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it2)->ThisCollider->posX + 5, (*it2)->ThisCollider->posY + 5, NULL, ("(" + to_string((*it2)->ThisCollider->posX) + "," + to_string((*it2)->ThisCollider->posY) + ")").c_str());
			//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it2)->x + 5, (*it2)->y + 25, NULL, ("(" + to_string((*it2)->x) + "," + to_string((*it2)->y) + ")").c_str());
		}
	}

	for (list<TetrisBlocks*>::iterator it = Unique_blocks.begin(); it != Unique_blocks.end(); it++) {
		(*it)->ThisCollider->DisplayFigureCollision();
	}


	for (list<Collider*>::iterator it = CollisionBlocks.begin(); it != CollisionBlocks.end(); it++) {
		(*it)->DisplayFigureCollision();
		//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it)->posX + 5, (*it)->posY + 5, NULL, ("(" + to_string((*it)->posX) + "," + to_string((*it)->posY) + ")").c_str());
		//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it)->posX + 5, (*it)->posY + 25, NULL, ("(" + to_string((*it)->posX+ (*it)->dimX) + "," + to_string((*it)->posY + (*it)->dimY) + ")").c_str());

	}

	// ******************** ********************* ********************
	
}

void drawText(int x, int y, string text) {
	al_draw_text(globalFont, colorblanco, x, y, NULL, text.c_str() );
}



void genereteFig() {

	auto it = queue_figures.begin();

	(*it)->funcMovTo(6 * 32, 1 * 32);


	figures.push_back(*it);
	queue_figures.erase(it);

	int posYAux = 32 * 9;

	for (auto it = queue_figures.begin(); it != queue_figures.end(); it++) {
		(*it)->funcMovTo(32 * 18, posYAux);
		posYAux += 32 * 6;
	}

	queue_figures.push_back(new TetrisFigures(32 * 18, 32 * 22, rand() % cantFig, rand() % 5, colorBlocks[rand() % 4], block_ghost));

}


void initialize() {
	srand(time(NULL));

	al_install_audio();
	al_init_acodec_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();

	al_reserve_samples(2);

	menu_song = al_load_sample("assets/music/neon-gaming.mp3");
	game_song0 = al_load_sample("assets/music/kim-lightyear-legends.mp3");
	game_song1 = al_load_sample("assets/music/machiavellian-nightmare.mp3");
	game_song2 = al_load_sample("assets/music/rasputin-russia-tetris.mp3");

	menu_song_Instance = al_create_sample_instance(menu_song);
	game_song0_Instance = al_create_sample_instance(game_song0);
	game_song1_Instance = al_create_sample_instance(game_song1);
	game_song2_Instance = al_create_sample_instance(game_song2);

	al_set_window_title(window, "TetrisChafa");


	if (widtht_W == 1920 && height_W == 1080) {

		window = al_create_display(widthORG, heightORG);
		isFullScreen = false;
		proportion_W = 1.0;
		proportion_H = 1.0;
		al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, false);
		al_set_display_flag(window, ALLEGRO_WINDOWED, true);
		al_set_window_position(window, widtht_W / 2 - widthORG / 2, (height_W / 2 - heightORG / 2));


	}
	else {

			window = al_create_display(widtht_W, height_W);
			isFullScreen = true;
			proportion_W = float(widtht_W) / float(widthORG);
			proportion_H = float(height_W) / float(heightORG);
			al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, true);
			al_set_display_flag(window, ALLEGRO_WINDOWED, false);
	}


	//al_set_window_position(window, widtht_W / 2 - widthORG / 2, (height_W / 2 - heighORG / 2));


	/*
		ALLEGRO_MONITOR_INFO info;
		int i = 0;
		do {
			al_get_monitor_info(i++, &info);
		} while (!(info.x1 == 0 && info.y1 == 0));
	*/

	secsTimer = al_create_timer(1.0);
	fallingTimer = al_create_timer(1.0);
	FPS = al_create_timer(1.0 / 60);
	event_queue = al_create_event_queue();

	//EVENT REGISTER
	al_register_event_source(event_queue, al_get_timer_event_source(secsTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(fallingTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(FPS));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(window));

	//START TIMERS
	al_start_timer(secsTimer);
	al_start_timer(fallingTimer);
	al_start_timer(FPS);

	//MUSIC MIXER
	al_set_sample_instance_playmode(menu_song_Instance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(game_song0_Instance, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(game_song1_Instance, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(game_song2_Instance, ALLEGRO_PLAYMODE_ONCE);

	al_attach_sample_instance_to_mixer(menu_song_Instance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(game_song0_Instance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(game_song1_Instance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(game_song2_Instance, al_get_default_mixer());


	al_set_sample_instance_gain(menu_song_Instance, 0.5);
	al_set_sample_instance_gain(game_song0_Instance, 0.0);
	al_set_sample_instance_gain(game_song1_Instance, 0.0);
	al_set_sample_instance_gain(game_song2_Instance, 0.0);

	songsList[0].song = game_song0_Instance;
	songsList[0].duration = 195;
	songsList[1].song = game_song1_Instance;
	songsList[1].duration = 349;
	songsList[2].song = game_song2_Instance;
	songsList[2].duration = 260;

	//FONTS 
	globalFont = al_load_font("assets/fonts/MinecraftTen-VGORe.ttf", 70, 0);

	//BITMAPS
	menu_sprite = al_load_bitmap("assets/img/backgrounds/menu.png");
	bg_sprite = al_load_bitmap("assets/img/backgrounds/bg.png");
	iconGame = al_load_bitmap("assets/img/icon/icon.png");

	block_green = al_load_bitmap("assets/img/blocks/green.png");
	block_blue = al_load_bitmap("assets/img/blocks/blue.png");
	block_red = al_load_bitmap("assets/img/blocks/red.png");
	block_black = al_load_bitmap("assets/img/blocks/black.png");
	block_white1 = al_load_bitmap("assets/img/blocks/white1.png");
	block_white2 = al_load_bitmap("assets/img/blocks/white2.png");
	block_ghost = al_load_bitmap("assets/img/blocks/ghost.png");

	colorBlocks[0] = block_green;
	colorBlocks[1] = block_blue;
	colorBlocks[2] = block_black;
	colorBlocks[3] = block_red;

	button_exit_t = al_load_bitmap("assets/img/buttons/exit_t.png");
	button_exit_f = al_load_bitmap("assets/img/buttons/exit_f.png");
	button_play_t = al_load_bitmap("assets/img/buttons/play_t.png");
	button_play_f = al_load_bitmap("assets/img/buttons/play_f.png");
	button_controls_t = al_load_bitmap("assets/img/buttons/controls_t.png");
	button_controls_f = al_load_bitmap("assets/img/buttons/controls_f.png");
	button_toFull_t = al_load_bitmap("assets/img/buttons/toFull_t.png");
	button_toFull_f = al_load_bitmap("assets/img/buttons/toFull_f.png");
	button_toWindow_t = al_load_bitmap("assets/img/buttons/toWindow_t.png");
	button_toWindow_f = al_load_bitmap("assets/img/buttons/toWindow_f.png");

	key_up = al_load_bitmap("assets/img/controls/up.png");
	key_down = al_load_bitmap("assets/img/controls/down.png");
	key_left = al_load_bitmap("assets/img/controls/left.png");
	key_right = al_load_bitmap("assets/img/controls/right.png");
	key_space = al_load_bitmap("assets/img/controls/space.png");
	key_z = al_load_bitmap("assets/img/controls/z.png");
	key_x = al_load_bitmap("assets/img/controls/x.png");
	key_leftClick = al_load_bitmap("assets/img/controls/click_left.png");
	key_esc = al_load_bitmap("assets/img/controls/esc.png");

	al_set_display_icon(window, iconGame);
}

void endProgram() {


	al_destroy_font(globalFont);

	al_destroy_timer(secsTimer);
	al_destroy_timer(FPS);
	al_destroy_timer(fallingTimer);

	al_destroy_event_queue(event_queue);

	al_destroy_sample(menu_song);
	al_destroy_sample(game_song0);
	al_destroy_sample(game_song1);
	al_destroy_sample(game_song2);

	al_destroy_sample_instance(menu_song_Instance);
	al_destroy_sample_instance(game_song0_Instance);
	al_destroy_sample_instance(game_song1_Instance);
	al_destroy_sample_instance(game_song2_Instance);

	al_destroy_bitmap(bg_sprite);
	al_destroy_bitmap(menu_sprite);
	al_destroy_bitmap(iconGame);

	al_destroy_bitmap(block_green);
	al_destroy_bitmap(block_blue);
	al_destroy_bitmap(block_red);
	al_destroy_bitmap(block_black);
	al_destroy_bitmap(block_white1);
	al_destroy_bitmap(block_white2);
	al_destroy_bitmap(block_ghost);

	al_destroy_bitmap(button_exit_t);
	al_destroy_bitmap(button_exit_f);
	al_destroy_bitmap(button_play_t);
	al_destroy_bitmap(button_play_f);
	al_destroy_bitmap(button_controls_t);
	al_destroy_bitmap(button_controls_f);
	al_destroy_bitmap(button_toFull_t);
	al_destroy_bitmap(button_toFull_f);
	al_destroy_bitmap(button_toWindow_t);
	al_destroy_bitmap(button_toWindow_f);

	al_destroy_bitmap(key_up);
	al_destroy_bitmap(key_down);
	al_destroy_bitmap(key_left);
	al_destroy_bitmap(key_right);
	al_destroy_bitmap(key_space);
	al_destroy_bitmap(key_z);
	al_destroy_bitmap(key_x);
	al_destroy_bitmap(key_leftClick);
	al_destroy_bitmap(key_esc);


	al_destroy_display(window);
}


void timerfunc() {
	cout << "Minutos: " << segundo / 60 << ", " << "segundos: " << segundo % 60 << endl;
}