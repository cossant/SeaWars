#include "fieldconfig.h";
#include "botlogic.h";

int main()
{
	setlocale(LC_CTYPE, "Russian");
	

	// Testing logic.
	botbrains dummy;
	char temp;
	dummy.initplay();
	do
	{
		dummy.generateshot();
		dummy.display();
		cin >> temp;
		dummy.handlereport(temp == 'h' ? REPORT::HIT : (temp == 'd') ? REPORT::DESTROYED : REPORT::MISS);
		system("cls");
	} while (temp != 'c');
	// EOTesting logic.
}