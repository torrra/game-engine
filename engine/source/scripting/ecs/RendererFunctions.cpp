extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "core/SceneGraph.h"
#include "core/components/Renderer.h"
#include "core/systems/ScriptSystem.h"
#include "resource/ResourceManager.h"
#include "resource/material/MeshMaterial.h"
#include "resource/model/Model.h"
#include "resource/shader/Shader.h"
#include "scripting/ecs/RendererFunctions.h"

#include "ConsoleLog.hpp"

int script_GetRendererRef(lua_State* luaState)
{
    engine::Renderer* renderer = nullptr;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        renderer = engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::Renderer>(handle);
    }

    lua_pushlightuserdata(luaState, renderer);
    return 1;
}

int script_SetRendererMaterial(lua_State* luaState)
{
    if (lua_gettop(luaState) != 3)
        luaL_error(luaState, "Expected 3 arguments (self, index, name)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        uint32 index = static_cast<uint32>(luaL_checkinteger(luaState, 2));
        std::string key = luaL_checkstring(luaState, 3);

        renderer->SetMaterial(index, key.c_str());
    }

    return 0;
}

int script_SetRendererModel(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, name)");

    if (engine::Renderer* renderer = (engine::Renderer*) lua_touserdata(luaState, 1))
    {
        std::string key = luaL_checkstring(luaState, 2);
        renderer->SetModel(key.c_str());
    }

    return 0;
}

int script_SetRendererShader(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, name)");

    if (engine::Renderer* renderer = (engine::Renderer*) lua_touserdata(luaState, 1))
    {
        std::string key = luaL_checkstring(luaState, 2);
        
        std::string programName = engine::ResourceManager::LoadShaderFromFrag(key.c_str());
        renderer->SetShader(programName.c_str());
        
    }

    return 0;
}

int script_GetRendererMaterial(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, index)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        uint32 index = static_cast<uint32>(luaL_checkinteger(luaState, 2));

        engine::ResourceRef<engine::MeshMaterial> mat = renderer->GetMaterial(index);

        if (const std::string* key = engine::ResourceManager::FindKeyByVal(mat))
        {
            lua_pushstring(luaState, key->c_str());
            return 1;
        }

    } 

    lua_pushstring(luaState, "invalid resource");
    return 1;
}

int script_GetRendererModel(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        engine::ResourceRef<engine::Model> model = renderer->GetModel();

        if (const std::string* key = engine::ResourceManager::FindKeyByVal(model))
        {
            lua_pushstring(luaState, key->c_str());
            return 1;
        }

    }

    lua_pushstring(luaState, "invalid resource");
    return 1;
}

int script_GetRendererShader(lua_State* luaState)
{

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        engine::ResourceRef<engine::ShaderProgram> shader = renderer->GetShader();

        lua_pushstring(luaState, shader->GetFragmentShaderName().c_str());
        return 1;
    }

    lua_pushstring(luaState, "invalid resource");
    return 1;
}

int script_SetAnimatorAnimation(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, name)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        std::string key = luaL_checkstring(luaState, 2);

       engine::ThreadManager::AddTask([key, renderer]()
       {

        engine::ResourceRef<engine::Animation> anim =
            engine::ResourceManager::GetResource<engine::Animation>(key);

            if (anim)
            {
                while (!anim->IsLoaded() && !anim->HasFailedToLoad());

                if (!renderer->GetModel())
                {
                    engine::PrintLog(engine::ErrorPreset(), "No model found on entity " +
                        std::to_string(renderer->GetOwner()) + "\n");

                    return;
                }

                if (!renderer->GetModel()->IsLoaded())
                {
                    engine::PrintLog(engine::WarningPreset(),
                        "Model not fully loaded yet. Animation will be set once it has finished loading");

                    while (!renderer->GetModel()->IsLoaded() && !renderer->GetModel()->HasFailedToLoad());

                    if (renderer->GetModel()->HasFailedToLoad())
                        return;
                }

                if (anim->IsLoaded())
                    renderer->GetAnimator().SetAnimation(std::move(anim));
            }
       });
    }

    return 0;
}

int script_PauseAnimatorAnimation(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        renderer->GetAnimator().PauseAnimation();

    return 0;
}

