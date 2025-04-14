#pragma once

#include "Component.h"

#include <engine/CoreTypes.h>
#include <engine/core/components/Transform.h>

// TODO: fix local? or global? space transform... (i still cant tell which space it is in)

namespace editor
{
    class TransformProperty : public ComponentProperty
    {
    public:
        TransformProperty(void);
        ~TransformProperty(void);

        void SetTransform(engine::Transform* transform);
    
    protected:
        virtual void SectionContent(void) override;

    private:
        bool InputField(const char* uid, f32* value, f32 increment);

        engine::Transform* m_transformData;
        math::Vector3f m_rotation;
    };
}