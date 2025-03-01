
#pragma region Includes

#include <string>
#include <Windows.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")

#include <cstdio>

#include <stdio.h>
#include <shlobj.h>
#include <objbase.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "Crypt32.lib")

using namespace std;

#include "Button.cpp"
#include "ButtonText.cpp"
#include "ButtonSlider.cpp"
#include "TetrisFigure.cpp"
#include "Probabilitys.h"
#pragma endregion

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


#pragma region GlobalVars
//ALLEGRO VARS
ALLEGRO_FONT* globalFont;
ALLEGRO_TIMER* FPS;
ALLEGRO_TIMER* secsTimer;
ALLEGRO_TIMER* fallingTimer;
ALLEGRO_TIMER* keysTimer;
ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_DISPLAY* window;
ALLEGRO_EVENT Evento;
ALLEGRO_TRANSFORM tr;
ALLEGRO_SAMPLE* sample;
ALLEGRO_SAMPLE_INSTANCE* instance;
ALLEGRO_BITMAP* menu_sprite;
ALLEGRO_BITMAP* menu1080_sprite;
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
ALLEGRO_BITMAP* button_toFull_t;
ALLEGRO_BITMAP* button_toFull_f;
ALLEGRO_BITMAP* button_toWindow_t;
ALLEGRO_BITMAP* button_toWindow_f;

ALLEGRO_BITMAP* sliderContainer;
ALLEGRO_BITMAP* slider;

ALLEGRO_BITMAP* button_p_exit_f;
ALLEGRO_BITMAP* button_p_exit_t;
ALLEGRO_BITMAP* button_p_continue_t;
ALLEGRO_BITMAP* button_p_continue_f;

ALLEGRO_BITMAP* Container_container;
ALLEGRO_BITMAP* Container_button1;

ALLEGRO_BITMAP* cursor_sprite;

ALLEGRO_BITMAP* key_up;
ALLEGRO_BITMAP* key_down;
ALLEGRO_BITMAP* key_left;
ALLEGRO_BITMAP* key_right;
ALLEGRO_BITMAP* key_space;
ALLEGRO_BITMAP* key_z;
ALLEGRO_BITMAP* key_x;
ALLEGRO_BITMAP* key_shift;
ALLEGRO_BITMAP* key_leftClick;
ALLEGRO_BITMAP* key_esc;

ALLEGRO_BITMAP* logo;


ALLEGRO_BITMAP* colorBlocks[4];

ALLEGRO_MOUSE_CURSOR* cursor;

vector<string> songsList;

int MusicDuration;
int idMusic = 0;
float volumeIntermediate = 0.0;

//WINDOW VARS
int widthORG = 32 * 36;
int heightORG = 32 * 29;
int widtht_W;
int height_W;
bool isFullScreen;

float proportion_W;
float proportion_H;
float relative_pos;

//OTHER VARS
int segundo = 1000;
int countFPS = 0;
bool ButtonPressed = false;
bool blockTaked = false;
int auxMinCount = 60;
int velocityLevel = 1;
bool alreadyHold = false;

int counterTake = 120;
int TakeOriginalPosX = 0;
int TakeOriginalPosY = 0;
bool AuxNoPauseText = false;

float masterVolume = 0.5f;

TetrisFigure* holdFigure = NULL;
ProbabilisticGenerator* generator = NULL;

list<TetrisFigure*> *figure_list = new list<TetrisFigure*>;
list<TetrisBlock*> *Unique_blocks = new list<TetrisBlock*>;
list<Collider*> *CollisionBlocks = new list<Collider*>;
list<TetrisFigure*> *queue_figure_list = new list<TetrisFigure*>;

#pragma endregion

#pragma region GlobalMethods
//Main Methods
int menu_game();
int game_in();
void menu_controls_in();
void menu_credits_in();
void initialize();
void endProgram();

