#include "botlogic.h";


class botbrains::sidePool
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


botbrains::botbrains() 
{	
	sides = new sidePool();
}


REPORT botbrains::shotconnected(int fieldindex)
{
	if (botfield->data[fieldindex] == STATUS::SHIP)
	{
		botfield->data[fieldindex] = STATUS::RECKS;
		if (botfield->PositionSafe(fieldindex))
			return REPORT::DESTROYED;
		else
			return REPORT::HIT;
	}

	botfield->data[fieldindex] = STATUS::MISSED_SHOT;


	return REPORT::MISS;
}


void botbrains::initplay()
{
	if (botfield != NULL || playfield != NULL)
	{
		delete botfield, playfield;
	}
	botfield = new fconfig();
	botfield->generateBotField();
	playfield = new fconfig();

	// Filling player's field with water.
	for (int i = 0; i < FIELDSIZE * FIELDSIZE; i++)
		playfield->data[i] = STATUS::SEA;

	shot_index = -1;
	gotpinch = false;
	sides->reset();
	other_side_length = 0;
	// Setting current ship which bot are aiming for as no ship.
	for (int i = 0; i < 4; i++) ActShpCord[i] = -1;
	ActShpLen = 0;

	// (-1) - UNDEF direction.
	pinch_dir = -1;
	// Impossible index.
	pinch_index = -1;
	// Setting all player ships as "Alive".
	for (int i = 0; i < 10; i++) PShpAlive[i] = true;
}


// Each scenario should be finished with output "The bot is shooting the <tile_cords> position".
void botbrains::generateshot()
{
	// Continuing to shoot pinched target.
	if (gotpinch)
	{
		// Error handler.
		if (pinch_dir == -1)
		{
			cout << "Error: Got into the \"pinch\" block without any direction set.";
			exit(-1);
		}

		int modX, modY, currX, currY;
		switch (pinch_dir)
		{
			// 0 - Up.
		case (0):
			modX = 0;
			modY = -1;
			break;
			// 0 - right.
		case (1):
			modX = 1;
			modY = 0;
			break;
			// 0 - down.
		case (2):
			modX = 0;
			modY = 1;
			break;
			// 0 - left.
		case (3):
			modX = -1;
			modY = 0;
			break;
		default:
			cout << "Something's wrong I can feel it, but this time in botlogic!\n";
			exit(-1);
			break;
		}

		currX = pinch_index % FIELDSIZE + modX * ActShpLen;
		currY = pinch_index / FIELDSIZE + modY * ActShpLen;
		if (currX > -1 && currX < FIELDSIZE && currY > -1 && currY < FIELDSIZE)
		{
			shot_index = currX + currY * FIELDSIZE;
			cout << "Бот совершает выстрел по позиции " << ChLtTransf((shot_index % FIELDSIZE)) << (shot_index) / FIELDSIZE + 1 << ".\n";
		}
		else
		{
			// Invert direction.
			pinch_dir = invertdir(pinch_dir);
			other_side_length = ActShpLen - 1;
			ActShpLen = 1;
			generateshot();
		}

	}
	// Random shot.
	else
	{
		shot_index = randomSeaPos();
		cout << "Бот совершает выстрел по позиции " << ChLtTransf((shot_index % FIELDSIZE)) << (shot_index / FIELDSIZE) + 1 << ".\n";
	}
	playfield->data[shot_index] = STATUS::ACTIVE_SHOT;
}


void botbrains::handlereport(REPORT result)
{
	// Miss scenario.
	if (result == REPORT::MISS)
	{
		playfield->data[shot_index] = STATUS::MISSED_SHOT;
		if (gotpinch)
		{
			// If some direction was already chosen.
			if ((ActShpLen + other_side_length) > 1)
			{
				pinch_dir = invertdir(pinch_dir);
				other_side_length = ActShpLen - 1;
				ActShpLen = 1;
			}
			else
			{
				// Chosing another direction.
				do
				{
					pinch_dir = sides->side();
				} while (!sideSafe(pinch_dir, pinch_index));
			}
		}
	}
	// Any hit scenario.
	else
	{
		playfield->data[shot_index] = STATUS::RECKS;
		ActShpCord[ActShpLen++ + other_side_length] = shot_index;
		if (!gotpinch)
		{
			do
			{
				pinch_dir = sides->side();
				pinch_index = shot_index;
			} while (!sideSafe(pinch_dir, pinch_index));
		}
		gotpinch = true;
		// The advanced scenario with actions to handle "destroyed ship" situation.
		if (result == REPORT::DESTROYED)
		{
			// Handle the two sides destruction situation.
			ActShpLen = ActShpLen + other_side_length;

			// Mark such leghth ship as destructed.
			int arrindex = 0;
			while ((ships[arrindex] != ActShpLen) || (PShpAlive[arrindex] == false))
			{
				arrindex++;
			}
			PShpAlive[arrindex] = false;

			// Place the "Miss" state everywhere around the destryed ship.
			for (int i = 0; i < ActShpLen; i++)
			{
				if (ActShpCord[i] != -1)
					battleMark(ActShpCord[i]);
				else
				{
					cout << "Error: Trying to place battleMark in undefined position.";
					exit(-1);
				}
				ActShpCord[i] = -1;
			}

			// Reset the destroyed deck counter.
			ActShpLen = 0;
			other_side_length = 0;

			// Reset the directions pool and leave the "pinch" state.
			sides->reset();
			pinch_dir = -1;
			gotpinch = false;
		}
	}
}


