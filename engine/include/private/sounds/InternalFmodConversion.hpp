#pragma once

#pragma region FMOD

#include <fmod/fmod.hpp>

#pragma endregion

#pragma region Math

#include <math/Vector3.hpp>

#pragma endregion

inline FMOD_VECTOR ToFmodVector(const math::Vector3f& inVector)
{
    return FMOD_VECTOR(inVector.GetX(), inVector.GetY(), inVector.GetZ());
}

inline math::Vector3f ToMathVector(const FMOD_VECTOR& inVector)
{
    return math::Vector3f(inVector.x, inVector.y, inVector.z);
}