//Other Methods
void displayInfoInGame(int);
void displayGridAndCoordinates();
void cleanAllInGame();
void displayAllColliders();
void drawText(int, int, string, float = 1.0);
void genereteFig();
int checkEachFile(ALLEGRO_BITMAP* sprite1, ALLEGRO_BITMAP* sprite2);
float RandomFunc(int limit_A, int limit_B);
void Takeaobj(TetrisBlock * obj, ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT Evento, bool globalTaked, float proportion_W, float proportion_H);
bool checkGameOver();
void DrawTakeObj();
void decryptData(const string& encryptedText,string& plainText);
void loadData(int* maxScore, float* masterVolume);
void encryptData(const string& plainText, string& encryptedText);
void saveData(int* maxScore, float* masterVolume);
void playMusic(int id, float volume, ALLEGRO_PLAYMODE mode, int* duration = nullptr);

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

	loadData(&MaxPoints, &masterVolume);

	playMusic(0, masterVolume, ALLEGRO_PLAYMODE_LOOP);
	al_set_timer_speed(fallingTimer, 0.2);


	Button toFullButton(relative_pos + 32 * 34, 32 * 1 - 16, 50, 50, button_toFull_f, button_toFull_t);
	Button toWindowButton(relative_pos + 32 * 34, 32 * 1 - 16, 50, 50, button_toWindow_f, button_toWindow_t);

	ButtonText playButton(relative_pos + 450, 250, Container_button1, "Play", globalFont, window);
	ButtonText controlsButton(relative_pos + 375, 400, Container_button1, "Controls", globalFont, window);
	ButtonText creditsButton(relative_pos + 395, 550, Container_button1, "Credits", globalFont, window);
	ButtonText exitButton(relative_pos + 450, 700, Container_button1, "Exit", globalFont, window);

	ButtonSlider volumeButtonSlider(sliderContainer, slider, 32 * 1, 32 * 25 - 16, 200, 25, 30, 50, masterVolume);


	while (true) {

		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_wait_for_event(event_queue, &Evento);

		al_identity_transform(&tr);
		al_scale_transform(&tr, proportion_W, proportion_H);
		al_use_transform(&tr);

		if (isFullScreen)
			al_draw_bitmap(menu1080_sprite, 0, 0, NULL);
		else
			al_draw_bitmap(menu_sprite, relative_pos, 0, NULL);


		//al_draw_line(0, 500, relative_pos, 500, colorblanco, 1.0);
		//al_draw_line(relative_pos + widthORG, 500, widtht_W, 500, colorblanco, 1.0);

		//Animation Menu
		for (auto f : *figure_list)
			f->Display();

		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == secsTimer) {
				int xPosAux = (rand() % 32) * 32;

				//oldGenerator -> rand() % cantFig
				figure_list->push_back(new TetrisFigure(relative_pos + xPosAux, 32 * -5, rand() % cantFig, rand() % 5, colorBlocks[rand() % 4], block_ghost));

			}
			if (Evento.timer.source == FPS) {

				for (auto it = figure_list->begin(); it != figure_list->end();) {
					if ((*it)->y <= 32 * 35) {
						(*it)->funcMov(0, -4);
						it++;
					}
					else {
						delete* it;
						it = figure_list->erase(it);
					}
				}

			}
			if (Evento.timer.source == secsTimer) {
				for (auto it = figure_list->begin(); it != figure_list->end();) {
					if ((*it)->y <= 32 * 35) {
						int auxRot = rand() % 4;
						if (auxRot == 0)
							(*it)->rotate(true, Unique_blocks, CollisionBlocks);
						else if (auxRot == 1)
							(*it)->rotate(false, Unique_blocks, CollisionBlocks);
						it++;
					}
					else {
						delete* it;
						it = figure_list->erase(it);
					}
				}

			}
		}


		drawText(32 * 8, 32 * 1, "TetrisChafa", 1.8);
		drawText(32 * 1, 32 * 28, "v1.28", 0.4);
		drawText(32 * 12, 32 * 4, "High score: " + to_string(MaxPoints), 0.7);

		drawText(32 * 2, 32 * 23, "Volume ", 0.6);


		al_draw_bitmap(logo, relative_pos + 32 * 32, 32 * 26 - 16, NULL);


		playButton.Display(event_queue, Evento, proportion_W, proportion_H, -10);
		controlsButton.Display(event_queue, Evento, proportion_W, proportion_H);
		creditsButton.Display(event_queue, Evento, proportion_W, proportion_H);
		exitButton.Display(event_queue, Evento, proportion_W, proportion_H);


		playButton.updatePosition(relative_pos + 450, 250);
		controlsButton.updatePosition(relative_pos + 375, 400);
		creditsButton.updatePosition(relative_pos + 395, 550);
		exitButton.updatePosition(relative_pos + 450, 700);



		volumeButtonSlider.Draw(relative_pos);
		volumeButtonSlider.Update(Evento, proportion_W, proportion_H, relative_pos, &masterVolume);
		if (instance) al_set_sample_instance_gain(instance, masterVolume);


		if (widtht_W >= 1920 && height_W >= 1080) {
			if (isFullScreen) {
				toWindowButton.Display(event_queue, Evento, proportion_W, proportion_H);
				toWindowButton.updatePosition(relative_pos + 32 * 34, 32 * 1 - 16);
				if (toWindowButton.Pressed() && WaitToChangeScreen <= 1) {
					isFullScreen = !isFullScreen;
					proportion_W = 1.0;
					proportion_H = 1.0;
					relative_pos = 0;

					al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, false);
					al_set_display_flag(window, ALLEGRO_WINDOWED, true);
					al_resize_display(window, widthORG, heightORG);
					al_set_window_position(window, widtht_W / 2 - widthORG / 2, (height_W / 2 - heightORG / 2));

					WaitToChangeScreen = 30;
				}
			}
			else {
				toFullButton.Display(event_queue, Evento, proportion_W, proportion_H);
				toFullButton.updatePosition(relative_pos + 32 * 34, 32 * 1 - 16);

				if (toFullButton.Pressed() && WaitToChangeScreen <= 1) {
					isFullScreen = !isFullScreen;

					proportion_W = float(height_W) / float(heightORG);
					proportion_H = float(height_W) / float(heightORG);

					float auxProportion_W = float(widthORG) / float(widtht_W);
					relative_pos = ((widtht_W - widthORG) * auxProportion_W) / 2;

					relative_pos = int(relative_pos) - int(relative_pos) % 32;


					al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, true);
					al_set_display_flag(window, ALLEGRO_WINDOWED, false);
					al_resize_display(window, widtht_W, height_W);


					WaitToChangeScreen = 30;

				}
			}

			if (WaitToChangeScreen > 0) WaitToChangeScreen--;
		}


		if (exitButton.Pressed()){
			saveData(&MaxPoints, &masterVolume);
			return 0;
		}

		if (playButton.Pressed()) {

			for (auto it = figure_list->begin(); it != figure_list->end();) {
				delete* it;
				it = figure_list->erase(it);
			}


			int tmpPoints = game_in();
			volumeButtonSlider.SetValue(masterVolume);
			playMusic(0, masterVolume, ALLEGRO_PLAYMODE_LOOP);

			al_set_timer_speed(fallingTimer, 0.2);

			if (MaxPoints < tmpPoints) {
				MaxPoints = tmpPoints;
				saveData(&MaxPoints, &masterVolume);
			};
		}

		if (controlsButton.Pressed())
			menu_controls_in();

		if (creditsButton.Pressed())
			menu_credits_in();

		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			saveData(&MaxPoints, &masterVolume);
			return 0;
		}

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !ButtonPressed) {
				saveData(&MaxPoints, &masterVolume);
				return 0;
			}
		}


		ButtonPressed = false;
		al_flip_display();
	}


}

