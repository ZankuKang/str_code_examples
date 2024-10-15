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
} mail_t;

Mail <mail_t,16> mail_box;
Thread thread;

void send_thread(void) {
    uint32_t i = 0;
    while (true) {
        i++;
        mail_t* mail = mail_box.alloc();
        mail->counter = i;
        mail->current = (i * 0.1) * 33;
        mail->voltage = (i * 0.1) * 11;
        mail_box.put(mail);
        ThisThread::sleep_for(1000ms);
    }
}

void test5() {
    thread.start(callback(send_thread));
    while (true) {
        osEvent evt = mail_box.get();
        if (evt.status == osEventMail) {
            mail_t* mail = (mail_t*)(evt.value.p);
            printf("\nVoltage: %2f V\r\n", mail->voltage);
            printf("\nCurrent: %2f A\r\n", mail->current);
            printf("\nNumber of cycles: %lu\r\n", mail->counter);
            mail_box.free(mail);
        }
    }
}