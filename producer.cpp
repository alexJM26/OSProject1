#include "sharedMemory.hpp"

// PRODUCER FUNCTION ========================================================== 
void producer(sharedMemory* ptr) {

    int it = 0; // initialize iterator

    while (it < size) {

        char item = char(65 + rand() % 25); // get random character A-Z

        sem_wait(&(ptr->elementsOpen)); // wait if table is full
        sem_wait(&(ptr->available)); // wait if another process/thread is in critical section

        ptr->table[ptr->in] = item; // produce item in next available spot
        ptr->in = ((ptr->in) + 1) % table_size; // increment next available spot

        std::cout << "Produced Item: " << item << std::endl;

        sem_post(&(ptr->elementsUsed)); // signal empty semaphore (decrement it)
        sem_post(&(ptr->available)); // signal that next process/thread can enter its critical section

        sleep(1); // fake delay for demonstration

        it++; 

    }

}


// MAIN =======================================================================
int main() {

    const char* name = "/sharedMemBuffer"; // name of shared memory object
    int fd; // file descriptor for shared memory object
    sharedMemory *ptr; // pointer to shared memory object

    // open shared memory object
    fd = shm_open(name, O_CREAT | O_RDWR, 0666); 
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
    sem_init(&ptr->elementsOpen,1,table_size);
    sem_init(&ptr->elementsUsed,1,0);

    // initialize in and out
    ptr->in = 0;
    ptr->out = 0;

    // set seed for random producer items
    srand(5);

    // make two producer threads 
    std::thread t1(producer, ptr);
    std::thread t2(producer, ptr);

    // join all threads
    t1.join();
    t2.join();

    return 0;
}
