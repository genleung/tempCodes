#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * semctl() additional (4th) parameter union
 */
union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    ushort *array;         /* used for GETALL and SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO, Linux specific */
};

/**
 * @brief Initialize the semaphore set
 *      from http://beej.us/guide/bgipc/output/html/singlepage/bgipc.html#semaphores 8.5 Sample programs
 * @param key Key generated by ftok()
 * @param nsems Count of semaphore's in the set
 * @param val Initialization value of all the semaphores in the set
 * @return The semaphore-set-id if succeeded, -1 otherwise
 */
int semInit(key_t key, int nsems, int val);

/**
 * @brief Set a semaphore's value
 * @param semid Semaphore set's id
 * @param semnum Semaphore index in the set
 * @param val Value the semaphore will set to
 * @return 0 if successful, -1 otherwise
 */
int semSetValue(int semid, int semnum, int val);

/**
 * @brief Get a semaphore's value
 * @param semid Semaphore set's id
 * @param semnum Semaphore index in the set
 * @return Semaphore's value if successful, -1 otherwise
 */
int semGetValue(int semid, int semnum);

/**
 * @brief mutex-semaphore pass-check operation
 *      If the semaphore less than or equal to 0, sem_p() would block
 * @param semid Semaphore set's id
 * @param semnum Semaphore index in the set
* @return 0 if successful, -1 otherwise
 */
int semP(int semid, int semnum);

/**
 * @brief mutex-semaphore release operation
 *      Increse the semaphore and return immediately
 * @param semid Semaphore set's id
 * @param semnum Semaphore index in the set
 * @return 0 if successful, -1 otherwise
 */
int semV(int semid, int semnum);

/**
 * @brief Remove the semaphore set
 * @param semid Semaphore set's id
 * @return 0 if successful, -1 otherwise
 */
int semRemove(int semid);

#ifdef __cplusplus
}
#endif

#endif /* End of header file */