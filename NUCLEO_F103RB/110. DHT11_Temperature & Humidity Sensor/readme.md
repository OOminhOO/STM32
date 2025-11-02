
# DHT11 Temperature & Humidity Sensor



---
## 프로그램 목표
DHT11 온습도 센서로 STM32F103RB(NUCLEO 보드)에서 디지털 데이터를 읽어와서 실시간으로 온도와 습도를 측정·표시
* 구현 사진

<img width="995" height="550" alt="DHT11_001" src="https://github.com/user-attachments/assets/a487c821-8c1c-4ec8-a241-ed420251dc1e" />
<br>


---

## DHT11 & board pin map
<details>
<summary>펼치기/접기 **image** </summary>
<img width="267" height="135" alt="133" src="https://github.com/user-attachments/assets/09ede793-cb31-4652-9575-fa08fdfe5ec3" />

   * PA0 - DATA  

   => DHT11: **신호가 High로 얼마나 유지되는지를 시간으로 구분** (타이머 필요)<br>
<img width="1047" height="180" alt="image" src="https://github.com/user-attachments/assets/3bd2911f-2200-42b2-897d-17c41703b173" /><br>
1)출력 모드로 18~20ms LOW → Start 신호 전송<br>
2)입력 모드로 전환 후 80us LOW + 80us HIGH 응답을 감지<br>
3)40비트 데이터 수신 (5바이트) :습도 정수, 습도 소수, 온도 정수, 온도 소수, 체크섬<br>


<img width="644" height="586" alt="F103RB-pin" src="https://github.com/user-attachments/assets/23e365b4-1bdf-4074-9724-d795ea1da5b7" />
<br>


</details>

---  







### 개발환경
<details>
<summary>펼치기/접기 **개발환경** </summary>

**OS** MS-Windows 11(64bit)

**Target** STM32 NUCLEO F103RB

**IDE** STM32 Cube IDE

**참고문헌** STM32CubeIDE를 이용한 STM32 따라하기(주)북랩 김남수 ∙ 이진형 지음 

</details>

---




### 프로젝트 생성
<details>
<summary>펼치기/접기 **프로젝트 생성** </summary>

**STM32CubeIDE** 실행 후, 아래와 같이 File - New - STM32 Project 선택 
<img width="875" height="573" alt="image" src="https://github.com/user-attachments/assets/6c8c3c53-fd43-4eff-85dc-6697a09c49b9" />
<br>

---

#### Target 선택

Tafget Selection 윈도우가 나타나면 **Board Selector** 탭을 클릭한다.  
<img width="1200" height="800" alt="image" src="https://github.com/user-attachments/assets/3960ff0f-8fa7-4002-9534-9af9f9ca96e7" />

<br>
Board selector 탭에서 **NUCLEO-F103RB** 검색  Board List에 **NUCLEO-F103RB**가 표시된다. 이를 선택 후, 하단의 Next > 버튼을 클릭한다.
<img width="1200" height="800" alt="image" src="https://github.com/user-attachments/assets/c5673109-e80d-4bfc-9aea-cb6043f8257c" />
<br>
<br>
STM32 Project 창이 나타나면 Project Name: 에 적당한 프로젝트 이름을 입력 후(예: LED_Blink) Finish 버튼을 클릭한다. 
<img width="500" height="600" alt="image" src="https://github.com/user-attachments/assets/50745ee0-67aa-478a-8ad8-f06da87e5c75" />
<br>
<br>

Board Project Options 대화창에서 Yes 버튼을 클릭한다.  
<img width="500" height="140" alt="image" src="https://github.com/user-attachments/assets/d5b04334-ce35-46e0-aed3-1854412ed372" />
<br>
<br>
Open Associated Perspective 대화창에서 Yes 버튼을 클릭하면 Device Configration Tool 창이 열린다.
<img width="500" height="140" alt="open_associated_perspective" src="https://github.com/user-attachments/assets/6a6a4353-f68e-4867-8a03-9bcd8062d709" />


</details>

---


