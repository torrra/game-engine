#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Geometry

#include "engine/physics/geometry/Geometry.h"

#pragma endregion

namespace engine
{
	class BoxGeometry : public Geometry
	{
	public :

		/// Constructor
		// Default constructor set to 1x1x1
		ENGINE_API			BoxGeometry(void);
		// Constructor with width, height and depth
		ENGINE_API			BoxGeometry(float inWidth, float inHeight, float inDepth);

		/// Destructor
		ENGINE_API			~BoxGeometry(void) override;

		/// Getters
		ENGINE_API void*	GetNativeGeometry(void) const override;

		ENGINE_API float	GetVolume(void) const override;

	private :

		struct GeometryImpl*	m_geometryImpl	= nullptr;

	}; // !Class BoxGeometry
} // !Namespace engine