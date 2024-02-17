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

	Vector2 pos_up;
	bool up_collision;
	Vector2 pos_down;
	bool down_collision;
	Vector2 pos_right;
	bool right_collision;
	Vector2 pos_left;
	bool left_collision;
}pacman;

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

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 38; j++) {
				DrawRectangle(blocks[i][j].rectangle.x, blocks[i][j].rectangle.y, blocks[i][j].rectangle.width, blocks[i][j].rectangle.height, blocks[i][j].color);
			}
		}
		if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))) {
			DrawCircleSector(pacman1.position, pacman1.radius, 0, 45, 360, YELLOW);
			DrawCircleSector(pacman1.position, pacman1.radius, 135, 360, 360, YELLOW);
		}
		else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))) {
			DrawCircleSector(pacman1.position, pacman1.radius, 0, 135, 360, YELLOW);
			DrawCircleSector(pacman1.position, pacman1.radius, 225, 360, 360, YELLOW);
		}
		else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))) {
			DrawCircleSector(pacman1.position, pacman1.radius, 0, 225, 360, YELLOW);
			DrawCircleSector(pacman1.position, pacman1.radius, 315, 360, 360, YELLOW);
		}
		else {
			DrawCircleSector(pacman1.position, pacman1.radius, 45, 315, 360, YELLOW);
		}
		DrawCircleV(pacman1.pos_right, 15, BLUE); //right
		DrawCircleV(pacman1.pos_left, 15, BLUE); //left
		DrawCircleV(pacman1.pos_up, 15, BLUE); //up
		DrawCircleV(pacman1.pos_down, 15, BLUE); //down
				
		DrawCircleV(food.position, food.radius, food.color);
		DrawText(TextFormat("Point: %i", point), 1350, 0, 40, RAYWHITE);

		DrawText(TextFormat(" Timer: %1i", framesCounter / 60), 20, 700, 40, RED);

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
	
	pacman1.pos_down.x = pacman1.position.x - SQUARE / 2;
	pacman1.pos_down.y = pacman1.position.y + SQUARE / 2;
	pacman1.pos_up.x = pacman1.position.x - SQUARE/2;
	pacman1.pos_up.y = pacman1.position.y - 3*SQUARE/2;
	pacman1.pos_right.x = pacman1.position.x + SQUARE / 2;
	pacman1.pos_right.y = pacman1.position.y - SQUARE/2;
	pacman1.pos_left.x = pacman1.position.x - 3*SQUARE/2;
	pacman1.pos_left.y = pacman1.position.y - SQUARE/2;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 38; j++) {
			
		}
	}

		
	if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && (pacman1.position.x + SQUARE / 2 < screenWidth) && pacman1.right_collision == false) {
		pacman1.position.x += 40.0f;
	}
	if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && (pacman1.position.x - SQUARE / 2 > 0) && pacman1.left_collision == false) {
		pacman1.position.x -= 40.0f;
	}
	if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && (pacman1.position.y - SQUARE / 2 > 0) && pacman1.up_collision == false) {
		pacman1.position.y -= 40.0f;
	}
	if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && (pacman1.position.y + SQUARE / 2 < screenHeight) && pacman1.down_collision == false) {
		pacman1.position.y += 40.0f;
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
}

void updateDrawFrame() {
	updateGame();
	drawGame();
}

void InitGame() {
	framesCounter = 0;
	gameOver = false;


	offset.x = screenWidth % SQUARE;
	offset.y = screenHeight % SQUARE;

	//pacman
	pacman1.position.x = (float)screenWidth / 2 + SQUARE / 2;
	pacman1.position.y = (float)screenHeight / 2 + SQUARE / 2;
	pacman1.radius = 19.5;

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