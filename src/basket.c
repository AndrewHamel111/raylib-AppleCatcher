#include "basket.h"
#include "defines.h"
#include "gamestate.h"

float _basketXPosition;
extern Texture2D _atlasBasket;

Vector2 GetBasketPosition(void)
{
	if (GetGameState() == PLAYING)
	{
		_basketXPosition = GetMousePosition().x;
	}

	return (Vector2){_basketXPosition, SCREEN_HEIGHT - BASKET_SOURCE_HEIGHT * 0.75};
}

void DrawBasket(void)
{
	Vector2 position = GetBasketPosition();

	position.x -= BASKET_SOURCE_WIDTH / 2;
	position.y -= BASKET_SOURCE_HEIGHT / 2;

	DrawTextureRec(_atlasBasket, BASKET_SOURCE_RECT, position, WHITE);
}