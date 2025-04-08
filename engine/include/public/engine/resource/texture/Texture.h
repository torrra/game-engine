#pragma once

#include "engine/resource/Resource.h"
#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>

namespace engine
{

	class Texture  final : public IResource
	{
	public:
		ENGINE_API Texture(void);
		ENGINE_API Texture(void* data, int32 width, int32 height, int32 channelCount);
		ENGINE_API ~Texture(void) = default;
		
		ENGINE_API bool LoadResource(const char* fileName) override;
		ENGINE_API void UseTexture(uint32 index = 0) const;
		ENGINE_API static void RemoveTexture(uint32 index = 0);

		uint32 GetTexture(void) const noexcept;
		f32 GetAspectRatio(void) const noexcept;
		math::Vector2i GetSize(void) const noexcept;
	
	private:


        void ProcessTexelData(void* data, int32 channelCount);

		enum ETextureWrapping
		{
			REPEAT = 0x2901,
			MIRROR_REPEAT = 0x8370,
			CLAMP_TO_EDGE = 0x812F,
			CLAMP_TO_BORDER = 0x812D
		};

		enum ETextureFiltering
		{
			NEAREST_NEIGHBOUR = 0x2600,
			BILINEAR = 0x2601
		};

		void SetWrapping(ETextureWrapping const& wrapMode);
		void SetFiltering(ETextureFiltering const& filterMode);
		int32 SetFormat(int32 channelCount);

		math::Vector2i m_size;
		f32 m_aspectRatio;
		uint32 m_texture;
	};
}