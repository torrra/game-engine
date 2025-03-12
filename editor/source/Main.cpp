#include <engine/utility/MemoryCheck.h>
#include <engine/utility/Timer.h>

#include <engine/Window.h>
#include <engine/input/Input.h>

#include <engine/core/SceneGraph.h>
#include <engine/core/components/Camera.h>

#include <engine/resource/ResourceManager.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/texture/Texture.h>
#include <engine/resource/shader/Shader.h>

#include <engine/core/systems/ScriptSystem.h>

extern "C"
{
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		engine::ScriptSystem::SetUserScriptLocation("../userScripts/");
		engine::ScriptSystem::Startup();

		// Create window

		engine::Window window("Camera Test", 960, 540);
		engine::SceneGraph graph;

		// Load resources
		engine::ShaderProgram shader("Default.vs", "Default.frag");
		engine::ResourceManager::Load<engine::Model>("padoru.obj");
		engine::ResourceManager::Load<engine::Texture>("padoru.png");

		// Register keybinds
		engine::Input::RegisterInput(KEY_W);
		engine::Input::RegisterInput(KEY_A);
		engine::Input::RegisterInput(KEY_S);
		engine::Input::RegisterInput(KEY_D);
		engine::Input::RegisterInput(KEY_ESCAPE);
		engine::Input::SetCursorMode(engine::ECursorMode::DISABLED);



		// Entity to move!!!
		engine::EntityHandle dude = graph.CreateEntity("dude");
		engine::EntityHandle camObject = graph.CreateEntity("cam");


		// Camera stuff
		[[maybe_unused]]
		engine::Frustum frustum(0.01f, 250.0f, 60.0f, window.GetAspectRatio());

		[[maybe_unused]]
		engine::Camera* camera = graph.CreateComponent<engine::Camera>(camObject);
		
		camera->SetFarPlane(frustum.m_far);
		/*camera->SetNearPlane(frustum.m_near);
		camera->SetFOV(frustum.m_fovRad);
		camera->*/

		[[maybe_unused]]
		engine::Transform* transform = graph.CreateComponent<engine::Transform>(dude);
		engine::Transform* transformCam = graph.CreateComponent<engine::Transform>(camObject);

		//transformCam->SetPosition({ 0.f, 1.f, -2.f });

		[[maybe_unused]]
		engine::Renderer* renderer = graph.CreateComponent<engine::Renderer>(dude);

		renderer->SetModel("padoru.obj");
		renderer->SetShader(&shader);
		renderer->SetTexture("padoru.png");

		//shader.Set("objectColor", math::Vector3<float>(0.0f, 0.0f, 1.0f));


		float speed = 5.f;

		engine::Time timer;

		transform = graph.GetComponent<engine::Transform>(dude);

		// Main loop
		while (!window.ShouldWindowClose())
		{
			graph.UpdateComponents<engine::Transform>();
			
			if (engine::Input::IsInputDown(KEY_W))
				transformCam->Move(math::Vector3f{ 0.0f, 0.0f, -1.0f } * (speed * timer.GetDeltaTime()));

			else if (engine::Input::IsInputDown(KEY_S))
				transformCam->Move(math::Vector3f{ 0.0f, 0.0f, 1.0f } * (speed * timer.GetDeltaTime()));

			if (engine::Input::IsInputDown(KEY_A))
				transformCam->Move(math::Vector3f{ -1.0f, 0.0f, 0.0f } * (speed * timer.GetDeltaTime()));

			else if (engine::Input::IsInputDown(KEY_D))
				transformCam->Move(math::Vector3f{ 1.0f, 0.0f, 0.0f } * (speed * timer.GetDeltaTime()));

			math::Vector2<float> cursorPos = engine::Input::GetCursorPosition<float>();
			static math::Vector2<float> lastCursorPos(cursorPos);

			math::Vector2<float> deltaPos = cursorPos - lastCursorPos;

			// Rotate camera, pass the amount to rotate camera
			transformCam->Rotate(
				deltaPos.GetY(),
				deltaPos.GetX(),
				0.0f
			);

			lastCursorPos = cursorPos;

			//std::cout << 

			if (engine::Input::IsInputPressed(KEY_ESCAPE))
				break;

			// Update bg color
			window.ClearWindow(0.2f, 0.2f, 0.2f, 1.f);

			graph.RenderScene();
			engine::Input::ResetKeys();
			window.UpdateBuffers();
			timer.Update();
		}

		// Free memory
		engine::Input::ShutDownInputManager();
		engine::ResourceManager::CloseResourceManager();
		window.Shutdown();
		engine::ScriptSystem::Shutdown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

	return 0;
}