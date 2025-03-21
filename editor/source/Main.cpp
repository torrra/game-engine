#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>

#include <engine/resource/ResourceManager.h>
#include <engine/input/Input.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/texture/Texture.h>
#include <engine/core/systems/ScriptSystem.h>


extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#define MODEL_FILE "..\\assets\\padoru.obj"
#define TEXTURE_FILE "..\\assets\\padoru.png"

#define SHADER_PROGRAM_NAME		"ModelTextured"
#define SHADER_PROGRAM_VERTEX	"..\\engineShader\\ModelTextured.vs"
#define SHADER_PROGRAM_FRAGMENT "..\\engineShader\\ModelTextured.frag"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{

		engine::Engine engine;
		if (engine.Startup("Editor", "C:\\Users\\maLev\\OneDrive\\Desktop\\project\\"))
			return -1;

		engine::Input::RegisterInput(KEY_UP);
		engine::Input::RegisterInput(KEY_DOWN);
		engine::Input::RegisterInput(KEY_LEFT);
		engine::Input::RegisterInput(KEY_RIGHT);
		engine::Input::RegisterInput(KEY_W);
		engine::Input::RegisterInput(KEY_S);
		engine::Input::RegisterInput(KEY_A);
		engine::Input::RegisterInput(KEY_D);
		engine::Input::RegisterInput(KEY_ESCAPE);

		// Load resources
		engine::ResourceManager::Load<engine::Model>(MODEL_FILE);
		engine::ResourceManager::Load<engine::Texture>(TEXTURE_FILE);
		engine::ResourceManager::LoadShader(
			SHADER_PROGRAM_NAME,
			SHADER_PROGRAM_VERTEX, 
			SHADER_PROGRAM_FRAGMENT
		);

		// Entity
		engine::ScriptSystem::CreateUserScript("..\\userScripts\\", "ControllerScript");
		
		engine::EntityHandle entityHandle = engine.GetGraph()->CreateEntity("Padoru");
		engine::Renderer* entityRenderer = engine.GetGraph()->CreateComponent<engine::Renderer>(entityHandle);
		engine::Transform* entityTransform = engine.GetGraph()->CreateComponent<engine::Transform>(entityHandle);
		engine::Script* controllerScript = engine.GetGraph()->CreateComponent<engine::Script>(entityHandle);
		
		controllerScript->AddScriptObject("ControllerScript");
		controllerScript->Start();
	
		entityRenderer->SetModel(MODEL_FILE);
		entityRenderer->SetTexture(TEXTURE_FILE);
		entityRenderer->SetShader(SHADER_PROGRAM_NAME);

		// Camera
		engine::EntityHandle cameraHandle = engine.GetGraph()->CreateEntity("Camera");
		engine::Camera* camera = engine.GetGraph()->CreateComponent<engine::Camera>(cameraHandle);
		engine::Transform* transformCam = engine.GetGraph()->CreateComponent<engine::Transform>(cameraHandle);

		while (!engine.GetWindow()->ShouldWindowClose())
		{
			math::Vector2<float> cursorPos = engine::Input::GetCursorPosition<float>();
			static math::Vector2<float> lastCursorPos(cursorPos);

			math::Vector2<float> deltaPos = cursorPos - lastCursorPos;
			camera->Rotate(
				deltaPos.GetY(),
				deltaPos.GetX(),
				0.0f,
				0.25F
			);

			if (engine::Input::IsInputDown(KEY_UP))
				camera->Move({0.0f, 0.0f, -1.0f}, 1.0f, engine.GetTime().GetDeltaTime());
			else if (engine::Input::IsInputDown(KEY_DOWN))
				camera->Move({0.0f, 0.0f, 1.0f}, 1.0f, engine.GetTime().GetDeltaTime());

			if (engine::Input::IsInputDown(KEY_LEFT))
				camera->Move({-1.0f, 0.0f, 0.0f}, 1.0f, engine.GetTime().GetDeltaTime());
			else if (engine::Input::IsInputDown(KEY_RIGHT))
				camera->Move({1.0f, 0.0f, 0.0f}, 1.0f, engine.GetTime().GetDeltaTime());


			static bool cursorState = true;
			if (engine::Input::IsInputReleased(KEY_ESCAPE))
				cursorState = !cursorState;

			engine::Input::SetCursorMode((cursorState) ? engine::ECursorMode::DISABLED : engine::ECursorMode::NORMAL);

			lastCursorPos = cursorPos;

			engine.Update();
		}

		engine.ShutDown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

	return 0;
}