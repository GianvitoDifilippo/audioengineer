#include "src\application.h"

using namespace audioengineer;
using namespace core;
using namespace utils;

std::string getVowel(uint index)
{
	switch (index) {
	case 0:
		return "A";
	case 1:
		return "E";
	case 2:
		return "I";
	case 3:
		return "O";
	case 4:
		return "U";
	default:
		return "";
	}
}

int main()
{
	Application* app = new Application();
	app->run();

	delete app;
	system("PAUSE");
	return 0;
}