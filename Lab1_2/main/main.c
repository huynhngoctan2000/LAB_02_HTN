#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"


#define ESP_INTR_FLAG_DEFAULT 0
 
#define BLINK_LED 13
#define GPIO_INPUT_IO_0 35
int buttonCount = 0;
int i = 0;
 
SemaphoreHandle_t xSemaphore = NULL;
 
TaskHandle_t printVariableTask = NULL;
 
void printVariable(void *pvParameter) {

    while (1) {
 
        printf("Pham Ngoc Manh - 1813045\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
        printf("Huynh Ngoc Tan - 1813952\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
        printf("Tran Quoc To - 1814385\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
        printf("Nguyen Thai Binh - 1910843\n");
        vTaskDelay(1000 / portTICK_RATE_MS);

    }
}
// interrupt service routine, called when the button is pressed
void IRAM_ATTR button_isr_handler(void* arg) {
 
    // notify the button task
    xSemaphoreGiveFromISR(xSemaphore, NULL);
 
}
// task that will react to button clicks
void button_task(void* arg) {
 
    // infinite loop
    for(;;) {
 
        // wait for the notification from the ISR
        if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE) {
            int buttonState = gpio_get_level(GPIO_INPUT_IO_0);
    
            while(buttonState == 1){ //code stucks here!!!!
                buttonCount++;
                printf("ESP32\n");
                vTaskDelay(100 / portTICK_RATE_MS);
                buttonState = gpio_get_level(GPIO_INPUT_IO_0);
                buttonState = 0;
            }
        }
    }
}
 
void app_main()
{
    // create the binary semaphore
    xSemaphore = xSemaphoreCreateBinary();
 
    // configure button and led pins as GPIO pins
    gpio_pad_select_gpio(GPIO_INPUT_IO_0);
    gpio_pad_select_gpio(BLINK_LED);
 
    // set the correct direction
    gpio_set_direction(GPIO_INPUT_IO_0, GPIO_MODE_INPUT);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);
 
    // enable interrupt on falling (1->0) edge for button pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_NEGEDGE);
 
    // start the task that will handle the button
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
 
    // install ISR service with default configuration
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
 
    // attach the interrupt service routine
    gpio_isr_handler_add(GPIO_INPUT_IO_0, button_isr_handler, NULL);
 
    int pass = 25;
    xTaskCreate(&printVariable, "printVariable", 2048, (void*) pass, 5, &printVariableTask);
 
}