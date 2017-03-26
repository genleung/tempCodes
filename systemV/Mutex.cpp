#include <string.h>

#include "Mutex.h"

namespace art
{

Mutex::Mutex(std::string idString, int projID)
{
    key_t key = ftok(idString.c_str(), projID);
    if (key < 0)
    {
        DLOGLF(FATAL, "Failed to generate mutex key: %s", strerror(errno));
    }

    semid = bsemInit(key, 1);
    if(semid==-1){
        DLOGLF(FATAL, "Failed to initialize mutex: %s", strerror(errno));
    }
}

Mutex::~Mutex()
{

}

void Mutex::remove()
{
    if (semid >= 0)
    {
        bsemRemove(semid);
        semid = -1;
    }
}

void Mutex::lock()
{
    if(bsemP(semid)==-1)
    {
        DLOGLF(ERROR, "Failed to lock: %s", strerror(errno));
    }
}

void Mutex::unlock()
{
    if(bsemV(semid)==-1)
    {
        DLOGLF(ERROR, "Failed to unlock: %s", strerror(errno));
    }
}

int Mutex::pendingCount()
{
    return bsemncnt(semid);
}

};