### Device Configuration
<details>
<summary>펼치기/접기  **Device Configuration Tool**
</summary>
  
### RCC설정
<img width="1688" height="741" alt="image" src="https://github.com/user-attachments/assets/a1c05663-3c5f-492b-9852-5b6cd6911ad8" />  
<br>
<br>

  
### 타이머 pwm 설정  

<img width="800" height="608" alt="SERVO_003" src="https://github.com/user-attachments/assets/a6ca154d-6616-407b-9e77-ab1566bb1a80" />
<br>
<img width="800" height="608" alt="SERVO_004" src="https://github.com/user-attachments/assets/f20ec8df-36e9-42e1-8671-6223fc108338" />
<br>


</details>

---

### 코드 작성 (기능 구현 코드)
<details>
<summary>펼치기/접기 코드</summary>

생성된 코드에서 다음 부분을 수정한다.

----

   * TIM2_CH1 - PA0
   * TIM3_CH1 - PA6

```c
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */
```

```c
/* USER CODE BEGIN PD */
#define MAX 125  // 2.5ms pulse width (최대 각도)
#define MIN 25   // 0.5ms pulse width (최소 각도)
#define CENTER 75 // 1.5ms pulse width (중앙 각도)
#define STEP 1
/* USER CODE END PD */

/* USER CODE BEGIN PV */
uint8_t ch;
uint8_t pos_pan = 75;
uint8_t pos_tilt = 75;
/* USER CODE END PV */
```

```c
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  if (ch == '\n')
    HAL_UART_Transmit (&huart2, (uint8_t*) "\r", 1, 0xFFFF);
  HAL_UART_Transmit (&huart2, (uint8_t*) &ch, 1, 0xFFFF);

  return ch;
}
```

```c
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  // 초기 위치 설정
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pos_pan);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pos_tilt);

  printf("Servo Control Ready\r\n");
  printf("Commands: w(up), s(down), a(left), d(right), i(center)\r\n");
  /* USER CODE END 2 */
```

```c
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(HAL_UART_Receive(&huart2, &ch, sizeof(ch), 10) == HAL_OK)
    {
      if(ch == 's')
      {
        printf("Down\r\n");
        if(pos_tilt + STEP <= MAX)
          pos_tilt = pos_tilt + STEP;
        else
          pos_tilt = MAX;
      }
      else if(ch == 'w')
      {
        printf("Up\r\n");
        if(pos_tilt - STEP >= MIN)
          pos_tilt = pos_tilt - STEP;
        else
          pos_tilt = MIN;
      }
      else if(ch == 'a')
      {
        printf("Left\r\n");
        if(pos_pan + STEP <= MAX)
          pos_pan = pos_pan + STEP;
        else
          pos_pan = MAX;
      }
      else if(ch == 'd')
      {
        printf("Right\r\n");
        if(pos_pan - STEP >= MIN)
          pos_pan = pos_pan - STEP;
        else
          pos_pan = MIN;
      }
      else if(ch == 'i')
      {
        printf("Center\r\n");
        pos_pan = CENTER;
        pos_tilt = CENTER;
      }

      // PWM 듀티 사이클 업데이트
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pos_pan);
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pos_tilt);

      printf("Pan: %d, Tilt: %d\r\n", pos_pan, pos_tilt);

      HAL_Delay(50); // 서보 응답 시간
    }

    /* USER CODE END WHILE */
```

---
# 각도표시
---

<img width="995" height="550" alt="servo_result" src="https://github.com/user-attachments/assets/c42adba2-96aa-4ff5-a119-5044486adb6e" />


```c
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */
```

```c
/* USER CODE BEGIN PD */
#define MAX 125      // 2.5ms pulse width (180도)
#define MIN 25       // 0.5ms pulse width (0도)
#define CENTER 75    // 1.5ms pulse width (90도)
#define STEP 5       // 이동 단위
/* USER CODE END PD */
```

```c
/* USER CODE BEGIN PV */
uint8_t ch;
uint8_t pos_pan = CENTER;
uint8_t pos_tilt = CENTER;
/* USER CODE END PV */
```

