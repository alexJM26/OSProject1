#include <iostream>
#include <thread>           // threads
#include <semaphore.h>      // sem_t methods
#include <sys/mman.h>       // shared memory object
#include <sys/stat.h>       // mode constants 
#include <fcntl.h>          // O_* constants 
#include <unistd.h>         // ftruncate

#define table_size 2 // table can only hold two items at once
#define size 10 // amount of items produced / consumed

struct sharedMemory {
    char table[table_size]; // table to hold items
    int in = 0, out = 0; // indicate next element in table to be produced/consumed

    sem_t available; // indicates if critical section is in use
    sem_t elementsOpen, elementsUsed; // indicates when table is full/empty
};
