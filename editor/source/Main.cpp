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
#include <engine/ui/elements/Rect.h>
#include <engine/ui/elements/ProgressBar.h>


#include <engine/utility/Timer.h>

extern "C" {
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#define UI_TEST 1
#define SCRIPT_TEST 0

#if SCRIPT_TEST == 1
#include <iostream>
#include <functional>

#include <engine/core/SceneGraph.h>
#include <engine/core/systems/ScriptSystem.h>

void Func(void)
{
	std::printf("Func invoked!\n");
}

void Invoke(std::function<void(void)> function)
{
	function();
}

engine::SceneGraph* graph = new engine::SceneGraph();

int main(void)
{
	Invoke(&Func);

	// Set user-defined script location
	engine::ScriptSystem::SetUserScriptLocation("../userScripts/");
	engine::ScriptSystem::Startup();
	engine::ScriptSystem::SetCurrentScene(graph);

	// Test if new script files open properly
	engine::ScriptSystem::CreateUserScript("", "TestOpenFile");
	auto test = graph->CreateEntity("TestEntity");
	
	engine::ScriptSystem::StartScript(test);

	// Lua interpreter
	engine::ScriptSystem::RunInterpreter();
	engine::ScriptSystem::Shutdown();
	delete graph;
	return 0;
}
#endif

#if UI_TEST == 1
void func(void)
{
	std::printf("button pressed\n");
}


int main(void)
{
	// TODO: move all main code
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		engine::Window window("Mustang Editor", 960, 540);
		engine::UIManager uiManager(window.GetWindowPtr());

		engine::ShaderProgram shader("..\\engineShader\\Default.vs", "..\\engineShader\\Default.frag");

		engine::ResourceManager::Load<engine::Model>("..\\assets\\padoru.obj");
		engine::ResourceManager::Load<engine::Texture>("..\\assets\\padoru.png");
		engine::ResourceManager::Load<engine::Texture>("..\\assets\\full-moon-png-6.png");
		engine::ResourceManager::Load<engine::Font>("..\\fonts\\SourceSans3-Regular.ttf");
		engine::ResourceManager::Load<engine::Font>("..\\fonts\\Pacifico-Regular.ttf");

		engine::Input::RegisterInput(KEY_W);
		engine::Input::RegisterInput(KEY_A);
		engine::Input::RegisterInput(KEY_S);
		engine::Input::RegisterInput(KEY_D);
		engine::Input::RegisterInput(KEY_E);
		engine::Input::RegisterInput(KEY_Q);
		engine::Input::RegisterInput(KEY_T);
		engine::Input::RegisterInput(KEY_ESCAPE);

		//engine::Input::SetCursorMode(engine::ECursorMode::DISABLED);

		engine::Canvas canvas({window.GetWidth<float>(), window.GetHeight<float>()}, { 0.0f, 0.0f, 0.0f, 0.0f });
		engine::Image* image = canvas.AddImage("..\\assets\\full-moon-png-6.png");
		image->SetPosition({800.0f, 100.0f});
		image->SetScale({100.0f, 100.0f});
		engine::Label* label = canvas.AddLabel("Text");
		label->SetPosition({800.0f, 200.0f});
		label->SetTextColor(0.0f, 1.0f, 0.0f, 1.0f);
		label->SetFont("..\\fonts\\SourceSans3-Regular.ttf", 18.0f);
		label->WrapText(500.0f);
		engine::Button* button = canvas.AddButton("Button test text eee", &func);
		button->SetPosition({200.0f, 200.0f});
		button->SetPadding({0, 0});
		button->SetRounding(1.0f);

		f32 val = 1.0f;
		engine::ProgressBar* bar = canvas.AddProgressBar({10.0f, 0.0f}, {940.0f, 15.0f}, {0.0f, 100.0f});
		bar->SetBgColor(RED, 1.0f);
		bar->SetFillColor(GREEN, 1.0f);
		bar->SetValue(val);

		math::Vector2f scale(450.0f, 300.0f);
		math::Vector2f center(
			(window.GetWidth() * 0.5f) - (scale[0] * 0.5f),
			(window.GetHeight() * 0.5f) - (scale[1] * 0.5f)
		);

		engine::Rectangle* rect = canvas.AddRectangle(center, scale);
		rect->SetColor(1.0f, 0.0f, 0.0f, 0.5f);

		engine::Frustum frustum(0.01f, 250.0f, 60.0f, window.GetAspectRatio());
		engine::Camera camera(frustum, {0.0f, 0.0f, -0.5f}, 1.f, 80.0f);
		math::Matrix4f cameraVal;
		cameraVal.Identity();

		while (!window.ShouldWindowClose())
		{
			if (engine::Input::IsInputDown(KEY_W))
				camera.Move(0.0f, 0.0f, -1.0f);
			else if (engine::Input::IsInputDown(KEY_S))
				camera.Move(0.0f, 0.0f, 1.0f);
			if (engine::Input::IsInputDown(KEY_A))
				camera.Move(-1.0f, 0.0f, 0.0f);
			else if (engine::Input::IsInputDown(KEY_D))
				camera.Move(1.0f, 0.0f, 0.0f);

			if (engine::Input::IsInputReleased(KEY_T))
			{
				val = (val > 0.0f) ?  val - 0.1f : 0.0f;
				bar->SetValue(val);
			}
		
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
		
			engine::ResourceManager::GetResource<engine::Texture>("..\\assets\\padoru.png")->UseTexture();
			engine::ResourceManager::GetResource<engine::Model>("..\\assets\\padoru.obj")->Update();
		
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
#endif