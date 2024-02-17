#include <raylib.h>
#include <raymath.h>

#define SQUARE 40

typedef struct bait {
	bool active;
	Vector2 position;
	int radius;
	Color color;
}bait;

typedef struct block {
	Rectangle rectangle;
	Color color;
}block;

typedef struct pacman {
	Vector2 position;
	int radius;
	Color color;
	
	block rect_up;
	block rect_down;
	block rect_left;
	block rect_right;

	bool collision_up;
	bool collision_down;
	bool collision_right;
	bool collision_left;
}pacman;

static int x_counter = 0;
static pacman pacman1 = { 0 };
static bait food = { 0 };
static block blocks[20][38] = {0};
static bool gameOver;
static int point;
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
							  {1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1},
							  {1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1},
							  {1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1},
							  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
							  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1} };

static int framesCounter;
static int timeCounter;

//screen variables
const int screenWidth = 1520;
const int screenHeight = 800;
//grid lines
static Vector2 offset = { 0 };

//functions
void updateGame();
void drawGame();
void updateDrawFrame();
void InitGame();

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

	ClearBackground(BLACK);
	if (!gameOver) {

		DrawRectangle(pacman1.rect_right.rectangle.x, pacman1.rect_right.rectangle.y, pacman1.rect_right.rectangle.width, pacman1.rect_right.rectangle.height, pacman1.rect_right.color); //right
		DrawRectangle(pacman1.rect_left.rectangle.x, pacman1.rect_left.rectangle.y, pacman1.rect_left.rectangle.width, pacman1.rect_left.rectangle.height, pacman1.rect_left.color); //left
		DrawRectangle(pacman1.rect_up.rectangle.x, pacman1.rect_up.rectangle.y, pacman1.rect_up.rectangle.width, pacman1.rect_up.rectangle.height, pacman1.rect_up.color); //up
		DrawRectangle(pacman1.rect_down.rectangle.x, pacman1.rect_down.rectangle.y, pacman1.rect_down.rectangle.width, pacman1.rect_down.rectangle.height, pacman1.rect_down.color); //down

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 38; j++) {
				DrawRectangle(blocks[i][j].rectangle.x, blocks[i][j].rectangle.y, blocks[i][j].rectangle.width, blocks[i][j].rectangle.height, blocks[i][j].color);
			}
		}

		if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))) {
			DrawCircleSector(pacman1.position, pacman1.radius, 0, 45, 360, pacman1.color);
			DrawCircleSector(pacman1.position, pacman1.radius, 135, 360, 360, pacman1.color);
		}
		else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))) {
			DrawCircleSector(pacman1.position, pacman1.radius, 0, 135, 360, pacman1.color);
			DrawCircleSector(pacman1.position, pacman1.radius, 225, 360, 360, pacman1.color);
		}
		else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))) {
			DrawCircleSector(pacman1.position, pacman1.radius, 0, 225, 360, pacman1.color);
			DrawCircleSector(pacman1.position, pacman1.radius, 315, 360, 360, pacman1.color);
		}
		else {
			DrawCircleSector(pacman1.position, pacman1.radius, 45, 315, 360, pacman1.color);
		}
				
		DrawCircleV(food.position, food.radius, food.color);
		DrawText(TextFormat("Point: %i", point), 1350, 0, 40, RAYWHITE);
		DrawText(TextFormat("x_counter %i", x_counter), 1000, 750, 40, RAYWHITE);
		DrawText(TextFormat(" Timer: %1i", timeCounter), 20, 700, 40, RED);

		for (int i = 0; i < screenWidth / SQUARE + 1; i++) {
			DrawLineV((Vector2) { SQUARE* i + offset.x, offset.y }, (Vector2) { SQUARE* i + offset.x, screenHeight - offset.y }, WHITE);
		}
		for (int i = 0; i < screenWidth / SQUARE + 1; i++) {
			DrawLineV((Vector2) { offset.x, SQUARE* i + offset.y }, (Vector2) { screenWidth - offset.x, SQUARE* i + offset.y }, WHITE);
		}
	}
	if (gameOver) {
		DrawText("GAMEOVER!!", 250, 500, 60, RAYWHITE);
	}
	EndDrawing();
}


