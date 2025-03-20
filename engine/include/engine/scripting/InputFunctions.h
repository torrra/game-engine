#pragma once

extern "C"
{
	int script_IsInputPressed(struct lua_State* luaState);
	int script_IsInputDown(struct lua_State* luaState);
	int script_IsInputHeld(struct lua_State* luaState);
	int script_IsInputReleased(struct lua_State* luaState);

	int script_GetCursorPosition(struct lua_State* luaState);
	int script_GeCursorDeltaPos(struct lua_State* luaState);
	int script_GetScrollDelta(struct lua_State* luaState);
}

namespace engine
{
	void RegisterInputFunctions(struct lua_State* luaState);
}