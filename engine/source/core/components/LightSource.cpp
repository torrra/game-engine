#include "core/components/LightSource.h"
#include "core/SceneGraph.h"

#include "resource/model/Buffer.h"
#include "serialization/TextSerializer.h"

#include "InternalOpenGLError.hpp"

namespace engine
{
    void LightSource::SerializeText(std::ostream& output, EntityHandle owner, uint64 index) const
    {
        output << "[LightSource]\n    ";

        if constexpr (UpdateAfterParent<LightSource>::m_value)
        {
            text::Serialize(output, "index", index);
            output << "\n    ";
        }

        text::Serialize(output, "owner", owner);
        output << "\n    ";
        text::Serialize(output, "ambientColor", m_ambientColor);
        output << "\n    ";
        text::Serialize(output, "diffuseColor", m_diffuseColor);
        output << "\n    ";
        text::Serialize(output, "specularColor", m_specularColor);
        output << "\n    ";
        text::Serialize(output, "attenuation", m_attenuation);
        output << "\n    ";

        text::Serialize(output, "type", (uint32)m_type);
        output << "\n    ";

        if (m_type == ELightType::SPOTLIGHT)
        {
            text::Serialize(output, "innerCutoff", m_innerCutoff);
            output << "\n    ";
            text::Serialize(output, "outerCutoff", m_outerCutoff);
            output << "\n    ";
        }

        text::Serialize(output, "flags", m_flags);
        output << '\n';
    }

    const char* LightSource::DeserializeText(const char* text, const char* end)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_owner);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_ambientColor);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_diffuseColor);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_specularColor);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_attenuation);

        uint32 type = 0;

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, type);

        m_type = (ELightType)type;

        if (m_type == ELightType::SPOTLIGHT)
        {
            MOVE_TEXT_CURSOR(text, end);
            text = text::DeserializeReal(text, m_innerCutoff);

            MOVE_TEXT_CURSOR(text, end);
            text = text::DeserializeReal(text, m_outerCutoff);
        }

        return text::DeserializeInteger(text, m_flags);
    }

    const math::Vector3f& LightSource::GetAmbientColor(void) const
    {
        return m_ambientColor;
    }

    const math::Vector3f& LightSource::GetDiffuseColor(void) const
    {
        return m_diffuseColor;
    }

    const math::Vector3f& LightSource::GetSpecularColor(void) const
    {
        return m_specularColor;
    }

    f32 LightSource::GetInnerCutoff(void) const
    {
        return m_innerCutoff;
    }

    f32 LightSource::GetOuterCutoff(void) const
    {
        return m_outerCutoff;
    }

    const math::Vector3f& LightSource::GetAttenuation(void) const
    {
        return m_ambientColor;
    }

    LightSource::ELightType LightSource::GetType(void) const
    {
        return m_type;
    }

    math::Vector3f& LightSource::AmbientColor(void)
    {
        return m_ambientColor;
    }

    math::Vector3f& LightSource::DiffuseColor(void)
    {
        return m_diffuseColor;
    }

    math::Vector3f& LightSource::SpecularColor(void)
    {
        return m_specularColor;
    }

    f32& LightSource::InnerCutoff(void)
    {
        return m_innerCutoff;
    }

    f32& LightSource::OuterCutoff(void)
    {
        return m_outerCutoff;
    }

    math::Vector3f& LightSource::Attenuation(void)
    {
        return m_attenuation;
    }

    LightSource::ELightType& LightSource::Type(void)
    {
        return m_type;
    }


    math::Vector3f LightSource::CalculateDirection(void) const
    {
        math::Vector3f direction{ 0.f };

        if (const Transform* transform = m_currentScene->GetCachedTransform(m_owner))
        {
            direction = Transform::ToWorldRotation(*transform).Rotate(math::Vector3f::Down());
        }

        return direction;
    }

    void LightSource::AddOmniToBuffer(Buffer& buffer, uint32 bytesOffset) const
    {
        constexpr uint64 omniDataSize = offsetof(LightSource, m_innerCutoff) - 
                                        offsetof(LightSource, m_ambientColor);

        math::Vector3f position{ 0.f };

        if (const Transform* transform = m_currentScene->GetCachedTransform(m_owner))
            position = Transform::ToWorldPosition(*transform);

        OpenGLError();
        buffer.SetData(&position, sizeof(position), bytesOffset);
        bytesOffset += sizeof(position);

        OpenGLError();
        buffer.SetData((void*) &m_ambientColor, omniDataSize, bytesOffset);

        OpenGLError();
    }

    void LightSource::AddDirectionalToBuffer(Buffer& buffer, uint32 bytesOffset) const
    {
        constexpr uint64 directionalDataSize = offsetof(LightSource, m_innerCutoff) -
                                               offsetof(LightSource, m_ambientColor);

        math::Vector3f direction = CalculateDirection();

        buffer.SetData(&direction, sizeof(direction), bytesOffset);
        bytesOffset += sizeof(direction);

        buffer.SetData((void*)&m_ambientColor, directionalDataSize, bytesOffset);
        OpenGLError();
    }

    void LightSource::AddSpotToBuffer(Buffer& buffer, uint32 bytesOffset) const
    {
        constexpr uint64 spotDataSize = offsetof(LightSource, m_type) -
            offsetof(LightSource, m_ambientColor);

        const Transform* transform = m_currentScene->GetCachedTransform(m_owner);



        math::Vector3f vectors[2] = { (transform) ? Transform::ToWorldPosition(*transform) : math::Vector3f{0.f}, CalculateDirection() };

        buffer.SetData(vectors, sizeof(vectors), bytesOffset);
        bytesOffset += sizeof(vectors);

        buffer.SetData((void*)&m_ambientColor, spotDataSize, bytesOffset);

        OpenGLError();
    }

}