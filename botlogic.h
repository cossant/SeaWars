#pragma once
#include "fieldconfig.h";

enum class REPORT { MISS, HIT, DESTROYED};

class botbrains
{
public:
	// Initialyzes inner bot pool.
	botbrains();
	// Checks if the player shot was landed and changes bot's field accrdingly.
	bool shotconnected(int);
	// Generates new playfield via "fieldconfig" and sets various variables the the starting states.
	void initplay();
	// Calculates the new position to be shot according to the previous aquired information.
	void generateshot();
	// Input information for the futurous using in the "generateshot".
	void handlereport(REPORT);
	// Prints the bot field using the "fieldconfig" methods.											TODO: DEBUGGING PURPOUSE ONLY. DELETE BEFORE RELEASE.
	void display();

private:
	// Provides unique side codes at user's request.
	class sidePool;

	// Pack of current situation information variables.
	fconfig* botfield;				// Bot's ship placement.
	fconfig* playfield;				// Bot's vision of the player's field.
	bool PShpAlive[10];				// Game status for the player's ships.
	int shot_index;					// Last shot position.
	int other_side_length;			// Saved length of the other part of the ship.

	// Pinch situattion info.
	bool gotpinch;					// Is there a "pinch" situation.
	// 0 - Up; 1 - Right; 2 - Down; 3 - Left; (-1) - UNDEF.
	int pinch_dir;					// Which direction is chosen for the continious pressing.
	int pinch_index,  ActShpLen;	// The first current "pinch" tile. Current attacing ship length. 
	sidePool * sides;				// Class exemplar.
	int ActShpCord[4];				// Destructed parts of the current ship.

	// General gamerules - ingame ships length.
	const int ships[10] = { 4,3,3,2,2,2,1,1,1,1 };

	// Checks side for allready aquired missed positions.
	bool sideSafe(int, int);
	// Returns opposite direction code.
	int invertdir(int);
	// Returns random field position with "SEA" status.
 	int randomSeaPos();
	// Covers everything around the chosen position with a "MISS" flags.
	void battleMark(int);
	// Two functions to ('A' -> 1) and (1 -> 'A').
	int ChLtTransf(char);
	char ChLtTransf(int);
};