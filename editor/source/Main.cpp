#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>

#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/rigidbody/RigidBodyDynamic.h"
#include "engine/physics/rigidbody/RigidBodyStatic.h"
#include "engine/physics/Raycast.h"
#include "engine/physics/TriangleMesh.h"

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

#include "engine/core/SceneGraph.h"
#include "engine/core/Entity.h"
#include "engine/core/components/Transform.h"
#include <engine/input/Input.h>
#include "engine/resource/ResourceManager.h"
#include <engine/resource/texture/Texture.h>
#include <engine/core/systems/ScriptSystem.h>
#include "engine/resource/model/Model.h"

#include "Example.h"

#include "engine/ConsoleLog.hpp"

// UI includes
#include "ui/MenuBar.h"
#include "ui/Properties.h"
#include "ui/SceneGraph.h"
#include "ui/Viewport.h"

// HACK: ductape style solution to re-purpose main
#define TESTING_PHYSX 1

// Use dedicated graphics card
extern "C" 
{
	_declspec(dllexport) unsigned int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#if TESTING_PHYSX == 1
int main(void)
{
	// Memory check
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		engine::Engine engine;
		
		if (engine.Startup("Editor", nullptr))
			return -1;

		ExampleProject project;
		project.StartUp(engine);

        /// ---------------- Create entity ---------------- 
        engine::EntityHandle floor = engine.GetGraph()->CreateEntity("Floor");

        /// ---------------- PhysicsEngine use ---------------- 
        engine::PhysicsEngine::Get().Init();

        /// ---------------- Create rigidbody dynamic ----------------
        std::cout << "\t" << std::endl;

        engine::RigidBodyDynamic* rb = engine::RigidBodyDynamicFactory::CreateDynamic(engine.GetGraph(), 
                                            engine.GetGraph()->GetEntity("Padoru")->GetHandle(), engine::BOX);
        rb->SetDebugVisualization(true);
        //rb->SetGravityDisabled(true);

        /// ---------------- Create rigidbody static ----------------
        std::cout << "\t" << std::endl;
        
        engine::RigidBodyStatic* floorRb = engine::RigidBodyStaticFactory::CreateStatic(
                                                engine.GetGraph(), floor, engine::BOX);
        floorRb->SetBoxHalfExtents(math::Vector3f (5.f, 0.5f, 5.f));
        floorRb->SetDebugVisualization(true);

        engine::Camera* camera = engine.GetGraph()->GetComponent<engine::Camera>(engine.GetGraph()->GetEntity("Camera")->GetHandle());

        engine::Raycast* raycast = new engine::Raycast();
        raycast->SetFlags(engine::ERaycastFlags::DYNAMIC);
        engine::Input::RegisterInput(MOUSE_BUTTON_LEFT);

        while (!engine.GetGraph()->GetComponent<engine::Renderer>(engine.GetGraph()->GetEntity("Padoru")->GetHandle())->GetModel()->CanRender())
               engine::ThreadManager::RenderScene(engine.GetGraph());

        engine::TriangleMesh* triangleMesh;
        if (engine.GetGraph()->GetComponent<engine::Renderer>(engine.GetGraph()->GetEntity("Padoru")->GetHandle())->GetModel()->CanRender())
        {
            triangleMesh = new engine::TriangleMesh(engine.GetGraph()->GetEntity("Padoru")->GetHandle(), engine.GetGraph());

            triangleMesh->CreateTriangleMesh();
        }

		while (!engine.GetWindow()->ShouldWindowClose())
		{
            math::Vector3f camPos = camera->GetPosition();
            math::Vector3f camDir = camera->GetRotationQuat().Rotate(-camera->GetPosition().Front());

            camPos.Y() -= 0.5f;

            if (engine::Input::IsInputPressed(MOUSE_BUTTON_LEFT))
            {
                raycast->SetRay(camPos, camDir, 1000.f);
                raycast->HasHit();
            }


            raycast->DrawRay();

            math::Matrix4f projViewMatrix = camera->ViewProjection();
            // Update physics
            engine::PhysicsEngine::Get().StepSimulation(1 / 60.f);
            // Update the entity in regard to the rigid body (gravity for example)
            rb->UpdateEntity();
			project.Update(engine);
			engine.Update();
            // Update the debug draw
            engine::PhysicsEngine::Get().UpdateDebugDraw(&projViewMatrix);
            // Update the rigid body in regard to the entity (movement by keyboard input for example)
            rb->UpdateRigidBody();

            engine.PostUpdate();
		}

        /// ---------------- Clean ---------------- 
        delete rb;
        delete floorRb;
        delete raycast;
        delete triangleMesh;
        engine::PhysicsEngine::Get().CleanUp();

		engine.ShutDown();
	}

	// Memory leak check
	if (!_CrtDumpMemoryLeaks())
		engine::PrintLog(engine::SuccessPreset(), "No memory leak detected.");

	return 0;
}
#else

