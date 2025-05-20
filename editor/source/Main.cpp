#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/InputHandler.h>
#include "ui/EditorApplication.h"

#include "engine/ConsoleLog.hpp"

// Use dedicated graphics card
extern "C" 
{
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int appMain(void)
{
	engine::Engine engine;
    engine.SetEditorApplication(new editor::EditorApplication("Mustang Editor", engine.GetGraph()));
    //engine.OpenProject("..\\testMustangProject\\superSeriousGame.mustang");
    engine.Startup();

	while (!engine.GetWindow()->ShouldWindowClose())
	{
        engine.UpdateGameplay();

        if (engine::InputHandler::IsInputPressed(KEY_SPACE))
        {
            engine.GetGraph()->GetComponent<engine::RigidBodyDynamic>(engine.GetGraph()->GetEntity("Padoru")->GetHandle())->AddTorque({ 0.f, 10.f, 0.f });
        }

        if ((engine::InputHandler::IsInputHeld(KEY_LEFT_CONTROL) ||
            engine::InputHandler::IsInputHeld(KEY_RIGHT_CONTROL)) &&
            engine::InputHandler::IsInputPressed(KEY_B))
        {
            engine.BuildProjectExecutable("..\\testProjectBuildFolder");
        }
        
        engine.UpdateApplicationWindow();
	}

	engine.ShutDown();
	return 0;
}

int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    int res = appMain();

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		engine::PrintLog(engine::SuccessPreset(), "No memory leak detected.");
    
    return res;
}
