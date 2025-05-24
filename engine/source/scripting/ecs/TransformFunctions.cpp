extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "core/SceneGraph.h"
#include "core/components/Transform.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/ecs/TransformFunctions.h"

int script_GetTransformRef(lua_State* luaState)
{
    engine::Transform* transform = nullptr;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        transform = engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::Transform>(handle);
    }

    lua_pushlightuserdata(luaState, transform);
    return 1;
}

int script_GetTransformPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::Transform* transform = (engine::Transform*) lua_touserdata(luaState, 1))
    {
        math::Vector3f pos = transform->GetPosition();

        lua_pushnumber(luaState, static_cast<lua_Number>(pos.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(pos.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(pos.GetZ()));
    }

    return 3;
}

int script_GetTransformRotation(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        math::Quatf rotation = transform->GetRotation();

        lua_pushnumber(luaState, static_cast<lua_Number>(rotation.GetW()));
        lua_pushnumber(luaState, static_cast<lua_Number>(rotation.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(rotation.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(rotation.GetZ()));
    }

    return 4;
}

int script_GetTransformScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        math::Vector3f scale = transform->GetScale();

        lua_pushnumber(luaState, static_cast<lua_Number>(scale.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(scale.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(scale.GetZ()));
    }

    return 3;
}

int script_CopyTransformPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (destination, source)");

    engine::Transform* destination = (engine::Transform*) lua_touserdata(luaState, 1);
    engine::Transform* source = (engine::Transform*) lua_touserdata(luaState, 2);

    if (destination && source)
        destination->CopyPosition(*source);

    return 0;
}

int script_CopyTransformRotation(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (destination, source)");

    engine::Transform* destination = (engine::Transform*)lua_touserdata(luaState, 1);
    engine::Transform* source = (engine::Transform*)lua_touserdata(luaState, 2);

    if (destination && source)
        destination->CopyRotation(*source);

    return 0;
}

int script_CopyTransformScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (destination, source)");

    engine::Transform* destination = (engine::Transform*)lua_touserdata(luaState, 1);
    engine::Transform* source = (engine::Transform*)lua_touserdata(luaState, 2);

    if (destination && source)
        destination->CopyScale(*source);

    return 0;
}

int script_SetTransformPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        transform->SetPosition({ xPos, yPos, zPos });
    }

    return 0;
}

int script_SetTransformRotation(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 arguments (self, w, x, y, z)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        f32 wRotation = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 xRotation = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 yRotation = static_cast<f32>(lua_tonumber(luaState, 4));
        f32 zRotation = static_cast<f32>(lua_tonumber(luaState, 5));

        transform->SetRotation({wRotation, xRotation, yRotation, zRotation });
    }

    return 0;
}

int script_SetTransformScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        f32 xScale = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yScale = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zScale = static_cast<f32>(lua_tonumber(luaState, 4));

        transform->SetScale({ xScale, yScale, zScale });
    }

    return 0;
}

int script_AddTransformTranslation(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        transform->AddTranslation({ xPos, yPos, zPos });
    }

    return 0;
}

int script_AddTransformEulerRotation(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        f32 xRotation = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yRotation = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zRotation = static_cast<f32>(lua_tonumber(luaState, 4));

        transform->AddRotation(xRotation, yRotation, zRotation);
    }

    return 0;
}

int script_AddTransformScale(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::Transform* transform = (engine::Transform*)lua_touserdata(luaState, 1))
    {
        f32 xScale = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yScale = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zScale = static_cast<f32>(lua_tonumber(luaState, 4));

        transform->AddScale({ xScale, yScale, zScale });
    }

    return 0;
}

void engine::RegisterTransformFunctions(lua_State* luaState)
{
    constexpr luaL_Reg transformFuncs[] =
    {
        {"GetTransformRef", script_GetTransformRef},

        {"GetPosition", script_GetTransformPosition},
        {"GetRotation", script_GetTransformRotation},
        {"GetScale", script_GetTransformScale},

        {"SetPosition", script_SetTransformPosition},
        {"SetRotation", script_SetTransformRotation},
        {"SetScale", script_SetTransformScale},

        {"AddTranslation", script_AddTransformTranslation},
        {"AddEulerRotation", script_AddTransformEulerRotation},
        {"AddScale", script_AddTransformScale},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, transformFuncs, 0);
    lua_setglobal(luaState, "TransformRef");
}
