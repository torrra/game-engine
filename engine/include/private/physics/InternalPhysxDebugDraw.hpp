//#pragma once
//
//#pragma region Engine
//
//#include "CoreTypes.h"
//
//#pragma endregion
//
//#pragma region Resource
//
//#include "resource/model/Buffer.h"
//#include "resource/model/Mesh.h"
//#include "resource/ResourceManager.h"
//#include "resource/shader/Shader.h"
//
//#pragma endregion
//
//#pragma region GLFW
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#pragma endregion
//
//#pragma region PhysX
//
//#include <physx/PxPhysicsAPI.h>
//
//#pragma endregion
//
//#pragma region Internal
//
//#include "InternalPhyxConversion.hpp"
//
//#pragma endregion
//
//#pragma region Core
//
//#include "core/components/Camera.h"
//#include "EngineExport.h"
//
//#pragma endregion
//
//namespace engine
//{
//    class DebugDraw
//    {
//    public :
//
//        ENGINE_API    DebugDraw(void);
//
//        ENGINE_API void UpdateDebugDraw(const physx::PxRenderBuffer& inRenderBuffer);
//        ENGINE_API void RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount);
//
//    private :
//
//        void InitDebugDraw(void);
//
//        ShaderProgram*  m_debugShadeProgram     = nullptr;
//        uint32          m_debugDrawVAO          = 0;
//        uint32          m_debugDrawVBO          = 0;
//
//    }; // !Class DebugDraw
//} // !Namespace engine
//
//engine::DebugDraw::DebugDraw(void)
//{
//    InitDebugDraw();
//}
//
//void engine::DebugDraw::InitDebugDraw(void)
//{
//    glGenVertexArrays(1, &m_debugDrawVAO);
//    glGenBuffers(1, &m_debugDrawVBO);
//
//    glBindVertexArray(m_debugDrawVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, m_debugDrawVBO);
//    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(math::Vector3f), (void*)0);
//
//    glBindVertexArray(0);
//
//    m_debugShadeProgram = new ShaderProgram("..\\engineShader\\DebugShader.vs",
//        "..\\engineShader\\DebugShader.frag");
//}
//
//void engine::DebugDraw::UpdateDebugDraw(const physx::PxRenderBuffer& inRenderBuffer)
//{
//    std::vector<math::Vector3f> lineVertices;
//
//    for (physx::PxU32 lineNum = 0; lineNum < inRenderBuffer.getNbLines(); ++lineNum)
//    {
//        const physx::PxDebugLine& line = inRenderBuffer.getLines()[lineNum];
//        lineVertices.push_back(ToVector3fPos0(line));
//        lineVertices.push_back(ToVector3fPos1(line));
//    }
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_debugDrawVBO);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(math::Vector3f),
//        lineVertices.data());
//}
//
//void engine::DebugDraw::RenderDebugDraw(math::Matrix4f* inProjViewMatrix, uint32 inLineCount)
//{
//    m_debugShadeProgram->Use();
//    m_debugShadeProgram->Set("projViewMatrix", inProjViewMatrix);
//    glBindVertexArray(m_debugDrawVAO);
//    glDrawArrays(GL_LINES, 0, inLineCount);
//    glBindVertexArray(0);
//}