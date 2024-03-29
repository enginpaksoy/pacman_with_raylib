﻿#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define SQUARE 40
#define COLOR1 (Color){ 3, 6, 45, 255 } 
#define COLOR2 (Color){ 50, 7, 73, 255 } 
#define COLOR3 (Color){ 104, 4, 75, 255 } 
#define COLOR4 (Color){ 135, 10, 93, 255 } 
#define COLOR5 (Color){ 106, 189, 200, 255 } 
#define COLOR6 (Color){ 0, 0, 0, 150 } 
#define COLOR7 (Color){ 0, 0, 0, 20 } 

typedef enum GameScreen { starting_screen, single_player, multi_player } GameScreen;
typedef enum crypto { bitcoin, ether, tether, doge } crypto;

typedef struct bait {
	bool active;
	Vector2 position;
	int radius;
	Color color;

	enum crypto coin;
	int crypto_num;
}bait;

typedef struct block {
	Rectangle rectangle;
	Color color;
}block;

typedef struct pacman {
	Vector2 position;
	int radius;
	Color color;
	int point;

	block rect_up;
	block rect_down;
	block rect_left;
	block rect_right;

	bool collision_up;
	bool collision_down;
	bool collision_right;
	bool collision_left;
}pacman;


static GameScreen current_screen = starting_screen;

//image loading 
static Image image[6];
static Texture2D texture[6];

static int x_counter = 0;
static pacman pacman_[2];
static bait food[2] = { 0 };
static block blocks[20][38] = { 0 };
static bool gameOver;

char button_single_text[] = "Single Player";
char button_multi_text[] = "Multi Player";
bool button_single_Clicked = false;
bool button_multi_Clicked = false;

float timerSinceMultiButtonPressed = 0.0f;
float timerSinceSingleButtonPressed = 0.0f;
float timerStopwatch = 0.0f;
float timerGame = 0.0f;

