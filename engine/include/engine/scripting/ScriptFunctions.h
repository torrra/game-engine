#pragma once

extern "C"
{
	int script_GetScriptRef(struct lua_State* luaState);
}


namespace engine
{
	void RegisterScriptComponentFunctions(struct lua_State* luaState);
}