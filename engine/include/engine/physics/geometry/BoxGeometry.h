#pragma once

#pragma region Geometry

#include "physics/geometry/Geometry.h"

#pragma endregion

namespace engine
{
	class BoxGeometry : public Geometry
	{
	public :

		/// Constructor
		// Default constructor set to 1x1x1
				BoxGeometry() = default;
		// Constructor with width, height and depth
				BoxGeometry(float inWidth, float inHeight, float inDepth);

		/// Destructor
				~BoxGeometry() override = default;

		/// Getters
		void*	GetNativeGeometry() override;

		float	GetVolume() override;

	private :

		float m_width	= 1.f;
		float m_height	= 1.f;
		float m_depth	= 1.f;

	}; // !Class BoxGeometry
} // !Namespace engine