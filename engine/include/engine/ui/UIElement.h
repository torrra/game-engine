#pragma once

/*
*	----- UI Elements -----
*	- Canvas
*	- Text label
*	- Image
*	- progress bar
*	- Button
*	- Dropdown
*	- combo box
*	----------------------
*/

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

namespace engine
{
	struct Transform
	{
		math::Vector2f m_position;
		math::Vector2f m_scale;

		Transform(void)
			: m_position(0.0f), m_scale(1.0f)
		{
		}
	};

	class UIElement
	{
	public:
		ENGINE_API UIElement(void);
		ENGINE_API virtual ~UIElement(void) = default;

		ENGINE_API virtual math::Vector2f GetPosition(void) const noexcept;
		ENGINE_API virtual math::Vector2f GetScale(void) const noexcept;

		ENGINE_API virtual void SetPosition(math::Vector2f const& position);
		ENGINE_API virtual void SetScale(math::Vector2f const& scale);

		ENGINE_API virtual void Render(void) = 0;

	protected:
		Transform m_transform;
	};
}