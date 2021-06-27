#include "fieldconfig.h";
#include "botlogic.h";
#include "SeaWManager.h";

int main()
{
	setlocale(LC_CTYPE, "Russian");
	
	botbrains * bot = new botbrains();
	SeaBattleGameflow game(bot);
	game.rungame();
	delete bot;
}