#include "apples.h"

#include "defines.h"
#include "score.h"
#include "basket.h"
#include "gamestate.h"

#include <math.h>

extern Texture2D _atlasApple;
extern void GameEnd(void);

void UnsetAppleAt(Apple* apples, int i)
{
	if (i < 0 || i >= APPLE_MAX_COUNT)
	{
		return;
	}

	apples[i]._active = false;
}

void SetAppleAt(Apple* apples, int i, Vector2 position, float fallSpeed)
{
	if (i < 0 || i >= APPLE_MAX_COUNT)
	{
		return;
	}

	apples[i].position = position;
	apples[i].fallSpeed = fallSpeed;
	apples[i]._active = true;
}

void DrawApples(Apple* apples)
{
	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		if (!apples[i]._active)
		{
			continue;
		}

		Vector2 position = apples[i].position;
		position.x -= APPLE_SOURCE_WIDTH / 2;
		position.y -= APPLE_SOURCE_HEIGHT / 2;
		DrawTextureRec(_atlasApple, APPLE_SOURCE_RECTANGLE, position, WHITE);
	}
}

void UpdateApples(Apple* apples)
{
	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		if (!apples[i]._active)
		{
			continue;
		}

		if (apples[i].position.y > SCREEN_HEIGHT + APPLE_SOURCE_HEIGHT / 2)
		{
			UnsetAppleAt(apples, i);

			if (!RemoveLives(1))
			{
				GameEnd();
				return;
			}

			continue;
		}

		Vector2 basketPosition = GetBasketPosition();
		if (fabsf(apples[i].position.x - basketPosition.x) < BASKET_SOURCE_WIDTH / 2 && 
			fabsf(apples[i].position.y - basketPosition.y) < BASKET_SOURCE_HEIGHT / 4)
		{
			UnsetAppleAt(apples, i);
			AddScore(SCORE_PER_APPLE);
			continue;
		}

		float offset = apples[i].fallSpeed * GetFrameTime();
		apples[i].position.y += offset;
	}
}

void SpawnApple(Apple* apples)
{
	int availableIndex = -1;
	for (int i = 0; i < APPLE_MAX_COUNT; i++)
	{
		if (!apples[i]._active)
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
	int posX = GetRandomValue(APPLE_SOURCE_WIDTH / 2, SCREEN_WIDTH - (APPLE_SOURCE_WIDTH / 2));

	SetAppleAt(apples, availableIndex, (Vector2){posX, -APPLE_SOURCE_HEIGHT}, fallSpeed);
}