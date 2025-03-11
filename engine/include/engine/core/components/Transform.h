#pragma once

#pragma region core

#include "core/Component.h"

#pragma endregion

#pragma region math

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

#pragma endregion


namespace engine
{
    class Transform final : public Component
    {
	private:

		struct AbsoluteCache
		{
			math::Quatf		m_rotation = math::Quatf(1.f, 0.f, 0.f, 0.f);
			math::Vector3f	m_position = math::Vector3f::Zero();
			math::Vector3f	m_scale = math::Vector3f::One();
			bool			m_cached = false;
		};

	public:

		using Component::Component;

		/// Public Constructors
		// Copy constructor set to default
		ENGINE_API 						Transform(const Transform& inTransform) = default;
		// Move constructor set to default
		ENGINE_API 						Transform(Transform&& inTransform) = default;

		/// Destructors
		// Destructor set to default
		ENGINE_API 						~Transform(void) = default;

		/// Transform to matrix
		// Convert transform to matrix without scale parameters
		ENGINE_API static math::Matrix4f	ToMatrixWithoutScale(Transform& inTransform);
		// Convert transform to matrix with scale parameters
		ENGINE_API static math::Matrix4f	ToMatrixWithScale(Transform& inTransform);
		// Interpolate two transforms
		ENGINE_API static Transform			Interpolate(Transform& inStartTransform,
														Transform& inEndTransform,
														f32 inTime);
		
		// Copy the position from another transform
		ENGINE_API void					CopyPosition(const Transform& inTransform);
		// Copy the rotation from another transform
		ENGINE_API void 				CopyRotation(const Transform& inTransform);
		// Copy the scale from another transform
		ENGINE_API void 				CopyScale(const Transform& inTransform);

		ENGINE_API void					Update(void) {}
		ENGINE_API void					Register(void) {}

		ENGINE_API void					Move(math::Vector3f translation);
		ENGINE_API void					Rotate(f32 angleX, f32 angleY, f32 angleZ);
		ENGINE_API void					Rotate(const math::Quatf& rotation);

		/// Getters
		// Get the position of the transform
		ENGINE_API math::Vector3f		GetPosition(void) const;
		// Get the rotation of the transform
		ENGINE_API math::Quatf			GetRotation(void) const;
		// Get the scale of the transform
		ENGINE_API math::Vector3f		GetScale(void) const;
		// Get the transform with all parameters
		ENGINE_API Transform			GetTransform(void) const;

		ENGINE_API math::Vector3f		GetAbsolutePosition(void);

		ENGINE_API math::Vector3f		GetAbsoluteScale(void);

		ENGINE_API math::Quatf			GetAbsoluteRotation(void);

		/// Setters
		// Set the position of the transform
		ENGINE_API void					SetPosition(const math::Vector3f& inPosition);
		// Set the rotation of the transform
		ENGINE_API void					SetRotation(const math::Quatf& inRotation);
		// Set the scale of the transform
		ENGINE_API void					SetScale(const math::Vector3f& inScale);
		// Set all parameters of the transform
		ENGINE_API void 				SetTransform(const math::Vector3f& inPosition,
													 const math::Quatf& inRotation,
													 const math::Vector3f& inScalein =
													 math::Vector3f(1.0f, 1.0f, 1.0f));

		/// Operators
		// Copy assignement set to default
		Transform&						operator=(const Transform& inTransform) = default;
		// Move assignement set to default
		Transform&						operator=(Transform&& inTransform) = default;
		// Operator to print a transform
		std::ostream&					operator<<(std::ostream& os);

	private:

		void UpdateAxes(void);
		void CacheParentTransform(void);

		/// Private members
		math::Quatf		m_rotation = math::Quatf(1.f, 0.f, 0.f, 0.f);
		math::Vector3f	m_position = math::Vector3f::Zero();
		math::Vector3f	m_scale = math::Vector3f::One();
		math::Vector3f  m_forward = math::Vector3f::Front();
		math::Vector3f  m_right = math::Vector3f::Right();
		math::Vector3f  m_up = math::Vector3f::Up();

		AbsoluteCache   m_parentCache;

    }; // !Class Transform

	template <>
	struct UpdateAfterParent<Transform>
	{
		static constexpr bool m_value = true;
	};

	template<>
	inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<Transform>()
	{
		return TRANSFORM;
	}

} // !Namespace engine