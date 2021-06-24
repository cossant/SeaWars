#include "manager.h";

int main()
{
	setlocale(LC_CTYPE, "Russian");
	fconfig * test = new fconfig();
	test->generateBotField();
	test->display();
}