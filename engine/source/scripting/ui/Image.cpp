extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "scripting/ui/Image.h"
#include "ui/elements/Image.h"

int script_SetPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);
    if (argumentCount != 3)
        luaL_error(luaState, "Expected 3 arguments (element, xPosition, yPosition)");
    else if (engine::Image* image = (engine::Image*) lua_touserdata(luaState, 1))
    {
        image->SetPosition({
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3))
        });
    }

    return 0;
}

int script_GetPosition(lua_State* luaState)
{
    math::Vector2f position(0.0f);

    int argumentCount = lua_gettop(luaState);
    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 arguments (element)");
    else
    {
        if (engine::Image* image = (engine::Image*) lua_touserdata(luaState, 1))
            position = image->GetPosition();
    }

    lua_pushnumber(luaState, position.GetX());
    lua_pushnumber(luaState, position.GetY());

    return 2;
}

int script_SetScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);
    if (argumentCount != 3)
        luaL_error(luaState, "Expected 3 arguments (element, xScale, yScale)");
    else if (engine::Image* image = (engine::Image*) lua_touserdata(luaState, 1))
    {
        image->SetScale({
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3))
        });
    }

    return 0;
}

int script_GetScale(lua_State* luaState)
{
    math::Vector2f scale(0.0f);

    int argumentCount = lua_gettop(luaState);
    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 arguments (element)");
    else
    {
        if (engine::Image* image = (engine::Image*) lua_touserdata(luaState, 1))
            scale = image->GetScale();
    }

    lua_pushnumber(luaState, scale.GetX());
    lua_pushnumber(luaState, scale.GetY());

    return 2;
}

void engine::RegisterUIImageFunctions(lua_State* luaState)
{
    constexpr luaL_Reg uiFuncs[]
    {
        {"SetPosition", script_SetPosition},
        {"GetPosition", script_GetPosition},
        {"SetScale", script_SetScale},
        {"GetScale", script_GetScale},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, uiFuncs, 0);
    lua_setglobal(luaState, "ImageRef");
}