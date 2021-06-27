#include "SeaWManager.h"
#include <string>;

SeaBattleGameflow::SeaBattleGameflow(botbrains* suggested_bot)
{
	{
		string player_responce;
		cout << "Начинается игра в морской бой.\n Для игры используется индексация поля буквами АНГЛИЙСКОГО алфавита по ВЕРХНЕЙ ГРАНИ поля СЛЕВА-НАПРАВО,\nи "
			<<"цифрами, начиная с единицы по ЛЕВОЙ грани СВЕРХУ-ВНИЗ.\nХотите ли вы ходить первым(Y/N)?: ";
		cin >> player_responce;
		cin.ignore();
		if (player_responce[0] == 'Y' || player_responce[0] == 'y') botturn = false;
		else botturn = true;
		bot = suggested_bot;
		suggested_bot->initplay();
	}
}

void SeaBattleGameflow::rungame()
{
	while (!bot->gamefinish())
	{
		bot->display();
		bool successfull_shot =  botturn ? botactions() : playeractions();
		if (!successfull_shot)
			if (botturn == true) botturn = false;
			else botturn = true;
	}
	if (botturn) gameoutcome(TEAMS::BOT);
	else gameoutcome(TEAMS::PLAYER);
}

void SeaBattleGameflow::gameoutcome(TEAMS win)
{
	cout << "Игра завершается " << (win == TEAMS::PLAYER ? "вашей победой!" : "победой бездушной машины.") << '\n';
	cout << (win == TEAMS::PLAYER ? "Вы - герой человечества!" : "Она старалась изо всех сил. Похвалите её :-)") << '\n';
}


bool SeaBattleGameflow::botactions()
{
	bot->generateshot();
	string answ;
	REPORT rep;
	bool gotvalidinput = true;
	do
	{
		cout << "Он промахивается, попадает по вашему кораблю или разрушает его?\n(miss/hit/kill): ";
		cin >> answ;
		cin.ignore();
		if (answ[0] == 'm')
		{
			rep = REPORT::MISS;
			gotvalidinput = true;
		}
		else if (answ[0] == 'h')
		{
			rep = REPORT::HIT;
			gotvalidinput = true;
		}
		else if (answ[0] == 'k')
		{
			rep = REPORT::DESTROYED;
			gotvalidinput = true;
		}
		else gotvalidinput = false;
		if (!gotvalidinput)
			cout << "Не распознан ввод. Пожалуйста, вводите слово так, как в примере\n";
	} while (!gotvalidinput);
	bot->handlereport(rep);
	if (rep == REPORT::MISS)
		return false;
	else
		return true;
}


bool SeaBattleGameflow::playeractions()
{
	bool result = true;
	string player_input;
	REPORT responce;
	cout << "Ваш ход. Куда будете атаковать?\nВведите координаты поля атаки в формате A1 или D10: ";
	cin >> player_input;
	cin.ignore();
	int cordX, cordY;
	cordX = bot->ChLtTransf(player_input[0]);
	cordY = stoi(player_input.substr(1, player_input.length() - 1)) - 1;
	responce = bot->shotconnected(cordX + cordY * FIELDSIZE);
	if ( responce == REPORT::DESTROYED)
		cout << "Бот заявляет, что вы потопили его корабль! Вы ходите ещё раз!\n";
	else if (responce == REPORT::HIT)
		cout << "Вы попали по вражескому кораблю - вы ходите ещё раз!\n";
	else
	{
		cout << "Не попали - ход переходит боту.\n";
		result = false;
	}
	cout << "Нажмите \"Enter\" для продолжения...\n";
	getline(cin,player_input);
	return result;
}