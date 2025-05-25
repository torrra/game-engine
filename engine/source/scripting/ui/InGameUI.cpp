extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "scripting/ui/InGameUI.h"
#include "ui/UIManager.h"
#include "ui/UIStyle.h"
#include "ui/Canvas.h"
#include "ui/UIElement.h"
#include "CoreTypes.h"
#include "Engine.h"
#include "resource/ResourceManager.h"
#include "resource/texture/Texture.h"

int script_GetCanvasRef(lua_State* luaState)
{
    engine::Canvas* canvas = nullptr;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 1)
        luaL_error(luaState, "Expected 1 argument (canvasName)");
    else
    {
        engine::Engine* engine = engine::Engine::GetEngine();
        std::string canvasName = luaL_checkstring(luaState, 1);
        printf("Creating canvas with name '%s'\n", canvasName.c_str());
        canvas = engine->GetUIManager().GetCanvas(canvasName);
    }

    lua_pushlightuserdata(luaState, canvas);

    return 1;
}

int script_CreateCanvas(lua_State* luaState)
{
    engine::Canvas* canvas = nullptr;
    math::Vector2f size;

    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 3)
        luaL_error(luaState, "Expected 3 argument (canvasName, canvasWidth, canvasHeight)");
    else
    {
        engine::Engine* engine = engine::Engine::GetEngine();
        std::string canvasName = luaL_checkstring(luaState, 1);
        size.X() = static_cast<f32>(lua_tonumber(luaState, 2));
        size.Y() = static_cast<f32>(lua_tonumber(luaState, 3));
        engine->GetUIManager().CreateCanvas(canvasName, size);
        canvas = engine->GetUIManager().GetCanvas(canvasName);
    }

    lua_pushlightuserdata(luaState, canvas);

    return 1;
}

int script_DestroyCanvas(lua_State* luaState)
{
    luaState;
    return 0;
}

int script_SetCanvasColor(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 5)
        luaL_error(luaState, "Expected 5 arguments (canvasID, red, green, blue, alpha)");
    else
    {
        std::string canvasName = luaL_checkstring(luaState, 1);
        engine::Canvas* canvas = engine::Engine::GetEngine()->GetUIManager().GetCanvas(canvasName);

        canvas->SetColor(
            static_cast<f32>(lua_tonumber(luaState, 2)), // red
            static_cast<f32>(lua_tonumber(luaState, 3)), // green
            static_cast<f32>(lua_tonumber(luaState, 4)), // blue
            static_cast<f32>(lua_tonumber(luaState, 5))  // alpha
        );
    }

    return 0;
}

int script_AddText(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (canvasID, text, xPos, yPos)");
    else
    {
        std::string canvasName = luaL_checkstring(luaState, 1);
        engine::Canvas* canvas = engine::Engine::GetEngine()->GetUIManager().GetCanvas(canvasName);

        std::string text = luaL_checkstring(luaState, 2);

        math::Vector2f position(
            static_cast<f32>(lua_tonumber(luaState, 3)),
            static_cast<f32>(lua_tonumber(luaState, 4))
        );

        engine::Label* label = canvas->AddLabel(text.c_str(), position);
        
        lua_pushlightuserdata(luaState, label);
       
    }

    return 1;
}

int script_AddImage(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (canvasID, path, xPos, yPos)");
    else
    {
        std::string canvasName = luaL_checkstring(luaState, 1);
        engine::Canvas* canvas = engine::Engine::GetEngine()->GetUIManager().GetCanvas(canvasName);

        std::string path = luaL_checkstring(luaState, 2);
        engine::ResourceManager::Load<engine::Texture>(path, true);

        math::Vector2f position(
            static_cast<f32>(lua_tonumber(luaState, 3)),
            static_cast<f32>(lua_tonumber(luaState, 4))
        );

        engine::Image* image = canvas->AddImage(path.c_str(), position);

        lua_pushlightuserdata(luaState, image);
    }

    return 1;
}

int script_AddButton(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 4)
        luaL_error(luaState, "Expected 4 arguments (canvasID, text, xPos, yPos)");
    else
    {
        std::string canvasName = luaL_checkstring(luaState, 1);
        engine::Canvas* canvas = engine::Engine::GetEngine()->GetUIManager().GetCanvas(canvasName);

        std::string text = luaL_checkstring(luaState, 2);
        math::Vector2f position(
            static_cast<f32>(lua_tonumber(luaState, 3)),
            static_cast<f32>(lua_tonumber(luaState, 4))
        );

        engine::Button* button = canvas->AddButton(text.c_str(), position, ui::GetTextSize(text.c_str()));

        lua_pushlightuserdata(luaState, button);
    }

    return 1;
}

int script_AddProgressBar(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 7)
        luaL_error(luaState, "Expected 7 arguments (canvasID, xPos, yPos, xSize, ySize, minRange, maxRange)");
    else
    {
        std::string canvasName = luaL_checkstring(luaState, 1);
        engine::Canvas* canvas = engine::Engine::GetEngine()->GetUIManager().GetCanvas(canvasName);

        math::Vector2f position(
            static_cast<f32>(lua_tonumber(luaState, 2)),
            static_cast<f32>(lua_tonumber(luaState, 3))
        );

        math::Vector2f size(
            static_cast<f32>(lua_tonumber(luaState, 4)),
            static_cast<f32>(lua_tonumber(luaState, 5))
        );

        math::Vector2f range(
            static_cast<f32>(lua_tonumber(luaState, 6)),
            static_cast<f32>(lua_tonumber(luaState, 7))
        );

        engine::ProgressBar* progressBar = canvas->AddProgressBar(position, size, range);

        lua_pushlightuserdata(luaState, progressBar);
    }

    return 1;
}

int script_AddRect(lua_State* luaState)
{
    luaState;
    return 0;
}

int script_RemoveElement(lua_State* luaState)
{
    int argumentCount = lua_gettop(luaState);

    if (argumentCount != 2)
        luaL_error(luaState, "Expected 2 arguments (canvasID, element)");
    else
    {
        std::string canvasName = luaL_checkstring(luaState, 1);
        engine::Canvas* canvas = engine::Engine::GetEngine()->GetUIManager().GetCanvas(canvasName);

        if (engine::UIElement* element = (engine::UIElement*) lua_touserdata(luaState, 2))
            canvas->RemoveElement(element);
        else
            luaL_error(luaState, "Expected an element as second argument");
    }

    return 0;
}

void engine::RegisterUIFunctions(lua_State* luaState)
{
    constexpr luaL_Reg uiFuncs[]
    {
        {"GetCanvasRef", script_GetCanvasRef},
        {"CreateCanvas", script_CreateCanvas},
        //{"DestroyCanvas", script_DestroyCanvas},
        {"SetCanvasColor", script_SetCanvasColor},
        {"AddText", script_AddText},
        {"AddImage", script_AddImage},
        {"AddButton", script_AddButton},
        {"AddProgressBar", script_AddProgressBar},
        //{"AddRect", script_AddRect},
        {"RemoveElement", script_RemoveElement},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, uiFuncs, 0);
    lua_setglobal(luaState, "CanvasRef");
}
