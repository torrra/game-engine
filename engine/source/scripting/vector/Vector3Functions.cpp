#include "scripting/vector/Vector3Functions.h"

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

static void Get2VectorArgs(lua_State* luaState,
    math::Vector3<lua_Number>& vectorA,
    math::Vector3<lua_Number>& vectorB)
{
    if (lua_gettop(luaState) != 6)
        luaL_error(luaState, "Expected 6 arguments (x1, y1, z1, x2, y2, z2)");

    vectorA =
    {
        lua_tonumber(luaState, 1),
        lua_tonumber(luaState, 2),
        lua_tonumber(luaState, 3)
    };

    vectorB =
    {
        lua_tonumber(luaState, 4),
        lua_tonumber(luaState, 5),
        lua_tonumber(luaState, 6)
    };
}

static void Get1VectorArg(lua_State* luaState, math::Vector3<lua_Number>& vector)
{
    if (lua_gettop(luaState) != 3)
        luaL_error(luaState, "Expected 3 arguments (x, y, z)");

    vector =
    {
        lua_tonumber(luaState, 1),
        lua_tonumber(luaState, 2),
        lua_tonumber(luaState, 3)
    };
}


int script_AngleFromVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);
    
    lua_Number result;

    if (vectorA.IsUnitVector() && vectorB.IsUnitVector())
        result = vectorA.AngleFromUnit(vectorB).Rad();

    else
        result = vectorA.AngleFrom(vectorB).Rad();

    lua_pushnumber(luaState, result);
    return 1;
}

int script_CrossVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    math::Vector3<lua_Number> result = vectorA.Cross(vectorB);

    lua_pushnumber(luaState, result.GetX());
    lua_pushnumber(luaState, result.GetY());
    lua_pushnumber(luaState, result.GetZ());
    return 3;
}

int script_DistanceFromVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number result = vectorA.DistanceFrom(vectorB);

    lua_pushnumber(luaState, result);
    return 1;
}

int script_DistanceSquaredFromVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number result = vectorA.DistanceSquaredFrom(vectorB);

    lua_pushnumber(luaState, result);
    return 1;
}

int script_Distance2DFromVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number result = vectorA.Distance2DFrom(vectorB);

    lua_pushnumber(luaState, result);
    return 1;
}

int script_Distance2DSquaredFromVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number result = vectorA.Distance2DSquaredFrom(vectorB);

    lua_pushnumber(luaState, result);
    return 1;
}

int script_DotVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    lua_Number result = vectorA.Dot(vectorB);

    lua_pushnumber(luaState, result);
    return 1;
}

int script_IsLongerThanVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    bool result = vectorA.IsLongerThan(vectorB);

    lua_pushboolean(luaState, result);
    return 1;
}

int script_IsShorterThanVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    bool result = vectorA.IsShorterThan(vectorB);

    lua_pushboolean(luaState, result);
    return 1;
}

int script_IsUnitVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vector;

    Get1VectorArg(luaState, vector);

    lua_pushboolean(luaState, vector.IsUnitVector());
    return 1;
}

int script_MagnitudeVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vector;

    Get1VectorArg(luaState, vector);

    lua_pushnumber(luaState, vector.Magnitude());
    return 1;
}

int script_MagnitudeSquaredVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vector;

    Get1VectorArg(luaState, vector);

    lua_pushnumber(luaState, vector.MagnitudeSquared());
    return 1;
}

int script_NormalizeVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vector;

    Get1VectorArg(luaState, vector);

    vector.Normalize();

    lua_pushnumber(luaState, vector.GetX());
    lua_pushnumber(luaState, vector.GetY());
    lua_pushnumber(luaState, vector.GetZ());
    return 3;
}

int script_ProjectOntoVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    if (!math::AlmostEqual(vectorB.MagnitudeSquared(), static_cast<lua_Number>(0.0)))
        vectorA.ProjectOnto(vectorB);
    else
        luaL_error(luaState, "Cannot project onto a vector of magnitude 0");

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    lua_pushnumber(luaState, vectorA.GetZ());
    return 3;
}

