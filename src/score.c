#include "score.h"
#include "defines.h"

int _score;
int _lives;

int GetScore(void)
{
	return _score;
}

int GetLives(void)
{
	return _lives;
}

void AddScore(int score)
{
	_score += score;
}

bool RemoveLives(int lives)
{
	_lives -= lives;

	return _lives > 0;
}

void ResetScore(void)
{
	_lives = INITIAL_LIVES;
	_score = 0;
}