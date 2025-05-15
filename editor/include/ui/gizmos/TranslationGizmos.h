#pragma once

#include "ui/gizmos/Gizmos.h"
#include <string>

namespace editor
{
    class TranslateGizmos : public Gizmos
    {
    public:
        TranslateGizmos(void);

        virtual void IsAxisSelected(std::string const& wndName, engine::Transform* transform) override;
    };
}