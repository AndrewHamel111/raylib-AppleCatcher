#ifndef SCORE_H_
#define SCORE_H_

#include "raylib.h"

int GetScore(void);
int GetLives(void);

void AddScore(int);
bool RemoveLives(int);
void ResetScore(void);

#endif