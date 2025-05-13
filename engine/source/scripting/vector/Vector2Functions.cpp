#include "scripting/vector/Vector2Functions.h"

#include <math/Vector2.hpp>
#include <math/Interpolation.hpp>

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

static void Get2VectorArgs(lua_State* luaState,
                             math::Vector2<lua_Number>& vectorA,
                             math::Vector2<lua_Number>& vectorB)
{
    if (lua_gettop(luaState) != 4)
        luaL_error(luaState, "Expected 4 arguments (x1, y1, x2, y2)");

    vectorA =
    {
        lua_tonumber(luaState, 1),
        lua_tonumber(luaState, 2)
    };

    vectorB = 
    {
        lua_tonumber(luaState, 3),
        lua_tonumber(luaState, 4)
    };
}

static void Get1VectorArg(lua_State* luaState, math::Vector2<lua_Number>& vector)
{
    if (lua_gettop(luaState) != 2)
        luaL_error(luaState, "Expected 2 arguments (x, y)");

    vector =
    {
        lua_tonumber(luaState, 1),
        lua_tonumber(luaState, 2)
    };
}


int script_AngleFromVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number angle = static_cast<lua_Number>(0.0);

    if (vectorA.IsUnitVector() && vectorB.IsUnitVector())
        angle = vectorA.AngleFromUnit(vectorB).Rad();

    else
        angle = vectorA.AngleFrom(vectorB).Rad();

    // return radians to keep things consistent with Lua 5.4 trig functions
    lua_pushnumber(luaState, angle);

    return 1;
}

int script_CrossVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number zComponent = vectorA.Cross(vectorB);

    lua_pushnumber(luaState, zComponent);

    return 1;
}

int script_DistanceFromVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number distance = vectorA.DistanceFrom(vectorB);

    lua_pushnumber(luaState, distance);

    return 1;
}

int script_DistanceSquaredFromVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number distance = vectorA.DistanceSquaredFrom(vectorB);

    lua_pushnumber(luaState, distance);

    return 1;
}

int script_DotVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number dot = vectorA.Dot(vectorB);

    lua_pushnumber(luaState, dot);

    return 1;
}

int script_IsLongerThanVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    bool result = vectorA.IsLongerThan(vectorB);

    lua_pushboolean(luaState, result);

    return 1;
}

int script_IsShorterThanVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    bool result = vectorA.IsShorterThan(vectorB);

    lua_pushboolean(luaState, result);

    return 1;
}

int script_IsUnitVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vector;

    Get1VectorArg(luaState, vector);
    lua_pushboolean(luaState, vector.IsUnitVector());
    return 1;
}

int script_MagnitudeVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vector;

    Get1VectorArg(luaState, vector);
    lua_pushnumber(luaState, vector.Magnitude());
    return 1;
}

int script_MagnitudeSquaredVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vector;

    Get1VectorArg(luaState, vector);
    lua_pushnumber(luaState, vector.MagnitudeSquared());
    return 1;
}

int script_NormalizeVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vector;

    Get1VectorArg(luaState, vector);
    vector.Normalize();

    lua_pushnumber(luaState, vector.GetX());
    lua_pushnumber(luaState, vector.GetY());
    return 2;
}

int script_NormalVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vector;

    Get1VectorArg(luaState, vector);
    vector = vector.Normal();

    lua_pushnumber(luaState, vector.GetX());
    lua_pushnumber(luaState, vector.GetY());
    return 2;
}

int script_ProjectOntoVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);
    vectorA.ProjectOnto(vectorB);

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    return 2;
}

int script_ReflectOntoVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);
    vectorA.ReflectOnto(vectorB);

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    return 2;
}

int script_RotateVector2(lua_State* luaState)
{
    if (lua_gettop(luaState) != 3)
        luaL_error(luaState, "Expected 3 arguments (x, y, angleRadians)");

    math::Vector2<lua_Number> vector
    {
        lua_tonumber(luaState, 1),
        lua_tonumber(luaState, 2)
    };

    vector.Rotate(math::Radian<lua_Number>(lua_tonumber(luaState, 3)));
    lua_pushnumber(luaState, vector.GetX());
    lua_pushnumber(luaState, vector.GetY());
    return 2;
}

int script_ScaleVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);
    vectorA.Scale(vectorB);

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    return 2;
}

int script_TranslateVector2(lua_State* luaState)
{
    math::Vector2<lua_Number> vectorA;
    math::Vector2<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);
    vectorA += vectorB;

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    return 2;
}

int script_LerpVector2(lua_State* luaState)
{  

    if (lua_gettop(luaState) != 5)
        luaL_error(luaState, "Expected 5 arguments (x1, y1, x2, y2, lerpRatio)");

    math::Vector2<lua_Number> vectorA
    {
        lua_tonumber(luaState, 1),
        lua_tonumber(luaState, 2)
    };

    math::Vector2<lua_Number> vectorB
    {
        lua_tonumber(luaState, 3),
        lua_tonumber(luaState, 4)
    };

    lua_Number ratio = lua_tonumber(luaState, 5);

    math::Vector2<lua_Number> result =
    {
        math::Lerp(vectorA.GetX(), vectorB.GetX(), ratio),
        math::Lerp(vectorA.GetY(), vectorB.GetY(), ratio)
    };

    lua_pushnumber(luaState, result.GetX());
    lua_pushnumber(luaState, result.GetY());
    return 2;
}

void engine::RegisterVector2Functions(lua_State* luaState)
{
    constexpr luaL_Reg vector2Funcs[] =
    {
        {"AngleFrom", script_AngleFromVector2},
        {"Cross", script_CrossVector2},

        {"DistanceFrom", script_DistanceFromVector2},
        {"DistanceSquaredFrom", script_DistanceSquaredFromVector2},

        {"Dot", script_DotVector2},

        {"IsLongerThan", script_IsLongerThanVector2},
        {"IsShorterThan", script_IsShorterThanVector2},

        {"IsUnitVector", script_IsUnitVector2},

        {"Magnitude", script_MagnitudeVector2},
        {"MagnitudeSquared", script_MagnitudeSquaredVector2},

        {"Normalize", script_NormalizeVector2},
        {"NormalVector", script_NormalVector2},

        {"ProjectOnto", script_ProjectOntoVector2},
        {"ReflectOnto", script_ReflectOntoVector2},

        {"Rotate", script_RotateVector2},
        {"Scale", script_ScaleVector2},
        {"Translate", script_TranslateVector2},

        {"Lerp", script_LerpVector2},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, vector2Funcs, 0);
    lua_setglobal(luaState, "Vec2C");
}
