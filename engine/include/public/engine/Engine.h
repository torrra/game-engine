#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/game/GameScene.h"
#include "engine/thread/ThreadManager.h"
#include "engine/ui/Application.h"

#include <string>

namespace engine
{
    class Engine
    {
    private:

        struct ProjectData
        {
            std::string       m_defaultGameScene;
            std::string       m_defaultEditorScene;
            std::string       m_executableName;
        };

    public:

        ENGINE_API			Engine(bool withEditor = true);
                            Engine(const Engine&) = delete;
                            Engine(Engine&&) = delete;
        ENGINE_API			~Engine(void) = default;

        ENGINE_API int16	Startup(const char* projectName, const char* projectDir = nullptr, 
                                    uint32 threadCount = DEFAULT_NUM_THREADS);

        ENGINE_API int16	Startup(uint32 threadCount = DEFAULT_NUM_THREADS);

        ENGINE_API void		ShutDown(void);

        ENGINE_API void         SetEditorApplication(Application* ptr);
        ENGINE_API Window*		GetWindow(void) const noexcept;


        ENGINE_API class SceneGraph*    GetGraph(void);
        ENGINE_API GameScene&           GetCurrentScene(void);

        ENGINE_API void UpdateGameplay(void);
        ENGINE_API void UpdateApplicationWindow(void);

        ENGINE_API void LoadNewScene(bool serialize, const std::filesystem::path& path);
        ENGINE_API void LoadDefaultScene(void);

        ENGINE_API bool CreateProject(const std::string& dir, const std::string& name);
        ENGINE_API void OpenProject(const std::filesystem::path& projFile);
        ENGINE_API void SaveProject(void);
        ENGINE_API void SaveProjectFile(void);

        ENGINE_API static bool HasEditor(void);

        ENGINE_API
        const std::filesystem::path& GetProjectDir(void) const;

        ENGINE_API std::string GetProjectName(void) const;

        ENGINE_API UIManager& GetUIManager(void);

        ENGINE_API void SetExecutableName(const std::string& name);
        ENGINE_API void SetDefaultGameScene(const std::string& relativePath);
        ENGINE_API void SetDefaultEditorScene(const std::string& relativePath);

        ENGINE_API
        void BuildProjectExecutable(const std::filesystem::path& destination);

        // Probably not thread safe
        ENGINE_API static Engine* GetEngine(void);

    private:

        inline int16		LoadEngineResources(void);

        void DeserializeProjectFile(const char* cursor, const char* end);

        GameScene               m_activeScene;

        ProjectData             m_currentProject;
        std::filesystem::path   m_projectDir;
        std::filesystem::path   m_projectFile;
        std::string             m_projectName;

        Application*            m_application = nullptr;
        UIManager               m_uiManager;
        bool                    m_hasEditor;

    };

    extern Engine* g_defaultEngine;
}