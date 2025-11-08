
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Software I2C GPIO 핀 정의 (원하는 핀으로 변경 가능!)
#define SW_I2C_SCL_PORT     GPIOC
#define SW_I2C_SCL_PIN      GPIO_PIN_8

#define SW_I2C_SDA_PORT     GPIOC
#define SW_I2C_SDA_PIN      GPIO_PIN_6

// I2C 타이밍 (클럭 속도 조절)
#define I2C_DELAY_US        5   // 5us = 약 100kHz

// K24C256 설정
#define K24C256_ADDR_WRITE  0xA0
#define K24C256_ADDR_READ   0xA1
#define K24C256_BLOCK1_WRITE 0xB0
#define K24C256_BLOCK1_READ  0xB1
#define EEPROM_PAGE_SIZE    64
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
// Software I2C 기본 함수
void SW_I2C_Init(void);
void SW_I2C_Delay(void);
void SW_I2C_SCL_High(void);
void SW_I2C_SCL_Low(void);
void SW_I2C_SDA_High(void);
void SW_I2C_SDA_Low(void);
uint8_t SW_I2C_SDA_Read(void);
void SW_I2C_Start(void);
void SW_I2C_Stop(void);
void SW_I2C_SendByte(uint8_t data);
uint8_t SW_I2C_ReceiveByte(uint8_t ack);
uint8_t SW_I2C_WaitAck(void);
void SW_I2C_Ack(void);
void SW_I2C_NAck(void);

// EEPROM 함수
void SW_I2C_Scan(void);
uint8_t EEPROM_WriteByte(uint8_t dev_addr, uint16_t mem_addr, uint8_t data);
uint8_t EEPROM_ReadByte(uint8_t dev_addr, uint16_t mem_addr, uint8_t *data);
uint8_t EEPROM_WriteBytes(uint8_t dev_addr, uint16_t mem_addr, uint8_t *data, uint16_t len);
uint8_t EEPROM_ReadBytes(uint8_t dev_addr, uint16_t mem_addr, uint8_t *data, uint16_t len);
void EEPROM_Test(void);
void Test_0xB0_Device(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  if (ch == '\n')
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r", 1, 0xFFFF);
  HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 0xFFFF);
  return ch;
}

/**
  * @brief  Microsecond delay
  */
void DelayUs(uint32_t us)
{
  uint32_t cycles = us * (SystemCoreClock / 1000000) / 5;
  while(cycles--) __NOP();
}

/**
  * @brief  Software I2C 초기화
  */
void SW_I2C_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // GPIO 클럭 활성화
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // SCL 핀 설정 (Open-Drain Output)
  GPIO_InitStruct.Pin = SW_I2C_SCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SW_I2C_SCL_PORT, &GPIO_InitStruct);

  // SDA 핀 설정 (Open-Drain Output)
  GPIO_InitStruct.Pin = SW_I2C_SDA_PIN;
  HAL_GPIO_Init(SW_I2C_SDA_PORT, &GPIO_InitStruct);

  // 초기 상태: HIGH (Idle)
  SW_I2C_SCL_High();
  SW_I2C_SDA_High();

  printf("Software I2C initialized on PC8(SCL), PC6(SDA)\n");
}

/**
  * @brief  I2C 딜레이 (클럭 속도 조절)
  */
void SW_I2C_Delay(void)
{
  DelayUs(I2C_DELAY_US);
}

/**
  * @brief  SCL을 HIGH로 설정
  */
void SW_I2C_SCL_High(void)
{
  HAL_GPIO_WritePin(SW_I2C_SCL_PORT, SW_I2C_SCL_PIN, GPIO_PIN_SET);
  SW_I2C_Delay();
}

/**
  * @brief  SCL을 LOW로 설정
  */
void SW_I2C_SCL_Low(void)
{
  HAL_GPIO_WritePin(SW_I2C_SCL_PORT, SW_I2C_SCL_PIN, GPIO_PIN_RESET);
  SW_I2C_Delay();
}

