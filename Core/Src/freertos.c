/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "application.h"
#include "../../Task/inc/taskHead.h"
#include "fmc.h"
#include "u_stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId VirtualBashHandle;
osThreadId LCDHandle;
osThreadId FIleSystemHandle;
osThreadId VirtualFileSystHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Shell(void const * argument);
void LCD_Display(void const * argument);
void FatFsTask(void const * argument);
void VFS(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of VirtualBash */
  osThreadDef(VirtualBash, Shell, osPriorityNormal, 0, 128);
  VirtualBashHandle = osThreadCreate(osThread(VirtualBash), NULL);

  /* definition and creation of LCD */
  osThreadDef(LCD, LCD_Display, osPriorityAboveNormal, 0, 512);
  LCDHandle = osThreadCreate(osThread(LCD), NULL);

  /* definition and creation of FIleSystem */
  osThreadDef(FIleSystem, FatFsTask, osPriorityNormal, 0, 1024);
  FIleSystemHandle = osThreadCreate(osThread(FIleSystem), NULL);

  /* definition and creation of VirtualFileSyst */
  osThreadDef(VirtualFileSyst, VFS, osPriorityNormal, 0, 1024);
  VirtualFileSystHandle = osThreadCreate(osThread(VirtualFileSyst), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  SPI_LCD_Init();
  SDRAM_Initialization_Sequence(&hsdram1);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Shell */
/**
  * @brief  Function implementing the VirtualBash thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Shell */
void Shell(void const * argument)
{
  /* USER CODE BEGIN Shell */
  /* Infinite loop */
  for(;;)
  {
      // 这句话不能少，否则会导致串口输出异常
     osDelay(1);
  }
  /* USER CODE END Shell */
}

/* USER CODE BEGIN Header_LCD_Display */
/**
* @brief Function implementing the spi5_lcd thread.
* @param argument: Not used
* @retval None
*/


/* USER CODE END Header_LCD_Display */
void LCD_Display(void const * argument)
{
  /* USER CODE BEGIN LCD_Display */
  /* Infinite loop */

  LCD_Task_Init();
  while(1)
      LCD_main();
  /* USER CODE END LCD_Display */
}

/* USER CODE BEGIN Header_FatFsTask */
/**
* @brief Function implementing the FIleSystem thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FatFsTask */
void FatFsTask(void const * argument)
{
  /* USER CODE BEGIN FatFsTask */
  /* Infinite loop */
  for(;;){
      ThreadLock;
      u_print("FatFsTask\r\n");
      ThreadUnlock;
      // FatFs_Check();
  }
  /* USER CODE END FatFsTask */
}

/* USER CODE BEGIN Header_VFS */
/**
* @brief Function implementing the VirtualFileSyst thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_VFS */
void VFS(void const * argument)
{
  /* USER CODE BEGIN VFS */
  /* Infinite loop */
  ram_init();
  for(;;){
      ram_main();
      // SDRAM_Test();
  }
  /* USER CODE END VFS */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
