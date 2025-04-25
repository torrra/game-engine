#include "utility/GraphicsHelperFunctions.h"
#include "input/Input.h"

#include <glad/glad.h>

void engine::SendCmdsToGPU(void)
{
    // Immediately send all commands to GPU & return when complete
    glFlush();
    glFinish();
}

void engine::SetPixelStorageMode(void)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void engine::GetPixelData(math::Vector2i position, math::Vector2i size, void* data)
{
    glReadnPixels(position.GetX(), position.GetY(), size.GetX(), size.GetY(), GL_RGBA, GL_UNSIGNED_BYTE, sizeof(data), data);
}

