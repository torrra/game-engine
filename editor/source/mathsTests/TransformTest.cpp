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
	Engine::Transform transformA = Engine::Transform(lm::Vector3f(5.f, 3.2f, 4.1f),
													 lm::Quatf(1.0f, 7.3f, 6.1f, 2.5f),
													 lm::Vector3f(1.0f, 1.0f, 1.0f));

	lm::Matrix4f matrixA = Engine::Transform::ToMatrixWithoutScale(transformA);

	float matrix1[4][4] = { 1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f };

	lm::Matrix4f matrixB = lm::Matrix4f(matrix1);

	bool result = Check(matrixA, matrixB);
	DisplayResult(result, "To Matrix Without Scale Test");

	Engine::Transform transformC = Engine::Transform(lm::Vector3f(5.f, 3.2f, 4.1f),
													 lm::Quatf(1.0f, 7.3f, 6.1f, 2.5f),
													 lm::Vector3f(2.2f, 2.2f, 2.2f));

	lm::Matrix4f matrixC = Engine::Transform::ToMatrixWithScale(transformA);

	float matrix2[4][4] = { 1.0f, 0.0f, 0.0f, 0.0f, 
						   0.0f, 1.0f, 0.0f, 0.0f, 
						   0.0f, 0.0f, 1.0f, 0.0f, 
						   0.0f, 0.0f, 0.0f, 1.0f };

	lm::Matrix4f matrixD = lm::Matrix4f(matrix2);

	result = Check(matrixC, matrixD);
	DisplayResult(result, "To Matrix With Scale Test");
}

void Editor::TransformInterpolateTest()
{
	Engine::Transform transformA = Engine::Transform(lm::Vector3f(5.f, 3.2f, 4.1f),
													 lm::Quatf(1.0f, 7.3f, 6.1f, 2.5f),
													 lm::Vector3f(1.0f, 1.0f, 1.0f));
	Engine::Transform transformB = Engine::Transform(lm::Vector3f(3.0f, 2.2f, 6.0f),
													 lm::Quatf(1.0f, 3.1f, 5.4f, 2.7f),
													 lm::Vector3f(3.3f, 4.4f, 5.5f));

	Engine::Transform transformC = Engine::Transform::Interpolate(transformA, transformB, 0.5f);

	Engine::Transform transformD = Engine::Transform(lm::Vector3f(0.f, 0.f, 0.f),
													 lm::Quatf(1.0f, 0.0f, 0.0f, 0.0f),
													 lm::Vector3f(1.0f, 1.0f, 1.0f));

	bool result =  Check(transformC, transformD);
	DisplayResult(result, "Interpolate Test");
}

void Editor::TransformCopyTest()
{
	Engine::Transform transformA = Engine::Transform(lm::Vector3f(3.0f, 2.2f, 6.0f),
													 lm::Quatf(1.0f, 3.1f, 5.4f, 2.7f),
													 lm::Vector3f(3.3f, 4.4f, 5.5f));
	Engine::Transform transformB = Engine::Transform();

	transformB.CopyPosition(transformA);
	transformB.CopyRotation(transformA);
	transformB.CopyScale(transformA);

	bool result = Check(transformA, transformB);
	DisplayResult(result, "Copy Test");
}

void Editor::TransformGetterTest()
{
	Engine::Transform transformA = Engine::Transform(lm::Vector3f(3.0f, 2.2f, 6.0f),
													 lm::Quatf(1.0f, 3.1f, 5.4f, 2.7f),
													 lm::Vector3f(3.3f, 4.4f, 5.5f));

	lm::Vector3f position = transformA.GetPosition();
	lm::Quatf rotation = transformA.GetRotation();
	lm::Vector3f scale = transformA.GetScale();

	Engine::Transform transformB = Engine::Transform(position, rotation, scale);

	bool result = Check(transformA, transformB);
	DisplayResult(result, "Getter Test");
}

void Editor::TransformSetterTest()
{
	Engine::Transform transformA = Engine::Transform();

	transformA.SetPosition(lm::Vector3f(3.0f, 2.2f, 6.0f));
	transformA.SetRotation(lm::Quatf(1.0f, 3.1f, 5.4f, 2.7f));
	transformA.SetScale(lm::Vector3f(3.3f, 4.4f, 5.5f));

	Engine::Transform transformB = Engine::Transform(lm::Vector3f(3.0f, 2.2f, 6.0f),
													 lm::Quatf(1.0f, 3.1f, 5.4f, 2.7f),
													 lm::Vector3f(3.3f, 4.4f, 5.5f));

	bool result = Check(transformA, transformB);
	DisplayResult(result, "Setter Test");
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
		std::cout << transformA << " ==\n" << transformB << std::endl;

		return true;
	}
	std::cout << transformA << " !=\n" << transformB << std::endl;
	return false;
}

bool Editor::Check(const lm::Matrix4f& matrixA, const lm::Matrix4f& matrixB)
{
	if (matrixA[0][0] == matrixB[0][0] &&
		matrixA[0][1] == matrixB[0][1] &&
		matrixA[0][2] == matrixB[0][2] &&
		matrixA[0][3] == matrixB[0][3] &&
		matrixA[1][0] == matrixB[1][0] &&
		matrixA[1][1] == matrixB[1][1] &&
		matrixA[1][2] == matrixB[1][2] &&
		matrixA[1][3] == matrixB[1][3] &&
		matrixA[2][0] == matrixB[2][0] &&
		matrixA[2][1] == matrixB[2][1] &&
		matrixA[2][2] == matrixB[2][2] &&
		matrixA[2][3] == matrixB[2][3] &&
		matrixA[3][0] == matrixB[3][0] &&
		matrixA[3][1] == matrixB[3][1] &&
		matrixA[3][2] == matrixB[3][2] &&
		matrixA[3][3] == matrixB[3][3])
	{
		DisplayMatrix(matrixA, matrixB);
		return true;
	}
	DisplayMatrix(matrixA, matrixB);
	return false;
}

void Editor::DisplayResult(const bool& result, const std::string& testName)
{
	if (result)
	{
		std::cout << testName << " : OK" << "\n" << std::endl;
	}
	else
	{
		std::cout << testName << " : NOK" << "\n" << std::endl;
	}
}

void Editor::DisplayMatrix(const lm::Matrix4f& matrixA, const lm::Matrix4f& matrixB)
{
	for (int row = 0; row < 4; row++)
	{
		std::cout << "| ";
		for (int col = 0; col < 4; col++)
		{
			std::cout << matrixA[row][col] << " ";
		}
		std::cout << " |" << std::endl;
	}
	std::cout << std::endl;
	for (int row = 0; row < 4; row++)
	{
		std::cout << "| ";
		for (int col = 0; col < 4; col++)
		{
			std::cout << matrixB[row][col] << " ";
		}
		std::cout << " |" << std::endl;
	}
}

std::ostream& Editor::operator<<(std::ostream& os, const Engine::Transform& transform)
{
	return os << "( " << transform.m_position.GetX() << "; " << transform.m_position.GetY() << "; " <<
				 transform.m_position.GetZ() << " )" 
			  << "( " << transform.m_rotation.GetW() << "; " << transform.m_rotation.GetX() << "; " <<
				 transform.m_rotation.GetY() << "; " << transform.m_rotation.GetZ() << " )"
			  << "( " << transform.m_scale.GetX() << "; " << transform.m_scale.GetY() << "; " <<
				 transform.m_scale.GetZ() << " )";
}
