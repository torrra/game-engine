#pragma once

#pragma region math

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

#pragma endregion

#pragma region core

#include "engine/core/Component.h"

#pragma endregion

namespace engine
{
    class Transform final : public Component
    {
    public:

        using Component::Component;

        /// Public Constructors
        // Copy constructor set to default
        ENGINE_API 							Transform(const Transform& inTransform) = default;
        // AddTranslation constructor set to default
        ENGINE_API 							Transform(Transform&& inTransform) = default;

        /// Destructors
        // Destructor set to default
        ENGINE_API 							~Transform(void) = default;

        /// Transform to matrix
        // Convert transform to matrix without scale parameters
        ENGINE_API	static math::Matrix4f	ToMatrixWithoutScale(const Transform& inTransform);
        // Convert transform to matrix with scale parameters
        ENGINE_API	static math::Matrix4f	ToMatrixWithScale(Transform& inTransform);
        // Interpolate two transforms
        ENGINE_API	static Transform		Interpolate(const Transform& inStartTransform,
                                                        const Transform& inEndTransform,
                                                        f32 inTime);


        ENGINE_API	static math::Matrix4f	ToWorldMatrix(Transform& inTransform);


        // Copy the position from another transform
        ENGINE_API	void					CopyPosition(const Transform& inTransform);
        // Copy the rotation from another transform
        ENGINE_API	void 					CopyRotation(const Transform& inTransform);
        // Copy the scale from another transform
        ENGINE_API	void 					CopyScale(const Transform& inTransform);
        // Update the transform in regard to the given transform
        ENGINE_API	void					Update(const Transform& inTransform);
        ENGINE_API	void					Register(void);

        /// Getters
        // Get the position of the transform
        ENGINE_API	math::Vector3f			GetPosition(void) const;
        // Get the rotation of the transform
        ENGINE_API	math::Quatf				GetRotation(void) const;
        // Get the scale of the transform
        ENGINE_API	math::Vector3f			GetScale(void) const;
        // Get the transform with all parameters
        ENGINE_API	Transform				GetTransform(void) const;

        /// Setters
        // Set the position of the transform
        ENGINE_API	Transform&				SetPosition(const math::Vector3f& inPosition);
        // Set the rotation of the transform
        ENGINE_API	Transform&              SetRotation(const math::Quatf& inRotation);
        // Set the scale of the transform
        ENGINE_API	Transform&              SetScale(const math::Vector3f& inScale);
        // Set the transform
        ENGINE_API  Transform&              SetTransform(const Transform& inTransform);
        // Set all parameters of the transform
        ENGINE_API	Transform&              SetTransform(const math::Vector3f& inPosition,
                                                         const math::Quatf& inRotation,
                                                         const math::Vector3f& inScalein =
                                                         math::Vector3f(1.0f, 1.0f, 1.0f));

        ENGINE_API math::Vector3f&           SetPosition(void);
        ENGINE_API math::Vector3f            GetEulerRotation(void) const noexcept;
        ENGINE_API math::Vector3f&           SetScale(void);

        // Add translation to the current translation value
        ENGINE_API void					AddTranslation(const math::Vector3f& inTranslation);

        // Add rotation in euler degrees to the current rotation
        ENGINE_API void					AddRotation(f32 angleX, f32 angleY, f32 angleZ);

        // Add quaternion rotation to current rotation
        ENGINE_API void					AddRotation(const math::Quatf& inRotation);

        // Multiply current scale by new scale
        ENGINE_API void					AddScale(const math::Vector3f& inScale);

        // Serialize rotation, position and scale in plain text
        ENGINE_API void					SerializeText(std::ostream& output,
                                                      EntityHandle owner,
                                                      uint64 index) const;

        ENGINE_API const char* DeserializeText(const char* text, const char* end);


        /// Operators
        // Copy assignement set to default
                    Transform&				operator=(const Transform& inTransform) = default;
        // AddTranslation assignement set to default
                    Transform&				operator=(Transform&& inTransform) = default;
        // Operator to print a transform
        std::ostream& operator<<(std::ostream& os);
    
        friend ENGINE_API std::ostream& operator<<(std::ostream& os, Transform& transform);

    private:

        void UpdateLocalMatrix(void);


        /// Private members

        math::Matrix4f  m_localMat	= math::Matrix4f{ 1.f };

        math::Quatf		m_rotation	= math::Quatf(1.f, 0.f, 0.f, 0.f);
        math::Vector3f	m_position	= math::Vector3f::Zero();
        math::Vector3f	m_scale		= math::Vector3f::One();

        bool			m_dirty		= false;

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

    template <>
    struct UpdateComponent<Transform>
    {
        static constexpr bool m_value = true;
    };

} // !Namespace engine
