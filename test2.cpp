#include "mbed.h"
#include "ConditionVariable.h"
#include "Kernel.h"
#include "Mutex.h"
#include "ThisThread.h"
#include "Thread.h"
#include <cstdint>
#include <cstdio>

void test2() {
    uint64_t now = Kernel::get_ms_count();
    ThisThread::sleep_for(10ms);
    uint64_t later = Kernel::get_ms_count();
    printf("Elapsed ms: %u \r\n", (uint32_t)(later - now));
}