#pragma once
#include <iostream>;
#include <string>;
#include <random>;
#include <ctime>;

using namespace std;

enum class STATUS { BLANK, SEA, SHIP, RECKS , MISSED_SHOT};
ostream& operator << (ostream& os, STATUS value)
{
	char output;
	switch (value)
	{
	case (STATUS::BLANK):
	{
		output = '@';
		break;
	}
	case (STATUS::SEA):
	{
		output = '~';
		break;
	}
	case (STATUS::SHIP):
	{
		output = 'O';
		break;
	}
	case (STATUS::RECKS):
	{
		output = '#';
		break;
	}
	case (STATUS::MISSED_SHOT):
	{
		output = 'X';
		break;
	}
	}
	cout << output;
	return os;
}
const int FIELDSIZE = 10;


class fconfig
{
public:
	fconfig() 
	{
		srand(time(NULL));
	}


	void display()
	{
		cout << "Вывод поля:\n";
		for (int i = 0; i < FIELDSIZE * FIELDSIZE; i++)
		{
			
			cout << data[i] << ' ';
			if (i % 10 == 9)
				cout << '\n';

		}
	}


	void generateBotField()
	{
		// Setting the ships pool.
		int shipspool[10] = { 4,3,3,2,2,2,1,1,1,1 };
		// Nullifying the whole field.
		for (int i = 0; i < FIELDSIZE * FIELDSIZE; i++)
		{
			data[i] = STATUS::BLANK;
		}
		// Filling.
		// Sides code: 0 - UP; 1 - Right; 2 - Down; 3 - Left;
		int curShip, curDir, curPosIndex;
		// The critical flag - ship was installed during this iteration .
		bool installed, OODir; 
		for (int curShpIndex = 0; curShpIndex < 10; curShpIndex++)
		{
			curShip = shipspool[curShpIndex];
			installed = false;
			sidePool directions;
			fieldsPool positions;
			do
			{ 
				// Pick new untried position.
				curPosIndex = positions.field();
				OODir = false;
				directions.reset();
				do
				{
					curDir = directions.side();
					if (curDir != -1)
						if (PlacementPossible(curShip, curDir, curPosIndex))
						{
							PlaceShip(curShip, curDir, curPosIndex);
							installed = true;
						}
				} while (curDir != -1);
			} while (!installed);
		}

		// Replacing BLANK positions to SEA positions.
		for (int i = 0; i < FIELDSIZE * FIELDSIZE; i++)
		{
			if (data[i] == STATUS::BLANK)
				data[i] = STATUS::SEA;
		}
	}


	int count(STATUS condition)
	{
		int countticker = 0;
		for (int i = 0; i < FIELDSIZE * FIELDSIZE; i++)
			if (data[i] == condition)
				countticker++;
		return countticker;
	}

	STATUS data[FIELDSIZE * FIELDSIZE];


private:

	class sidePool
	{
	public:
		sidePool()
		{
			for (int i = 0; i < 4; i++)
			{
				isPossible[i] = true;
			}
		}
		int side()
		{
			if (freesides() == 0)
				return (-1);
			// Random modifier.
			int mod = rand() % freesides();
			// Impossible side.
			int chosen_side = 0;
			do
			{
				if (isPossible[chosen_side])
					mod--;
				if (mod > -1)
					chosen_side++;
				if (chosen_side > 3)
					chosen_side = 0;
			} while (mod > -1);
			// Marking this variant as allready picked.
			isPossible[chosen_side] = false;
			return (chosen_side);
		}
		void reset()
		{
			for (int i = 0; i < 4; i++)
			{
				isPossible[i] = true;
			}
		}
	private:
		int freesides()
		{
			int count = 0;
			for (int i = 0; i < 4; i++)
			{
				if (isPossible[i] == true)
					count++;
			}
			return count;
		}
		bool isPossible[4];
	};

