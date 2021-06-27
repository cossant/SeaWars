#include "SeaWManager.h"
#include <string>;

SeaBattleGameflow::SeaBattleGameflow(botbrains* suggested_bot)
{
	{
		string player_responce;
		cout << "���������� ���� � ������� ���.\n ��� ���� ������������ ���������� ���� ������� ����������� �������� �� ������� ����� ���� �����-�������,\n� "
			<<"�������, ������� � ������� �� ����� ����� ������-����.\n������ �� �� ������ ������(Y/N)?: ";
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
	cout << "���� ����������� " << (win == TEAMS::PLAYER ? "����� �������!" : "������� ��������� ������.") << '\n';
	cout << (win == TEAMS::PLAYER ? "�� - ����� ������������!" : "��� ��������� ��� ���� ���. ��������� � :-)") << '\n';
}


bool SeaBattleGameflow::botactions()
{
	bot->generateshot();
	string answ;
	REPORT rep;
	bool gotvalidinput = true;
	do
	{
		cout << "�� �������������, �������� �� ������ ������� ��� ��������� ���?\n(miss/hit/kill): ";
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
			cout << "�� ��������� ����. ����������, ������� ����� ���, ��� � �������\n";
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
	cout << "��� ���. ���� ������ ���������?\n������� ���������� ���� ����� � ������� A1 ��� D10: ";
	cin >> player_input;
	cin.ignore();
	int cordX, cordY;
	cordX = bot->ChLtTransf(player_input[0]);
	cordY = stoi(player_input.substr(1, player_input.length() - 1)) - 1;
	responce = bot->shotconnected(cordX + cordY * FIELDSIZE);
	if ( responce == REPORT::DESTROYED)
		cout << "��� ��������, ��� �� �������� ��� �������! �� ������ ��� ���!\n";
	else if (responce == REPORT::HIT)
		cout << "�� ������ �� ���������� ������� - �� ������ ��� ���!\n";
	else
	{
		cout << "�� ������ - ��� ��������� ����.\n";
		result = false;
	}
	cout << "������� \"Enter\" ��� �����������...\n";
	getline(cin,player_input);
	return result;
}