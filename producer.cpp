#include "sharedMemory.hpp"

// PRODUCER FUNCTION ========================================================== 
void producer(char item, sharedMemory* ptr) {

    sem_wait(&(ptr->full)); // wait if table is full
    sem_wait(&(ptr->available)); // wait if another process/thread is in critical section

    ptr->table[ptr->in] = item; // produce item in next available spot
    ptr->in = ((ptr->in) + 1) % table_size; // increment next available spot

    std::cout << "Produced Item: " << item << std::endl;

    sem_post(&(ptr->empty)); // signal empty semaphore (decrement it)
    sem_post(&(ptr->available)); // signal next process/thread can enter its critical section

}


// MAIN =======================================================================
int main() {

    const char* name = "/sharedMemBuffer"; // name of shared memory object
    int fd; // name of file descriptor for shared memory object
    sharedMemory *ptr; // pointer to shared memory object

    fd = shm_open(name, O_CREAT | O_RDWR, 0666); // get fd of shared memory object
    if (fd == -1) {
        printf("Error: shm_open Has Failed.");
        return -1;
    }

    // get correct size of shared memory object
    if (ftruncate(fd, sizeof(sharedMemory)) == -1) {
        printf("Error: ftruncate Has Failed.");
        return -1;
    } 

    // get pointer to shared memory object
    ptr = static_cast<sharedMemory*>(mmap(NULL, sizeof(sharedMemory), PROT_WRITE, MAP_SHARED, fd, 0));

    // initialize semaphores
    sem_init(&ptr->available,1,1);
    sem_init(&ptr->full,1,table_size);
    sem_init(&ptr->empty,1,0);

    // makes threads producing items 
    std::thread t1(producer,'a', ptr);
    std::thread t2(producer,'b', ptr);
    std::thread t3(producer,'c', ptr);
    std::thread t4(producer,'d', ptr);

    // join all threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
