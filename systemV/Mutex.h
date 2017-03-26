#pragma once

#include <string>

#include "bsemaphore.h"
#include "Log/Log.h"

namespace art{
    class Mutex{
    public:
        Mutex(std::string idString, int projID);
        ~Mutex();

        void lock();
        void unlock();
        int pendingCount();

        void remove();
    private:
        int semid;
    };

};
