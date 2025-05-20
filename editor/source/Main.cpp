#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/Input.h>
#include "ui/EditorApplication.h"

#include "engine/ConsoleLog.hpp"

#include <engine/ui/Canvas.h>


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

    engine.Startup();

    

	while (!engine.GetWindow()->ShouldWindowClose())
	{
        engine.UpdateGameplay();

        if (engine::Input::IsInputDown(KEY_LEFT_CONTROL) ||
            engine::Input::IsInputDown(KEY_RIGHT_CONTROL))

        {
            if (engine::Input::IsInputReleased(KEY_B))
            {
                engine.BuildProjectExecutable("..\\testProjectBuildFolder");
            }

            else if (engine::Input::IsInputReleased(KEY_S))
                engine.SaveProject();
        }

        /*if (engine::Input::IsInputDown(KEY_R))
        {
            auto size = engine.GetWindow()->GetSize<f32>();

            engine::Canvas* sceneCanvas = new engine::Canvas(size, math::Vector4f::Zero());

            auto rec = sceneCanvas->AddRectangle(size / 2.f, { 2.f, 2.f });

            rec->SetColor(1.f, 1.f, 1.f, 1.f);
        }*/

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