int script_ReflectOntoVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    if (vectorA.IsUnitVector() && vectorB.IsUnitVector())
        vectorA.ReflectOntoUnit(vectorB);

    else if (math::AlmostEqual(vectorB.MagnitudeSquared(), static_cast<lua_Number>(0.0)))
        luaL_error(luaState, "Cannot reflect onto a vector of magnitude 0");

    else
        vectorA.ReflectOnto(vectorB);

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    lua_pushnumber(luaState, vectorA.GetZ());
    return 3;
}

int script_RotateVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vector;
    math::Vector3<lua_Number> angles;

    Get2VectorArgs(luaState, vector, angles);

    math::Quaternion rotator
    {
        math::Radian(angles.GetX()),
        math::Radian(angles.GetY()),
        math::Radian(angles.GetZ())
    };

    rotator.Normalize();
    vector = rotator.Rotate(vector);

    lua_pushnumber(luaState, vector.GetX());
    lua_pushnumber(luaState, vector.GetY());
    lua_pushnumber(luaState, vector.GetZ());
    return 3;
}

int script_ScaleVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    vectorA.Scale(vectorB);

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    lua_pushnumber(luaState, vectorA.GetZ());
    return 3;
}

int script_TranslateVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;

    Get2VectorArgs(luaState, vectorA, vectorB);

    vectorA.Translate(vectorB);

    lua_pushnumber(luaState, vectorA.GetX());
    lua_pushnumber(luaState, vectorA.GetY());
    lua_pushnumber(luaState, vectorA.GetZ());
    return 3;
}

int script_LerpVector3(lua_State* luaState)
{
    math::Vector3<lua_Number> vectorA;
    math::Vector3<lua_Number> vectorB;


    if (lua_gettop(luaState) != 7)
        luaL_error(luaState, "Expected 7 arguments (x1, y1, z1, x2, y2, z2, lerpRatio)");


    vectorA =
    {
        lua_tonumber(luaState, 1),
        lua_tonumber(luaState, 2),
        lua_tonumber(luaState, 3)
    };

    vectorB =
    {
        lua_tonumber(luaState, 4),
        lua_tonumber(luaState, 5),
        lua_tonumber(luaState, 6)
    };

    lua_Number ratio = lua_tonumber(luaState, 7);

    math::Vector3<lua_Number> result =
    math::Vector3<lua_Number>::Lerp(vectorA, vectorB, ratio);

    lua_pushnumber(luaState, result.GetX());
    lua_pushnumber(luaState, result.GetY());
    lua_pushnumber(luaState, result.GetZ());
    return 3;
}

void engine::RegisterVector3Functions(lua_State* luaState)
{
    constexpr luaL_Reg vector3Funcs[] =
    {
        {"AngleFrom", script_AngleFromVector3},
        {"Cross", script_CrossVector3},

        {"DistanceFrom", script_DistanceFromVector3},
        {"DistanceSquaredFrom", script_DistanceSquaredFromVector3},

        {"Distance2DFrom", script_Distance2DFromVector3},
        {"Distance2DSquaredFrom", script_Distance2DSquaredFromVector3},

        {"Dot", script_DotVector3},

        {"IsLongerThan", script_IsLongerThanVector3},
        {"IsShorterThan", script_IsShorterThanVector3},

        {"IsUnitVector", script_IsUnitVector3},

        {"Magnitude", script_MagnitudeVector3},
        {"MagnitudeSquared", script_MagnitudeSquaredVector3},

        {"Normalize", script_NormalizeVector3},

        {"ProjectOnto", script_ProjectOntoVector3},
        {"ReflectOnto", script_ReflectOntoVector3},

        {"Rotate", script_RotateVector3},
        {"Scale", script_ScaleVector3},
        {"Translate", script_TranslateVector3},

        {"Lerp", script_LerpVector3},
        {NULL, NULL}
    };

    lua_newtable(luaState);
    luaL_setfuncs(luaState, vector3Funcs, 0);
    lua_setglobal(luaState, "Vec3C");
}