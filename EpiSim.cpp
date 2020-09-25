// EpiSim.cpp : Defines the entry point for the application.
//

#include "EpiSim.h"
#ifdef TRACY_ENABLE
#pragma message("Tracy is enabled")
#endif
int main(int argc, char* argv[])
{
	renderer* rend = new sdlRenderer();
	rend->init(1024,1024);
	human testSubject;
	testSubject.infect_info = infectInfo::infectious;
	testSubject.x = 10;
	testSubject.y = 10;
	while (true) {
		testSubject.x++;
		testSubject.x %= 1024;
		std::vector<human> humans = { testSubject };
		rend->drawScreen(humans);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}
	return 0;
}