```c
/* USER CODE BEGIN PFP */
uint16_t pwm_to_angle(uint8_t pwm_value);
void display_servo_status(uint8_t pan, uint8_t tilt);
/* USER CODE END PFP */
```

```c
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  if (ch == '\n')
    HAL_UART_Transmit (&huart2, (uint8_t*) "\r", 1, 0xFFFF);
  HAL_UART_Transmit (&huart2, (uint8_t*) &ch, 1, 0xFFFF);

  return ch;
}

/**
  * @brief  PWM 값을 각도로 변환하는 함수
  * @param  pwm_value: PWM 듀티 사이클 값 (25~125)
  * @retval 각도 값 (0~1800, 실제 각도 x 10)
  */
uint16_t pwm_to_angle(uint8_t pwm_value)
{
  // PWM 25~125 범위를 0~180도로 변환
  // 소수점 계산을 위해 10배로 확대 (0~1800)
  // 공식: angle = (pwm_value - 25) * 1800 / (125 - 25)
  return ((uint16_t)(pwm_value - MIN) * 1800) / (MAX - MIN);
}

/**
  * @brief  서보모터 상태를 화면에 출력하는 함수
  * @param  pan: Pan 서보 PWM 값
  * @param  tilt: Tilt 서보 PWM 값
  * @retval None
  */
void display_servo_status(uint8_t pan, uint8_t tilt)
{
  uint16_t pan_angle = pwm_to_angle(pan);
  uint16_t tilt_angle = pwm_to_angle(tilt);
  
  printf("Pan: %3d (%3d.%d°) | Tilt: %3d (%3d.%d°)\r\n", 
         pan, pan_angle/10, pan_angle%10,
         tilt, tilt_angle/10, tilt_angle%10);
}
/* USER CODE END 0 */
```

```c
  /* USER CODE BEGIN 2 */
  // PWM 시작
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  
  // 초기 위치 설정
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pos_pan);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pos_tilt);
  
  printf("\r\n=== SG90 Servo Control System ===\r\n");
  printf("Commands: w(up), s(down), a(left), d(right), i(center)\r\n");
  printf("Initial Position:\r\n");
  display_servo_status(pos_pan, pos_tilt);
  printf("Ready!\r\n\r\n");
  /* USER CODE END 2 */

```

```c
 /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(HAL_UART_Receive(&huart2, &ch, sizeof(ch), 10) == HAL_OK)
    {
      // 명령 처리
      if(ch == 's')  // Down
      {
        printf("Command: Down\r\n");
        if(pos_tilt + STEP <= MAX) 
          pos_tilt = pos_tilt + STEP;
        else 
          pos_tilt = MAX;
      }
      else if(ch == 'w')  // Up
      {
        printf("Command: Up\r\n");
        if(pos_tilt - STEP >= MIN) 
          pos_tilt = pos_tilt - STEP;
        else 
          pos_tilt = MIN;
      }
      else if(ch == 'a')  // Left
      {
        printf("Command: Left\r\n");
        if(pos_pan + STEP <= MAX)	
          pos_pan = pos_pan + STEP;
        else 
          pos_pan = MAX;
      }
      else if(ch == 'd')  // Right
      {
        printf("Command: Right\r\n");
        if(pos_pan - STEP >= MIN)	
          pos_pan = pos_pan - STEP;
        else 
          pos_pan = MIN;
      }
      else if(ch == 'i')  // Center
      {
        printf("Command: Center\r\n");
        pos_pan = CENTER;
        pos_tilt = CENTER;
      }
      else
      {
        printf("Invalid command: %c\r\n", ch);
        continue;  // 잘못된 명령이면 PWM 업데이트 하지 않음
      }

      // PWM 듀티 사이클 업데이트
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pos_pan);
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pos_tilt);
      
      // 상태 출력 (pwm_to_angle 함수 실제 사용됨)
      display_servo_status(pos_pan, pos_tilt);
      
      HAL_Delay(50); // 서보 응답 시간
    }
    
    /* USER CODE END WHILE */
```

</details>
