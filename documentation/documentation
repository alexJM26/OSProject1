# Producer-Consumer Problem Wiki

## sharedMemory.hpp

```C++
struct sharedMemory { ... }
```
Shared memory object containing a table for items and the semaphores shared between the producer and consumer processes.

```C++
char table[table_size]; // table to hold items
int in = 0, out = 0;
```
The table to hold items. in and out are used to implement table as a circular array of characters. out indicates the index of the item to be consumed next, and in indicates the index where the next item to be produced will be placed. 

```C++
sem_t available;
```
This semaphore is used to indicate when a process or thread is in its critical section. When available == 0, a process or thread is in its critical section and no other process or thread is able to enter its critical section. When available == 1, no process or thread is in its critical section and the next process or thread can enter its critical section. 

```C++
sem_t full,empty;
```
These semaphores are used to indicate when the shared memory table is full or empty, respectively.<br>
When full > 0, there are spaces in the table that are empty. When full == 0, the table is full and any producer threads must wait for the items in the table to be consumed before proceeding.<br>
When empty > 0, there are spaces in the table being used. When empty == 0, the table is empty and any consumer threads must wait for items to be produced and added to the table before proceeding. 

## producer.cpp

## consumer.cpp
