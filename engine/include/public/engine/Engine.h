#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/utility/Timer.h"
#include "engine/thread/ThreadManager.h"
#include "engine/Window.h"
#include "engine/core/SceneGraph.h"
#include "engine/ui/UIManager.h"
#include "engine/viewport/Viewport.h"
#include "engine/ui/UIWindow.h"

#include <string>

namespace engine
{
    class Engine
    {
    public:
        ENGINE_API			Engine(void);
        ENGINE_API			~Engine(void) = default;

        ENGINE_API int16	Startup(const char* projectName, const char* projectDir = nullptr, 
                                    uint32 threadCount = DEFAULT_NUM_THREADS);
        ENGINE_API void		ShutDown(void);
        ENGINE_API void		Update(void);
        ENGINE_API void		PostUpdate(void);
        ENGINE_API void		RunTimeUpdate(void);
                   void		FixedUpdate(void);
        ENGINE_API void         SetGameState(bool run);
        ENGINE_API void			SetProject(const char* projectDir);
        ENGINE_API Window*		GetWindow(void) const noexcept;
        ENGINE_API Time&		GetTime(void) noexcept;
        ENGINE_API SceneGraph*	GetGraph(void);
    private:
        void                RunGame();
        inline int16		InitScriptSystem(const char* projectDir);
        inline int16		InitWindow(const char* projectName);
        inline int16		LoadEngineResources(void);

        std::string		m_projectDir;
        Window*			m_window;
        SceneGraph*		m_graph;
        UIManager		m_uiManager;
        Viewport*		m_viewport;
        Time			m_time;
        f32				m_timeScale;
        std::atomic<bool> m_gameState;
        
    };
}