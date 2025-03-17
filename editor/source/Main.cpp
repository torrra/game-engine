#include <iostream>

#include <engine/utility/MemoryCheck.h>

#include "engine/physics/PhysicsEngine.h"

extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
	std::printf("Starting editor\n");

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		engine::PhysicsEngine::Get().Init(); 

		engine::PhysicsEngine::Get().CleanUp();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

	return 0;
}