void menu_controls_in() {

	while (true) {

		al_wait_for_event(event_queue, &Evento);

		al_clear_to_color(al_map_rgb(0, 0, 0));


		if (isFullScreen)
			al_draw_bitmap(menu1080_sprite, 0, 0, NULL);
		else
			al_draw_bitmap(menu_sprite, relative_pos, 0, NULL);


		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				ButtonPressed = true;
				break;
			}


		int relativeX_Img = 32 * 9;
		int relativeX_Text = 32 * 12;

		drawText(relativeX_Text-32*2, 32 * 0 + 16, "Controls", 1.8);

		al_draw_bitmap(key_right, relative_pos + relativeX_Img, 32 * 5, NULL);
		drawText(relativeX_Text, 32 * 5, "Move piece to rigth", 0.7);

		al_draw_bitmap(key_left, relative_pos + relativeX_Img, 32 * 7, NULL);
		drawText(relativeX_Text, 32 * 7, "Move piece to left", 0.7);

		al_draw_bitmap(key_down, relative_pos + relativeX_Img, 32 * 9, NULL);
		drawText(relativeX_Text, 32 * 9, "Fast fall", 0.7);

		al_draw_bitmap(key_up, relative_pos + relativeX_Img, 32 * 11, NULL);
		drawText(relativeX_Text, 32 * 11, "Hold current piece", 0.7);

		al_draw_bitmap(key_z, relative_pos + relativeX_Img, 32 * 13, NULL);
		drawText(relativeX_Text, 32 * 13, "Counterclockwise rotation", 0.7);

		al_draw_bitmap(key_x, relative_pos + relativeX_Img, 32 * 15, NULL);
		drawText(relativeX_Text, 32 * 15, "Clockwise rotation", 0.7);

		al_draw_bitmap(key_space, relative_pos + relativeX_Img, 32 * 17, NULL);
		drawText(relativeX_Text + 32*4, 32 * 17, "Instant fall", 0.7);

		al_draw_bitmap(key_shift, relative_pos + relativeX_Img, 32 * 19, NULL);
		drawText(relativeX_Text + 32*2, 32 * 19, "and", 0.7);

		al_draw_bitmap(key_leftClick, relative_pos + relativeX_Img + 32*8, 32 * 19, NULL);
		drawText(relativeX_Text + 32*8, 32 * 19, "Take a block", 0.7);


		al_draw_bitmap(key_esc, relative_pos + relativeX_Img, 32 * 23, NULL);
		drawText(relativeX_Text, 32 * 23, "Exit or Pause", 0.7);


		al_flip_display();

	}
}




void menu_credits_in() {

	while (true) {

		al_wait_for_event(event_queue, &Evento);

		al_clear_to_color(al_map_rgb(0, 0, 0));


		if (isFullScreen)
			al_draw_bitmap(menu1080_sprite, 0, 0, NULL);
		else
			al_draw_bitmap(menu_sprite, relative_pos, 0, NULL);


		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				ButtonPressed = true;
				break;
			}


		int relativeX_Img = 32 * 9;
		int relativeX_Text = 32 * 12;

		string autor1 = "Abstraction";
		string autor2 = "Kim Lightyear";
		string autor3 = "Melody Ayres-Griffiths";

		drawText(relativeX_Text - 32 * 7, 32 * 0 + 16, "Credits", 1.8);

		drawText(relativeX_Text - 32 * 7, 32 * 4, "Game music");

		drawText(relativeX_Text - 32 * 7, 32 * 6, "Ludum Dare 38 - Track 5 by " + autor1, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 7, "Kim Lightyear Legends by " + autor2, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 8, "Machiavellian nightmare by " + autor3, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 9, "Rasputin Russia Tetris by " + autor3, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 10, "Samurai showdown by " + autor3, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 11, "Running in place by " + autor3, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 12, "Cossack dance by " + autor3, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 13, "Misspent youth by " + autor3, 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 14, "Ghosts of the past by " + autor3, 0.5);


		drawText(relativeX_Text - 32 * 7, 32 * 15, "Game Art");

		drawText(relativeX_Text - 32 * 7, 32 * 17, "Menu controls - Godot input prompts by John Pennycook ", 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 18, "Menu background - PixelSpace by Deep-Fold", 0.5);

		drawText(relativeX_Text - 32 * 7, 32 * 19, "Blocks, buttons, cursor and block board by Francisco Rosa", 0.5);

		
		drawText(relativeX_Text - 32 * 7, 32 * 20, "Game Font");

		drawText(relativeX_Text - 32 * 7, 32 * 22, "ThaleahFat by Rick Hoppmann (Tiny Worlds)", 0.5);


		drawText(relativeX_Text - 32 * 7, 32 * 23, "Game Programming");

		drawText(relativeX_Text - 32 * 7, 32 * 25, "More than 2k lines in C++ by Francisco Rosa", 0.5);

		
		drawText(relativeX_Text - 32 * 7, 32 * 27, "Created by Francisco Rosa (Wand Games)", 0.5);



		al_flip_display();

	}
}



