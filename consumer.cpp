#include "sharedMemory.hpp"

// CONSUMER FUNCTION ==========================================================
void consumer(sharedMemory* ptr) {

    sem_wait(&ptr->empty); // wait if table is empty
    sem_wait(&ptr->available); // wait if another thread/process is in its critical section

    char itemConsumed = ptr->table[ptr->out]; // consume item in next consumable spot
    ptr->out = ((ptr->out) + 1) % table_size; // increment next consumable spot

    std::cout << "Consumed item: " << itemConsumed << std::endl;

    sem_post(&ptr->full); // signal full semaphore (decrement it)
    sem_post(&ptr->available); // signal that next process/thread can enter its critical section

}


// MAIN FUNCTION ==============================================================
int main() {

    const char* name = "/sharedMemBuffer"; // name of shared memory object
    int fd; // name of file descriptor for shared memory object
    sharedMemory *ptr; // pointer to shared memory object

    // open shared memory object
    fd = shm_open(name, O_CREAT | O_RDWR, 0666); 
    if (fd == -1) {
        printf("Error: shm_open Has Failed.");
        return -1;
    }

    // get correct size of shared memory object
    if (ftruncate(fd, sizeof(sharedMemory)) == -1) {
        printf("Error: ftruncate  Has Failed");
        return -1;
    } 

    // get pointer to shared memory object
    ptr = static_cast<sharedMemory*>(mmap(NULL, sizeof(sharedMemory), PROT_WRITE, MAP_SHARED, fd, 0));

    // make threads that will consume all produced items
    std::thread t1(consumer, ptr);
    std::thread t2(consumer, ptr);
    std::thread t3(consumer, ptr);
    std::thread t4(consumer, ptr);

    // join all threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // unlink from the shared memory object
    shm_unlink(name);

    return 0;
}
