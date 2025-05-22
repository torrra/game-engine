#pragma once

#pragma region Engine

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"

#pragma endregion

#pragma region Core

#include "engine/core/Component.h"

#pragma endregion

#pragma region Math

#include "math/Vector3.hpp"
#include <math/Matrix4.hpp>

#pragma endregion

namespace engine
{
    class NavigationPoint : public Component
    {
    public :

        /// Constructor
        /*
            Component constructor
            <param> [in] inOwner : EntityHandle : The owner of this component
            <param> [in] inScene : SceneGraph   : The current scene
        */
        ENGINE_API 
                                NavigationPoint(EntityHandle inOwner, class SceneGraph* inScene);
        // Move constructor
        ENGINE_API 
                                NavigationPoint(NavigationPoint&& inOther) noexcept = default;

        /// Destructor
        ENGINE_API 
                                ~NavigationPoint(void) override = default;

        /// Operator
        // Move assignement
        ENGINE_API 
            NavigationPoint&    operator=(NavigationPoint&& inOther) = default;

        /// Getter
        // Get the navigation point position
        ENGINE_API [[nodiscard]]
            math::Vector3f      GetPosition(void) const;

        /// Setter
        /*
            Set the position of the navigation point
            <param> [in] inPosition : Vector3f : Position to set the navigation point
        */
        ENGINE_API
            void                SetPosition(const math::Vector3f& inPosition);

        /// Functions
        /*
            Serialize rigidbody's member
            <param> [in] output : output stream
            <param> [in] owner  : Owner of the rigidbody component
            <param> [in] index  : Index of this component in an array of rigidbody component
        */
        ENGINE_API
            void			    SerializeText(std::ostream& output,
                                              EntityHandle owner,
                                              uint64 index) const override;
        /*
            Deserialize rigidibody's member
            <param> [in] text   : The text to deserialize
            <param> [in] end    : The end of the file
        */
        ENGINE_API
            const char*         DeserializeText(const char* text, const char* end) override;

        ENGINE_API
            void		        Register(void) override;
        ENGINE_API
            void                Unregister(void) override;

        ENGINE_API
            void                RenderNavPoint(const math::Matrix4f& inProjView);


    private :

        /// Constructor
        // Default constructor deleted
                                NavigationPoint(void) = delete;
        // Copy constructor deleted
                                NavigationPoint(const NavigationPoint& inOther) = delete;

        /// Operator
        // Copy assignement
            NavigationPoint&    operator=(const NavigationPoint& inOther) = delete;

        /// Private members
        math::Vector3f m_position = math::Vector3f::Zero();

    }; // !Class NavigationPoint

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<NavigationPoint>()
    {
        return NAVIGATION_POINT;
    }

} // !Namespace engine