#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "bsemaphore.h"


int main(int argc, char **argv)
{
    key_t key;
    int shmid;
    int semid;
    char *data;
    int needRemove = 0;
    char* key_string="/home/genleung/temp/test.txt";

    key = ftok(key_string, 'a');
    if (key < 0)
    {
        perror("ftok");
        return -1;
    }

    /* 重新初始化 */
    semid = bsemInit(key,  1);
    if (semid < 0)
    {
        perror("bsemInit");
        return -1;
    }

    printf("semid: %d\n", semid);

    // shmget(IPC_PRIVATE, ... ) 可以确保创建的shm是唯一的，问题是别人无法获取它的key
    shmid = shmget(key, 1024, 0644 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("shmget");
        return -1;
    }

    if (argc == 1)
    {
        data = shmat(shmid, (void *)0, SHM_RDONLY);

    }
    else
    {
        data = shmat(shmid, (void *)0, 0);
    }

    if (data == (char *)(-1))
    {
        perror("shmat");
        return -1;
    }

    if (argc == 1)
    {
        bsemP(semid);
        printf("get shared contents: %s\n", data);
        bsemV(semid);
    }
    else
    {
        if (!strcmp(argv[1], "remove"))
        {
            needRemove = 1;
        }
        else
        {
            bsemP(semid);
            snprintf(data, 1024, "%s", argv[1]);
            bsemV(semid);
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

    printf("semid: %d\n", semid);

    /* 删除信号量 */
    bsemRemove(semid);

}
