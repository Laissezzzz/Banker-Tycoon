#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include "deltaTime.h"
#include "spriteAtlas.h"

DeltaTime deltaTime;
//Sprite declarations
SpriteAtlas officeSprite;

bool initGame()
{
	//Sprite initializations
	officeSprite.load("officeAssets.png", 2, 9);
	return true;
}

bool updateGame()
{
	deltaTime.update();
	officeSprite.draw(0, 0, 150, 53, 3);
	officeSprite.draw(1, 0, 30, 20, 3);
	officeSprite.draw(0, 1, 40, 56, 3);
	return true;
}

void closeGame()
{
}
