#pragma once

#include "engine/EngineExport.h"
#include "engine/resource/Resource.h"
#include "engine/resource/model/DynamicMesh.h"
#include "engine/resource/model/Mesh.h"

#include <vector>

namespace engine
{
    class Model final : public IResource
    {
        
    public:
        ENGINE_API		Model(void) = default;
        ENGINE_API      ~Model(void) = default;

        ENGINE_API void LoadResource(const char* fileName) override;

        ENGINE_API bool CanRender(void) const;

        ENGINE_API bool IsDynamic(void) const;

        // This function is meant to be used by renderer components, and ONLY from the
        // render thread.
        ENGINE_API
        const std::vector<Mesh>& GetStaticMeshes(void) const;

        ENGINE_API
        const std::vector<DynamicMesh>& GetDynamicMeshes(void) const;

    private:

        void            ProcessTextures(const void* scene);
        //void            ProcessMaterials(const void* scene);

        void            ProcessMeshes(const void* scene, const void* node);
        void            WorkerThreadLoad(const std::string& name);
        void            RenderThreadSetup(void);

        std::vector<DynamicMesh>      m_dynamicMeshes;
        std::vector<Mesh>			  m_staticMeshes;
        uint8						  m_loadStatus = 0;
        bool                          m_isDynamic = false;
    };
}