static int matrix[20][38] = { {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
							  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
							  {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
							  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1},
							  {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
							  {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
							  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
							  {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
							  {1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1},
							  {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
							  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
							  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1} };

static int framesCounter;

//screen variables
const int screenWidth = 1520;
const int screenHeight = 800;
//grid lines
static Vector2 offset = { 0 };

//functions

void InitGame();
void updateGame();
void drawGame();

void updateDrawFrame();
void pacman_position_collision(pacman pacman1);


void draw_first_pacman_game();
void draw_second_pacman_game();

void draw_food(bait food);

int main() {

	InitWindow(screenWidth, screenHeight, "PACMAN");
	SetTargetFPS(60);

	InitGame();
	while (!WindowShouldClose()) {
		updateDrawFrame();
	}
	CloseWindow();
	return 0;
}

void drawGame() {
	BeginDrawing();
	timerGame += GetFrameTime();

	ClearBackground(COLOR5);

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			DrawRectangle(blocks[i][j].rectangle.x, blocks[i][j].rectangle.y, blocks[i][j].rectangle.width, blocks[i][j].rectangle.height, blocks[i][j].color);
		}
	}

	//GRIDLINES
	for (int i = 0; i < screenWidth / SQUARE + 1; i++) {
		DrawLineV((Vector2) { SQUARE* i + offset.x, offset.y }, (Vector2) { SQUARE* i + offset.x, screenHeight - offset.y }, COLOR7);
	}
	for (int i = 0; i < screenWidth / SQUARE + 1; i++) {
		DrawLineV((Vector2) { offset.x, SQUARE* i + offset.y }, (Vector2) { screenWidth - offset.x, SQUARE* i + offset.y }, COLOR7);
	}
	switch (current_screen)
	{
	case starting_screen:
	{	
		DrawRectangle(0, 0, screenWidth, screenHeight, COLOR6);
		DrawText("Crypto PAC", 560, 80, 80, GOLD);
		if(!button_single_Clicked){
			if (GuiButton((Rectangle) { screenWidth / 2 - 60, screenHeight / 2 - 25, 120, 50 }, button_multi_text)) {
				button_multi_Clicked = !button_multi_Clicked;
				timerSinceMultiButtonPressed = 0.0f;  // Reset timer
				strcpy(button_multi_text, "MULTI SELECTED!");
			}

			if (button_multi_Clicked) {
				timerSinceMultiButtonPressed += GetFrameTime();
				if (timerSinceMultiButtonPressed >= 5.0f) {
					button_multi_Clicked = true;
					//strcpy(button_multi_text, "Click me!"); // Optionally reset button text
				}
			}

			if (button_multi_Clicked) {
				DrawText("Player1", 200, 240, 30, GOLD);
				DrawText("Player2", 1200, 240, 30, GOLD);
				DrawTexture(texture[4], 120, 320, WHITE);  // Draw the texture for 5 seconds
				DrawTexture(texture[5], 1120, 320, WHITE);  // Draw the texture for 5 seconds
			}
		}

		if(!button_multi_Clicked){
			if (GuiButton((Rectangle) { screenWidth / 2 - 60, screenHeight / 2 - 125, 120, 50 }, button_single_text)) {
				button_single_Clicked = !button_single_Clicked;
				timerSinceSingleButtonPressed = 0.0f;  // Reset timer
				strcpy(button_single_text, "SINGLE SELECTED!");
			}
			if (button_single_Clicked) {
				timerSinceSingleButtonPressed += GetFrameTime();
				if (timerSinceSingleButtonPressed >= 5.0f) {
					button_single_Clicked = true;
					//strcpy(button_multi_text, "Click me!"); // Optionally reset button text
				}
			}
			if (button_single_Clicked) {
				DrawText("Player1", 200, 240, 30, GOLD);
				DrawText("Ghost", 1200, 240, 30, GOLD);
				DrawTexture(texture[4], 120, 320, WHITE);  // Draw the texture for 5 seconds
				DrawTexture(texture[5], 120, 480, WHITE);  // Draw the texture for 5 seconds
			}
		}
		if (button_multi_Clicked || button_single_Clicked) {
			timerStopwatch += GetFrameTime();
		}
		if (button_multi_Clicked || button_single_Clicked) {
			DrawText(TextFormat("%.2f", 3 - timerStopwatch), 720, 640, 60, GOLD);
		}
		if (timerStopwatch > 3.0f) {
			if (button_single_Clicked) {
				current_screen = single_player;
			}
			else if (button_multi_Clicked) {
				current_screen = multi_player;
			}
		}
	} break;
	case single_player:
	{
		draw_first_pacman_game();
	} break;
	case multi_player:
	{
		draw_first_pacman_game();
		draw_second_pacman_game();
	} break;
	default: break;
	}

	framesCounter++;
	EndDrawing();
}


void updateGame() {

	switch (current_screen)
	{
	case starting_screen:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			current_screen = single_player;
		}
		else if (IsKeyPressed(KEY_ENTER)) {
			current_screen = multi_player;
		}
	} break;
	case single_player:
	{

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 38; j++) {
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_down.rectangle)) {
					pacman_[0].collision_down = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_up.rectangle)) {
					pacman_[0].collision_up = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_right.rectangle)) {
					pacman_[0].collision_right = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_left.rectangle)) {
					pacman_[0].collision_left = true;
				}
				if (CheckCollisionCircleRec(food[0].position, food[0].radius, blocks[i][j].rectangle)) {
					food[0].active = false;
				}
			}
		}

		pacman_[0].rect_down.rectangle.x = pacman_[0].position.x - SQUARE / 2 + 2.5f;
		pacman_[0].rect_down.rectangle.y = pacman_[0].position.y + SQUARE / 2 + 2.5f;
		pacman_[0].rect_up.rectangle.x = pacman_[0].position.x - SQUARE / 2 + 2.5f;
		pacman_[0].rect_up.rectangle.y = pacman_[0].position.y - 3 * SQUARE / 2 + 2.5f;
		pacman_[0].rect_right.rectangle.x = pacman_[0].position.x + SQUARE / 2 + 2.5f;
		pacman_[0].rect_right.rectangle.y = pacman_[0].position.y - SQUARE / 2 + 2.5f;
		pacman_[0].rect_left.rectangle.x = pacman_[0].position.x - 3 * SQUARE / 2 + 2.5f;
		pacman_[0].rect_left.rectangle.y = pacman_[0].position.y - SQUARE / 2 + 2.5;

		pacman_[0].rect_down.rectangle.height = SQUARE - 5;
		pacman_[0].rect_up.rectangle.height = SQUARE - 5;
		pacman_[0].rect_right.rectangle.height = SQUARE - 5;
		pacman_[0].rect_left.rectangle.height = SQUARE - 5;

		pacman_[0].rect_down.rectangle.width = SQUARE - 5;
		pacman_[0].rect_up.rectangle.width = SQUARE - 5;
		pacman_[0].rect_right.rectangle.width = SQUARE - 5;
		pacman_[0].rect_left.rectangle.width = SQUARE - 5;

		pacman_[0].rect_down.color = COLOR6;
		pacman_[0].rect_up.color = COLOR6;
		pacman_[0].rect_right.color = COLOR6;
		pacman_[0].rect_left.color = COLOR6;

		if (framesCounter % 5 == 0) {
			if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && (pacman_[0].position.x + SQUARE / 2 < screenWidth) && pacman_[0].collision_right == false) {
				pacman_[0].position.x += 40.0f;
			}
			else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && (pacman_[0].position.x - SQUARE / 2 > 0) && pacman_[0].collision_left == false) {
				pacman_[0].position.x -= 40.0f;
			}
			else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && (pacman_[0].position.y - SQUARE / 2 > 0) && pacman_[0].collision_up == false) {
				pacman_[0].position.y -= 40.0f;
			}
			else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && (pacman_[0].position.y + SQUARE / 2 < screenHeight) && pacman_[0].collision_down == false) {
				pacman_[0].position.y += 40.0f;
			}
		}

		if (CheckCollisionCircles(pacman_[0].position, pacman_[0].radius, food[0].position, food[0].radius)) {
			if (food[0].coin == bitcoin) {
				pacman_[0].point += 10;
			}
			else if (food[0].coin == ether) {
				pacman_[0].point += 5;
			}
			else if (food[0].coin == tether) {
				pacman_[0].point += 2;
			}
			else if (food[0].coin == doge) {
				if (GetRandomValue(0, 4) % 2 == 0) {
					pacman_[0].point += 8;
				}
				else {
					pacman_[0].point -= 8;
				}
			}
			food[0].active = false;
		}
		if (!food[0].active) {
			food[0].active = true;
			food[0].position = (Vector2){ GetRandomValue(0, screenWidth - 1), GetRandomValue(0, screenHeight - 1) };
			food[0].crypto_num = GetRandomValue(0, 3);
			food[0].coin = food[0].crypto_num;
		}

	} break;
	case multi_player:
	{
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 38; j++) {
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_down.rectangle)) {
					pacman_[0].collision_down = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_up.rectangle)) {
					pacman_[0].collision_up = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_right.rectangle)) {
					pacman_[0].collision_right = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[0].rect_left.rectangle)) {
					pacman_[0].collision_left = true;
				}
				if (CheckCollisionCircleRec(food[0].position, food[0].radius, blocks[i][j].rectangle)) {
					food[0].active = false;
				}
				if (CheckCollisionCircleRec(food[1].position, food[1].radius, blocks[i][j].rectangle)) {
					food[1].active = false;
				}
			}
		}

		pacman_[0].rect_down.rectangle.x = pacman_[0].position.x - SQUARE / 2 + 2.5f;
		pacman_[0].rect_down.rectangle.y = pacman_[0].position.y + SQUARE / 2 + 2.5f;
		pacman_[0].rect_up.rectangle.x = pacman_[0].position.x - SQUARE / 2 + 2.5f;
		pacman_[0].rect_up.rectangle.y = pacman_[0].position.y - 3 * SQUARE / 2 + 2.5f;
		pacman_[0].rect_right.rectangle.x = pacman_[0].position.x + SQUARE / 2 + 2.5f;
		pacman_[0].rect_right.rectangle.y = pacman_[0].position.y - SQUARE / 2 + 2.5f;
		pacman_[0].rect_left.rectangle.x = pacman_[0].position.x - 3 * SQUARE / 2 + 2.5f;
		pacman_[0].rect_left.rectangle.y = pacman_[0].position.y - SQUARE / 2 + 2.5;

		pacman_[0].rect_down.rectangle.height = SQUARE - 5;
		pacman_[0].rect_up.rectangle.height = SQUARE - 5;
		pacman_[0].rect_right.rectangle.height = SQUARE - 5;
		pacman_[0].rect_left.rectangle.height = SQUARE - 5;

		pacman_[0].rect_down.rectangle.width = SQUARE - 5;
		pacman_[0].rect_up.rectangle.width = SQUARE - 5;
		pacman_[0].rect_right.rectangle.width = SQUARE - 5;
		pacman_[0].rect_left.rectangle.width = SQUARE - 5;

		pacman_[0].rect_down.color = COLOR6;
		pacman_[0].rect_up.color = COLOR6;
		pacman_[0].rect_right.color = COLOR6;
		pacman_[0].rect_left.color = COLOR6;

		if (framesCounter % 5 == 0) {
			if (IsKeyDown(KEY_D) && (pacman_[0].position.x + SQUARE / 2 < screenWidth) && pacman_[0].collision_right == false) {
				pacman_[0].position.x += 40.0f;
			}
			else if (IsKeyDown(KEY_A) && (pacman_[0].position.x - SQUARE / 2 > 0) && pacman_[0].collision_left == false) {
				pacman_[0].position.x -= 40.0f;
			}
			else if (IsKeyDown(KEY_W) && (pacman_[0].position.y - SQUARE / 2 > 0) && pacman_[0].collision_up == false) {
				pacman_[0].position.y -= 40.0f;
			}
			else if (IsKeyDown(KEY_S) && (pacman_[0].position.y + SQUARE / 2 < screenHeight) && pacman_[0].collision_down == false) {
				pacman_[0].position.y += 40.0f;
			}
		}
		///////////////

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 38; j++) {
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[1].rect_down.rectangle)) {
					pacman_[1].collision_down = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[1].rect_up.rectangle)) {
					pacman_[1].collision_up = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[1].rect_right.rectangle)) {
					pacman_[1].collision_right = true;
				}
				if (CheckCollisionRecs(blocks[i][j].rectangle, pacman_[1].rect_left.rectangle)) {
					pacman_[1].collision_left = true;
				}
				if (CheckCollisionCircleRec(food[0].position, food[0].radius, blocks[i][j].rectangle)) {
					food[0].active = false;
				}
				if (CheckCollisionCircleRec(food[1].position, food[1].radius, blocks[i][j].rectangle)) {
					food[1].active = false;
				}
			}
		}

		pacman_[1].rect_down.rectangle.x = pacman_[1].position.x - SQUARE / 2 + 2.5f;
		pacman_[1].rect_down.rectangle.y = pacman_[1].position.y + SQUARE / 2 + 2.5f;
		pacman_[1].rect_up.rectangle.x = pacman_[1].position.x - SQUARE / 2 + 2.5f;
		pacman_[1].rect_up.rectangle.y = pacman_[1].position.y - 3 * SQUARE / 2 + 2.5f;
		pacman_[1].rect_right.rectangle.x = pacman_[1].position.x + SQUARE / 2 + 2.5f;
		pacman_[1].rect_right.rectangle.y = pacman_[1].position.y - SQUARE / 2 + 2.5f;
		pacman_[1].rect_left.rectangle.x = pacman_[1].position.x - 3 * SQUARE / 2 + 2.5f;
		pacman_[1].rect_left.rectangle.y = pacman_[1].position.y - SQUARE / 2 + 2.5;

		pacman_[1].rect_down.rectangle.height = SQUARE - 5;
		pacman_[1].rect_up.rectangle.height = SQUARE - 5;
		pacman_[1].rect_right.rectangle.height = SQUARE - 5;
		pacman_[1].rect_left.rectangle.height = SQUARE - 5;

		pacman_[1].rect_down.rectangle.width = SQUARE - 5;
		pacman_[1].rect_up.rectangle.width = SQUARE - 5;
		pacman_[1].rect_right.rectangle.width = SQUARE - 5;
		pacman_[1].rect_left.rectangle.width = SQUARE - 5;

		pacman_[1].rect_down.color = COLOR6;
		pacman_[1].rect_up.color = COLOR6;
		pacman_[1].rect_right.color = COLOR6;
		pacman_[1].rect_left.color = COLOR6;

		if (framesCounter % 5 == 0) {
			if (IsKeyDown(KEY_RIGHT) && (pacman_[1].position.x + SQUARE / 2 < screenWidth) && pacman_[1].collision_right == false) {
				pacman_[1].position.x += 40.0f;
			}
			else if (IsKeyDown(KEY_LEFT) && (pacman_[1].position.x - SQUARE / 2 > 0) && pacman_[1].collision_left == false) {
				pacman_[1].position.x -= 40.0f;
			}
			else if (IsKeyDown(KEY_UP) && (pacman_[1].position.y - SQUARE / 2 > 0) && pacman_[1].collision_up == false) {
				pacman_[1].position.y -= 40.0f;
			}
			else if (IsKeyDown(KEY_DOWN) && (pacman_[1].position.y + SQUARE / 2 < screenHeight) && pacman_[1].collision_down == false) {
				pacman_[1].position.y += 40.0f;
			}
		}

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				if (CheckCollisionCircles(pacman_[i].position, pacman_[i].radius, food[j].position, food[j].radius)) {
					if (food[j].coin == bitcoin) {
						pacman_[i].point += 10;
					}
					else if (food[j].coin == ether) {
						pacman_[i].point += 5;
					}
					else if (food[j].coin == tether) {
						pacman_[i].point += 2;
					}
					else if (food[j].coin == doge) {
						if (GetRandomValue(0, 4) % 2 == 0) {
							pacman_[i].point += 8;
						}
						else {
							pacman_[i].point -= 8;
						}
					}
					food[j].active = false;
				}
			}
		}
		for (int i = 0; i < 2; i++) {
			if (!food[i].active) {
				food[i].active = true;
				food[i].position = (Vector2){ GetRandomValue(0, screenWidth - 1), GetRandomValue(0, screenHeight - 1) };
				food[i].crypto_num = GetRandomValue(0, 3);
				food[i].coin = food[i].crypto_num;
			}
		}
	} break;
	default: break;
	}

	for (int i = 0; i < 2; i++) {
		pacman_[i].collision_down = false;
		pacman_[i].collision_up = false;
		pacman_[i].collision_right = false;
		pacman_[i].collision_left = false;
	}
}

