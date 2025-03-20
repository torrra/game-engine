#pragma once

extern "C"
{
	int script_GetCameraRef(struct lua_State* luaState);
	
	int script_MoveCamera(struct lua_State* luaState);
	int script_RotateCameraEulers(struct lua_State* luaState);

	int script_GetCameraPosition(struct lua_State* luaState);
	int script_GetCameraRotation(struct lua_State* luaState);

	int script_GetCameraFOV(struct lua_State* luaState);
	int script_GetCameraNearPlane(struct lua_State* luaState);
	int script_GetCameraFarPlace(struct lua_State* luaState);

	int script_SetCameraPosition(struct lua_State* luaState);
	int script_SetCameraRotation(struct lua_State* luaState);

	int script_SetCameraFOV(struct lua_State* luaState);
	int script_SetCameraNearPlace(struct lua_State* luaState);
	int script_SetCameraFarPlane(struct lua_State* luaState);
}

namespace engine
{
	void RegisterCameraFunctions(struct lua_State* luaState);
}