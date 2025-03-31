#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/viewport/FrameBuffer.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

namespace engine
{
	class Viewport
	{
	public:
		ENGINE_API Viewport(void) = default;
		ENGINE_API ~Viewport(void) = default;

		ENGINE_API void Update(FrameBuffer& fbo);

	};

	class ViewportV2
	{
	public:
		ENGINE_API ViewportV2(FrameBuffer const& fbo);
		ENGINE_API ~ViewportV2(void) = default;

		ENGINE_API void Update(void);
		ENGINE_API void RenderScene(class SceneGraph* sceneGraph, math::Vector4f bgColor);
	
	private:
		math::Vector2i m_prevSize;
		FrameBuffer m_fbo;
		bool m_render;
	};
}