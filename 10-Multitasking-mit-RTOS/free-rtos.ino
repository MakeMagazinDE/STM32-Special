/*
  FreeRTOS https://www.freertos.org/
  needs STM32duino 
  2 LEDs and 1 Button
*/

#include <STM32FreeRTOS.h>

const uint16_t KeyPin = PB6;
const uint16_t LEDPIN1 = PB7;
const uint16_t LEDPIN2 = PB8;

void TaskKey(void *pvParameters );    // prototype
void TaskBlink1(void *pvParameters );    // prototype
void TaskBlink2(void *pvParameters );    // prototype

TaskHandle_t taskBlink1Handle;      // handle

void setup() {
  Serial.begin(9600);
  
  xTaskCreate(      // set up tasks
    TaskKey,
    (const portCHAR *)"Key",   // A name just for humans
    128,   // This stack size can be checked & adjusted by reading the Stack Highwater
    NULL,
    1,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    NULL);

  xTaskCreate(      // set up tasks
    TaskBlink1,
    (const portCHAR *)"Blink1",   // A name just for humans
    128,   // This stack size can be checked & adjusted by reading the Stack Highwater
    NULL,
    1,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    &taskBlink1Handle );

  xTaskCreate(TaskBlink2, (const portCHAR *)"Blink2", 128, NULL, 1, NULL);
  
  vTaskStartScheduler();    // start scheduler
}

void loop() {}  // Empty. Endless. Things are done in Tasks.

/*---------------------- Tasks ---------------------*/
void TaskKey(void *pvParameters)  // This is a task.
{
  (void) pvParameters;    // prevent warning
  pinMode(KeyPin, INPUT_PULLDOWN);

  while(1) // A Task shall never return or exit.
    if (digitalRead (KeyPin))
      vTaskResume (taskBlink1Handle);
}

void TaskBlink1(void *pvParameters)  // This is a task.
{
  (void) pvParameters;    // prevent warning
  pinMode(LEDPIN1, OUTPUT);

  while(1) // A Task shall never return or exit.
  {
    digitalWrite(LEDPIN1, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for ms 
    digitalWrite(LEDPIN1, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for ms
    Serial.print("High : "); Serial.println(uxTaskGetStackHighWaterMark(NULL));     // Info about unused stack
  }
}

void TaskBlink2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;    // prevent warning
  pinMode(LEDPIN2, OUTPUT);
  uint8_t cnt = 0;

  while(1) // A Task shall never return or exit.
  {
    digitalWrite(LEDPIN2, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for ms 
    digitalWrite(LEDPIN2, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for ms
    
    cnt++;
    if (cnt == 5)
    {
      vTaskSuspend (taskBlink1Handle);
      cnt = 0;    // next resume: again 5 times
    }
  }
}

