#pragma once

namespace engine
{
	class Geometry
	{
	public :

		virtual ~Geometry(void) = default;

		virtual void* GetNativeGeometry(void) const = 0;

		virtual float GetVolume(void) const = 0;

	}; // !Class Geometry
} // !Namespace engine
