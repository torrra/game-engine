#pragma once

namespace engine
{
    class ICollisionListener
    {
    public :

        /// Destructor
        virtual ~ICollisionListener() {}

        /// Functions
        virtual void OnCollisionEnter(void* inOther)    {};
        virtual void OnCollisionExit(void* inOther)     {};
        virtual void OnCollisionStay(void* inOther)     {};

        virtual void OnTriggerEnter(void* inOther)      {};
        virtual void OnTriggerExit(void* inOther)       {};
        virtual void OnTriggerStay(void* inOther)       {};

    }; // !Class ICollisionListener
} // !Namespace engine