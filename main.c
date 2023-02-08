// raylib Apple Catcher
// Andrew Hamel Codes

#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#define APPLE_SOURCE_WIDTH 60
#define APPLE_SOURCE_HEIGHT 60
#define APPLE_MAX_COUNT 8
#define APPLE_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, APPLE_SOURCE_WIDTH, APPLE_SOURCE_HEIGHT}

#define BASKET_SOURCE_WIDTH 128
#define BASKET_SOURCE_HEIGHT 128
#define BASKET_SOURCE_RECT CLITERAL(Rectangle){0, 0, BASKET_SOURCE_WIDTH, BASKET_SOURCE_HEIGHT}

#define FALL_SPEED_MIN 150
#define FALL_SPEED_MAX 250

#define TEXT_GAME_OVER "Game Over!"
#define TEXT_RESTART "Press 'R' to Play Again!"

#define TIME_BETWEEN_APPLES 1.0f
#define INITIAL_LIVES 3
#define SCORE_PER_APPLE 100;

const int screenWidth = 600;
const int screenHeight = 600;
const Vector2 screenSize = {screenWidth, screenHeight};

typedef struct Apple
{
	Vector2 position;
	int fallSpeed;

	bool _active;
} Apple;

Apple _apples[APPLE_MAX_COUNT];

typedef enum GameState
{
	PLAYING,
	END
} GameState;

GameState _state;

float _timeGameStarted;
float _timeGameEnded;

Texture2D _atlasApple;
Texture2D _atlasBasket;

float _nextAppleTimer;
int _score;
int _lives;
float _basketXPosition;

void UnsetAppleAt(int);
void SetAppleAt(int, Vector2, float);
void DrawApples(void);
void UpdateApples(void);

void SpawnApple(void);
Vector2 GetBasketPosition(void);

void DrawBasket(void);

void GameInit(void);
void GameEnd(void);
void UpdateDrawFrame(void);

int main()
{
	srand(time(0));

	InitWindow(screenWidth, screenHeight, "Raylib Template by Andrew Hamel");

	_atlasApple = LoadTexture("resources/apple_atlas.png");
	_atlasBasket = LoadTexture("resources/basket.png");

	GameInit();

	while(!WindowShouldClose())
	{
		UpdateDrawFrame();
	}

	CloseWindow();
	
	return 0;
}

void UnsetAppleAt(int i)
{
	if (i < 0 || i >= APPLE_MAX_COUNT)
	{
		return;
	}

	_apples[i]._active = false;
}

void SetAppleAt(int i, Vector2 position, float fallSpeed)
{
	if (i < 0 || i >= APPLE_MAX_COUNT)
	{
		return;
	}

	_apples[i].position = position;
	_apples[i].fallSpeed = fallSpeed;
	_apples[i]._active = true;
}

void GameInit(void)
{
	_state = PLAYING;
	_timeGameStarted = GetTime();
	_nextAppleTimer = TIME_BETWEEN_APPLES;

	_lives = INITIAL_LIVES;
	_score = 0;

	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		UnsetAppleAt(i);
	}
}

void DrawApples(void)
{
	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		if (!_apples[i]._active)
		{
			continue;
		}

		Vector2 position = _apples[i].position;
		position.x -= APPLE_SOURCE_WIDTH / 2;
		position.y -= APPLE_SOURCE_HEIGHT / 2;
		DrawTextureRec(_atlasApple, APPLE_SOURCE_RECTANGLE, position, WHITE);
	}
}

void UpdateApples(void)
{
	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		if (!_apples[i]._active)
		{
			continue;
		}

		if (_apples[i].position.y > screenHeight + APPLE_SOURCE_HEIGHT / 2)
		{
			UnsetAppleAt(i);

			_lives--;
			if (_lives <= 0)
			{
				GameEnd();
				return;
			}

			continue;
		}

		Vector2 basketPosition = GetBasketPosition();
		if (fabsf(_apples[i].position.x - basketPosition.x) < BASKET_SOURCE_WIDTH / 2 && 
			fabsf(_apples[i].position.y - basketPosition.y) < BASKET_SOURCE_HEIGHT / 4)
		{
			UnsetAppleAt(i);
			_score += SCORE_PER_APPLE;
			continue;
		}

		float offset = _apples[i].fallSpeed * GetFrameTime();
		_apples[i].position.y += offset;
	}
}

void SpawnApple(void)
{
	int availableIndex = -1;
	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		if (!_apples[i]._active)
		{
			availableIndex = i;
			break;
		}
	}

	if (availableIndex == -1)
	{
		// TREMENDOUS ERROR!!
		return;
	}

	int fallSpeed = GetRandomValue(FALL_SPEED_MIN, FALL_SPEED_MAX);
	int posX = GetRandomValue(APPLE_SOURCE_WIDTH / 2, screenWidth - (APPLE_SOURCE_WIDTH / 2));

	SetAppleAt(availableIndex, (Vector2){posX, -APPLE_SOURCE_HEIGHT}, fallSpeed);
}

Vector2 GetBasketPosition(void)
{
	if (_state == PLAYING)
	{
		_basketXPosition = GetMousePosition().x;
	}

	return (Vector2){_basketXPosition, screenHeight - BASKET_SOURCE_HEIGHT * 0.75};
}

void DrawBasket(void)
{
	Vector2 position = GetBasketPosition();

	position.x -= BASKET_SOURCE_WIDTH / 2;
	position.y -= BASKET_SOURCE_HEIGHT / 2;

	DrawTextureRec(_atlasBasket, BASKET_SOURCE_RECT, position, WHITE);
}

void GameEnd(void)
{
	_state = END;
	_timeGameEnded = GetTime();
}

void UpdateDrawFrame(void)
{
	// Update
	if (_state == END)
	{
		// Reset game check
		if (IsKeyPressed(KEY_R))
		{
			GameInit();
		}
	}
	else
	{
		UpdateApples();

		_nextAppleTimer -= GetFrameTime();
		if (_nextAppleTimer < 0)
		{
			_nextAppleTimer = TIME_BETWEEN_APPLES;
			SpawnApple();
		}
	}

	BeginDrawing();

		ClearBackground(RAYWHITE);

		// Draw Game
		DrawBasket();
		DrawApples();

		if (_state == END)
		{
			const char* gameOver = TEXT_GAME_OVER;
			const char* pressRToRestart = TEXT_RESTART;

			DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, 0.8f));
			DrawText(gameOver, screenWidth / 2 - MeasureText(gameOver, 40) / 2, screenHeight / 2 - 10, 40, DARKGRAY);
			DrawText(pressRToRestart, screenWidth / 2 - MeasureText(pressRToRestart, 20) / 2, screenHeight * 0.75f - 10, 20, DARKGRAY);

			const char* scoreText = TextFormat("SCORE: %d", _score);
			DrawText(scoreText, 16, 8, 56, BLACK);

			int minutes = (int)(_timeGameEnded - _timeGameStarted) / 60;
			int seconds = (int)(_timeGameEnded - _timeGameStarted) % 60;
			DrawText(TextFormat("Time played: %d minutes, %d seconds.", minutes, seconds), 20, screenHeight - 40, 20, DARKGRAY);
		}
		else
		{
			const char* scoreText = TextFormat("SCORE: %d", _score);
			const char* livesText = TextFormat("LIVES: %d", _lives);

			DrawText(scoreText, 16, 8, 56, BLACK);
			DrawText(livesText, 16, 56, 24, DARKGRAY);
		}

	EndDrawing();
}