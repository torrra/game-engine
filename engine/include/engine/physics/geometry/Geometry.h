#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#include "BoxGeometry.h"

namespace engine
{
	class Geometry
	{
	public :

		/// Destructor
		// Default virtual destructor
		ENGINE_API virtual			~Geometry(void) = default;

		/// Getter
		// Virtual getter for the native geometry
		ENGINE_API virtual void*	GetNativeGeometry(void) const = 0;
		// Virtual getter for the volume
		ENGINE_API virtual float	GetVolume(void) const = 0;

	}; // !Class Geometry
} // !Namespace engine
