#pragma once

extern "C"
{
	// Component:IsValid() native implementation. Calls Component::IsValid
	int script_IsComponentValid(struct lua_State* luaState);

	// Component:Activate() native implementation. Calls Component::Activate
	int script_ActivateComponent(struct lua_State* luaState);

	// Component:IsActive() native implementation. Calls Component::IsActive
	int script_IsComponentActive(struct lua_State* luaState);

	// Compoenent:Invalidate() native implementation. Calls Component::Invalidate
	int script_InvalidateComponent(struct lua_State* luaState);
}

namespace engine
{
	void RegisterComponentFunctions(struct lua_State* luaState);
}