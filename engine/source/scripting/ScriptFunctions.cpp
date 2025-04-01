extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "core/SceneGraph.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/ScriptFunctions.h"

#include <iostream>

int script_GetScriptRef(lua_State* luaState)
{
    engine::Script* result = nullptr;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        result = engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::Script>(handle);
    }

    lua_pushlightuserdata(luaState, result);
    return 1;
}


namespace engine
{
    void RegisterScriptComponentFunctions(lua_State* luaState)
    {
        constexpr luaL_Reg scriptFuncs[]
        {
            {"GetScriptRef", script_GetScriptRef},
            {NULL, NULL}
        };

        lua_newtable(luaState);
        luaL_setfuncs(luaState, scriptFuncs, 0);
        lua_setglobal(luaState, "ScriptRef");
    }
}

