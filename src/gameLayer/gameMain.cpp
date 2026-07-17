#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include "deltaTime.h"
#include "office.h"
#include "document.h"

DeltaTime deltaTime;

//Sprite declarations
OfficeSprite officeSprite;
Document document;

bool initGame()
{
	//Sprite initializations
	officeSprite.load();
	document.load();
	return true;
}

bool updateGame()
{
	deltaTime.update();
	officeSprite.drawOffice();
	document.draw(250,200);
	return true;
}

void closeGame()
{
}
