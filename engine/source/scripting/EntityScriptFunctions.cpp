extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "core/SceneGraph.h"
#include "core/systems/ScriptSystem.h"
#include "scripting/EntityScriptFunctions.h"

#include <iostream>

int script_IsEntityValid(lua_State* luaState)
{
	int result = false;
	int argumentCount = lua_gettop(luaState);

	if (argumentCount != 1)
		luaL_error(luaState, "Expected 1 argument (self)");

	else
	{
		if (engine::Entity* entity = (engine::Entity*)lua_touserdata(luaState, 1))
			result = entity->IsValid();
	}

	lua_pushboolean(luaState, result);
	return 1;
}

int script_GetEntityRef(lua_State* luaState)
{
	engine::Entity* result = nullptr;

	int argumentCount = lua_gettop(luaState);

	if (argumentCount != 1)
		luaL_error(luaState, "Expected 1 argument (handle)");

	else
	{
		engine::EntityHandle handle = luaL_checkinteger(luaState, 1);
		result = engine::ScriptSystem::GetCurrentScene()->GetEntity(handle);
	}

	lua_pushlightuserdata(luaState, result);
	return 1;
}

int script_ActivateEntity(lua_State* luaState)
{
	int argumentCount = lua_gettop(luaState);

	if (argumentCount != 2)
		luaL_error(luaState, "Expected 2 arguments (handle)");

	else if (engine::Entity* entity = (engine::Entity*)lua_touserdata(luaState, 1))
	{
		bool activeState = lua_toboolean(luaState, 2);
		entity->Activate(activeState);
	}

	return 0;
}

namespace engine
{
	void RegisterEntityFunctions(lua_State* luaState)
	{
		constexpr luaL_Reg entityFuncs[]
		{
			{"IsValid", script_IsEntityValid},
			{"GetEntityRef", script_GetEntityRef},
			{"Activate", script_ActivateEntity},
			{NULL, NULL}
		};

		lua_newtable(luaState);
		luaL_setfuncs(luaState, entityFuncs, 0);
		lua_setglobal(luaState, "EntityRef");
	}

}
