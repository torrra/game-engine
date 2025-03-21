#include "Example.h"

#include <engine/resource/ResourceManager.h>
#include <engine/input/Input.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/texture/Texture.h>
#include <engine/core/systems/ScriptSystem.h>

#define MODEL_FILE "..\\assets\\padoru.obj"
#define TEXTURE_FILE "..\\assets\\padoru.png"
#define CONTROLLER_SCRIPT_FILE "ControllerScript"

#define SHADER_PROGRAM_NAME		"ModelTextured"
#define SHADER_PROGRAM_VERTEX	"..\\engineShader\\ModelTextured.vs"
#define SHADER_PROGRAM_FRAGMENT "..\\engineShader\\ModelTextured.frag"

ExampleProject::ExampleProject(void)
	: m_cameraHandle(-1), m_modelHandle(-1)
{
}

void ExampleProject::StartUp(engine::Engine& engine)
{
	RegisterInputs();
	LoadResources();

	// Add component to model entity
	m_modelHandle = engine.GetGraph()->CreateEntity("Padoru");
	engine.GetGraph()->CreateComponent<engine::Transform>(m_modelHandle);
	engine::Renderer* entityRenderer = engine.GetGraph()->CreateComponent<engine::Renderer>(m_modelHandle);
	engine::Script* controllerScript = engine.GetGraph()->CreateComponent<engine::Script>(m_modelHandle);

	controllerScript->AddScriptObject(CONTROLLER_SCRIPT_FILE);
	controllerScript->Start();

	entityRenderer->SetModel(MODEL_FILE);
	entityRenderer->SetTexture(TEXTURE_FILE);
	entityRenderer->SetShader(SHADER_PROGRAM_NAME);

	// Add component to camera entity
	m_cameraHandle = engine.GetGraph()->CreateEntity("Camera");
	engine.GetGraph()->CreateComponent<engine::Camera>(m_cameraHandle);
	engine.GetGraph()->CreateComponent<engine::Transform>(m_cameraHandle);
}

void ExampleProject::Update(engine::Engine& engine)
{
	math::Vector2<float> cursorPos = engine::Input::GetCursorPosition<float>();
	static math::Vector2<float> lastCursorPos(cursorPos);
	math::Vector2<float> deltaPos = cursorPos - lastCursorPos;

	engine::Camera* camera = engine.GetGraph()->GetComponent<engine::Camera>(m_cameraHandle);
	
	camera->Rotate(deltaPos.GetY(), deltaPos.GetX(), 0.0f, 0.25F);

	// Move camera
	if (engine::Input::IsInputDown(KEY_UP))
		camera->Move({0.0f, 0.0f, -1.0f}, 1.0f, engine.GetTime().GetDeltaTime());
	else if (engine::Input::IsInputDown(KEY_DOWN))
		camera->Move({0.0f, 0.0f, 1.0f}, 1.0f, engine.GetTime().GetDeltaTime());

	if (engine::Input::IsInputDown(KEY_LEFT))
		camera->Move({-1.0f, 0.0f, 0.0f}, 1.0f, engine.GetTime().GetDeltaTime());
	else if (engine::Input::IsInputDown(KEY_RIGHT))
		camera->Move({1.0f, 0.0f, 0.0f}, 1.0f, engine.GetTime().GetDeltaTime());

	// Change cursor state (lock / unlock)
	static bool cursorState = true;
	if (engine::Input::IsInputReleased(KEY_ESCAPE))
		cursorState = !cursorState;

	engine::Input::SetCursorMode((cursorState) ? 
		engine::ECursorMode::DISABLED : engine::ECursorMode::NORMAL);

	lastCursorPos = cursorPos;
}

void ExampleProject::RegisterInputs(void)
{
	// Enable the following keys
	int32 keys[] = 
		{KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_W, KEY_S, KEY_A, KEY_D, KEY_ESCAPE};

	for (int32 key : keys)
		engine::Input::RegisterInput(key);
}

void ExampleProject::LoadResources(void)
{
	engine::ResourceManager::Load<engine::Model>(MODEL_FILE);
	engine::ResourceManager::Load<engine::Texture>(TEXTURE_FILE);
	
	engine::ResourceManager::LoadShader(
		SHADER_PROGRAM_NAME,
		SHADER_PROGRAM_VERTEX,
		SHADER_PROGRAM_FRAGMENT
	);

	engine::ScriptSystem::CreateUserScript("..\\userScripts\\", CONTROLLER_SCRIPT_FILE);
}
