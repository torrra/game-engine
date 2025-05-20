#include "engine/core/components/NavigationPoint.h"

#pragma region Serialization

#include "serialization/TextSerializer.h"

#pragma endregion

#pragma region Internal

#include "InternalOpenGLError.hpp"

#pragma endregion

#pragma region Resource

#include "engine/resource/shader/Shader.h"
#include "engine/resource/ResourceManager.h"

#pragma endregion

engine::NavigationPoint::NavigationPoint(EntityHandle inOwner, SceneGraph* inScene)
{
    inOwner = m_owner;
    inScene = m_currentScene;
}

}

