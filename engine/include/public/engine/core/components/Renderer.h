#pragma once

#include "engine/EngineExport.h"
#include "engine/core/Component.h"
#include "engine/core/ComponentArray.h"
#include "engine/core/components/Transform.h"

#include <fstream>

namespace engine
{
	class Renderer final : public Component
	{
	public:

		using Component::Component;

		ENGINE_API
		void Register(void) override;

		ENGINE_API
		void Render(const math::Matrix4f& viewProjection,
				    ComponentArray<Transform>& transforms);

		ENGINE_API
		const class Model* GetModel(void) const;

		ENGINE_API
		const class ShaderProgram* GetShader(void) const;

		ENGINE_API
		const class Texture* GetTexture(void) const;

		ENGINE_API
		void SetModel(const class Model* model);

		ENGINE_API
		void SetModel(const char* key);

		ENGINE_API
		void SetShader(const class ShaderProgram* shader);

		ENGINE_API
		void SetShader(const char* key);

		ENGINE_API
		void SetTexture(const class Texture* texture);

		ENGINE_API
		void SetTexture(const char* key);

		ENGINE_API
		void SerializeText(std::ofstream& output,
						   EntityHandle owner,
						   uint64 index) const;

	private:

		void DrawModel(void) const;

		const class Model* m_model = nullptr;
		const class ShaderProgram* m_shader = nullptr;
		const class Texture* m_texture = nullptr;

	};


	template<>
	inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<Renderer>()
	{
		return RENDERER;
	}

}