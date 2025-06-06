#pragma once

extern "C"
{
    int script_GetCanvasRef(struct lua_State* luaState);
    int script_CreateCanvas(struct lua_State* luaState);
    int script_DestroyCanvas(struct lua_State* luaState);
    int script_SetCanvasColor(struct lua_State* luaState);
    int script_AddText(struct lua_State* luaState);
    int script_AddImage(struct lua_State* luaState);
    int script_AddButton(struct lua_State* luaState);
    int script_AddProgressBar(struct lua_State* luaState);
    int script_RemoveElement(struct lua_State* luaState);
    int script_GetScreenSize(struct lua_State* luaState);
}

namespace engine
{
    void RegisterUIFunctions(struct lua_State* luaState);
}