void updateGame() {
	
	pacman1.rect_down.rectangle.x = pacman1.position.x - SQUARE / 2 + 2.5f;
	pacman1.rect_down.rectangle.y = pacman1.position.y + SQUARE / 2 + 2.5f;
	pacman1.rect_up.rectangle.x = pacman1.position.x - SQUARE / 2 + 2.5f;
	pacman1.rect_up.rectangle.y = pacman1.position.y - 3 * SQUARE / 2 + 2.5f;
	pacman1.rect_right.rectangle.x = pacman1.position.x + SQUARE / 2 + 2.5f ;
	pacman1.rect_right.rectangle.y = pacman1.position.y - SQUARE / 2 + 2.5f;
	pacman1.rect_left.rectangle.x = pacman1.position.x - 3 * SQUARE / 2 + 2.5f;
	pacman1.rect_left.rectangle.y = pacman1.position.y - SQUARE / 2 + 2.5;

	pacman1.rect_down.rectangle.height = SQUARE-5;
	pacman1.rect_up.rectangle.height = SQUARE-5;
	pacman1.rect_right.rectangle.height = SQUARE-5;
	pacman1.rect_left.rectangle.height = SQUARE-5;

	pacman1.rect_down.rectangle.width = SQUARE-5;
	pacman1.rect_up.rectangle.width = SQUARE-5;
	pacman1.rect_right.rectangle.width = SQUARE-5;
	pacman1.rect_left.rectangle.width = SQUARE-5;

	pacman1.rect_down.color = GREEN;
	pacman1.rect_up.color = DARKBROWN;
	pacman1.rect_right.color = GREEN;
	pacman1.rect_left.color = GREEN;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			if (CheckCollisionRecs(blocks[i][j].rectangle, pacman1.rect_down.rectangle)) {
				pacman1.collision_down = true;
			}
			if (CheckCollisionRecs(blocks[i][j].rectangle, pacman1.rect_up.rectangle)) {
				pacman1.collision_up = true;
			}
			if (CheckCollisionRecs(blocks[i][j].rectangle, pacman1.rect_right.rectangle)) {
				pacman1.collision_right = true;
			}
			if (CheckCollisionRecs(blocks[i][j].rectangle, pacman1.rect_left.rectangle)) {
				pacman1.collision_left = true;
			}
		}
	}

	if (framesCounter % 10 == 0) {
		if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && (pacman1.position.x + SQUARE / 2 < screenWidth) && pacman1.collision_right == false && GetTime()) {
			pacman1.position.x += 40.0f;
		}
		else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && (pacman1.position.x - SQUARE / 2 > 0) && pacman1.collision_left == false) {
			pacman1.position.x -= 40.0f;
		}
		else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && (pacman1.position.y - SQUARE / 2 > 0) && pacman1.collision_up == false) {
			pacman1.position.y -= 40.0f;
		}
		else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && (pacman1.position.y + SQUARE / 2 < screenHeight) && pacman1.collision_down == false) {
			pacman1.position.y += 40.0f;
		}
	}
	if (CheckCollisionCircles(pacman1.position, pacman1.radius, food.position, food.radius) == true) {
		point++;
		food.active = false;
	}
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			
		}
	}
	if (!food.active) {
		food.active = true;
		food.position = (Vector2){ GetRandomValue(0, screenWidth - 1), GetRandomValue(0, screenHeight - 1) };
		food.color = RED;
		food.radius = 10;
	}
	framesCounter++;

	pacman1.collision_down = false;
	pacman1.collision_up = false;
	pacman1.collision_right = false;
	pacman1.collision_left = false;
}

void updateDrawFrame() {
	updateGame();
	drawGame();
}

void InitGame() {
	framesCounter = 0;
	timeCounter = (int)(framesCounter / 60);
	gameOver = false;


	offset.x = screenWidth % SQUARE;
	offset.y = screenHeight % SQUARE;

	//pacman
	pacman1.position.x = (float)screenWidth / 2 + SQUARE / 2;
	pacman1.position.y = (float)screenHeight / 2 + SQUARE / 2;
	pacman1.radius = 19.5;
	pacman1.color = YELLOW;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			if (matrix[i][j] == 1) {
				blocks[i][j].rectangle.x = SQUARE * j;
				blocks[i][j].rectangle.y = SQUARE * i;
				blocks[i][j].rectangle.width = SQUARE;
				blocks[i][j].rectangle.height = SQUARE;
			}
			if ((i + j) % 2 == 0) {
				blocks[i][j].color = RED;
			}
			else if ((i + j) % 2 == 1) {
				blocks[i][j].color = BLUE;
			}
		}
	}

}