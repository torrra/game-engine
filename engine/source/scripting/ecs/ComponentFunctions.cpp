extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "core/Component.h"
#include "scripting/ecs/ComponentFunctions.h"

int script_IsComponentValid(lua_State* luaState)
{
    int result = false;
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else
    {
        if (engine::Component* component = (engine::Component*)lua_touserdata(luaState, 1))
            result = component->IsValid();
    }

    lua_pushboolean(luaState, result);
    return 1;
}


int script_ActivateComponent(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (self, active)");

    else if (engine::Component* component = (engine::Component*)lua_touserdata(luaState, 1))
    {
        bool activeState = lua_toboolean(luaState, 2);
        component->Activate(activeState);
    }

    return 0;
}

int script_IsComponentActive(lua_State* luaState)
{
    int result = false;
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else
    {
        if (engine::Component* component = (engine::Component*)lua_touserdata(luaState, 1))
            result = component->IsActive();
    }

    lua_pushboolean(luaState, result);
    return 1;
}

int script_InvalidateComponent(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::Component* component = (engine::Component*)lua_touserdata(luaState, 1))
        component->Invalidate();

    return 0;
}

namespace engine
{
    void RegisterComponentFunctions(lua_State* luaState)
    {
        constexpr luaL_Reg componentFuncs[]
        {
            {"IsValid", script_IsComponentValid},
            {"Activate", script_ActivateComponent},
            {"IsActive", script_IsComponentActive},
            {"Invalidate", script_InvalidateComponent},
            {NULL, NULL}
        };

        lua_newtable(luaState);
        luaL_setfuncs(luaState, componentFuncs, 0);
        lua_setglobal(luaState, "ComponentRef");
    }
}