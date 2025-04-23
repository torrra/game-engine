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
    public:

        ENGINE_API			Engine(bool withEditor = true);
                            Engine(const Engine&) = delete;
                            Engine(Engine&&) = delete;
        ENGINE_API			~Engine(void) = default;

        ENGINE_API int16	Startup(const char* projectName, const char* projectDir = nullptr, 
                                    uint32 threadCount = DEFAULT_NUM_THREADS);
        ENGINE_API void		ShutDown(void);

        ENGINE_API void			SetProject(const char* projectDir);
        ENGINE_API void         SetEditorApplication(Application* ptr);
        ENGINE_API Window*		GetWindow(void) const noexcept;
        ENGINE_API class SceneGraph* GetGraph(void);

        ENGINE_API void UpdateGameplay(void);
        ENGINE_API void UpdateApplicationWindow(void);

        ENGINE_API void LoadNewScene(bool serialize, const std::filesystem::path& path);

        ENGINE_API void CreateProject(const std::string& dir, const std::string& name);
        ENGINE_API void OpenProject(const std::filesystem::path& projFile);

        ENGINE_API static bool HasEditor(void);

    private:

        inline int16		InitScriptSystem(const char* projectDir);
        inline int16		InitWindow(const char* projectName);
        inline int16		LoadEngineResources(void);


        ENGINE_API void         FreezeUI(void);
        ENGINE_API void         UnfreezeUI(void);

        GameScene         m_activeScene;
        std::string		  m_projectDir;
        Application*      m_application = nullptr;
        UIManager         m_uiManager;

        bool              m_hasEditor;
        std::atomic<bool> m_frozenUI = false;
    };

    extern Engine* g_defaultEngine;
}