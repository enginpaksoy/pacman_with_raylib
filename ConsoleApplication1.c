#include <raylib.h>
#include <raymath.h>

#define SQUARE 40
#define COLOR1 (Color){ 3, 6, 45, 255 } 
#define COLOR2 (Color){ 50, 7, 73, 255 } 
#define COLOR3 (Color){ 104, 4, 75, 255 } 
#define COLOR4 (Color){ 135, 10, 93, 255 } 
#define COLOR5 (Color){ 106, 189, 200, 255 } 


typedef struct bait {
	bool active;
	Vector2 position;
	int radius;
	Color color;
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
	
	block rect_up;
	block rect_down;
	block rect_left;
	block rect_right;

	bool collision_up;
	bool collision_down;
	bool collision_right;
	bool collision_left;
}pacman;

static Image image[3];
static Texture2D texture[3];

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

	ClearBackground(COLOR5);
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
		
		//DrawCircleV(food.position, food.radius, food.color);
		
		DrawText(TextFormat("Point: %i", point), 1350, 0, 40, RAYWHITE);
		DrawText(TextFormat("x_counter %i", x_counter), 1000, 750, 40, RAYWHITE);
		DrawText(TextFormat("Timer: %1i", timeCounter), 20, 700, 40, RED);

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

	framesCounter++;
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

	pacman1.rect_down.color = RAYWHITE;
	pacman1.rect_up.color = RAYWHITE;
	pacman1.rect_right.color = RAYWHITE;
	pacman1.rect_left.color = RAYWHITE;

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
			if (CheckCollisionCircleRec(food.position, food.radius, blocks[i][j].rectangle)) {
				food.active = false;
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

	if (!food.active) {
		food.active = true;
		food.position = (Vector2){ GetRandomValue(0, screenWidth - 1), GetRandomValue(0, screenHeight - 1) };
		food.crypto_num = GetRandomValue(0, 2);
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

	//gridlines
	offset.x = screenWidth % SQUARE;
	offset.y = screenHeight % SQUARE;

	//pacman
	pacman1.position.x = (float)screenWidth / 2 + SQUARE / 2;
	pacman1.position.y = (float)screenHeight / 2 + SQUARE / 2;
	pacman1.radius = 19.5;
	pacman1.color = GOLD;

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
	food.radius = 15;
	food.color = MAGENTA;

	//btc
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/btc_resized.png"); // Load image data into CPU memory (RAM)}
		}  
		else if(i == 1){
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/etherium.png");  // Load image data into CPU memory (RAM)}
		}
		else if (i == 2) {
			image[i] = LoadImage("C:/Users/Engin Paksoy/source/repos/ConsoleApplication1/tether.png");  // Load image data into CPU memory (RAM)}
		}
		texture[i] = LoadTextureFromImage(image[i]);       // Image converted to texture, GPU memory (RAM -> VRAM)
		UnloadImage(image[i]);                                    // Unload image data from CPU memory (RAM)

		image[i] = LoadImageFromTexture(texture[i]);                 // Load image from GPU texture (VRAM -> RAM)
		UnloadTexture(texture[i]);                                // Unload texture from GPU memory (VRAM)

		texture[i] = LoadTextureFromImage(image[i]);                 // Recreate texture from retrieved image data (RAM -> VRAM)
		UnloadImage(image[i]);                                    // Unload retrieved image data from CPU memory (RAM)
	}
}