void updateDrawFrame() {
	updateGame();
	drawGame();
}

void InitGame() {
	framesCounter = 0;
	gameOver = false;

	//gridlines
	offset.x = screenWidth % SQUARE;
	offset.y = screenHeight % SQUARE;

	//pacman0
	pacman_[0].position.x = (float)screenWidth / 2 + SQUARE / 2;
	pacman_[0].position.y = (float)screenHeight / 2 + SQUARE / 2;
	pacman_[0].radius = 19.5;
	pacman_[0].color = YELLOW;

	//pacman1
	pacman_[1].position.x = (float)screenWidth / 2 + -SQUARE / 2;
	pacman_[1].position.y = (float)screenHeight / 2 - 3 * SQUARE / 2;
	pacman_[1].radius = 19.5;
	pacman_[1].color = GREEN;


	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			if (matrix[i][j] == 1) {
				blocks[i][j].rectangle.x = SQUARE * j;
				blocks[i][j].rectangle.y = SQUARE * i;
				blocks[i][j].rectangle.width = SQUARE;
				blocks[i][j].rectangle.height = SQUARE;
			}
			if ((i + j) % 4 == 0) {
				blocks[i][j].color = COLOR1;
			}
			else if ((i + j) % 4 == 1) {
				blocks[i][j].color = COLOR2;
			}
			else if ((i + j) % 4 == 2) {
				blocks[i][j].color = COLOR3;
			}
			else if ((i + j) % 4 == 3) {
				blocks[i][j].color = COLOR4;
			}
		}
	}

	//food
	for (int i = 0; i < 2; i++) {
		food[i].radius = 15;
		food[i].color = MAGENTA;
	}

	//btc
	for (int i = 0; i < 6; i++)
	{
		if (i == 0) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/png/btc_resized.png"); // Load image data into CPU memory (RAM)}
		}
		else if (i == 1) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/png/etherium.png");  // Load image data into CPU memory (RAM)}
		}
		else if (i == 2) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/png/tether.png");  // Load image data into CPU memory (RAM)}
		}
		else if (i == 3) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/png/dogecoin.png");  // Load image data into CPU memory (RAM)}
		}
		else if (i == 4) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/png/wasd.png");  // Load image data into CPU memory (RAM)}
		}
		else if (i == 5) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/png/arrows.png");  // Load image data into CPU memory (RAM)}
		}

		texture[i] = LoadTextureFromImage(image[i]);       // Image converted to texture, GPU memory (RAM -> VRAM)
		UnloadImage(image[i]);                                    // Unload image data from CPU memory (RAM)

		image[i] = LoadImageFromTexture(texture[i]);                 // Load image from GPU texture (VRAM -> RAM)
		UnloadTexture(texture[i]);                                // Unload texture from GPU memory (VRAM)

		texture[i] = LoadTextureFromImage(image[i]);                 // Recreate texture from retrieved image data (RAM -> VRAM)
		UnloadImage(image[i]);                                    // Unload retrieved image data from CPU memory (RAM)
	}
}

