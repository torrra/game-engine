#pragma once

extern "C"
{
	// Entity:IsValid() native implementation. Calls Entity::IsValid()
	int script_IsEntityValid(struct lua_State* luaState);

	// Entity:RefreshRef() native implementation. Calls SceneGraph::GetEntity()
	int script_GetEntityRef(struct lua_State* luaState);

	// Entity:Activate() native implementation. Calls Entity::Activate()
	int script_ActivateEntity(struct lua_State* luaState);

}

namespace engine
{
	void RegisterEntityFunctions(struct lua_State* luaState);
	void RunEntityConfigScript(struct lua_State* luaState);
}