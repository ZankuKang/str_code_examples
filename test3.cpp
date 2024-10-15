#include "mbed.h"
#include "ConditionVariable.h"
#include "Kernel.h"
#include "Mutex.h"
#include "ThisThread.h"
#include "Thread.h"
#include <cstdint>
#include <cstdio>

void test_thread(void) {
    printf("\ntest thread started");
    ThisThread::sleep_for(2000ms);
    printf("\ntest thread finished");
}

void thread_terminate_hook(osThreadId_t id) {
    printf("\n idle hook invoked");
    Kernel::attach_idle_hook(NULL);
}

void test3() {
    printf("\nattach thread terminate hook demo:");
    Kernel::attach_thread_terminate_hook(thread_terminate_hook);
    Thread* newThread = new Thread(osPriorityAboveNormal1, 1024, NULL, NULL);
    newThread->start(callback(test_thread));
    ThisThread::sleep_for(4000ms);
    printf("\n\n");
    while(true) {
        ThisThread::sleep_for(3000ms);
    }
    printf("\n\n");
}