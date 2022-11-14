#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


float temperatura;

xSemaphoreHandle mutexI2C; // Semaphore para as Tasks


// Função que usa I2C para leitura de um sensor e para escrita no display
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


// Função que recebe a leitura do sensor 
void le_sensor(void * params)
{
  while(1)
  {
    if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)) // Pega o Semaphore se ele estiver disponivel
    {
      temperatura = acessa_i2c(1);
      Serial.printf("Leitura - Temperatura lida: %f", temperatura);
      xSemaphoreGive(mutexI2C); // Devolve o Semaphore após terminar a função
    }
    else
    {
      Serial.println("Leitura, Não foi possível ler o sensor");
    }
    vTaskDelay(1000/ portTICK_RATE_MS);
  }
}

// Função que escreve no display
void ldc_display(void *params)
{
  while(1)
  {
    if (xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)) // Pega o Semaphore se ele estiver disponivel
    {
      Serial.println("I2C - Escrita LCD");
      Serial.printf("Display - Escreve no LCD");
      acessa_i2c(2);
      xSemaphoreGive(mutexI2C); // Devolve o Semaphore após terminar a função

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