void draw_first_pacman_game() {
	DrawRectangle(pacman_[0].rect_right.rectangle.x, pacman_[0].rect_right.rectangle.y, pacman_[0].rect_right.rectangle.width, pacman_[0].rect_right.rectangle.height, pacman_[0].rect_right.color); //right
	DrawRectangle(pacman_[0].rect_left.rectangle.x, pacman_[0].rect_left.rectangle.y, pacman_[0].rect_left.rectangle.width, pacman_[0].rect_left.rectangle.height, pacman_[0].rect_left.color); //left
	DrawRectangle(pacman_[0].rect_up.rectangle.x, pacman_[0].rect_up.rectangle.y, pacman_[0].rect_up.rectangle.width, pacman_[0].rect_up.rectangle.height, pacman_[0].rect_up.color); //up
	DrawRectangle(pacman_[0].rect_down.rectangle.x, pacman_[0].rect_down.rectangle.y, pacman_[0].rect_down.rectangle.width, pacman_[0].rect_down.rectangle.height, pacman_[0].rect_down.color); //down

	if (current_screen == single_player) {
		if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))) {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 0, 45, 360, pacman_[0].color);
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 135, 360, 360, pacman_[0].color);
		}
		else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))) {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 0, 135, 360, pacman_[0].color);
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 225, 360, 360, pacman_[0].color);
		}
		else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))) {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 0, 225, 360, pacman_[0].color);
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 315, 360, 360, pacman_[0].color);
		}
		else {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 45, 315, 360, pacman_[0].color);
		}
	}
	else if (current_screen == multi_player) {
		if (IsKeyDown(KEY_S)) {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 0, 45, 360, pacman_[0].color);
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 135, 360, 360, pacman_[0].color);
		}
		else if (IsKeyDown(KEY_A)) {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 0, 135, 360, pacman_[0].color);
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 225, 360, 360, pacman_[0].color);
		}
		else if (IsKeyDown(KEY_W)) {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 0, 225, 360, pacman_[0].color);
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 315, 360, 360, pacman_[0].color);
		}
		else {
			DrawCircleSector(pacman_[0].position, pacman_[0].radius, 45, 315, 360, pacman_[0].color);
		}
	}
	draw_food(food[0]);

	DrawText(TextFormat("Pacman1: %i", pacman_[0].point), 210, 0, 40, RAYWHITE);
	DrawText(TextFormat("Timer: %1.2f", timerGame), 20, 740, 60, GOLD);
}