int script_StopAnimatorAnimation(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        renderer->GetAnimator().StopAnimation();

    return 0;
}

int script_PlayAnimatorAnimation(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        renderer->GetAnimator().PlayAnimation();

    return 0;
}

int script_GetAnimatorAnimation(lua_State* luaState)
{
    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        engine::ResourceRef<engine::Animation> anim = renderer->GetAnimator().GetAnimation();

        if (const std::string* key = engine::ResourceManager::FindKeyByVal(anim))
        {
            lua_pushstring(luaState, key->c_str());
            return 1;
        }

    }

    lua_pushstring(luaState, "invalid resource");
    return 1;
}

int script_SetAnimatorKeyFrame(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, frame)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        int32 keyframe = static_cast<int32>(luaL_checkinteger(luaState, 2));
        renderer->GetAnimator().SetKeyFrame(keyframe);
    }

    return 0;
}

int script_SetAnimatorAnimationSpeed(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, speed)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        f32 speed = static_cast<f32>(luaL_checknumber(luaState, 2));
        renderer->GetAnimator().SetAnimationSpeed(speed);
    }

    return 0;
}

int script_GetAnimatorKeyFrame(lua_State* luaState)
{
    int32 keyframe = -1;

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        keyframe = renderer->GetAnimator().GetCurrentKeyFrame();

    lua_pushinteger(luaState, keyframe);
    return 1;
}

int script_GetAnimatorAnimationSpeed(lua_State* luaState)
{
    f32 speed = 0.f;

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        speed = renderer->GetAnimator().GetAnimationSpeed();

    lua_pushnumber(luaState, speed);
    return 1;
}

int script_IsAnimatorLooped(lua_State* luaState)
{
    bool isLooped = false;

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        isLooped = renderer->GetAnimator().IsLooped();

    lua_pushboolean(luaState, isLooped);
    return 1;
}

int script_SetAnimatorLooped(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (self, isLooped)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
    {
        bool looped = lua_toboolean(luaState, 2);
        renderer->GetAnimator().SetLooped(looped);
    }

    return 0;
}

int script_IsAnimatorPlaying(lua_State* luaState)
{
    bool isPlaying = false;

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        isPlaying = renderer->GetAnimator().IsPlaying();

    lua_pushboolean(luaState, isPlaying);
    return 1;
}

int script_IsAnimatorPaused(lua_State* luaState)
{
    bool isPaused = false;

    if (lua_gettop(luaState) != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (engine::Renderer* renderer = (engine::Renderer*)lua_touserdata(luaState, 1))
        isPaused = renderer->GetAnimator().IsPaused();

    lua_pushboolean(luaState, isPaused);
    return 1;
}

void engine::RegisterRendererFunctions(lua_State* luaState)
{
    constexpr luaL_Reg rendererFuncs[]
    {
        {"GetRendererRef", script_GetRendererRef},

        {"SetMaterial", script_SetRendererMaterial},
        {"SetModel", script_SetRendererModel},
        {"SetShader", script_SetRendererShader},

        {"GetMaterial", script_GetRendererMaterial},
        {"GetModel", script_GetRendererModel},
        {"GetShader", script_GetRendererShader},

        {"SetAnimation", script_SetAnimatorAnimation},
        {"PauseAnimation", script_PauseAnimatorAnimation},
        {"StopAnimation", script_StopAnimatorAnimation},
        {"PlayAnimation", script_PlayAnimatorAnimation},

        {"GetAnimation", script_GetAnimatorAnimation},

        {"SetAnimationKeyFrame", script_SetAnimatorKeyFrame},
        {"SetAnimationSpeed", script_SetAnimatorAnimationSpeed},

        {"GetAnimationKeyFrame", script_GetAnimatorKeyFrame},
        {"GetAnimationSpeed", script_GetAnimatorAnimationSpeed},

        {"IsAnimationLooped", script_IsAnimatorLooped},
        {"SetAnimationLooped", script_SetAnimatorLooped},

        {"IsAnimationPlaying", script_IsAnimatorPlaying},
        {"IsAnimationPaused", script_IsAnimatorPaused},

        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, rendererFuncs, 0);
    lua_setglobal(luaState, "RendererRef");
}
