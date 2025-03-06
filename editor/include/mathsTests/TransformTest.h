#pragma once

#include <maths/Transform.h>

namespace Editor
{
	void TransformConstructorTest();

	void TransformToMatrixTest();

	void TransformInterpolateTest();

	void TransformCopyTest();

	void TransformGetterTest();

	void TransformSetterTest();

	bool Check(const Engine::Transform& transformA, const Engine::Transform& transformB);

	void DisplayResult(const bool& result, const std::string& testName);

} // !Namespace Editor