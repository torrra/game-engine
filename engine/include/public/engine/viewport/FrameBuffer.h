#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/resource/texture/Texture.h"

#include <math/Vector2.hpp>

namespace engine
{
	class FrameBuffer
	{
	public:
		//FrameBuffer(void) = delete;
		FrameBuffer(void);
		ENGINE_API ~FrameBuffer(void);

		void Init(int32 width, int32 height);
		void RescaleFBO(int32 width, int32 height);
		ENGINE_API void Bind(void) const;
		ENGINE_API void UnBind(void) const;

		uint64 GetFrameTexture(void) const noexcept;

	private:
		uint32 m_texture;
		uint32 m_fbo;
		uint32 m_rbo;
	};
}