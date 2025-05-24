#pragma once

#include "ui/components/Component.h"
#include <engine/core/components/Camera.h>
#include <math/Vector3.hpp>

namespace editor
{
    class CameraComponent : public BaseComponent
    {
    public:
        CameraComponent(void);
        ~CameraComponent(void);

    protected:
        virtual void SectionContent(void) override;

    private:
        void Rotation(engine::Camera* camera);
        void FOV(engine::Camera* camera);
        void NearPlane(engine::Camera* camera);
        void FarPlane(engine::Camera* camera);

        inline void InputRotation(engine::Camera* camera, const char* uid, uint8 index);
        inline void SyncRotation(engine::Camera* camera);

        math::Vector3f m_rotation;
        math::Vector3f m_lastRot;
        bool m_rotationSynced;

    };
}