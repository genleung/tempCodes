#include "ReadGuard.h"
#include "Log/Log.h"

namespace art
{

ReadGuard::ReadGuard(std::shared_ptr<Mutex> res, std::shared_ptr<Mutex> rtry, std::shared_ptr<Mutex> rmt,  int *rcount)
    : resource(res),readTry(rtry), rmutex(rmt), readingCount(*rcount)
{
}

ReadGuard::~ReadGuard()
{
    if(pairwiseCallCount % 2){
        DLOGL(FATAL, "ReadGuard improper usage!");
    }
}

void ReadGuard::begin()
{
    if (!(readTry && rmutex && resource))
        return;
    pairwiseCallCount++;

    readTry->lock();
    rmutex->lock();
    readingCount++;
    if (readingCount == 1)
        resource->lock();
    rmutex->unlock();
    readTry->unlock();
}

void ReadGuard::end()
{
    if (!(readTry && rmutex && resource))
        return;
    pairwiseCallCount++;    

    rmutex->lock();
    readingCount--;
    if (readingCount == 0)
        resource->unlock();
    rmutex->unlock();
}

// how many readers are reading the resource
int ReadGuard::getReadingCount()
{
    return readingCount;
}

// how many readers are trying to read the resource (in queue)
int ReadGuard::getPendingCount()
{
    return readTry->pendingCount();
}

};