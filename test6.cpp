#include "mbed.h"
#include "ConditionVariable.h"
#include "Kernel.h"
#include "Mutex.h"
#include "ThisThread.h"
#include "Thread.h"
#include <cstdint>
#include <cstdio>
#include <type_traits>
#include "cmsis_os2.h"
#include "protocol_abstract.h"

typedef struct {
    float voltage;
    float current;
    uint32_t counter; 
} message_t;

MemoryPool<message_t, 16> mpool;
Queue<message_t, 16> queue;
Thread thread;

void send_thread(void) {
    uint32_t i = 0;
    while (true) {
        i++;
        message_t* msg = mpool.try_alloc();
        msg->counter = i;
        msg->current = (i * 0.1) * 33;
        msg->voltage = (i * 0.1) * 11;
        queue.put(msg);
        ThisThread::sleep_for(1000ms);
    }
}
void test6() {
    thread.start(callback(send_thread));
    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t* msg = (message_t*)(evt.value.p);
            printf("\nVoltage: %2f V\r\n", msg->voltage);
            printf("\nCurrent: %2f A\r\n", msg->current);
            printf("\nNumber of cycles: %lu\r\n", msg->counter);
            mpool.free(msg);
        }
    }
}