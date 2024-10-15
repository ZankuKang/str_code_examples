#include "mbed.h"
#include <iterator>

// Definició de períodes i temps d'execució per a les tasques (en milisegons)
#define T1_PERIOD 8000   // Període de Tarea 1 (8000 ms)
#define T1_EXECUTION 2000 // Temps d'execució de Tarea 1 (2000 ms)
#define T1_ID 1

#define T2_PERIOD 12000 // Període de Tarea 2 (12000 ms)
#define T2_EXECUTION 3000 // Temps d'execució de Tarea 2 (3000 ms)
#define T2_ID 2

#define T3_PERIOD 16000 // Període de Tarea 3 (16000 ms)
#define T3_EXECUTION 4000 // Temps d'execució de Tarea 3 (4000 ms)
#define T3_ID 3

typedef struct {
    int period;
    int exec;
    int id;
    int counts;
    bool done;
} Task_t;

// Tickers per a cada tasca
Ticker task1_ticker;
Ticker task2_ticker;
Ticker task3_ticker;

// Contadors per al temps d'execució
Task_t task1 = {T1_PERIOD, T1_EXECUTION, T1_ID, 0, false};
Task_t task2 = {T2_PERIOD, T2_EXECUTION, T2_ID, 0, false};
Task_t task3 = {T3_PERIOD, T3_EXECUTION, T3_ID, 0, false};

int current_task = 0;
int ticks_elapsed = 0;

int scheduling_tasks[8] = {1, 2, 3, 1, 3, 2, 1, 3};
int next_task = 0;

// Funcions de tasca
void isr(void) {
    // Check the current task performed and if it finished
    switch(current_task) {
        case T1_ID:
            task1.counts++;
            ticks_elapsed++;
            if (task1.counts == T1_EXECUTION) {
                task1.counts = 0;
                task1.done = true;
                next_task += 1 % 8;
                if (scheduling_tasks[next_task] == 2 && !task2.done) {
                    current_task = scheduling_tasks[next_task];
                } 
                else if (scheduling_tasks[next_task] == 3 && !task3.done) {
                    current_task = scheduling_tasks[next_task];
                }
            }

            break;
        case T2_ID:
            task2.counts++;
            if (task2.counts == T2_EXECUTION) {
                task2.counts = 0;
                next_task += 1 % 8;
                if (scheduling_tasks[next_task] == 1 && !task1.done) {
                    current_task = scheduling_tasks[next_task];
                }
                else if (scheduling_tasks[next_task] == 3 && !task3.done) {
                    current_task = scheduling_tasks[next_task];
                } else {
                    current_task = 0;
                }
                
            }
                 
            break;
        case T3_ID:
            task3.counts++;
            if (task3.counts == T3_EXECUTION) {
                task3.counts = 0;
                next_task += 1 % 8;
                current_task = scheduling_tasks[next_task];
            }
          
            break;
    }
}

int main() {
    // Iniciar els tickers amb el període corresponent (en segons)
    while (true) {
        // En aquest bucle es poden executar altres lògiques si és necessari
        ThisThread::sleep_for(100); // Petita pausa per no saturar la CPU
    }
}