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

	bool Check(const lm::Matrix4f& matrixA, const lm::Matrix4f& matrixB);

	void DisplayResult(const bool& result, const std::string& testName);

	void DisplayMatrix(const lm::Matrix4f& matrixA, const lm::Matrix4f& matrixB);

	std::ostream& operator<<(std::ostream& os, const Engine::Transform& transform);

} // !Namespace Editor