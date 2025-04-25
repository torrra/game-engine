#pragma once

#include "Component.h"
#include <engine/CoreTypes.h>
#include <engine/core/components/Transform.h>

namespace editor
{
    class TransformComponent : public BaseComponent
    {
    public:
        TransformComponent(void);
        ~TransformComponent(void);

    protected:
        virtual void SectionContent(void) override;

    private:
        inline void SyncRotation(engine::Transform* transform);
        inline void InputRotation(engine::Transform* transform, const char* uid, uint8 index);

        math::Vector3f m_lastRot;
        math::Vector3f m_rotation;
        bool m_isRotationSynced;
    };
}