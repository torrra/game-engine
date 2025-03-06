#include <iostream>

#include "mathsTests/TransformTest.h"

int main(void)
{
	std::printf("Starting editor\n");

	Editor::TransformConstructorTest();
	Editor::TransformToMatrixTest();
	Editor::TransformInterpolateTest();
	Editor::TransformCopyTest();
	Editor::TransformGetterTest();
	Editor::TransformSetterTest();

	return 0;
}