void draw_second_pacman_game() {
	DrawRectangle(pacman_[1].rect_right.rectangle.x, pacman_[1].rect_right.rectangle.y, pacman_[1].rect_right.rectangle.width, pacman_[1].rect_right.rectangle.height, pacman_[1].rect_right.color); //right
	DrawRectangle(pacman_[1].rect_left.rectangle.x, pacman_[1].rect_left.rectangle.y, pacman_[1].rect_left.rectangle.width, pacman_[1].rect_left.rectangle.height, pacman_[1].rect_left.color); //left
	DrawRectangle(pacman_[1].rect_up.rectangle.x, pacman_[1].rect_up.rectangle.y, pacman_[1].rect_up.rectangle.width, pacman_[1].rect_up.rectangle.height, pacman_[1].rect_up.color); //up
	DrawRectangle(pacman_[1].rect_down.rectangle.x, pacman_[1].rect_down.rectangle.y, pacman_[1].rect_down.rectangle.width, pacman_[1].rect_down.rectangle.height, pacman_[1].rect_down.color); //down

	if (IsKeyDown(KEY_DOWN)) {
		DrawCircleSector(pacman_[1].position, pacman_[1].radius, 0, 45, 360, pacman_[1].color);
		DrawCircleSector(pacman_[1].position, pacman_[1].radius, 135, 360, 360, pacman_[1].color);
	}
	else if (IsKeyDown(KEY_LEFT)) {
		DrawCircleSector(pacman_[1].position, pacman_[1].radius, 0, 135, 360, pacman_[1].color);
		DrawCircleSector(pacman_[1].position, pacman_[1].radius, 225, 360, 360, pacman_[1].color);
	}
	else if (IsKeyDown(KEY_UP)) {
		DrawCircleSector(pacman_[1].position, pacman_[1].radius, 0, 225, 360, pacman_[1].color);
		DrawCircleSector(pacman_[1].position, pacman_[1].radius, 315, 360, 360, pacman_[1].color);
	}
	else {
		DrawCircleSector(pacman_[1].position, pacman_[1].radius, 45, 315, 360, pacman_[1].color);
	}

	draw_food(food[1]);
	DrawText(TextFormat("Pacman2: %i", pacman_[1].point), 1150, 0, 40, RAYWHITE);
}

