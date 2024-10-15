#include "mbed.h"
#include "Mutex.h"
#include "ConditionVariable.h"
#include <cstdio>


Mutex mutex;
ConditionVariable cond(mutex);
uint32_t counter = 0;
bool done = false;

void worker_thread() {
    mutex.lock();
    do {
        printf("Worker : Count %lu\r\n", counter);
        cond.wait();
    } while (!done);
}

// main() runs in its own thread in the OS
void real()
{
    Thread thread;
    thread.start(worker_thread);
    for (int i = 0; i < 5; i++) {
        mutex.lock();
        counter++;
        printf("Main: Set count to %lu\r\n", counter);
        cond.notify_all();
        mutex.unlock();
        ThisThread::sleep_for(1000ms);
    }
    mutex.lock();
    done = true;
    printf("Main: set done\r\n");
    cond.notify_all();
    mutex.unlock();
    thread.join();
}