int game_in() {

	bool activePause = false;
	bool gameover = false;

	int BONUSPOINTS = 0;
	bool CollidersTetrisON = false;
	bool CollidersBackgroundON = false;
	bool auxTaked = false;
	bool auxTakedInProgress = false;
	float auxVolumeCount = 0;
	velocityLevel = 1;
	auxMinCount = 60;
	counterTake = 120;
	AuxNoPauseText = false;

	idMusic = (rand() % (songsList.size()-1)) + 1;

	volumeIntermediate = 0.0;

	playMusic(idMusic, 0, ALLEGRO_PLAYMODE_ONCE, &MusicDuration);

	al_set_timer_speed(fallingTimer, 1.0 - (velocityLevel - 1) * 0.15);


	//Initialize Objects in game

	Button exitButton((widthORG / 2 - 400 / 2) + 75, (heightORG / 2 - 400 / 2) + 150, 100, 100, button_p_exit_f, button_p_exit_t);
	Button continueButton((widthORG / 2 - 400 / 2) + 225, (heightORG / 2 - 400 / 2) + 200, 100, 100, button_p_continue_f, button_p_continue_t);

	ButtonSlider volumeButtonSlider(sliderContainer, slider, 32 * 1, 32 * 25 - 16, 200, 25, 30, 50, masterVolume);

	CollisionBlocks->push_back(new Collider(relative_pos + 32 * 0, 0, 32 * 3, 32 * 29, 0, true));
	CollisionBlocks->push_back(new Collider(relative_pos + 32 * 13, 0, 32 * 3, 32 * 29, 0, true));
	CollisionBlocks->push_back(new Collider(relative_pos + 32 * 3, 32 * 26, 32 * 10, 32 * 3, 0, true));
	//CollisionBlocks->push_back(new Collider(relative_pos + 32 * 11, 32 * 16, 32 * 1, 32 * 1, 0, true));  //Use TetrisBlocks on the table because it is not a limit


	for (int i = 0; i < 3; i++)
		queue_figure_list->push_front(new TetrisFigure(32 * 18, 32 * 12 + (i * 6 * 32), generator->generate(), rand() % 5, colorBlocks[rand() % 4], block_ghost));


	while (true) {

		al_wait_for_event(event_queue, &Evento);

		++countFPS %= 60;

		if (Evento.type == ALLEGRO_EVENT_TIMER) {
			if (Evento.timer.source == secsTimer) {

				if (!activePause and !gameover and !auxTaked and velocityLevel < 12) {
					if (auxMinCount < 1) {
						velocityLevel++;

						if (velocityLevel < 10) auxMinCount = 60;
						else auxMinCount = 120;

						if (velocityLevel <= 7) {
							al_set_timer_speed(fallingTimer, 1.0 - (velocityLevel - 1) * 0.15);
						}
						else if (velocityLevel >= 8) {
							al_set_timer_speed(fallingTimer, float(0.1 / float((velocityLevel-7) * 2.0)) );
						}

					}
					else auxMinCount--;
				}


				if (MusicDuration <= 0) {
					idMusic++;
					if (idMusic > songsList.size() ) idMusic = 1;
					playMusic(idMusic, 0, ALLEGRO_PLAYMODE_ONCE, &MusicDuration);

				}
				else {
					MusicDuration--;
				}


			}
			if (Evento.timer.source == FPS) {

				if (auxTaked and !activePause and !gameover) {
					counterTake--;

				}

				if (counterTake < 1) {
					counterTake = 120;
					auxTaked = false;
				}


				if (gameover) {
					if (MusicDuration <= 2) {
						auxVolumeCount -= 0.02;

					}
					else {
						if (volumeIntermediate < 0.05) {
							auxVolumeCount += 0.01;
						}
						if (volumeIntermediate > 0.05) {
							auxVolumeCount -= 0.01;
						}
						
					}
					volumeIntermediate = exp(auxVolumeCount - 5);


				}
				else {
					if (MusicDuration <= 2) {
						auxVolumeCount -= 0.02;
						volumeIntermediate = exp(auxVolumeCount - 5);

					}
					else {

						if (volumeIntermediate < 0.5) {
							auxVolumeCount += 0.01;
							volumeIntermediate = exp(auxVolumeCount - 5);
						}
						else {
							volumeIntermediate = 0.5;
						}
					}
				}
				
				if(instance) al_set_sample_instance_gain(instance, volumeIntermediate*masterVolume);

			}
			//cout << volumeIntermediate << endl;
		}
		

		if (Evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			cleanAllInGame();
			return 0;
		}

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (Evento.keyboard.keycode == ALLEGRO_KEY_UP and !alreadyHold and !activePause and !gameover) {
				auto actual = figure_list->begin();
				if (!figure_list->empty() and (*actual)->blocks.size() != 0 and !auxTaked) {
					if (!holdFigure) {
						figure_list->erase(actual);
						holdFigure = (*actual);
					}
					else {
						auto aux = holdFigure;
						figure_list->erase(actual);
						holdFigure = (*actual);
						aux->funcMovTo(relative_pos + 6 * 32, 1 * 32);
						aux->StaticTime = 0;
						aux->auxStatic = false;
						figure_list->push_back(aux);
					}
					holdFigure->funcMovTo(relative_pos + 32 * 28, 32 * 10);
					alreadyHold = true;
				}
			}
		}

		al_clear_to_color(al_map_rgb(64, 4, 38));


		al_draw_bitmap(bg_sprite, relative_pos + 32 * 2, 32 * 2, 0);

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			if ((Evento.keyboard.keycode == ALLEGRO_KEY_LSHIFT or Evento.keyboard.keycode == ALLEGRO_KEY_RSHIFT) and !auxTaked and !activePause and !gameover and BONUSPOINTS >= 400 * velocityLevel) {
				BONUSPOINTS -= 400 * velocityLevel;
				auxTaked = true;
			}
		}

		for (list<TetrisBlock*>::iterator it = Unique_blocks->begin(); it != Unique_blocks->end(); it++) {
			(*it)->draw();

			if (auxTaked and !activePause and !gameover)
				Takeaobj(*it, event_queue, Evento, blockTaked, proportion_W, proportion_H);

			if ((*it)->taked) {
				blockTaked = true;
			}
			

		}

		if (!auxTaked) {
			blockTaked = false;
		}


		if (!figure_list->empty()) {
			for (list<TetrisFigure*>::iterator it = figure_list->begin(); it != figure_list->end();) {
				if (!(*it)->Empty()) {
					if (!activePause and !gameover and !auxTaked) (*it)->update(event_queue, Evento, fallingTimer, secsTimer, keysTimer, Unique_blocks, CollisionBlocks);
					(*it)->Display();
					it++;
				}
				else {
					delete* it;
					it = figure_list->erase(it);
				}
			}
		}
		else {
			genereteFig();
			alreadyHold = false;
		}





		BONUSPOINTS += checkEachFile(block_white1, block_white2);

		if (BONUSPOINTS >= 999999) {
			BONUSPOINTS = 999999;
			gameover = true;
		}

		for (auto it = queue_figure_list->begin(); it != queue_figure_list->end(); it++)
			(*it)->Display();

		if (holdFigure)
			holdFigure->Display();

		if (checkGameOver()) {
			gameover = true;
		}

		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN)
			if (Evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				if (!auxTaked)
					activePause = !activePause;
				else
					AuxNoPauseText = true;
			}

		if (Evento.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT and !auxTaked)
			activePause = true;

		if(!activePause and !gameover)
			DrawTakeObj();

		if (120 > counterTake and counterTake > 0) {
			if (AuxNoPauseText)
				drawText(32 * 4, 32 * 5, "NO PAUSE :)", 0.8);

			drawText(32 * 5, 32 * 1, to_string(counterTake / 60) + "." + to_string(counterTake % 60), 2);
		}
		else
			AuxNoPauseText = false;
		


		displayInfoInGame(BONUSPOINTS);


		if (activePause and !gameover) {
			al_draw_filled_rectangle(0, 0, widtht_W, height_W, al_map_rgba(0, 0, 0, 150));
			int centerX = widthORG / 2;
			int centerY = heightORG / 2;
			al_draw_bitmap(Container_container, relative_pos + centerX - 400 / 2, centerY - 400 / 2, NULL);
			drawText(centerX - 125, centerY - 150, "Pause", 1.5);

			exitButton.Display(event_queue, Evento, proportion_W, proportion_H);
			continueButton.Display(event_queue, Evento, proportion_W, proportion_H);

			exitButton.updatePosition(relative_pos + (widthORG / 2 - 400 / 2) + 75, (heightORG / 2 - 400 / 2) +150);
			continueButton.updatePosition(relative_pos + (widthORG / 2 - 400 / 2) +225, (heightORG / 2 - 400 / 2) +200);
 
			volumeButtonSlider.Draw(relative_pos);
			volumeButtonSlider.Update(Evento, proportion_W, proportion_H, relative_pos, &masterVolume);
			
			drawText(32 * 2, 32 * 23, "Volume ", 0.6);

			if (exitButton.Pressed()) {
				cleanAllInGame();
				ButtonPressed = true;
				return BONUSPOINTS;
			}
			else if (continueButton.Pressed()) {
				activePause = !activePause;

			}
		}


		if (gameover) {
			al_draw_filled_rectangle(0, 0, widtht_W, height_W, al_map_rgba(0, 0, 0, 150));

			int centerX = widthORG / 2;
			int centerY = heightORG / 2;
			al_draw_bitmap(Container_container, relative_pos + centerX - 400 / 2, centerY - 400 / 2, NULL);

			if (BONUSPOINTS >= 999999) {
				drawText(centerX - 32*10, centerY - 32*10, "LIMIT SCORE :)", 2);
				drawText(centerX - 112 + 32, centerY - 150, "You", 1.6);
				drawText(centerX - 112 + 48, centerY - 75, "Win", 1.6);

			}
			else {
				drawText(centerX - 112, centerY - 150, "Game", 1.6);
				drawText(centerX - 112, centerY - 75, "Over", 1.6);
			}


			exitButton.Display(event_queue, Evento, proportion_W, proportion_H);
			exitButton.updatePosition(relative_pos + (widthORG / 2 - 100/2), (heightORG / 2 - 100/2) + 75);
			if (exitButton.Pressed()) {
				cleanAllInGame();
				ButtonPressed = true;
				return BONUSPOINTS;
			}
		}

		//displayGridAndCoordinates();
		//displayAllColliders();


		al_flip_display();
	}
}

