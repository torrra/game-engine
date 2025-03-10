#pragma once

#pragma region mathlib

#include "math/VectorGeneric.hpp"
#include "math/MatrixGeneric.hpp"
#include "math/Quaternion.hpp"

#pragma endregion

#include "engine/EngineExport.h"

#include <ostream>

namespace Engine
{
	class Transform
	{
	public:

		/// Constructors
		ENGINE_API 						Transform(void)	= default;
		ENGINE_API explicit				Transform(const lm::Vector3f& inPosition);
		ENGINE_API explicit				Transform(const lm::Quatf& inRotation);
		ENGINE_API 						Transform(const lm::Vector3f& inPosition,
												  const lm::Quatf& inRotation,
												  const lm::Vector3f& inScale = 
												  lm::Vector3f(1.0f, 1.0f, 1.0f));
		ENGINE_API 						Transform(const Transform& inTransform) = default;
		ENGINE_API 						Transform(Transform&& inTransform) = default;

		/// Destructors
		ENGINE_API 						~Transform(void) = default;
	
		/// Transform to matrix
		ENGINE_API static lm::Matrix4f	ToMatrixWithoutScale(Transform& inTransform);
		ENGINE_API static lm::Matrix4f	ToMatrixWithScale(Transform& inTransform);
		ENGINE_API static Transform		Interpolate(Transform& inStartTransform,
													Transform& inEndTransform,
													float inTime);
		ENGINE_API void					CopyPosition(const Transform& inTransform);
		ENGINE_API void 				CopyRotation(const Transform& inTransform);
		ENGINE_API void 				CopyScale(const Transform& inTransform);

		/// Getters
		ENGINE_API lm::Vector3f			GetPosition(void) const;
		ENGINE_API lm::Quatf			GetRotation(void) const;
		ENGINE_API lm::Vector3f			GetScale(void) const;
		ENGINE_API Transform			GetTransform(void) const;

		/// Setters
		ENGINE_API void					SetPosition(const lm::Vector3f& inPosition);
		ENGINE_API void					SetRotation(const lm::Quatf& inRotation);
		ENGINE_API void					SetScale(const lm::Vector3f& inScale);
		ENGINE_API void 				SetTransform(const lm::Vector3f& inPosition,
													 const lm::Quatf& inRotation, 
													 const lm::Vector3f& inScalein = 
													 lm::Vector3f(1.0f, 1.0f, 1.0f));

		/// Operators
		Transform&						operator=(const Transform& inTransform) = default;
		Transform&						operator=(Transform&& inTransform) = default;
		std::ostream&					operator<<(std::ostream& os);

		/// Private members
		lm::Vector3f	m_position	= lm::Vector3f(0.f, 0.f, 0.f);
		lm::Quatf		m_rotation	= lm::Quatf(1.f, 0.f, 0.f, 0.f);
		lm::Vector3f	m_scale		= lm::Vector3f(1.f, 1.f, 1.f);

	}; // !Class Transform
} // !Namespace Engine