/**
  * @brief  SDA를 HIGH로 설정
  */
void SW_I2C_SDA_High(void)
{
  HAL_GPIO_WritePin(SW_I2C_SDA_PORT, SW_I2C_SDA_PIN, GPIO_PIN_SET);
  SW_I2C_Delay();
}

/**
  * @brief  SDA를 LOW로 설정
  */
void SW_I2C_SDA_Low(void)
{
  HAL_GPIO_WritePin(SW_I2C_SDA_PORT, SW_I2C_SDA_PIN, GPIO_PIN_RESET);
  SW_I2C_Delay();
}

/**
  * @brief  SDA 상태 읽기
  */
uint8_t SW_I2C_SDA_Read(void)
{
  return HAL_GPIO_ReadPin(SW_I2C_SDA_PORT, SW_I2C_SDA_PIN);
}

/**
  * @brief  I2C Start 조건
  */
void SW_I2C_Start(void)
{
  SW_I2C_SDA_High();
  SW_I2C_SCL_High();
  SW_I2C_Delay();
  SW_I2C_SDA_Low();   // SDA: HIGH -> LOW while SCL is HIGH
  SW_I2C_Delay();
  SW_I2C_SCL_Low();
}

/**
  * @brief  I2C Stop 조건
  */
void SW_I2C_Stop(void)
{
  SW_I2C_SDA_Low();
  SW_I2C_SCL_High();
  SW_I2C_Delay();
  SW_I2C_SDA_High();  // SDA: LOW -> HIGH while SCL is HIGH
  SW_I2C_Delay();
}

/**
  * @brief  1 바이트 전송
  */
void SW_I2C_SendByte(uint8_t data)
{
  for(int i = 0; i < 8; i++)
  {
    SW_I2C_SCL_Low();

    if(data & 0x80)
      SW_I2C_SDA_High();
    else
      SW_I2C_SDA_Low();

    data <<= 1;
    SW_I2C_Delay();
    SW_I2C_SCL_High();
    SW_I2C_Delay();
  }
  SW_I2C_SCL_Low();
}

/**
  * @brief  1 바이트 수신
  */
uint8_t SW_I2C_ReceiveByte(uint8_t ack)
{
  uint8_t data = 0;

  SW_I2C_SDA_High();  // Release SDA for reading

  for(int i = 0; i < 8; i++)
  {
    data <<= 1;
    SW_I2C_SCL_Low();
    SW_I2C_Delay();
    SW_I2C_SCL_High();

    if(SW_I2C_SDA_Read())
      data |= 0x01;

    SW_I2C_Delay();
  }

  SW_I2C_SCL_Low();

  // Send ACK or NACK
  if(ack)
    SW_I2C_Ack();
  else
    SW_I2C_NAck();

  return data;
}

/**
  * @brief  ACK 대기
  */
uint8_t SW_I2C_WaitAck(void)
{
  uint8_t ack;

  SW_I2C_SDA_High();  // Release SDA
  SW_I2C_Delay();
  SW_I2C_SCL_High();
  SW_I2C_Delay();

  ack = SW_I2C_SDA_Read();

  SW_I2C_SCL_Low();
  SW_I2C_Delay();

  return ack;  // 0 = ACK, 1 = NACK
}

/**
  * @brief  ACK 전송
  */
void SW_I2C_Ack(void)
{
  SW_I2C_SCL_Low();
  SW_I2C_SDA_Low();
  SW_I2C_Delay();
  SW_I2C_SCL_High();
  SW_I2C_Delay();
  SW_I2C_SCL_Low();
  SW_I2C_SDA_High();  // Release SDA
}

/**
  * @brief  NACK 전송
  */
void SW_I2C_NAck(void)
{
  SW_I2C_SCL_Low();
  SW_I2C_SDA_High();
  SW_I2C_Delay();
  SW_I2C_SCL_High();
  SW_I2C_Delay();
  SW_I2C_SCL_Low();
}

