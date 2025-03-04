#pragma once

#include "engine/resource/Resource.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>

#include <stdint.h>


namespace engine
{

	class Texture : public IResource
	{
	public:
		ENGINE_API Texture(void);
		ENGINE_API ~Texture(void) = default;
		
		ENGINE_API virtual void LoadResource(const char* fileName) override;
		ENGINE_API void UseTexture(void);
		ENGINE_API static void RemoveTexture(void);
	
	private:
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

		math::Vector2<int> m_size;
		uint32_t m_texture;
	};
}