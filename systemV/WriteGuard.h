#pragma once
#include "Mutex.h"

#include <memory>

namespace art
{
    /// @class Concurrent writer guard.
    /// @details https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem, second readers-writers problem solution.
    ///         This IPC solution is for multi-processes readers safely sharing some resource.
    class WriteGuard{
    public:
        WriteGuard(std::shared_ptr<Mutex> res, std::shared_ptr<Mutex> rtry, std::shared_ptr<Mutex> wmt, int *wcount);
        void begin();
        void end();
        
        // how many writers are writing or pending (in queue) to write the resource
        int getWritersCount();    
     
    protected:
        std::shared_ptr<Mutex> readTry = nullptr; ///< Indicates a reader is trying to access the resource.
        std::shared_ptr<Mutex> wmutex = nullptr;   ///<  Lock to protect writecount;
        std::shared_ptr<Mutex> resource = nullptr; ///< Lock to protect resource;

        // writing writers count + pending writers count
        int& writersCount;
    };

};


