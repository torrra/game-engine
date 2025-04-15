#pragma once

#include <functional>

namespace engine
{
    class Engine;
}

namespace editor
{
    class MenuBar
    {
    public:
        MenuBar(void) = default;

        void Render(class ::engine::Engine& engine);
    private:
    };
}