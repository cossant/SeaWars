#pragma once
#include <iostream>;
#include <string>;
#include <random>;
#include <ctime>;

using namespace std;

const int FIELDSIZE = 10;
enum class STATUS { BLANK, SEA, SHIP, RECKS , MISSED_SHOT, ACTIVE_SHOT};

ostream& operator << (ostream& os, STATUS value);


class fconfig
{
public:
	// The main one-demensional array wich stores information about the field tiles.
	STATUS data[FIELDSIZE * FIELDSIZE];
	// Initializes external libs ("rand" for now).
	fconfig();
	// Prints the actual data[] content in ten strings with ten elements in each.
	void display();
	// Generation the field to be used by bot during the game.
	void generateBotField();
	// Gets the amount of fields which are in the requested state at the moment.
	int count(STATUS condition);
private:
	// The bank of the sides code numbers(0 - 3) which can be provided by the request.
	class sidePool;
	// The bank of the fields indexes(0-99) which can be provided by the request.
	class fieldsPool;
	// Checks if there are no ships in range of one around chosen position.
	bool PositionSafe(int);
	// Using the "PositionSafe" function checks if the ship can be fully installed by the chosen parametres.
	bool PlacementPossible(int, int, int);
	// Actually places the ship by modifying the "data[]".
	void PlaceShip(int, int, int);
};

