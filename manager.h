#pragma once
#include <iostream>;
#include <string>;
#include <random>;
#include <ctime>;

using namespace std;

enum class STATUS { BLANK, SEA, SHIP, RECKS };

const int FIELDSIZE = 10;

class fconfig
{
public:
	fconfig() 
	{
		srand(time(NULL));
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
	}
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
	STATUS data[FIELDSIZE * FIELDSIZE];
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
};