void draw_food(bait food) {
	/*for (size_t i = 0; i < 3; i++)
	{
		DrawTexture(texture[i], food.position.x - 15, food.position.y - 15, WHITE);
	}*/

	if (food.crypto_num == 0) {
		DrawTexture(texture[0], food.position.x - 15, food.position.y - 15, WHITE);
	}
	else if (food.crypto_num == 1) {
		DrawTexture(texture[1], food.position.x - 15, food.position.y - 15, WHITE);
	}
	else if (food.crypto_num == 2) {
		DrawTexture(texture[2], food.position.x - 15, food.position.y - 15, WHITE);
	}
	else if (food.crypto_num == 3) {
		DrawTexture(texture[3], food.position.x - 15, food.position.y - 15, WHITE);
	}
	//DrawCircleV(food.position, food.radius, food.color);
}

//#include "raylib.h"
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"
//
//int main(void) {
//    // Initialization
//    const int screenWidth = 800;
//    const int screenHeight = 450;
//    InitWindow(screenWidth, screenHeight, "raylib - Show Image on Button Press");
//
//    // Load texture
//    Texture2D image = LoadTexture("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/wasd_1.png"); // Replace with your image path
//
//    bool buttonPressed = false;
//    float timeSinceButtonPressed = 0.0f;
//
//    SetTargetFPS(60);
//
//    // Main game loop
//    while (!WindowShouldClose()) {
//        // Update
//
//        BeginDrawing();
//        ClearBackground(RAYWHITE);
//
//        if (GuiButton((Rectangle) { 10, 10, 125, 30 }, "Show Image")) {
//            buttonPressed = true;
//            timeSinceButtonPressed = 0.0f; // Reset the timer
//        }
//
//        if (buttonPressed) {
//            timeSinceButtonPressed += GetFrameTime();
//            if (timeSinceButtonPressed >= 5.0f) { // 5 seconds
//                buttonPressed = false;
//            }
//        }
//
//        // Draw
//        
//
//        if (buttonPressed) {
//            // Draw the image
//            DrawTexture(image, screenWidth / 2 - image.width / 2, screenHeight / 2 - image.height / 2, WHITE);
//        }
//
//        // Other drawing...
//        EndDrawing();
//    }
//
//    // De-Initialization
//    UnloadTexture(image); // Unload texture
//    CloseWindow();
//
//    return 0;
//}
