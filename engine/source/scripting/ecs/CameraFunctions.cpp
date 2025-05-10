extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "CoreTypes.h"
#include "core/SceneGraph.h"
#include "core/components/Camera.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/ecs/CameraFunctions.h"

int script_GetCameraRef(lua_State* luaState)
{
    engine::Camera* cam = nullptr;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (handle)");

    else
    {
        engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
        cam = engine::ScriptSystem::GetCurrentScene()->GetComponent<engine::Camera>(handle);
    }

    lua_pushlightuserdata(luaState, cam);
    return 1;
}

int script_MoveCamera(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 6)
        luaL_error(luaState, "Expected 6 arguments (self, x, y, z, speed, deltaTime)");

    else if (engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {	
        f32 xMovement = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yMovement = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zMovement = static_cast<f32>(lua_tonumber(luaState, 4));

        f32 speed = static_cast<f32>(lua_tonumber(luaState, 5));
        f32 deltaTime = static_cast<f32>(lua_tonumber(luaState, 6));

        camera->Move({ xMovement, yMovement, zMovement }, speed, deltaTime);
    }

    return 0;
}

int script_RotateCameraEulers(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 arguments (self, x, y, z, speed)");

    else if (engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        f32 xRotation = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yRotation = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zRotation = static_cast<f32>(lua_tonumber(luaState, 4));

        f32 speed = static_cast<f32>(lua_tonumber(luaState, 5));

        camera->Rotate(xRotation, yRotation, zRotation, speed);
    }

    return 0;
}

int script_GetCameraPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (const engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        math::Vector3f camPos = camera->GetPosition();

        lua_pushnumber(luaState, static_cast<lua_Number>(camPos.GetX()));
        lua_pushnumber(luaState, static_cast<lua_Number>(camPos.GetY()));
        lua_pushnumber(luaState, static_cast<lua_Number>(camPos.GetZ()));
    }

    return 3;
}

int script_GetCameraRotation(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    else if (const engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        math::Vector3f camEulers = camera->GetRotation();

        lua_pushnumber(luaState, static_cast<lua_Number>(math::Degree(camEulers.GetX()).Deg()));
        lua_pushnumber(luaState, static_cast<lua_Number>(math::Degree(camEulers.GetY()).Deg()));
        lua_pushnumber(luaState, static_cast<lua_Number>(math::Degree(camEulers.GetZ()).Deg()));
    }

    return 3;
}

int script_GetCameraFOV(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (const engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
            lua_pushnumber(luaState, static_cast<lua_Number>(camera->GetFOV()));

    return 1;
}

int script_GetCameraNearPlane(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (const engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
        lua_pushnumber(luaState, static_cast<lua_Number>(camera->GetNearPlane()));

    return 1;
}

int script_GetCameraFarPlace(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (self)");

    if (const engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
        lua_pushnumber(luaState, static_cast<lua_Number>(camera->GetFarPlane()));

    return 1;
}

int script_SetCameraPosition(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        f32 xPos = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yPos = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zPos = static_cast<f32>(lua_tonumber(luaState, 4));

        camera->Position() = { xPos, yPos, zPos };
    }

    return 0;
}

int script_SetCameraRotation(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (self, x, y, z)");

    else if (engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        f32 xRotation = static_cast<f32>(lua_tonumber(luaState, 2));
        f32 yRotation = static_cast<f32>(lua_tonumber(luaState, 3));
        f32 zRotation = static_cast<f32>(lua_tonumber(luaState, 4));

        camera->Rotation() = { xRotation, yRotation, zRotation };
    }

    return 0;
}

int script_SetCameraFOV(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (self, fov");

    else if (engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        f32 fov = static_cast<f32>(lua_tonumber(luaState, 2));

        camera->SetFOV(fov);
    }

    return 0;
}

int script_SetCameraNearPlace(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (self, nearPlane");

    else if (engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        f32 near = static_cast<f32>(lua_tonumber(luaState, 2));

        camera->SetNearPlane(near);
    }

    return 0;
}

int script_SetCameraFarPlane(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (self, farPlane");

    else if (engine::Camera* camera = (engine::Camera*)lua_touserdata(luaState, 1))
    {
        f32 far = static_cast<f32>(lua_tonumber(luaState, 2));

        camera->SetFarPlane(far);
    }

    return 0;
}

void engine::RegisterCameraFunctions(lua_State* luaState)
{
    constexpr luaL_Reg camFuncs[]
    {

        {"GetCamRef", script_GetCameraRef},

        {"Move", script_MoveCamera},
        {"Rotate", script_RotateCameraEulers},

        {"GetPosition", script_GetCameraPosition},
        {"GetRotation", script_GetCameraRotation},

        {"GetFOV", script_GetCameraFOV},
        {"GetNearPlane", script_GetCameraNearPlane},
        {"GetFarPlane", script_GetCameraFarPlace},

        {"SetPosition", script_SetCameraPosition},
        {"SetRotation", script_SetCameraRotation},

        {"SetFOV", script_SetCameraFOV},
        {"SetNearPlane", script_SetCameraNearPlace},
        {"SetFarPlane", script_SetCameraFarPlane},

        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, camFuncs, 0);
    lua_setglobal(luaState, "CamRef");
}
