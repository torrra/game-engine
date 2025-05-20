#pragma once

#pragma region Engine

#include "engine/EngineExport.h"

#pragma endregion

#pragma region Core

#include "engine/core/Component.h"

#pragma endregion

#pragma region Math

#include "math/Vector3.hpp"

#pragma endregion

namespace engine
{
    class NavigationPoint : public Component
    {
    public :

        /// Constructor
        // Component constructor
        ENGINE_API 
                                NavigationPoint(EntityHandle inOwner, class SceneGraph* inScene);
        // Move constructor
        ENGINE_API 
                                NavigationPoint(NavigationPoint&& inOther) = default;

        /// Destructor
        ENGINE_API 
                                ~NavigationPoint(void) override = default;

        /// Operator
        // Move assignement
        ENGINE_API 
            NavigationPoint&    operator=(NavigationPoint&& inOther) = default;

        /// Getter
        ENGINE_API
            math::Vector3f      GetPosition(void) const;

        /// Setter
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

    private :

        /// Constructor
        // Default constructor deleted
                                NavigationPoint(void) = delete;
        // Copy constructor deleted
                                NavigationPoint(const NavigationPoint& inOther) = delete;

        /// Operator
        // Copy assignement
        NavigationPoint&        operator=(const NavigationPoint& inOther) = delete;

        /// Private members
        math::Vector3f m_position = math::Vector3f::Zero();

    }; // !Class NavigationPoint
} // !Namespace engine