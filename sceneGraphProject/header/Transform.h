#pragma once

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

#include "Component.h"

namespace engine
{
    class Transform final : public Component
    {

    private:

	    math::Quatf    m_localRotation;
	    math::Quatf    m_absoluteRotation;

	    math::Vector3f m_localPosition;
	    math::Vector3f m_localScale;

	    math::Vector3f m_absolutePosition;
	    math::Vector3f m_absoluteScale;
    };

	template <>
	struct UpdateAfterParent<Transform>
	{
		static constexpr bool m_value = true;
	};
}