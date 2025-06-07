extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include "resource/ResourceManager.h"

#include "resource/animation/Animation.h"
#include "resource/font/Font.h"
#include "resource/material/MeshMaterial.h"
#include "resource/model/Model.h"
#include "resource/texture/Texture.h"
#include "sounds/Sound.h"
#include "scripting/ResourceFunctions.h"

// Must match with types in Utils.lua
enum class EResourceType : lua_Integer
{
    MODEL = 0,
    SHADER = 1,
    AUDIO = 2,
    TEXTURE = 3,
    FONT = 4,
    ANIMATION = 5,
    MATERIAL = 6,
};

int script_LoadResource(lua_State* luaState)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (type, key)");

    EResourceType type = static_cast<EResourceType>(luaL_checkinteger(luaState, 1));
    std::string key = luaL_checkstring(luaState, 2);

    switch (type)
    {
    case EResourceType::MODEL:
        engine::ResourceManager::Load<engine::Model>(key);
        break;
    case EResourceType::SHADER:
        engine::ResourceManager::LoadShaderFromFrag(key);
        break;
    case EResourceType::AUDIO:
        engine::ResourceManager::Load<engine::Sound>(key);
        break;
    case EResourceType::TEXTURE:
        engine::ResourceManager::Load<engine::Texture>(key);
        break;
    case EResourceType::FONT:
        engine::ResourceManager::Load<engine::Font>(key);
        break;
    case EResourceType::ANIMATION:
        engine::ResourceManager::Load<engine::Animation>(key);
        break;
    case EResourceType::MATERIAL:
        engine::ResourceManager::Load<engine::MeshMaterial>(key);
        break;
    default:
        break;
    }
    
    return 0;
}

void engine::RegisterResourceFunctions(lua_State* luaState)
{
    constexpr luaL_Reg resourceFuncs[]
    {
        {"LoadResource", script_LoadResource},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, resourceFuncs, 0);
    lua_setglobal(luaState, "ResourceC");
}
