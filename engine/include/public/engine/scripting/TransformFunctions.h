#pragma once

extern "C"
{
	int script_GetTransformRef(struct lua_State* luaState);

	//int script_InterpolateTransforms(struct lua_State* luaState);
	
	int script_GetTransformPosition(struct lua_State* luaState);
	int script_GetTransformRotation(struct lua_State* luaState);
	int script_GetTransformScale(struct lua_State* luaState);

	int script_CopyTransformPosition(struct lua_State* luaState);
	int script_CopyTransformRotation(struct lua_State* luaState);
	int script_CopyTransformScale(struct lua_State* luaState);

	int script_SetTransformPosition(struct lua_State* luaState);
	int script_SetTransformRotation(struct lua_State* luaState);
	int script_SetTransformScale(struct lua_State* luaState);

	int script_AddTransformTranslation(struct lua_State* luaState);
	int script_AddTransformEulerRotation(struct lua_State* luaState);
	int script_AddTransformScale(struct lua_State* luaState);
}

namespace engine
{
	void RegisterTransformFunctions(struct lua_State* luaState);
}