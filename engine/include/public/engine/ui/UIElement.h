#pragma once

/*
*	----- UI Elements -----
*	- Dropdown			
*	- combo box			
*	----------------------
*/

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#include <mutex>

namespace engine
{
    /*
        2D transform for UI elements.

        All members are measured in pixels although type is
        float for ImGui compatibility.
    */
    struct UITransform
    {
        math::Vector2f m_position;
        math::Vector2f m_sizePx;

        UITransform(void)
            : m_position(0.0f), m_sizePx(0.0f)
        {
        }
    };

    class UIElement
    {
    public:
        ENGINE_API							UIElement(void);

                                            UIElement(const UIElement&) = delete;
        ENGINE_API                          UIElement(UIElement&&) noexcept = default;

        ENGINE_API virtual					~UIElement(void) = default;

        ENGINE_API virtual math::Vector2f	GetPosition(void) const noexcept;
        ENGINE_API virtual math::Vector2f	GetScale(void) const noexcept;

        ENGINE_API std::mutex&              GetMutex(void);

        ENGINE_API virtual void				SetPosition(math::Vector2f const& position);
        ENGINE_API virtual void				SetScale(math::Vector2f const& scale);
        ENGINE_API virtual void				SetAutoScale(bool autoResize);
        
        ENGINE_API virtual void				Render(void) = 0;
    
    protected:
        void	AutoScale(f32 regionRatio);

    private:
        void	SetUID(int32 const& uid);

    protected:

        std::mutex  m_mutex;
        UITransform	m_transform;
        int32		m_uid;
        bool		m_autoScale;
        
        friend class Canvas;
    };
}