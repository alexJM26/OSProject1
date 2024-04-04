#include <iostream>
#include <thread>           // threads
#include <semaphore.h>      // sem_t methods
#include <sys/mman.h>       // shared memory object
#include <sys/stat.h>       // mode constants 
#include <fcntl.h>          // O_* constants 
#include <unistd.h>         // ftruncate

#define table_size 2 // table can only hold two items at once
#define size 5 // amount of items produced / consumed

struct sharedMemory {
    char table[table_size]; // table to hold items
    int in = 0, out = 0; // indicate next element in table to be produced/consumed

    sem_t available; // indicates critical section in use
    sem_t full, empty; // indicates when table is full/empty
};
