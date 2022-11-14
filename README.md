# freeRTOS TaskMutex
 
Arbitragem de tarefas usando **xSemaphore.

As Tarefas só executadas se puderem pegar o Semaphore **(xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS))
Após finaliza a sequencia da tarefa a mesma deve devolver o Semaphore #(xSemaphoreGive(mutexI2C))

Com isso, com as tarefas sempre concorrendo pela possibilidade de executar, não vai acontecer de alguma tarefa não ser executada.
Isso por que, ela vai esperar pela liberação do Semaphore. xSemaphoreTake(mutexI2C, **TempoAguardado** / portTICK_PERIOD_MS
