// raylib Apple Catcher
// Andrew Hamel Codes
// https://github.com/AndrewHamel111/raylib-AppleCatcher
// https://youtu.be/jWo3uzYbb7Y

#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "defines.h"
#include "apples.h"
#include "score.h"
#include "gamestate.h"
#include "basket.h"

Apple _apples[APPLE_MAX_COUNT];

float _timeGameStarted;
float _timeGameEnded;
float _nextAppleTimer;

Texture2D _atlasApple;
Texture2D _atlasBasket;

void GameInit(Apple*);
void GameEnd(void);
void UpdateDrawFrame(void);

int main()
{
	srand(time(0));

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Template by Andrew Hamel");

	_atlasApple = LoadTexture("resources/apple_atlas.png");
	_atlasBasket = LoadTexture("resources/basket.png");

	GameInit(_apples);

	while(!WindowShouldClose())
	{
		UpdateDrawFrame();
	}

	UnloadTexture(_atlasApple);
	UnloadTexture(_atlasBasket);

	CloseWindow();
	
	return 0;
}

void GameInit(Apple* apples)
{
	SetGameState(PLAYING);
	_timeGameStarted = GetTime();
	_nextAppleTimer = TIME_BETWEEN_APPLES;

	ResetScore();

	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		UnsetAppleAt(apples, i);
	}
}

void GameEnd(void)
{
	SetGameState(END);
	_timeGameEnded = GetTime();
}

void UpdateDrawFrame(void)
{
	// Update
	if (GetGameState() == END)
	{
		// Reset game check
		if (IsKeyPressed(KEY_R))
		{
			GameInit(_apples);
		}
	}
	else
	{
		UpdateApples(_apples);

		_nextAppleTimer -= GetFrameTime();
		if (_nextAppleTimer < 0)
		{
			_nextAppleTimer = TIME_BETWEEN_APPLES;
			SpawnApple(_apples);
		}
	}

	BeginDrawing();

		ClearBackground(RAYWHITE);

		// Draw Game
		DrawBasket();
		DrawApples(_apples);

		if (GetGameState() == END)
		{
			const char* gameOver = TEXT_GAME_OVER;
			const char* pressRToRestart = TEXT_RESTART;

			DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(WHITE, 0.8f));
			DrawText(gameOver, SCREEN_WIDTH / 2 - MeasureText(gameOver, 40) / 2, SCREEN_HEIGHT / 2 - 10, 40, DARKGRAY);
			DrawText(pressRToRestart, SCREEN_WIDTH / 2 - MeasureText(pressRToRestart, 20) / 2, SCREEN_HEIGHT * 0.75f - 10, 20, DARKGRAY);

			const char* scoreText = TextFormat("SCORE: %d", GetScore());
			DrawText(scoreText, 16, 8, 56, BLACK);

			int minutes = (int)(_timeGameEnded - _timeGameStarted) / 60;
			int seconds = (int)(_timeGameEnded - _timeGameStarted) % 60;
			DrawText(TextFormat("Time played: %d minutes, %d seconds.", minutes, seconds), 20, SCREEN_HEIGHT - 40, 20, DARKGRAY);
		}
		else
		{
			const char* scoreText = TextFormat("SCORE: %d", GetScore());
			const char* livesText = TextFormat("LIVES: %d", GetLives());

			DrawText(scoreText, 16, 8, 56, BLACK);
			DrawText(livesText, 16, 56, 24, DARKGRAY);
		}

	EndDrawing();
}