	class fieldsPool
	{
	public:
		fieldsPool()
		{
			for (int i = 0; i < FIELDSIZE*FIELDSIZE; i++)
			{
				isPossible[i] = true;
			}
		}
		int field()
		{
			if (freeFields() == 0)
				return (-1);
			// Random modifier.
			int mod = rand() % freeFields();
			// Impossible side.
			int chosen_field = 0;
			do
			{
				if (isPossible[chosen_field])
					mod--;
				if (mod > -1)
					chosen_field++;
				if (chosen_field > FIELDSIZE*FIELDSIZE - 1)
					chosen_field = 0;
			} while (mod > -1);
			// Marking this variant as allready picked.
			isPossible[chosen_field] = false;
			return (chosen_field);
		}
	private:
		int freeFields()
		{
			int count = 0;
			for (int i = 0; i < FIELDSIZE*FIELDSIZE; i++)
			{
				if (isPossible[i] == true)
					count++;
			}
			return count;
		}
		bool isPossible[FIELDSIZE*FIELDSIZE];
	};


	bool PositionSafe(int index)
	{
		/* 0__
		   ___
	       ___*/
		int posX = index % 10 - 1 ;
		int posY = index / 10 - 1 ;
		// Impossible index.
		int currIndex = -1;
		for (int indexMod = 0; indexMod < 9; indexMod++)
		{
			// If modified position is still on field.
			if ((posX + indexMod % 3) > -1 && (posX + indexMod % 3) < FIELDSIZE && (posY + indexMod / 3) > -1 && (posY + indexMod / 3) < FIELDSIZE)
			{
				currIndex = posX + indexMod % 3 + posY * FIELDSIZE + (indexMod / 3) * FIELDSIZE;
				if (data[currIndex] == STATUS::SHIP)
					return false;
			}
		}
		return true;
	}


	bool PlacementPossible(int shipsize, int direction, int start_index)
	{
		// Modifiers which affects transitioning 2d cord into the 1d cords.
		int modX, modY;
		int currPositionIndex, currX, currY;
		// Set modifyers depending on the direction.
		switch (direction)
		{
		// 0 - Up.
		case (0): 
			modX = 0;
			modY = -1;
			break;
			// 0 - Up.
		case (1):
			modX = 1;
			modY = 0;
			break;
			// 0 - Up.
		case (2):
			modX = 0;
			modY = 1;
			break;
			// 0 - Up.
		case (3):
			modX = -1;
			modY = 0;
			break;
		default:
			cout << "Something's wrong I can fell it!\n";
			exit(-1);
			break;
		}

		for (int i = 0; i < shipsize; i++)
		{
			currX = start_index % FIELDSIZE + i * modX;
			currY = start_index / FIELDSIZE + i * modY;
			if (currX > -1 && currX < FIELDSIZE && currY > -1 && currY < FIELDSIZE)
			{
				currPositionIndex = currX + currY * FIELDSIZE;
				if (!PositionSafe(currPositionIndex))
					return false;
			}
			else
				return false;
		}
		return true;
	}


	void PlaceShip(int shipsize, int direction, int start_index)
	{
		int modX, modY;
		switch (direction)
		{
			// 0 - Up.
		case (0):
			modX = 0;
			modY = -1;
			break;
			// 0 - Up.
		case (1):
			modX = 1;
			modY = 0;
			break;
			// 0 - Up.
		case (2):
			modX = 0;
			modY = 1;
			break;
			// 0 - Up.
		case (3):
			modX = -1;
			modY = 0;
			break;
		default:
			cout << "Something's wrong I can fell it, fell it!\n";
			exit(-1);
			break;
		}

		int currX, currY, currPositionIndex;
		for (int i = 0; i < shipsize; i++)
		{
			currX = start_index % FIELDSIZE + i * modX;
			currY = start_index / FIELDSIZE + i * modY;
			currPositionIndex = currX + currY * FIELDSIZE;

			data[currPositionIndex] = STATUS::SHIP;
		}
	}
};

