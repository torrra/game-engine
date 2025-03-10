#pragma once

#pragma region math

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

#pragma endregion

#pragma region core

#include "core/Component.h"

#pragma endregion

namespace engine
{
    class Transform final : public Component
    {
	private:

		enum E_TransformFlag : uint8
		{
			ABSOLUTE = 1
		};

	public:

		using Component::Component;

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

		ENGINE_API void					Update(void) {}

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

	private:

		/// Private members
		lm::Vector3f	m_position = lm::Vector3f(0.f, 0.f, 0.f);
		lm::Quatf		m_rotation = lm::Quatf(1.f, 0.f, 0.f, 0.f);
		lm::Vector3f	m_scale = lm::Vector3f(1.f, 1.f, 1.f);
		uint8			m_transformFlags = 0;

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