#pragma once

#include "engine/CoreTypes.h"
#include "engine/core/Component.h"

#include <math/Vector3.hpp>

namespace engine
{
    class LightSource : public Component
    {

    public:

        enum class ELightType : uint32
        {
            OMNIDIRECTIONAL,
            DIRECTIONAL,
            SPOTLIGHT
        };

        using Component::Component;

        ENGINE_API void Register(void) override {}
        ENGINE_API void Unregister(void) override {}

        ENGINE_API void SerializeText(std::ostream& output,
                                      EntityHandle owner, uint64 index) const override;

        ENGINE_API const char* DeserializeText(const char* text, const char* end) override;

        ENGINE_API const math::Vector3f& GetAmbientColor(void) const;
        ENGINE_API const math::Vector3f& GetDiffuseColor(void) const;
        ENGINE_API const math::Vector3f& GetSpecularColor(void) const;

        ENGINE_API f32                   GetInnerCutoff(void) const;
        ENGINE_API f32                   GetOuterCutoff(void) const;
        ENGINE_API const math::Vector3f& GetAttenuation(void) const;
        ENGINE_API ELightType            GetType(void) const;


        ENGINE_API math::Vector3f& AmbientColor(void);
        ENGINE_API math::Vector3f& DiffuseColor(void);
        ENGINE_API math::Vector3f& SpecularColor(void);

        ENGINE_API f32&            InnerCutoff(void);
        ENGINE_API f32&            OuterCutoff(void);
        ENGINE_API math::Vector3f& Attenuation(void);
        ENGINE_API ELightType&     Type(void);

        ENGINE_API void AddOmniToBuffer(class Buffer& buffer, uint32 bytesOffset) const;
        ENGINE_API void AddDirectionalToBuffer(class Buffer& buffer, uint32 bytesOffset) const;
        ENGINE_API void AddSpotToBuffer(class Buffer& buffer, uint32 bytesOffset) const;


       static constexpr uint32 GetOmniSize(void);
       static constexpr uint32 GetDirectionalSize(void);
       static constexpr uint32 GetSpotSize(void);


    private:

        math::Vector3f CalculateDirection(void) const;

        // Do not move these values, the layout must match the shader buffer

        math::Vector3f  m_ambientColor{ 1.f };
        math::Vector3f  m_diffuseColor{ 1.f };
        math::Vector3f  m_specularColor{ 1.f };
        math::Vector3f  m_attenuation{ 1.0f, 0.007f, 0.0002f };

        f32             m_innerCutoff = 0.81f;
        f32             m_outerCutoff = 0.92f;
        ELightType      m_type = ELightType::OMNIDIRECTIONAL;

    };


    template<> inline
    constexpr Entity::EComponentFlags Entity::GetComponentFlag<LightSource>(void)
    {
        return LIGHT_SOURCE;
    }


    inline constexpr uint32 LightSource::GetOmniSize(void)
    {
        return offsetof(LightSource, m_innerCutoff) -
            offsetof(LightSource, m_ambientColor) + sizeof(math::Vector3f);
    }

    inline constexpr uint32 LightSource::GetDirectionalSize(void)
    {
        return offsetof(LightSource, m_innerCutoff) -
            offsetof(LightSource, m_ambientColor) + sizeof(math::Vector3f);
    }


    inline constexpr uint32 LightSource::GetSpotSize(void)
    {
        return  offsetof(LightSource, m_type) -
            offsetof(LightSource, m_ambientColor) + (sizeof(math::Vector3f) * 2);
    }
}