#pragma once
#include "Mutex.h"

#include <memory>

namespace art
{
    /// @class Concurrent reader guard.
    /// @details https://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem, second readers-writers problem solution.
    ///         This IPC solution is for multi-processes readers safely sharing some resource.
    class ReadGuard{
    public:
        ReadGuard(std::shared_ptr<Mutex> res, std::shared_ptr<Mutex> rtry, std::shared_ptr<Mutex> rmt,  int *rcount);
        ~ReadGuard();
        
        // try to read the resource
        // should be called with end() pairwise
        void begin();

        // read operation done
        void end();

        // how many readers are reading the resource
        int getReadingCount();

        // how many readers are pending (in queue) to read the resource 
        int getPendingCount();
     
    protected:
        std::shared_ptr<Mutex> readTry = nullptr; ///< Indicates a reader is trying to access the resource.
        std::shared_ptr<Mutex> rmutex = nullptr;   ///<  Lock to protect readingCount;
        std::shared_ptr<Mutex> resource = nullptr; ///< Lock to protect resource;
        int& readingCount; ///< count of readers reading the resource
    
    private:
        /// record the begin() & end() call counts, to ensure this guard is used properly
        /// obviously, this counter number should be even (not odd number)
        int pairwiseCallCount = 0; 

    };

};


