#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/Input.h>
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
    engine.Startup();

    //for (uint64 entityNum = 0; entityNum < 64; ++entityNum)
    //{
    //    engine::EntityHandle newEntity = engine.GetGraph()->CreateEntity("testCollision " + std::to_string(entityNum));
    //    engine::RigidBodyDynamic* newRigidBody = engine.GetGraph()->CreateComponent<engine::RigidBodyDynamic>(newEntity);
    //    newRigidBody->SwitchShape(newRigidBody, engine::EGeometryType::BOX);
    //}

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
