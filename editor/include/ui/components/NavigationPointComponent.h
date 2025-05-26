#pragma once

#pragma region Editor

#include "Component.h"

#pragma endregion

#pragma region Engine

#include <engine/core/components/NavigationPoint.h>

#pragma endregion

namespace editor
{
    class NavigationPointComponent : public BaseComponent
    {
    public:

        /// Constructor
                        NavigationPointComponent(void);

        /// Destructor
                        ~NavigationPointComponent(void) = default;

    protected:

        virtual void    SectionContent(void) override;
    }; // !Class NavigationPointComponent
} // !Namespace editor