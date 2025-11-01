
# ADC_TemperatureSensor

---
## 프로그램 목표
STM32F103RB ADC1의 Temerature Sensor 채널에 연결된 내부의 온도센서를 이용하여 온도를 읽어 와서 tera term 터미널에 출력
<br>

* 구현 영상

<img width="995" height="550" alt="temp_013" src="https://github.com/user-attachments/assets/3a757270-4e1d-4f5d-86ef-6cedc029c2cb" />
<br><br><br>

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

1. **RCC 설정**

RCC 설정을 위해 다음 그림과 같이 Device Configuration 창에서 Pinout & Configuration 탭의 System Core 항목 중 RCC를 선택 후 우측의 RCC Mode and Configuration 의 Mode의 High Speed Clock(HSE), Low Speed Clock(LSE) 모두 Disable로 변경한다.  
<img width="1688" height="741" alt="image" src="https://github.com/user-attachments/assets/a1c05663-3c5f-492b-9852-5b6cd6911ad8" />  
<br>
<br>

2. **USART2 설정**

RCC 다음에 설정할  **`peripheral`**은 **USART2**(Universal Synchronous ASynchronous RecieverReceiver/Transmitter)이다. **`USART2`**를 이용한 **`printf()`**함수를 사용할 수 있도록 하기위해 USART2 장치를 설정해보자. 

- NUCLEO-F103RB 타겟보드는 3개의 USART(USART1, USART2, USART3)가 지원된다. 그 중 USB 포트에 바로 연결된 장치가 USART2이므로 USART2 장치를 표준 출력장치로 설정하여 printf() 함수로 출력한 내용이 시리얼로 출력되도록 USART2를 설정한다. 먼저 Device Configuration 창에서 Pinout & Configuration 탭의 Conectivity 항목 중 USART2를 선택 후 우측의 USART Mode snd Configuration 의 Mode를 Asynchronous로, Hardware Flow Control(RS232)을 Disable로 변경한다. 

  ![](./img/usart2_mode_config.png)



USART2  Parameter Settings

앞서 설정한 USART2 Mode 항목 아래에 Configuration 항목의 Parameter Settings 탭을 선택하고 

**Baud Rate** - 115200, **World Length** : 8bit, **Parity** : None, **Stop Bits** : 1, **Data Direction** : Recieve and Transmit 등의 설정값을 확인

한다.

![](./img/usart2_ParameterSettings.png)



USART2 NVIC Settings

NVIC Settings 탭을 선택하고, USART2 Global Interrupt항목의 Enabled를 체크한다.

![](./img/usart2_nvic_config.png)



##### NVIC 설정

USART 다음에 설정할  **`peripheral`**은 **NVIC**(Nested Vector Interrupt Controller)이다.  peripheral 설정 중 해당 peripheral의 NVIC Settings 탭을 설정했다면 반드시 System Core의 NVIC에 반영 여부를 확인하자. 

System Core 항목에서 NVIC을 선택하고, USART2 Global Interrupt 항목의 체크를 해제한다.

![](./img/usart2_nvic_config2.png)


3. **ADC1 설정**

  Pinout & Configuration탭의 Analog의 하위항목 중 ADC1을 선택한다.  ADC1 Mode and Configuration의 Mode에서 Temperature Sensor Channel을 체크, Configuration의 Parameter Settings 탭에서 Mode 는 Independent mode, Data Alignment는 Right Alignment, Scan Conversion Mode는 Disabled, Continuous Conversion Mode는 Enabled, Discontinuous Coversion Mode는 Disabled, Enable Regular Coversions는 Enable, Number of Conversion은 1, Rank는 1, Enable Injected Conversions는 Disable로 설정하고. 

  
  
  ![](./img/adc_mode_n_config.png)

`>`RANK 앞의 `<` 를 클릭하면 나타나는 Sampling Time은 13.5 Cycle 로 설정한다.  그리고 나서 클럭 설정 확인을 위해 Clock Configuration 탭을 클릭하면클럭 설정 문제와 자동 설정으로 해결하려면 원하는 버튼을 클릭하라는 대화창이 나타나는데, 여기서 YES 버튼을 클릭한다. 

![](./img/auto_clock_issue_solver.png)



ADC Clock에 문제가 있을 경우 Clock Configuration 탭에 표시 이상이 있음이 표시(적색 화살표  참조)되고, 동시에 우측 하단에 ADC1, 2 클럭에 문제가 있음이 표시된다. 

![](./img/resolve_clock_issue1.png)

이 때 상단의 Resolve Clock Issue 버튼을 클릭하면 클럭 설정이 적당한 값으로변경되어 Clock Issue가 해결된다. 

![](./img/resolve_clock_issue2.png)


  

4. **Use float with printf from newlib-nano 옵션을 활성화하여 printf 함수로 부동소수점(float) 값을 출력 가능하도록 설정**
<img width="1225" height="274" alt="temp_010" src="https://github.com/user-attachments/assets/78be6ebb-bf14-4f3d-aee2-b632df80b4ab" />
<br>
<img width="853" height="832" alt="temp_011" src="https://github.com/user-attachments/assets/8d605061-129a-41e3-8a16-14c6708e42e5" />
<br>
<img width="853" height="832" alt="temp_012" src="https://github.com/user-attachments/assets/e6accd8e-6208-4d61-9460-ccd2de35d717" />
<br>





지금까지 진행한 의 설정을 반영한 코드 생성하기 위해 Project 메뉴의 Generate Code 메뉴를 선택한다. 

![](./img/generate_code.png)
</details>

---

### 코드 작성 (기능 구현 코드)
<details>
<summary>펼치기/접기 코드</summary>

생성된 코드에서 다음 부분을 수정한다.



```c
/* USER CODE BEGIN PV */
const float AVG_SLOPE = 4.3E-03;
const float V25 = 1.43;
const float ADC_TO_VOLT = 3.3 / 4096;
/* USER CODE END PV */
```


```c
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */
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
  uint16_t adc1;

  float vSense;
  float temp;

  if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
  {
	  Error_Handler();
  }

  if(HAL_ADC_Start(&hadc1) != HAL_OK)
  {
	  Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_ADC_PollForConversion(&hadc1, 100);
	  adc1 = HAL_ADC_GetValue(&hadc1);
	  //printf("ADC1_temperature : %d \n",adc1);

	  vSense = adc1 * ADC_TO_VOLT;
	  temp = (V25 - vSense) / AVG_SLOPE + 25.0;
	  printf("temperature : %d, %f \n",adc1, temp);

	  HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```

</details>

