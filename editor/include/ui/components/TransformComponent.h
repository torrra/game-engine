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

        void SetTransform(engine::Transform* transform);
    
    protected:
        virtual void SectionContent(void) override;

    private:
        engine::Transform* m_transformData;
        math::Vector3f m_lastRot;
        math::Vector3f m_rotation;
    };
}