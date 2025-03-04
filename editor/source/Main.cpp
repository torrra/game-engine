#include <engine/Window.h>
#include <engine/utility/MemoryCheck.h>

#include <engine/resource/shader/Shader.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/texture/Texture.h>
#include <engine/resource/ResourceManager.h>
#include <engine/input/Input.h>
#include <engine/camera/Camera.h>
#include <engine/utility/Timer.h>

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

	engine::Input::SetCursorMode(engine::ECursorMode::DISABLED);
	engine::Camera camera({0.0f, 0.0f, 0.0f}, 2.5f);

	math::Matrix4f cameraVal;
	cameraVal.Identity();

	while (!window.ShouldWindowClose())
	{
	
		camera.CameraInput(engine::DeltaTime());
		camera.MouseMotion(engine::Input::GetCursorPosition<float>(), engine::DeltaTime());
		
		auto perspective = camera.GetPerspectiveMatrix(0.01f, 250.0f, 60.0f, window.GetAspectRatio());
		auto view = camera.GetViewMatrix();

		window.ClearWindow();
		shader.Use();
		shader.Set("viewPos", camera.GetPosition());
		shader.Set("lightPos", math::Vector3<float>(0.0f, 5.0f, 0.0f));
		shader.Set("lightColor", math::Vector3<float>(1.0f, 1.0f, 1.0f));
		shader.Set("objectColor", math::Vector3<float>(0.0f, 0.0f, 1.0f));
		shader.Set("model", &cameraVal);
		shader.Set("view", &view);
		shader.Set("projection", &perspective);


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
