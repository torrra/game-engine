#pragma once

extern "C"
{
    int script_IsButtonPressed(struct lua_State* luaState);
    int script_SetColor(struct lua_State* luaState);
    int script_SetHoveredColor(struct lua_State* luaState);
    int script_SetPressedColor(struct lua_State* luaState);
    int script_SetButtonSize(struct lua_State* luaState);
}

namespace engine
{
    void RegisterUIButtonFunctions(struct lua_State* luaState);
}