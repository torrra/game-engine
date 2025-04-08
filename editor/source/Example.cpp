#include "Example.h"

#include <engine/resource/ResourceManager.h>
#include <engine/input/Input.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/texture/Texture.h>
#include <engine/core/systems/ScriptSystem.h>

#define MODEL_FILE ".\\assets\\padoru.obj"
#define TEXTURE_FILE ".\\assets\\padoru.png"
#define CONTROLLER_SCRIPT_FILE "ControllerScript"

#define SHADER_PROGRAM_NAME		"ModelTextured"
#define SHADER_PROGRAM_VERTEX	".\\shaders\\ModelTextured.vs"
#define SHADER_PROGRAM_FRAGMENT ".\\shaders\\ModelTextured.frag"

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
    engine.GetGraph()->CreateComponent<engine::Transform>(m_modelHandle)->SetPosition(math::Vector3f(0.f, 5.f, 0.f));
	engine::Renderer* entityRenderer = engine.GetGraph()->CreateComponent<engine::Renderer>(m_modelHandle);
	engine::Script* controllerScript = engine.GetGraph()->CreateComponent<engine::Script>(m_modelHandle);

	controllerScript->AddScriptObject(CONTROLLER_SCRIPT_FILE);
	controllerScript->Start();

	entityRenderer->SetModel(MODEL_FILE);
	entityRenderer->SetTexture(TEXTURE_FILE);
	entityRenderer->SetShader(SHADER_PROGRAM_NAME);

	// Add component to camera entity
	m_cameraHandle = engine.GetGraph()->CreateEntity("Camera");
	engine::Camera* camera = engine.GetGraph()->CreateComponent<engine::Camera>(m_cameraHandle);
	engine.GetGraph()->CreateComponent<engine::Transform>(m_cameraHandle);

	camera->Rotation() = {-10.5f, 26.5f, 0.0f};
	camera->Position() = {1.5f, 3.0f, 3.2f};
}

void ExampleProject::Update(engine::Engine& engine)
{
	// Get camera component
	engine::Camera* camera = engine.GetGraph()->GetComponent<engine::Camera>(m_cameraHandle);
	
	// Rotate camera
	math::Vector2f cursorDelta = engine::Input::GetCursorDeltaPos<f32>();
	camera->Rotate(cursorDelta.GetY(), cursorDelta.GetX(), 0.0f, 0.20F);

	// Change cursor state (lock / unlock)
	static bool cursorState = true;
	if (engine::Input::IsInputReleased(KEY_ESCAPE))
		cursorState = !cursorState;

	engine::Input::SetCursorMode((cursorState) ? 
		engine::ECursorMode::DISABLED : engine::ECursorMode::NORMAL);
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

	engine::ScriptSystem::CreateUserScript(".\\userScripts\\", CONTROLLER_SCRIPT_FILE);
}
