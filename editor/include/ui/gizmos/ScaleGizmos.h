#pragma once

#include "ui/gizmos/Gizmos.h"
#include <string>

namespace editor
{
    class ScaleGizmos : public Gizmos
    {
    public:
        ScaleGizmos(void);

        virtual void IsAxisSelected(std::string const& wndName, engine::Transform* transform) override;
    };
}