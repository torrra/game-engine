#pragma once

#pragma region mathlib

#include "math/VectorGeneric.hpp"
#include "math/MatrixGeneric.hpp"
#include "math/Quaternion.hpp"

#pragma endregion

namespace Engine
{
	class Transform
	{
	public:

		/// Constructors
							Transform(void)	= default;
		explicit			Transform(const lm::Vector3f& inPosition);
		explicit			Transform(const lm::Quatf& inRotation);
							Transform(const lm::Vector3f& inPosition, 
									  const lm::Quatf& inRotation,
									  const lm::Vector3f& inScale = 
									  lm::Vector3f(1.0f, 1.0f, 1.0f));
							Transform(const Transform& inTransform) = default;
							Transform(Transform&& inTransform) = default;

		/// Destructors
							~Transform(void) = default;
	
		/// Transform to matrix
		lm::Matrix4f		ToMatrixWithoutScale(Transform& inTransform);
		lm::Matrix4f		ToMatrixWithScale(Transform& inTransform);
		static Transform	Interpolate(Transform& inStartTransform, 
										Transform& inEndTransform,
										float inTime);
		void				CopyPosition(const Transform& inTransform);
		void 				CopyRotation(const Transform& inTransform);
		void 				CopyScale(const Transform& inTransform);

		/// Getters
		lm::Vector3f		GetPosition(void) const;
		lm::Quatf			GetRotation(void) const;
		lm::Vector3f		GetScale(void) const;
		Transform			GetTransform(void) const;

		/// Setters
		void				SetPosition(const lm::Vector3f& inPosition);
		void				SetRotation(const lm::Quatf& inRotation);
		void				SetScale(const lm::Vector3f& inScale);
		void 				SetTransform(const lm::Vector3f& inPosition, 
										 const lm::Quatf& inRotation, 
										 const lm::Vector3f& inScalein = 
										 lm::Vector3f(1.0f, 1.0f, 1.0f));

		/// Operators
		Transform&			operator=(const Transform& inTransform) = default;
		Transform&			operator=(Transform&& inTransform) = default;

		/// Private members
		lm::Vector3f		m_position	= lm::Vector3f(0.f, 0.f, 0.f);
		lm::Quatf			m_rotation	= lm::Quatf(1.f, 0.f, 0.f, 0.f);
		lm::Vector3f		m_scale		= lm::Vector3f(1.f, 1.f, 1.f);

	}; // !Class Transform
} // !Namespace Engine