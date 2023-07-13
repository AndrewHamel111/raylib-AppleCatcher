#ifndef APPLES_H_
#define APPLES_H_

#include "raylib.h"

typedef struct Apple
{
	Vector2 position;
	int fallSpeed;

	bool _active;
} Apple;

void UnsetAppleAt(Apple*, int);
void SetAppleAt(Apple*, int, Vector2, float);
void DrawApples(Apple*);
void UpdateApples(Apple*);
void SpawnApple(Apple*);

#endif