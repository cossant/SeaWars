#include "fieldconfig.h";
#include "botlogic.h";

int main()
{
	setlocale(LC_CTYPE, "Russian");
	

	// Testing logic.
	botbrains dummy;
	dummy.initplay();
	char answ;
	cin >> answ;
	while (answ != 'c')
	{
		dummy.generateshot();
		cout << "Report is: ";
		cin >> answ;
		if (answ == 'h')
			dummy.handlereport(REPORT::HIT);
		else if (answ == 'd')
		{
			dummy.handlereport(REPORT::DESTROYED);
		}
		else
			dummy.handlereport(REPORT::MISS);
		system("cls");
	}
	// EOTesting logic.
}