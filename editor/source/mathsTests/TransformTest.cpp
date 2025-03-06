#include <mathsTests/TransformTest.h>

#include <iostream>

void Editor::TransformConstructorTest()
{
	Engine::Transform transformA = Engine::Transform();
	Engine::Transform transformB = Engine::Transform(lm::Vector3f(0.f, 0.f, 0.f), 
													 lm::Quatf(1.0f, 0.0f, 0.0f, 0.0f), 
													 lm::Vector3f(1.0f, 1.0f, 1.0f));
	bool result = Check(transformA, transformB);
	DisplayResult(result, "Empty Constructor Test");

	Engine::Transform transformC = Engine::Transform(lm::Vector3f(3.0f, 2.2f, 6.0f),
													 lm::Quatf(1.0f, 3.1f, 5.4f, 2.7f),
													 lm::Vector3f(3.3f, 4.4f, 5.5f));
	Engine::Transform transformD = Engine::Transform(transformC);
	result = Check(transformC, transformD);
	DisplayResult(result, "Copy Constructor Test");
}

void Editor::TransformToMatrixTest()
{

}

void Editor::TransformInterpolateTest()
{
	Engine::Transform transformA = Engine::Transform(lm::Vector3f(0.f, 0.f, 0.f),
													 lm::Quatf(1.0f, 0.0f, 0.0f, 0.0f),
													 lm::Vector3f(1.0f, 1.0f, 1.0f));
	Engine::Transform transformB = Engine::Transform(lm::Vector3f(0.f, 0.f, 0.f),
													 lm::Quatf(1.0f, 0.0f, 0.0f, 0.0f),
													 lm::Vector3f(1.0f, 1.0f, 1.0f));

	Engine::Transform transformC = Engine::Transform::Interpolate(transformA, transformB, 0.5f);
}

void Editor::TransformCopyTest()
{

}

void Editor::TransformGetterTest()
{

}

void Editor::TransformSetterTest()
{

}

bool Editor::Check(const Engine::Transform& transformA, const Engine::Transform& transformB)
{
	if (transformA.GetPosition().X() == transformB.GetPosition().X() &&
		transformA.GetPosition().Y() == transformB.GetPosition().Y() &&
		transformA.GetPosition().Z() == transformB.GetPosition().Z() &&
		transformA.GetRotation().X() == transformB.GetRotation().X() &&
		transformA.GetRotation().Y() == transformB.GetRotation().Y() &&
		transformA.GetRotation().Z() == transformB.GetRotation().Z() &&
		transformA.GetScale().X() == transformB.GetScale().X() &&
		transformA.GetScale().Y() == transformB.GetScale().Y() &&
		transformA.GetScale().Z() == transformB.GetScale().Z())
	{
		return true;
	}
	return false;
}

void Editor::DisplayResult(const bool& result, const std::string& testName)
{
	if (result)
	{
		std::cout << testName << " : OK" << std::endl;
	}
else
	{
		std::cout << testName << " : NOK" << std::endl;
	}
}
