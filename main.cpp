#include "fieldconfig.h";
#include "botlogic.h";

int main()
{
	setlocale(LC_CTYPE, "Russian");
	

	// Testing logic.
	botbrains dummy;
	dummy.initplay();
	char answ;
	string _;
	cin >> answ;
	while (answ != 'c')
	{
		dummy.generateshot();
		dummy.display();
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
		cout << endl;
	}
	// EOTesting logic.
}