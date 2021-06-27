#pragma once
#include "botlogic.h";

class SeaBattleGameflow
{
public:
	SeaBattleGameflow(botbrains*);
	void rungame();	
private:
	enum class TEAMS {PLAYER, BOT, NONE};
	botbrains* bot;
	bool botturn;
	void gameoutcome(TEAMS);
	bool botactions();
	bool playeractions();

};