#pragma once

#include "ui/gizmos/Gizmos.h"
#include <string>

namespace editor
{
    class RotationGizmos : public Gizmos
    {
    public:
        RotationGizmos(void);

        virtual void IsAxisSelected(std::string const& wndName, engine::Transform* transform) override;
    };
}