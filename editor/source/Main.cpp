#include <engine/Window.h>
#include <engine/utility/MemoryCheck.h>

#include <engine/resource/shader/Shader.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/texture/Texture.h>
#include <engine/resource/font/Font.h>
#include <engine/resource/ResourceManager.h>
#include <engine/input/Input.h>
#include <engine/core/components/Camera.h>

#include <engine/ui/UIManager.h>
#include <engine/ui/Canvas.h>
#include <engine/ui/elements/Label.h>
#include <engine/ui/elements/Image.h>

#include <engine/utility/Timer.h>

extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int main(void)
{
	// TODO: move all main code
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{

	
	engine::Window window("Mustang Editor", 960, 540);
	engine::UIManager uiManager(window.GetWindowPtr());

	engine::ShaderProgram shader("Default.vs", "Default.frag");

	engine::ResourceManager::Load<engine::Model>("padoru.obj");
	engine::ResourceManager::Load<engine::Texture>("padoru.png");
	engine::ResourceManager::Load<engine::Texture>("full-moon-png-6.png");
	engine::ResourceManager::Load<engine::Font>("SourceSans3-Regular.ttf");
	engine::ResourceManager::Load<engine::Font>("Pacifico-Regular.ttf");

	engine::Input::RegisterInput(KEY_W);
	engine::Input::RegisterInput(KEY_A);
	engine::Input::RegisterInput(KEY_S);
	engine::Input::RegisterInput(KEY_D);
	engine::Input::RegisterInput(KEY_E);
	engine::Input::RegisterInput(KEY_Q);
	engine::Input::RegisterInput(KEY_ESCAPE);

	//engine::Input::SetCursorMode(engine::ECursorMode::DISABLED);

	engine::Canvas canvas({0.0f, 0.0f, 1.0f, 1.0f});
	engine::Label* label = canvas.AddLabel("I am still unsure how to expose the feature. I don't think it should be in by default on ImGui::Text() because that can be used for so many reason (detailed/intricate layouts). I am also hesitant to introduce new API because there's already variants (TextV, TextColored, etc.) and adding a new one would lead to combinatory.explosion.");
	label->SetPosition({200.0f, 200.0f});
	label->SetTextColor(0.98f, 0.6f, 0.7, 1.0f);
	label->SetFont("Pacifico-Regular.ttf", 18.0f);
	label->WrapText(500.0f);

	engine::Image* image = canvas.AddImage("full-moon-png-6.png");
	image->SetPosition({200.0f, 200.0f});
	image->SetScale({500.0f, 100.0f});

	engine::Frustum frustum(0.01f, 250.0f, 60.0f, window.GetAspectRatio());
	engine::Camera camera(frustum, {0.0f, 0.0f, -2.5f}, 1.f, 80.0f);
	math::Matrix4f cameraVal;
	cameraVal.Identity();

	while (!window.ShouldWindowClose())
	{
		if (engine::Input::IsInputDown(KEY_W))
			camera.Move(0.0f, 0.0f, 1.0f);
		else if (engine::Input::IsInputDown(KEY_S))
			camera.Move(0.0f, 0.0f, -1.0f);
		if (engine::Input::IsInputDown(KEY_A))
			camera.Move(1.0f, 0.0f, 0.0f);
		else if (engine::Input::IsInputDown(KEY_D))
			camera.Move(-1.0f, 0.0f, 0.0f);
	
		math::Vector2<float> cursorPos = engine::Input::GetCursorPosition<float>();
		static math::Vector2<float> lastCursorPos(cursorPos);
		
		math::Vector2<float> deltaPos = cursorPos - lastCursorPos;
	
		// Rotate camera, pass the amount to rotate camera
		camera.Rotate(
			deltaPos.GetY(),
			deltaPos.GetX(),
			0.0f
		);
		lastCursorPos = cursorPos;
		
		auto viewProjection = camera.ViewProjection();
	
		if (engine::Input::IsInputPressed(KEY_ESCAPE))
			break;
	
		window.ClearWindow();
		uiManager.NewFrame();
		canvas.Render();
		shader.Use();
		shader.Set("viewPos", camera.GetPosition());
		shader.Set("lightPos", math::Vector3<float>(0.0f, 5.0f, 0.0f));
		shader.Set("lightColor", math::Vector3<float>(1.0f, 1.0f, 1.0f));
		shader.Set("objectColor", math::Vector3<float>(0.0f, 0.0f, 1.0f));
		shader.Set("model", &cameraVal);
		shader.Set("viewProjection", &viewProjection);
	
		engine::ResourceManager::GetResource<engine::Texture>("padoru.png")->UseTexture();
		engine::ResourceManager::GetResource<engine::Model>("padoru.obj")->Update();
	
		uiManager.UpdateUI();
		engine::Input::ResetKeys();
		window.UpdateBuffers();
	}

	engine::Input::ShutDownInputManager();
	engine::ResourceManager::CloseResourceManager();
	canvas.Clear();
	uiManager.ShutDown();
	window.Shutdown();
	}
	if (!_CrtDumpMemoryLeaks())
		std::printf("No memory leaks detected\n");

	return 0;
}
