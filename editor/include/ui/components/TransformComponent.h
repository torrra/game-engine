#pragma once

#include "Component.h"

#include <engine/CoreTypes.h>
#include <engine/core/components/Transform.h>

// TODO: fix local? or global? space transform... (i still cant tell which space it is in)

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
        math::Vector3f m_rotation;
    };
}