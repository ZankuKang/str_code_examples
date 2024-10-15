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

#define SAMPLE_FLAG1 (1UL << 0)
#define SAMPLE_FLAG2 (1UL << 9)

EventFlags event_flags;

void worker_thread_f() {
    printf("Waiting for any flag form 0x%08lx.\r\n", SAMPLE_FLAG1|SAMPLE_FLAG2);
    uint32_t flags_read = 0;
    while (true) {
        flags_read = event_flags.wait_any(SAMPLE_FLAG1|SAMPLE_FLAG2);
        printf("Got: 0x%08lx\r\n", flags_read);
    }
}

void test4() {
    Thread worker_thread;
    worker_thread.start(mbed::callback(worker_thread_f));
    while(true) {
        ThisThread::sleep_for(1s);
        event_flags.set(SAMPLE_FLAG1);
        ThisThread::sleep_for(500ms);
        event_flags.set(SAMPLE_FLAG2);
    }
}
