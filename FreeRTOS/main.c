#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>


QueueHandle_t xQueue;


void vTask1(void *pvParameters) {
    while (1) {
        printf("Task 1: Temporização ativa a cada 1 segundo.\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

void vTask2(void *pvParameters) {
    int a = 5, b = 10;
    while (1) {
        printf("Task 2: Soma = %d\n", a + b);
        printf("Task 2: Produto = %d\n", a * b);
        vTaskDelay(pdMS_TO_TICKS(2000)); 
    }
}

void vTask3(void *pvParameters) {
    const char *message = "Mensagem da Task 3";
    while (1) {
        if (xQueueSend(xQueue, &message, pdMS_TO_TICKS(500)) == pdPASS) {
            printf("Task 3: Mensagem enviada para a fila.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(3000)); 
    }
}

void vTask4(void *pvParameters) {
    char *receivedMessage;
    while (1) {
        if (xQueueReceive(xQueue, &receivedMessage, pdMS_TO_TICKS(1000)) == pdPASS) {
            printf("Task 4: Mensagem recebida: %s\n", receivedMessage);
        } else {
            printf("Task 4: Nenhuma mensagem na fila.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); 
    }
}

int main(void) {
  
    xQueue = xQueueCreate(5, sizeof(char *));
    if (xQueue == NULL) {
        printf("Falha ao criar a fila.\n");
        return 1;
    }

    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(vTask4, "Task 4", configMINIMAL_STACK_SIZE, NULL, 4, NULL);

    vTaskStartScheduler();

    for (;;);
}
