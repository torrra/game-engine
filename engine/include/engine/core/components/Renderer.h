#pragma once

#include "engine/EngineExport.h"
#include "engine/core/Component.h"

namespace engine
{
	class Renderer final : public Component
	{
	public:

		using Component::Component;

		ENGINE_API
		void Register(void) override;

		ENGINE_API
		void Render(const math::Matrix4f& viewProjection);

		ENGINE_API
		const class Model* GetModel(void);

		ENGINE_API
		const class ShaderProgram* GetShader(void);

		ENGINE_API
		const class Texture* GetTexture(void);

		ENGINE_API
		void SetModel(Model* model);

		ENGINE_API
		void SetModel(const char* key);

		ENGINE_API
		void SetShader(class ShaderProgram* shader);

		ENGINE_API
		void SetShader(const char* key);

		ENGINE_API
		void SetTexture(class Texture* texture);

		ENGINE_API
		void SetTexture(const char* key);

	private:

		void DrawModel(void);

		class Model* m_model = nullptr;
		class ShaderProgram* m_shader = nullptr;
		class Texture* m_texture = nullptr;

	};
}