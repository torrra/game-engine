#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/Input.h>
#include "ui/EditorApplication.h"


#include "engine/physics/PhysicsEngine.h"


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
    engine.SetEditorApplication(new editor::EditorApplication("Mustang Editor"));

    engine.OpenProject("..\\testMustangProject\\superSeriousGame.mustang");
    engine.Startup();
    
    /// ---------------- Create entity ---------------- 
    //engine::EntityHandle floor = engine.GetGraph()->GetEntity("Floor")->GetHandle();

    ///// ---------------- Create rigidbody dynamic ----------------
    //std::cout << "\t" << std::endl;

    //engine::RigidBodyDynamic* rb = engine::RigidBodyDynamicFactory::CreateDynamic(engine.GetGraph(), 
    //                                    engine.GetGraph()->GetEntity("Padoru")->GetHandle(), engine::BOX);
    //rb->SetDebugVisualization(true);
    //rb->SetGravityDisabled(false);

    ///// ---------------- Create rigidbody static ----------------
    //std::cout << "\t" << std::endl;
    //
    //engine::RigidBodyStatic* floorRb = engine::RigidBodyStaticFactory::CreateStatic(
    //                                        engine.GetGraph(), floor, engine::BOX);
    //floorRb->SetBoxHalfExtents(math::Vector3f (5.f, 0.5f, 5.f));
    //floorRb->SetDebugVisualization(true);

    engine::Camera* camera = engine.GetGraph()->GetComponent<engine::Camera>(engine.GetGraph()->GetEntity("Camera")->GetHandle());

    engine.SaveProject();
	while (!engine.GetWindow()->ShouldWindowClose())
	{
        engine.UpdateGameplay();

        if ((engine::Input::IsInputDown(KEY_LEFT_CONTROL) ||
            engine::Input::IsInputDown(KEY_RIGHT_CONTROL)) &&
            engine::Input::IsInputPressed(KEY_B))
        {
            //engine.BuildProjectExecutable(".\\tests");
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