void cleanAllInGame() {

	for (auto f : *figure_list)
		delete f;
	for (auto b : *Unique_blocks)
		delete b;
	for (auto b : *CollisionBlocks)
		delete b;
	for (auto b : *queue_figure_list)
		delete b;

	figure_list->clear();
	Unique_blocks->clear();
	CollisionBlocks->clear();
	queue_figure_list->clear();

	if (holdFigure) {
		delete holdFigure;
		holdFigure = NULL;
	}
}



void displayInfoInGame(int BonusPoints) {

	int relativeX = 32 * 15;

	drawText(relativeX, 32 * 2, "Move a block for " + to_string(400 * velocityLevel) , 0.7);
	drawText(relativeX, 32 * 3, "Score: " + to_string(BonusPoints), 0.7);
	drawText(relativeX, 32 * 4, "Level: " + to_string(velocityLevel), 0.7);
	if (velocityLevel <= 11)
		drawText(relativeX, 32 * 5, "Next level in: " + to_string(auxMinCount), 0.7);
	else
		drawText(relativeX, 32 * 5, "Final level", 0.7);
	drawText(relativeX + 32 * 1, 32 * 7, "Next figure ", 0.7);
	al_draw_line((relative_pos + relativeX + 32 * 11) , 32 * 7, (relative_pos + relativeX + 32 * 11), 32 * 27, al_map_rgb(255,255,255), 4.0f);
	drawText(relativeX + 32 * 14, 32 * 7, "Hold ", 0.7);

}

void displayGridAndCoordinates() {
	int block_pixels = 32;
	for (int i = 0; i <= widthORG / block_pixels; i++)
		al_draw_line(i * block_pixels, 0, i * block_pixels, heightORG, al_map_rgb(255, 255, 255), 1.0f);

	for (int j = 0; j <= heightORG / block_pixels; j++)
		al_draw_line(0, j * block_pixels, widthORG, j * block_pixels, al_map_rgb(255, 255, 255), 1.0f);



	for (int i = 0; i <= widthORG / block_pixels; i++) {
		for (int j = 0; j <= heightORG / block_pixels; j++) {
			drawText(i * block_pixels * 5, (j * block_pixels * 5) + 20, to_string(i * block_pixels), 0.2);
			drawText(i * block_pixels * 5, (j * block_pixels * 5) + 80, to_string(j * block_pixels), 0.2);
		}
	}

}