void botbrains::display()
{
	cout << "Bot vision field for debug purposes\n";
	cout << "Player field:\n";
	playfield->display();
	cout << "Bot field:\n";
	botfield->display();
}


// To prevent collizions with previos seted misses.
bool botbrains::sideSafe(int sidecode, int field_index)
{
	int modX, modY, currX, currY;
	switch (sidecode)
	{
		// 0 - Up.
	case (0):
		modX = 0;
		modY = -1;
		break;
		// 0 - right.
	case (1):
		modX = 1;
		modY = 0;
		break;
		// 0 - down.
	case (2):
		modX = 0;
		modY = 1;
		break;
		// 0 - left.
	case (3):
		modX = -1;
		modY = 0;
		break;
	default:
		cout << "Something's wrong I can feel it, but this time in botlogic!\n";
		exit(-1);
		break;
	}
	currX = field_index % FIELDSIZE + modX;
	currY = field_index / FIELDSIZE + modY;
	if (playfield->data[currX + currY * FIELDSIZE] != STATUS::SEA)
		return false;
	return true;
}


int botbrains::invertdir(int currdir)
{
	// Invert direction.
	switch (currdir)
	{
	case (0):
		return 2;
		break;
	case (1):
		return 3;
		break;
	case (2):
		return 0;
	case (3):
		return 1;
		break;
	default:
		cout << "Uncnown direction inversion input.";
		exit(-1);
		break;
	}
}


int botbrains::randomSeaPos()
{
	// Random modifyer. 
	int RMod = rand() % playfield->count(STATUS::SEA);
	int chosenPos = 0;
	do
	{
		if (playfield->data[chosenPos] == STATUS::SEA)
			RMod--;
		if (RMod > -1)
			chosenPos++;
		if (chosenPos > FIELDSIZE * FIELDSIZE - 1)
			chosenPos = 0;
	} while (RMod > -1);
	return chosenPos;
}


void botbrains::battleMark(int index)
{
	int posX = index % 10 - 1;
	int posY = index / 10 - 1;
	// Impossible index.
	int currIndex = -1;
	for (int indexMod = 0; indexMod < 9; indexMod++)
	{
		// If modified position is still on field.
		if ((posX + indexMod % 3) > -1 && (posX + indexMod % 3) < FIELDSIZE && (posY + indexMod / 3) > -1 && (posY + indexMod / 3) < FIELDSIZE)
		{
			currIndex = posX + indexMod % 3 + posY * FIELDSIZE + (indexMod / 3) * FIELDSIZE;
			if (playfield->data[currIndex] != STATUS::RECKS)
				playfield->data[currIndex] = STATUS::MISSED_SHOT;

			// Alarm catch.
			if (playfield->data[currIndex] == STATUS::SHIP || playfield->data[currIndex] == STATUS::BLANK)
			{
				cout << "Uncorrect battleMark input data - impossible positions met.";
				exit(-1);
			}
		}
	}
}


int botbrains::ChLtTransf(char input)
{
	switch (input)
	{
	case ('A'):
		return 0;
		break;
	case ('B'):
		return 1;
		break;
	case ('C'):
		return 2;
		break;
	case ('D'):
		return 3;
		break;
	case ('E'):
		return 4;
		break;
	case ('F'):
		return 5;
		break;
	case ('G'):
		return 6;
		break;
	case ('H'):
		return 7;
		break;
	case ('I'):
		return 8;
		break;
	case ('J'):
		return 9;
		break;
	default:
		cout << "Wait, thats illegal!";
		exit(-1);
		break;
	}
}

char botbrains::ChLtTransf(int input)
{
	switch (input)
	{
	case (0):
		return 'A';
		break;
	case (1):
		return 'B';
		break;
	case (2):
		return 'C';
		break;
	case (3):
		return 'D';
		break;
	case (4):
		return 'E';
		break;
	case (5):
		return 'F';
		break;
	case (6):
		return 'G';
		break;
	case (7):
		return 'H';
		break;
	case (8):
		return 'I';
		break;
	case (9):
		return 'J';
		break;
	default:
		cout << "Wait, thats totally illegal!";
		exit(-1);
		break;
	}
}


bool botbrains::gamefinish()
{
	bool statement1 = true;
	bool statement2 = true;
	for (int i = 0; i < 10; i++)
		if (PShpAlive[i])
			statement1 = false;
	if (botfield->count(STATUS::SHIP))
		statement2 = false;
	return (statement1 || statement2);
}