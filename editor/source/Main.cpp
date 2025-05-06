#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/Input.h>
#include "ui/EditorApplication.h"

#include "engine/ConsoleLog.hpp"

#include "engine/sounds/SoundsEngine.h"
#include "engine/sounds/PlayerAudio.h"

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
    
    engine::SoundEngine::Get().InitSoundEngine();

    engine::EntityHandle music = engine.GetGraph()->CreateEntity("Music");
    engine::PlayerAudio* playerAudio = 
        engine.GetGraph()->CreateComponent<engine::PlayerAudio>(music);

    if (playerAudio->LoadSound("Music", 
                               "..\\workspace\\assets\\music\\Falling_In_Reverse-Im_Not_A_Vampire.mp3"))
        PrintLog(engine::SuccessPreset(), "Music loaded.");

    /* /// ---------------- Create entity ---------------- 
    engine::EntityHandle floor = engine.GetGraph()->GetEntity("Floor")->GetHandle();

    /// ---------------- PhysicsEngine use ---------------- 
    engine::PhysicsEngine::Get().Init();

    /// ---------------- Create rigidbody dynamic ----------------
    std::cout << "\t" << std::endl;

    engine::RigidBodyDynamic* rb = engine::RigidBodyDynamicFactory::CreateDynamic(engine.GetGraph(), 
                                        engine.GetGraph()->GetEntity("Padoru")->GetHandle(), engine::BOX);
    rb->SetDebugVisualization(true);
    rb->SetGravityDisabled(false);

    /// ---------------- Create rigidbody static ----------------
    std::cout << "\t" << std::endl;
    
    engine::RigidBodyStatic* floorRb = engine::RigidBodyStaticFactory::CreateStatic(
                                            engine.GetGraph(), floor, engine::BOX);
    floorRb->SetBoxHalfExtents(math::Vector3f (5.f, 0.5f, 5.f));
    floorRb->SetDebugVisualization(true);

    engine::Camera* camera = engine.GetGraph()->GetComponent<engine::Camera>(engine.GetGraph()->GetEntity("Camera")->GetHandle());*/

    engine.SaveProject();
	while (!engine.GetWindow()->ShouldWindowClose())
	{
        //math::Matrix4f projViewMatrix = camera->ViewProjection();
        engine.UpdateGameplay();

        // Update the debug draw
        //engine::PhysicsEngine::Get().UpdateDebugDraw(&projViewMatrix);

        engine::SoundEngine::Get().UpdateSoundEngine();

        static bool isPlaying = false;
        if (engine::Input::IsInputPressed(MOUSE_BUTTON_LEFT) && !isPlaying)
        {
            playerAudio->PlaySound("Music");
            isPlaying = true;
            engine::PrintLog(engine::InfoPreset(), "Music played.");
        }
        if (engine::Input::IsInputPressed(MOUSE_BUTTON_RIGHT) && isPlaying)
        {

            playerAudio->StopSound("Music");
            isPlaying = false;
            engine::PrintLog(engine::InfoPreset(), "Music stopped.");
        }

        if ((engine::Input::IsInputDown(KEY_LEFT_CONTROL) ||
            engine::Input::IsInputDown(KEY_RIGHT_CONTROL)) &&
            engine::Input::IsInputPressed(KEY_B))
        {
            //engine.BuildProjectExecutable(".\\tests");
            engine.BuildProjectExecutable("..\\testProjectBuildFolder");

        }
        engine.UpdateApplicationWindow();
	}

    engine::SoundEngine::Get().CloseSoundEngine();

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
