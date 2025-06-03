#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/input/InputHandler.h>
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

        if (engine::InputHandler::IsInputDown(KEY_LEFT_CONTROL) ||
            engine::InputHandler::IsInputDown(KEY_RIGHT_CONTROL))

        {
            if (engine::InputHandler::IsInputReleased(KEY_S))
                engine.SaveProject();
        }
        if (engine::InputHandler::IsInputReleased(KEY_U))
        {
            if (engine::Entity* padoru = engine.GetGraph()->GetEntity("Padoru"))
            {
                engine::Renderer* renderer = engine.GetGraph()->GetComponent<engine::Renderer>(padoru->GetHandle());
                engine::SkeletonAnimator& animator = renderer->GetAnimator();

                //engine::ResourceRef anim = engine::ResourceManager::GetResource<engine::Animation>("mixamo.com");

                //if (anim)
                
                    animator.PlayAnimation();
                    animator.SetLooped(true);
               //}
            }
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
