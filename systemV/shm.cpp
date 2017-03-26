#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <memory>
#include "WriteGuard.h"
#include "ReadGuard.h"


int main(int argc, char **argv)
{
    key_t key;
    int shmid;
    int semid;
    char *data;
    int needRemove = 0;
    const char* key_string="/proc/cpuinfo";

    auto readTry = std::make_shared<art::Mutex>(key_string, 1);
    auto rmutex = std::make_shared<art::Mutex>(key_string, 2);
    auto wmutex = std::make_shared<art::Mutex>(key_string, 3);
    auto resource = std::make_shared<art::Mutex>(key_string, 4);
    int readcount ,writecount;

    art::ReadGuard rguard(resource, readTry, rmutex, &readcount);
    art::WriteGuard wguard(resource, readTry, wmutex, &writecount);

    // shmget(IPC_PRIVATE, ... ) 可以确保创建的shm是唯一的，问题是别人无法获取它的key
    
    shmid = shmget(ftok(key_string,1), 1024, 0644 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("shmget");
        return -1;
    }

    if (argc == 1)
    {
        data = (char*)shmat(shmid, (void *)0, SHM_RDONLY);

    }
    else
    {
        data = (char*)shmat(shmid, (void *)0, 0);
    }

    if (data == (char *)(-1))
    {
        perror("shmat");
        return -1;
    }

    if (argc == 1)
    {
        printf("%d readers\n", rguard.getReadingCount());
        rguard.begin();
                printf("%d readers\n", rguard.getReadingCount());
        printf("get shared contents: %s\n", data);
        rguard.end();
                printf("%d readers\n", rguard.getReadingCount());
    }
    else
    {
        if (!strcmp(argv[1], "remove"))
        {
            needRemove = 1;
        }
        else
        {
            wguard.begin();
            snprintf(data, 1024, "%s", argv[1]);
            wguard.end();
        }
    }

    if (shmdt(data) == -1)
    {
        perror("shmdt");
        return -1;
    }

    if (needRemove == 1)
    {
        shmctl(shmid, IPC_RMID, 0);
    }


}
