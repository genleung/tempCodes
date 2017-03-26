#include "WriteGuard.h"

namespace art
{

WriteGuard::WriteGuard(std::shared_ptr<Mutex> res, std::shared_ptr<Mutex> rtry, std::shared_ptr<Mutex> wmt, int *wcount)
    : resource(res), readTry(rtry), wmutex(wmt), writersCount(*wcount)
{
}

void WriteGuard::begin()
{
    if (!(readTry && wmutex && resource))
        return;
    
    wmutex->lock();
    writersCount++;
    if (writersCount == 1)
        readTry->lock();
    wmutex->unlock();

    // lock the resource, ready to write
    resource->lock();
}

void WriteGuard::end()
{
    if (!(readTry && wmutex && resource))
        return;
    
    // write done, release the resource
    resource->unlock();

    wmutex->lock();
    writersCount--;
    if (writersCount == 0)
        readTry->unlock();
    wmutex->unlock();
}

int WriteGuard::getWritersCount()
{
    return writersCount;
}

};