void displayAllColliders() {

	// ******************** MOSTRAR COLLIDERS ********************

	for (list<TetrisFigure*>::iterator it = figure_list->begin(); it != figure_list->end(); it++) {
		for (list<TetrisBlock*>::iterator it2 = (*it)->blocks.begin(); it2 != (*it)->blocks.end(); it2++) {
			(*it2)->DisplayFigureCollision();
			//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it2)->ThisCollider->posX + 5, (*it2)->ThisCollider->posY + 5, NULL, ("(" + to_string((*it2)->ThisCollider->posX) + "," + to_string((*it2)->ThisCollider->posY) + ")").c_str());
			//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it2)->x + 5, (*it2)->y + 25, NULL, ("(" + to_string((*it2)->x) + "," + to_string((*it2)->y) + ")").c_str());
		}
	}

	for (list<TetrisBlock*>::iterator it = Unique_blocks->begin(); it != Unique_blocks->end(); it++) {
		(*it)->DisplayFigureCollision();
	}


	for (list<Collider*>::iterator it = CollisionBlocks->begin(); it != CollisionBlocks->end(); it++) {
		(*it)->DisplayFigureCollision();
		//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it)->posX + 5, (*it)->posY + 5, NULL, ("(" + to_string((*it)->posX) + "," + to_string((*it)->posY) + ")").c_str());
		//al_draw_text(globalFont, al_map_rgb(255, 255, 255), (*it)->posX + 5, (*it)->posY + 25, NULL, ("(" + to_string((*it)->posX+ (*it)->dimX) + "," + to_string((*it)->posY + (*it)->dimY) + ")").c_str());

	}

	// ******************** ********************* ********************

}

void drawText(int x, int y, string text, float scale) {

	ALLEGRO_TRANSFORM originalTransform;
	al_copy_transform(&originalTransform, al_get_current_transform());


	al_identity_transform(&tr);
	al_scale_transform(&tr, proportion_W * scale, proportion_H * scale);
	al_use_transform(&tr);

	x /= scale;
	y /= scale;

	int auxRelative_pos = relative_pos / scale;

	al_draw_text(globalFont, colorblanco, auxRelative_pos + x, y, NULL, text.c_str());

	al_use_transform(&originalTransform);

}



void genereteFig() {

	auto it = queue_figure_list->begin();

	(*it)->funcMovTo(relative_pos + 6 * 32, 1 * 32);


	figure_list->push_back(*it);
	queue_figure_list->erase(it);

	int posYAux = 32 * 9;

	for (auto it = queue_figure_list->begin(); it != queue_figure_list->end(); it++) {
		(*it)->funcMovTo(relative_pos + 32 * 18, posYAux);
		posYAux += 32 * 6;
	}

	queue_figure_list->push_back(new TetrisFigure(relative_pos + 32 * 18, 32 * 22, generator->generate(), rand() % 5, colorBlocks[rand() % 4], block_ghost));

}



float RandomFunc(int limit_A, int limit_B) {
	return (limit_A + rand() % limit_B);
}


int checkEachFile(ALLEGRO_BITMAP* sprite1, ALLEGRO_BITMAP* sprite2) {
	int countBlockFile = 0;
	int countFiles = 0;
	float combo = 1;

	list<TetrisBlock*> lineBlocks_list;

	for (int i = 0 + 6; i < 20 + 6; i++) {
		for (auto it = Unique_blocks->begin(); it != Unique_blocks->end(); it++) {
			auto block = *it;

			if (block->get_y() == i * 32 and !block->taked) {
				countBlockFile++;
				lineBlocks_list.push_back(block);
			}

		}
		if (countBlockFile == 10) {
			bool MoveAbove = false;
			for (auto it = lineBlocks_list.begin(); it != lineBlocks_list.end(); it++) {
				auto blockToDelete = *it;

				blockToDelete->die = true;
				blockToDelete->deleting(sprite1, sprite2);
				if (blockToDelete->dieTime == 0) {
					Unique_blocks->remove(blockToDelete);
					delete blockToDelete;
					MoveAbove = true;

				}


			}
			if (MoveAbove) {
				for (auto blockToMove : *Unique_blocks) {
					if (blockToMove->get_y() < i * 32)
						blockToMove->update();
				}
				countFiles++;

			}

		}

		countBlockFile = 0;
		lineBlocks_list.clear();
	}


	for (int i = 0; i < countFiles - 1; i++)
		combo += 0.5;

	return countFiles * 200 * combo;
}

void DrawTakeObj() {
	ALLEGRO_MOUSE_STATE mouseState;
	al_get_mouse_state(&mouseState);

	int Mx = mouseState.x / proportion_W;
	int My = mouseState.y / proportion_H;

	if (Mx >= 32 * 3 + relative_pos && Mx <= 32 * 13 - 1 + relative_pos && My >= 32 * 6 && My <= 32 * 26 - 1) {
		float x = Mx - Mx%32;
		float y = My - My%32;

		al_draw_rectangle(
			x, y,
			x + 32, y + 32,
			colorblanco, 2);
	}

}



void Takeaobj(TetrisBlock* obj, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT Evento, bool globalTaked, float proportion_W, float proportion_H) {
	ALLEGRO_MOUSE_STATE mouseState;
	al_get_mouse_state(&mouseState);

	int Mx = mouseState.x / proportion_W;
	int My = mouseState.y / proportion_H;

	if (obj->taked == false and !obj->die and !globalTaked) {

		if (Mx >= obj->get_x() && Mx <= obj->get_x() + obj->sprite_w && My >= obj->get_y() && My <= obj->get_y() + obj->sprite_h) {
			if (Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

				if (Evento.mouse.button & 1) {
					obj->taked = true;
					TakeOriginalPosX = obj->get_x();
					TakeOriginalPosY = obj->get_y();
				}
			}
		}
	}
	else {
		if (obj->taked == true) {
			if (Mx >= 32 * 3 + relative_pos && Mx <= 32 * 13 - 1 + relative_pos  && My >= 32 * 6 && My <= 32 * 26 - 1) {
				bool isColling = false;
				obj->set_x(Mx - Mx % 32);
				obj->set_y(My - My % 32);

				if (counterTake <= 2) {
					obj->set_x(TakeOriginalPosX);
					obj->set_y(TakeOriginalPosY);
					obj->taked = false;
				}

				if (Evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
					if (Evento.mouse.button & 1) {
						for (auto fig : *figure_list)
							for (auto block : fig->blocks)
								if (obj->hitBlock(block))
									isColling = true;

						for (auto block : *Unique_blocks)
							if (obj->hitBlock(block) and block != obj)
								isColling = true;

						if (!isColling) {
							counterTake = 0;
							obj->taked = false;
						}
					}
				}

			}
		}
	}
}