/**
  * @brief  I2C 주소 스캔
  */
void SW_I2C_Scan(void)
{
  printf("\n=== Software I2C Address Scan ===\n");
  printf("Scanning I2C bus...\n\n");

  uint8_t count = 0;

  for(uint8_t addr = 0; addr < 128; addr++)
  {
    SW_I2C_Start();
    SW_I2C_SendByte(addr << 1);  // Write address

    if(SW_I2C_WaitAck() == 0)  // ACK received
    {
      printf("Found I2C device at address: 0x%02X (7-bit: 0x%02X)\n", addr << 1, addr);
      count++;

      if((addr << 1) >= 0xA0 && (addr << 1) <= 0xAE)
      {
        printf("  -> K24C256 EEPROM detected\n");
      }
      else if((addr << 1) == 0xB0)
      {
        printf("  -> Possible Block 1 or other device\n");
      }
    }

    SW_I2C_Stop();
    HAL_Delay(1);
  }

  printf("\nTotal %d I2C device(s) found.\n", count);
  printf("=================================\n\n");
}

/**
  * @brief  EEPROM 1바이트 쓰기
  */
uint8_t EEPROM_WriteByte(uint8_t dev_addr, uint16_t mem_addr, uint8_t data)
{
  SW_I2C_Start();

  // Device address + Write
  SW_I2C_SendByte(dev_addr);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;  // Error
  }

  // Memory address (High byte)
  SW_I2C_SendByte((mem_addr >> 8) & 0xFF);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Memory address (Low byte)
  SW_I2C_SendByte(mem_addr & 0xFF);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Data
  SW_I2C_SendByte(data);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  SW_I2C_Stop();
  HAL_Delay(5);  // Write cycle time

  return 0;  // Success
}

/**
  * @brief  EEPROM 1바이트 읽기
  */
uint8_t EEPROM_ReadByte(uint8_t dev_addr, uint16_t mem_addr, uint8_t *data)
{
  SW_I2C_Start();

  // Device address + Write (to set memory address)
  SW_I2C_SendByte(dev_addr & 0xFE);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Memory address (High byte)
  SW_I2C_SendByte((mem_addr >> 8) & 0xFF);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Memory address (Low byte)
  SW_I2C_SendByte(mem_addr & 0xFF);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Repeated Start
  SW_I2C_Start();

  // Device address + Read
  SW_I2C_SendByte(dev_addr | 0x01);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Read data
  *data = SW_I2C_ReceiveByte(0);  // NACK

  SW_I2C_Stop();

  return 0;  // Success
}

/**
  * @brief  EEPROM 여러 바이트 쓰기 (페이지 쓰기 지원)
  */
uint8_t EEPROM_WriteBytes(uint8_t dev_addr, uint16_t mem_addr, uint8_t *data, uint16_t len)
{
  uint16_t written = 0;

  while(written < len)
  {
    uint16_t page_offset = mem_addr % EEPROM_PAGE_SIZE;
    uint16_t bytes_to_write = EEPROM_PAGE_SIZE - page_offset;

    if(bytes_to_write > (len - written))
      bytes_to_write = len - written;

    // Page write
    SW_I2C_Start();
    SW_I2C_SendByte(dev_addr);
    if(SW_I2C_WaitAck() != 0) {
      SW_I2C_Stop();
      return 1;
    }

    SW_I2C_SendByte((mem_addr >> 8) & 0xFF);
    if(SW_I2C_WaitAck() != 0) {
      SW_I2C_Stop();
      return 1;
    }

    SW_I2C_SendByte(mem_addr & 0xFF);
    if(SW_I2C_WaitAck() != 0) {
      SW_I2C_Stop();
      return 1;
    }

    for(uint16_t i = 0; i < bytes_to_write; i++)
    {
      SW_I2C_SendByte(data[written + i]);
      if(SW_I2C_WaitAck() != 0) {
        SW_I2C_Stop();
        return 1;
      }
    }

    SW_I2C_Stop();
    HAL_Delay(5);  // Write cycle time

    written += bytes_to_write;
    mem_addr += bytes_to_write;
  }

  return 0;
}

