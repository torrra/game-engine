#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

namespace engine
{
	struct GeometryImpl;

	enum EGeometryType
	{
		BOX,
		SPHERE,
		CAPSULE
	};

	class Geometry
	{
	public :

		/// Constructor
		// Default constructor set to a box geometry by default
		ENGINE_API					Geometry(void);
		// Constructor with enum parameter to choose the geometry
		// Values are set by default
		ENGINE_API					Geometry(EGeometryType geometryType);

		/// Destructor
		ENGINE_API					~Geometry(void);

		/// Getter
		ENGINE_API GeometryImpl&	GetGeometryImpl(void) const;

		/// Public members
		EGeometryType	m_geometryType = EGeometryType::BOX;

	private :

		/// Private members
		GeometryImpl*	m_geometryImpl = nullptr;

	}; // !Class Geometry
} // !Namespace engine
