#pragma once

#pragma region Editor

#include "Component.h"

#pragma endregion

#pragma region Standard

#include <string>

#pragma endregion

namespace editor
{
    class AudioComponent : public BaseComponent
    {
    public:

        /// Constructor
                        AudioComponent(void);

        /// Destructor
                        ~AudioComponent(void);

    protected:

        /// Functions
        virtual void    SectionContent(void) override;

    private:

        /// Function
        void            DragAndDrop(engine::AudioPlayer* inAudioPlayer);
        void            Sound3DUI(engine::AudioPlayer* inAudioPlayer);
        void            ListenerUI(engine::AudioPlayer* inAudioPlayer);

        /// Private members
        std::string m_soundName = "";

    }; // !Class AudioComponent
} // !Namespace editor