bool checkGameOver() {
	for (auto block : *Unique_blocks)
		if (block->get_y() < 32 * 6)
			return true;
	return false;
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

	al_reserve_samples(1);

	songsList = {
		"Ludum Dare 38 - Track 5.wav",
		"kim-lightyear-legends.mp3",
		"machiavellian-nightmare.mp3",
		"rasputin-russia-tetris.mp3",
		"ghosts-of-the-past.mp3",
		"misspent-youth.mp3",
		"cossack-dance.mp3",
		"running-in-place.mp3",
		"samurai-showdown.mp3",
	};

	sample = nullptr;
	instance = nullptr;

	al_set_window_title(window, "TetrisChafa");

	widtht_W = GetSystemMetrics(SM_CXSCREEN);
	height_W = GetSystemMetrics(SM_CYSCREEN);



	
	if (widtht_W >= 1920 && height_W >= 1080) {
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
		proportion_W = float(height_W) / float(heightORG);
		proportion_H = float(height_W) / float(heightORG);
		al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, true);
		al_set_display_flag(window, ALLEGRO_WINDOWED, false);
	}
	



	/*
	window = al_create_display(widtht_W, height_W);
	isFullScreen = true;
	proportion_W = float(height_W) / float(heightORG);
	proportion_H = float(height_W) / float(heightORG);

	float auxProportion_W =  float(widthORG) / float(widtht_W);
	relative_pos = ((widtht_W - widthORG )* auxProportion_W) / 2;

	al_set_display_flag(window, ALLEGRO_FULLSCREEN_WINDOW, true);
	al_set_display_flag(window, ALLEGRO_WINDOWED, false);
	*/

	//al_set_window_position(window, widtht_W / 2 - widthORG / 2, (height_W / 2 - heighORG / 2));



	ALLEGRO_MONITOR_INFO info;
	int i = 0;
	do {
		al_get_monitor_info(i++, &info);
	} while (!(info.x1 == 0 && info.y1 == 0));


	//cout << "X: " << info.x1 << " " << info.x2 << endl;
	//cout << "Y: " << info.y1 << " " << info.y2 << endl;

	secsTimer = al_create_timer(1.0);
	keysTimer = al_create_timer(0.06);
	fallingTimer = al_create_timer(1.0);
	FPS = al_create_timer(1.0 / 60);
	event_queue = al_create_event_queue();

	//EVENT REGISTER
	al_register_event_source(event_queue, al_get_timer_event_source(secsTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(keysTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(fallingTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(FPS));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(window));

	//START TIMERS
	al_start_timer(secsTimer);
	al_start_timer(keysTimer);
	al_start_timer(fallingTimer);
	al_start_timer(FPS);


	//FONTS 
	globalFont = al_load_font("assets/fonts/ThaleahFat.ttf", 70, 0);

	//BITMAPS
	menu_sprite = al_load_bitmap("assets/img/backgrounds/menu.png");
	menu1080_sprite = al_load_bitmap("assets/img/backgrounds/menu1080.png");

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

	button_toFull_t = al_load_bitmap("assets/img/buttons/toFull_t.png");
	button_toFull_f = al_load_bitmap("assets/img/buttons/toFull_f.png");
	button_toWindow_t = al_load_bitmap("assets/img/buttons/toWindow_t.png");
	button_toWindow_f = al_load_bitmap("assets/img/buttons/toWindow_f.png");

	sliderContainer = al_load_bitmap("assets/img/buttons/sliderContainer.png");
	slider = al_load_bitmap("assets/img/buttons/slider.png");

	button_p_exit_f = al_load_bitmap("assets/img/buttons/p_exit_f.png");
	button_p_exit_t = al_load_bitmap("assets/img/buttons/p_exit_t.png");
	button_p_continue_t = al_load_bitmap("assets/img/buttons/p_continue_t.png");
	button_p_continue_f = al_load_bitmap("assets/img/buttons/p_continue_f.png");

	Container_container = al_load_bitmap("assets/img/buttons/container.png");

	Container_button1 = al_load_bitmap("assets/img/buttons/button1.png");

	cursor_sprite = al_load_bitmap("assets/img/cursors/cursor.png");


	key_up = al_load_bitmap("assets/img/controls/up.png");
	key_down = al_load_bitmap("assets/img/controls/down.png");
	key_left = al_load_bitmap("assets/img/controls/left.png");
	key_right = al_load_bitmap("assets/img/controls/right.png");
	key_space = al_load_bitmap("assets/img/controls/space.png");
	key_z = al_load_bitmap("assets/img/controls/z.png");
	key_x = al_load_bitmap("assets/img/controls/x.png");
	key_shift = al_load_bitmap("assets/img/controls/shift.png");
	key_leftClick = al_load_bitmap("assets/img/controls/click_left.png");
	key_esc = al_load_bitmap("assets/img/controls/esc.png");

	logo = al_load_bitmap("assets/img/logo/WandGames.png");

	al_set_display_icon(window, iconGame);

	cursor = al_create_mouse_cursor(cursor_sprite, 0, 0);

	//al_hide_mouse_cursor(window);
	al_set_mouse_cursor(window, cursor);



	list<double> figureProbabilityList;

	for (int i = 0; i < cantFig; i++)
		figureProbabilityList.push_back(figureModels[i].probability);

	generator = new ProbabilisticGenerator(figureProbabilityList);

}

void endProgram() {


	al_destroy_font(globalFont);

	al_destroy_timer(secsTimer);
	al_destroy_timer(FPS);
	al_destroy_timer(fallingTimer);
	al_destroy_timer(keysTimer);

	al_destroy_event_queue(event_queue);

	al_destroy_sample(sample);

	al_destroy_sample_instance(instance);


	al_destroy_bitmap(bg_sprite);
	al_destroy_bitmap(menu_sprite);
	al_destroy_bitmap(menu1080_sprite);
	al_destroy_bitmap(iconGame);

	al_destroy_bitmap(block_green);
	al_destroy_bitmap(block_blue);
	al_destroy_bitmap(block_red);
	al_destroy_bitmap(block_black);
	al_destroy_bitmap(block_white1);
	al_destroy_bitmap(block_white2);
	al_destroy_bitmap(block_ghost);

	al_destroy_bitmap(button_toFull_t);
	al_destroy_bitmap(button_toFull_f);
	al_destroy_bitmap(button_toWindow_t);
	al_destroy_bitmap(button_toWindow_f);

	al_destroy_bitmap(sliderContainer);
	al_destroy_bitmap(slider);

	al_destroy_bitmap(button_p_exit_f);
	al_destroy_bitmap(button_p_exit_t);
	al_destroy_bitmap(button_p_continue_t);
	al_destroy_bitmap(button_p_continue_f);

	al_destroy_bitmap(Container_container);

	al_destroy_bitmap(cursor_sprite);

	al_destroy_bitmap(key_up);
	al_destroy_bitmap(key_down);
	al_destroy_bitmap(key_left);
	al_destroy_bitmap(key_right);
	al_destroy_bitmap(key_space);
	al_destroy_bitmap(key_z);
	al_destroy_bitmap(key_x);
	al_destroy_bitmap(key_shift);
	al_destroy_bitmap(key_leftClick);
	al_destroy_bitmap(key_esc);

	al_destroy_bitmap(logo);

	al_destroy_mouse_cursor(cursor);


	al_destroy_display(window);
}


void playMusic(int id, float volume, ALLEGRO_PLAYMODE mode, int* duration) {

	if (instance) {
		al_destroy_sample_instance(instance);
		instance = nullptr;
	}
	if (sample) {
		al_destroy_sample(sample);
		sample = nullptr;
	}

	if (id < 0 || id >= songsList.size()) {
		if (duration) *duration = 0;
		return;
	}

	sample = al_load_sample(("assets/music/"+ songsList[id]).c_str());

	if (sample) {
		instance = al_create_sample_instance(sample);
		if (instance) {
			al_set_sample_instance_playmode(instance, mode);
			al_set_sample_instance_gain(instance, volume);
			al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());

			al_play_sample_instance(instance);

			if (duration) {
				unsigned long samples = al_get_sample_length(sample);
				unsigned int sample_rate = al_get_sample_frequency(sample);
				*duration = static_cast<int>(samples / (float)sample_rate);
			}
		}
		else {
			if (duration) *duration = 0;
		}
	}
	else {
		if (duration) *duration = 0;
	}

}


