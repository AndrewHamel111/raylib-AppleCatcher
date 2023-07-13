#include "gamestate.h"

GameState _state;

GameState GetGameState(void)
{
	return _state;
}

void SetGameState(GameState state)
{
	_state = state;
}