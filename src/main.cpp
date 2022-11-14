#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


float temperatura;

xSemaphoreHandle mutexI2C;

float acessa_i2c(int comando)
{

  if(comando == 1)
  {
    Serial.println("I2C - Leitura do sensor de temperatura");
    return 20.0 + ((float) rand() / (float)(RAND_MAX)/10);
  }
  else
  {
     Serial.println("I2C - Escrita LCD");
     Serial.printf("Tela LCD - Temperatura = %f", temperatura);
  }
  return 0;
}


void le_sensor(void * params)
{
  while(1)
  {
    if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS))
    {
      temperatura = acessa_i2c(1);
      Serial.printf("Leitura - Temperatura lida: %f", temperatura);
      xSemaphoreGive(mutexI2C);
    }
    else
    {
      Serial.println("Leitura, Não foi possível ler o sensor");
    }
    vTaskDelay(1000/ portTICK_RATE_MS);
  }
}

void ldc_display(void *params)
{
  while(1)
  {
    if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS))
    {
      Serial.println("I2C - Escrita LCD");
      Serial.printf("Display - Escreve no LCD");
      acessa_i2c(2);
      xSemaphoreGive(mutexI2C);

    }
    else
    {
      Serial.println("Display, Não foi possível escrever no display");
    }
    vTaskDelay(2000/ portTICK_RATE_MS);
  }

}


void setup() 
{
  Serial.begin(9600);

  mutexI2C = xSemaphoreCreateMutex();
  
  xTaskCreate(&le_sensor, "Leitura Sensor", 2048, NULL, 2, NULL);
  xTaskCreate(&ldc_display, "Atualização do Display", 2048, NULL, 2, NULL);

}

void loop() 
{


}
