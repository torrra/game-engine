#include <engine/Window.h>
#include <engine/utility/MemoryCheck.h>

#include <engine/resource/shader/Shader.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/texture/Texture.h>
#include <engine/resource/ResourceManager.h>
#include <engine/input/Input.h>
#include <engine/camera/CameraV2.h>
#include <engine/utility/Timer.h>

extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
	// TODO: move all main code
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	engine::Window window("Mustang Editor", 960, 540);
	
	engine::ShaderProgram shader("Default.vs", "Default.frag");
	
	engine::ResourceManager::Load<engine::Model>("padoru.obj");
	engine::ResourceManager::Load<engine::Texture>("padoru.png");

	engine::Input::RegisterInput(KEY_W);
	engine::Input::RegisterInput(KEY_A);
	engine::Input::RegisterInput(KEY_S);
	engine::Input::RegisterInput(KEY_D);
	engine::Input::RegisterInput(KEY_E);
	engine::Input::RegisterInput(KEY_Q);
	engine::Input::RegisterInput(KEY_ESCAPE);

	engine::Input::SetCursorMode(engine::ECursorMode::DISABLED);

	engine::Frustum frustum(0.01f, 250.0f, 80.0f, window.GetAspectRatio());
	engine::CameraV2 camera(frustum, {0.0f, 0.0f, -2.5f}, 2.5f, 30.0f);
	

	//engine::Camera camera({0.0f, 0.0f, 0.0f}, 2.5f);

	math::Matrix4f cameraVal;
	cameraVal.Identity();

	while (!window.ShouldWindowClose())
	{
		if (engine::Input::IsInputHeld(KEY_W))
			camera.Move({0.0f, 0.0f, -1.0f});
		else if (engine::Input::IsInputHeld(KEY_S))
			camera.Move({0.0f, 0.0f, 1.0f});
		if (engine::Input::IsInputHeld(KEY_A))
			camera.Move({-1.0f, 0.0f, 0.0f});
		else if (engine::Input::IsInputHeld(KEY_D))
			camera.Move({1.0f, 0.0f, 0.0f});
		
		math::Vector2<float> cursorPos = engine::Input::GetCursorPosition<float>();
		static math::Vector2<float> lastCursorPos(cursorPos);

		math::Vector2<float> deltaPos = cursorPos - lastCursorPos;

		lastCursorPos = cursorPos;
		static float pitch = 0.0f;
		
		// Rotate camera, pass the amount to rotate camera
		camera.Rotate({
			deltaPos.GetY(),
			deltaPos.GetX(),
			0.0f
		});
		
		
		pitch += 0.1f;
		auto viewProjection = camera.ViewProjection();

		if (engine::Input::IsInputPressed(KEY_ESCAPE))
			break;

		window.ClearWindow();
		shader.Use();
		shader.Set("viewPos", camera.GetPosition());
		shader.Set("lightPos", math::Vector3<float>(0.0f, 5.0f, 0.0f));
		shader.Set("lightColor", math::Vector3<float>(1.0f, 1.0f, 1.0f));
		shader.Set("objectColor", math::Vector3<float>(0.0f, 0.0f, 1.0f));
		shader.Set("model", &cameraVal);
		shader.Set("viewProjection", &viewProjection);

		engine::ResourceManager::GetResource<engine::Texture>("padoru.png")->UseTexture();
		engine::ResourceManager::GetResource<engine::Model>("padoru.obj")->Update();

		window.UpdateBuffers();
	}

	engine::Input::ShutDownInputManager();
	engine::ResourceManager::CloseResourceManager();

	window.Shutdown();

	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

	return 0;
}
