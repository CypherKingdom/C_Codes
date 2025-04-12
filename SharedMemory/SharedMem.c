#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

int CreeSegment(int size, char *name, int cle)
{
    key_t key = ftok(name, cle);
    if (key == -1)
    {
        perror("ftok");
        return -1;
    }
    int shmid = shmget(key, size, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return -1;
    }
    return shmid;
}

void ControlSegment(int shmid)
{
    struct shmid_ds shmid_ds;
    if (shmctl(shmid, IPC_STAT, &shmid_ds) == -1)
    {
        perror("shmctl");
        return;
    }
    printf("Segment ID: %d\n", shmid);
    printf("Owner UID: %d\n", shmid_ds.shm_perm.uid);
    printf("Owner PID: %d\n", shmid_ds.shm_cpid);
    printf("Creator UID: %d\n", shmid_ds.shm_perm.cuid);
    printf("Segment size: %ld\n", shmid_ds.shm_segsz);
}

void RemoveSegment(int shmid)
{
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        perror("shmctl");
    else
        printf("Shared memory segment deleted successfully.\n");
}