#ifndef GAMESTATE_H_
#define GAMESTATE_H_

typedef enum GameState
{
	PLAYING,
	END
} GameState;

GameState GetGameState(void);
void SetGameState(GameState);

#endif