/**
  * @brief  EEPROM 여러 바이트 읽기
  */
uint8_t EEPROM_ReadBytes(uint8_t dev_addr, uint16_t mem_addr, uint8_t *data, uint16_t len)
{
  SW_I2C_Start();

  // Set memory address
  SW_I2C_SendByte(dev_addr & 0xFE);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  SW_I2C_SendByte((mem_addr >> 8) & 0xFF);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  SW_I2C_SendByte(mem_addr & 0xFF);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Repeated Start
  SW_I2C_Start();

  // Read mode
  SW_I2C_SendByte(dev_addr | 0x01);
  if(SW_I2C_WaitAck() != 0) {
    SW_I2C_Stop();
    return 1;
  }

  // Read data
  for(uint16_t i = 0; i < len; i++)
  {
    data[i] = SW_I2C_ReceiveByte(i < (len - 1) ? 1 : 0);  // ACK except last byte
  }

  SW_I2C_Stop();

  return 0;
}

/**
  * @brief  EEPROM 테스트
  */
void EEPROM_Test(void)
{
  printf("=== EEPROM Test (0xA0) ===\n");

  char write_str[] = "Hello Software I2C!";
  uint8_t read_buf[50] = {0};
  uint16_t len = strlen(write_str);

  printf("Writing: \"%s\" (%d bytes)\n", write_str, len);

  if(EEPROM_WriteBytes(K24C256_ADDR_WRITE, 0x0000, (uint8_t*)write_str, len) == 0)
  {
    printf("Write: SUCCESS\n");
    HAL_Delay(10);

    if(EEPROM_ReadBytes(K24C256_ADDR_READ, 0x0000, read_buf, len) == 0)
    {
      printf("Read: SUCCESS\n");
      printf("Data: \"%s\"\n", read_buf);

      if(memcmp(write_str, read_buf, len) == 0)
        printf("** Verification PASSED! **\n");
      else
        printf("** Verification FAILED! **\n");
    }
    else
      printf("Read: FAILED\n");
  }
  else
    printf("Write: FAILED\n");

  printf("===========================\n\n");
}

/**
  * @brief  0xB0 블럭 테스트
  */
void Test_0xB0_Device(void)
{
  printf("=== Testing 0xB0 Device ===\n");

  char write_str[] = "Block1!";
  uint8_t read_buf[20] = {0};
  uint16_t len = strlen(write_str);

  printf("Writing to 0xB0: \"%s\"\n", write_str);

  if(EEPROM_WriteBytes(K24C256_BLOCK1_WRITE, 0x0100, (uint8_t*)write_str, len) == 0)
  {
    printf("Write to 0xB0: SUCCESS\n");
    HAL_Delay(10);

    if(EEPROM_ReadBytes(K24C256_BLOCK1_READ, 0x0100, read_buf, len) == 0)
    {
      printf("Read from 0xB0: SUCCESS\n");
      printf("Data: \"%s\"\n", read_buf);

      if(memcmp(write_str, read_buf, len) == 0)
        printf("** 0xB0 is valid EEPROM block! **\n");
    }
    else
      printf("Read from 0xB0: FAILED\n");
  }
  else
    printf("Write to 0xB0: FAILED\n");

  printf("===========================\n\n");
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("\n\n");
  printf("==========================================\n");
  printf("  STM32F103 Software I2C EEPROM Test     \n");
  printf("  System Clock: 64MHz                    \n");
  printf("  SW I2C: PC8(SCL), PC6(SDA)             \n");
  printf("==========================================\n\n");

  // Software I2C 초기화
  SW_I2C_Init();

  // I2C 주소 스캔
  SW_I2C_Scan();

  // EEPROM 테스트
  EEPROM_Test();

  // 0xB0 테스트
  Test_0xB0_Device();

  printf("All tests completed!\n\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
