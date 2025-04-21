#pragma once

#include <engine/Engine.h>
#include <engine/utility/MemoryCheck.h>

class ExampleProject
{
public:
	ExampleProject(void);
	~ExampleProject(void) = default;

	void StartUp(engine::Engine& engine);
	void Update(engine::Engine& engine);

private:
	void RegisterInputs(void);
	void LoadResources(void);

	engine::EntityHandle m_cameraHandle;
	engine::EntityHandle m_modelHandle;
    engine::EntityHandle m_test1;
    engine::EntityHandle m_test2;
    engine::EntityHandle m_test3;
    engine::EntityHandle m_test4;
};