#define MODEL_FILE ".\\assets\\padoru.obj"
#define TEXTURE_FILE ".\\assets\\padoru.png"
#define CONTROLLER_SCRIPT_FILE "ControllerScript"

#define SHADER_PROGRAM_NAME		"ModelTextured"
#define SHADER_PROGRAM_VERTEX	".\\shaders\\ModelTextured.vert"
#define SHADER_PROGRAM_FRAGMENT ".\\shaders\\ModelTextured.frag"


int main(void)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    {
        engine::Engine engine;
        if (engine.Startup("Editor", nullptr))
            return -1;

        // All the stuff in the following region is temporary & for testing only
#pragma region LoadTemporaryStuff
        // Enable the following keys
        int32 keys[] =
        {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_W, KEY_S, KEY_A, KEY_D, KEY_ESCAPE};

        for (int32 key : keys)
            engine::Input::RegisterInput(key);
        engine::ResourceManager::Load<engine::Model>(MODEL_FILE);
        engine::ResourceManager::Load<engine::Texture>(TEXTURE_FILE);

        engine::ResourceManager::LoadShader(
            SHADER_PROGRAM_NAME,
            SHADER_PROGRAM_VERTEX,
            SHADER_PROGRAM_FRAGMENT
        );

        engine::ScriptSystem::CreateUserScript(".\\userScripts\\", CONTROLLER_SCRIPT_FILE);
        engine::EntityHandle padoruHandle = engine.GetGraph()->CreateEntity("Padoru");
        engine.GetGraph()->CreateComponent<engine::Transform>(padoruHandle)->SetPosition(math::Vector3f(1.5f, 3.0f, 1.0f));
        engine::Renderer* entityRenderer = engine.GetGraph()->CreateComponent<engine::Renderer>(padoruHandle);
        engine::Script* controllerScript = engine.GetGraph()->CreateComponent<engine::Script>(padoruHandle);

        controllerScript->AddScriptObject(CONTROLLER_SCRIPT_FILE);
        controllerScript->Start();

        entityRenderer->SetModel(MODEL_FILE);
        entityRenderer->SetShader(SHADER_PROGRAM_NAME);

        engine::EntityHandle cameraHandle = engine.GetGraph()->CreateEntity("Camera");
        engine::Camera* camera = engine.GetGraph()->CreateComponent<engine::Camera>(cameraHandle);
        engine.GetGraph()->CreateComponent<engine::Transform>(cameraHandle);
        camera->Rotation() = {-10.5f, 26.5f, 0.0f};
        camera->Position() = {1.5f, 3.0f, 3.2f};
#pragma endregion

        editor::MenuBar menuBar;
        editor::SceneGraphUI sceneGraph("Scene Graph", engine.GetGraph());
        editor::PropertyWnd properties(engine.GetGraph());
        editor::Viewport* viewport = new editor::Viewport("Viewport", {0.1f, 0.1f, 0.1f, 1.0f});

        while (!engine.GetWindow()->ShouldWindowClose())
        {
            engine.Update();

            // Viewport
            viewport->RenderToViewport(engine.GetGraph());

            // Main menu bar ui
            menuBar.Render(engine);

            // Scene graph window ui
            sceneGraph.Render();

            // Property window ui
            if (sceneGraph.IsNewEntitySelected())
                properties.SetHandle(sceneGraph.GetSelectedEntity());

            properties.Render();


            engine.PostUpdate(viewport);
        }

        if (viewport)
            delete viewport;

        engine.ShutDown();
    }

    // Memory leak check
    if (!_CrtDumpMemoryLeaks())
        std::printf("No memory leaks found\n");

    return 0;
}
#endif