void decryptData(const string& encryptedText, string& plainText) {
	DATA_BLOB dataIn;
	DATA_BLOB dataOut;
	DATA_BLOB entropy;

	BYTE entropyBytes[] = { 0xA3, 0xB4, 0xC5, 0xD6, 0xE7, 0xF8 };
	entropy.pbData = entropyBytes;
	entropy.cbData = sizeof(entropyBytes);

	dataIn.pbData = (BYTE*)encryptedText.c_str();
	dataIn.cbData = (DWORD)encryptedText.size();

	if (CryptUnprotectData(&dataIn, NULL, &entropy, NULL, NULL, 0, &dataOut)) {
		plainText = std::string((char*)dataOut.pbData, dataOut.cbData);
		LocalFree(dataOut.pbData);
	}
}

void loadData(int* maxScore, float* masterVolume) {
	PWSTR path = NULL;
	HRESULT r;
	json j;
	r = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &path);
	if (path != NULL) {
		wstring savedGamesPath(path);
		CoTaskMemFree(path);
		wstring jsonFilePath = savedGamesPath + L"\\My Games\\TetrisChafa\\data.dat";
		const wchar_t* jsonPath = jsonFilePath.c_str();
		ifstream f(jsonPath, ios::binary);
		if (f.is_open()) {
			std::string encryptedText((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			f.close();

			std::string plainText;
			decryptData(encryptedText, plainText);

			try {
				j = json::parse(plainText);
				if (j.contains("MaxScore")) {
					*maxScore = j["MaxScore"];
				}
				else {
					*maxScore = 0;
				}
				if (j.contains("Volume")) {
					*masterVolume = j["Volume"];
				}
				else {
					*masterVolume = 0.5f;
				}
			}
			catch (json::parse_error& e) {
				*maxScore = 0;
				*masterVolume = 0.5f;
				_wremove(jsonPath);
			}
		}
		else {
			*maxScore = 0;
			*masterVolume = 0.5f;
		}
	}
}




void encryptData(const string& plainText, string& encryptedText) {
	DATA_BLOB dataIn;
	DATA_BLOB dataOut;
	DATA_BLOB entropy;

	BYTE entropyBytes[] = { 0xA3, 0xB4, 0xC5, 0xD6, 0xE7, 0xF8 };
	entropy.pbData = entropyBytes;
	entropy.cbData = sizeof(entropyBytes);

	dataIn.pbData = (BYTE*)plainText.c_str();
	dataIn.cbData = (DWORD)(plainText.size() + 1);

	if (CryptProtectData(&dataIn, NULL, &entropy, NULL, NULL, 0, &dataOut)) {
		encryptedText = std::string((char*)dataOut.pbData, dataOut.cbData);
		LocalFree(dataOut.pbData);
	}
}



void saveData(int* maxScore, float* masterVolume) {
	PWSTR path = NULL;
	HRESULT r;
	json j;
	r = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &path);
	if (path != NULL) {
		wstring savedGamesPath(path);
		CoTaskMemFree(path);
		wstring jsonFilePath = savedGamesPath + L"\\My Games\\TetrisChafa\\data.dat";
		const wchar_t* jsonPath = jsonFilePath.c_str();

		ifstream fin(jsonPath, ios::binary);
		if (fin.is_open()) {
			try {
				fin >> j;
			}
			catch (json::parse_error& e) {
				j = json{};
			}
			fin.close();
		}

		j["MaxScore"] = *maxScore;
		j["Volume"] = *masterVolume;

		std::string plainText = j.dump();
		std::string encryptedText;
		encryptData(plainText, encryptedText);

		ofstream fout(jsonPath, ios::binary | ios::trunc);
		if (fout.is_open()) {
			fout.write(encryptedText.c_str(), encryptedText.size());
			fout.close();
		}
	}
}
