#include <engine/Window.h>
#include <engine/utility/MemoryCheck.h>

int main(void)
{
	// TODO: move all main code
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	engine::Window window("Mustang Editor", 960, 540);

	while (!window.ShouldWindowClose())
	{
		window.ClearWindow();

		window.UpdateBuffers();
	}

	window.Shutdown();

	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

	return 0;
}