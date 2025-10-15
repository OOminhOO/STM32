# 1. LED_Blink
## 프로그램 목표
GPIO 출력 기능을 이용한 LED 제어 
1초마다 LED를 깜박이게 구현
<br>
- 구현 영상
  
https://github.com/user-attachments/assets/7815f03f-7688-4da4-b877-ecaa8652235e

---

#### 개발환경

**OS** MS-Windows 10(64bit)

**Target** STM32 NUCLEO F103RB

**IDE** STM32 Cube IDE

**참고문헌** STM32CubeIDE를 이용한 STM32 따라하기(주)북랩 김남수 ∙ 이진형 지음 

#### 프로젝트 생성

**STM32CubeIDE** 실행 후, 아래와 같이 File - New - STM32 Project 선택 

![](.\img\stm32cube_ide_new_project.png)



#### Target 선택

Tafget Selection 윈도우가 나타나면 **Board Selector** 탭을 클릭한다.

![](./img/board_selector.png)

Board selector 탭의 Type에서 NUCLEO64를 체크, MCU/MPU Series에서 STM32F1을 체크 하면 Board List에 **NUCLEO-F103RB**가 표시된다. 이를 선택 후, 하단의 Next > 버튼을 클릭한다.

![](./img/board_type.png)

STM32 Project 창이 나타나면 Project Name: 에 적당한 프로젝트 이름을 입력 후(예: LED_Blink) Finish 버튼을 클릭한다. 

![](./img/stm32_project_name.png)

Board Project Options 대화창에서 Yes 버튼을 클릭한다.



![](./img/board_project_options.png)

Open Associated Perspective 대화창에서 Yes 버튼을 클릭하면 Device Configration Tool 창이 열린다.



![](./img/open_associated_perspective.png)



#### Device Configuration Tool



![](./img/device_config_tool.png)

- **RCC 설정**

RCC 설정을 위해 다음 그림과 같이 Device Configuration 창에서 Pinout & Configuration 탭의 System Core 항목 중 RCC를 선택 후 우측의 RCC Mode and Configuration 의 Mode의 High Speed Clock(HSE), Low Speed Clock(LSE) 모두 Disable로 변경한다.

![](./img/system_core_rcc.png)



**클럭 설정 확인**

Clock Configuration 화면에서

SYS_CLK이 64(MHz)로 설정되었는 지 확인한다.

![](./img/clock_config.png)



- **GPIO 설정**

  PA5 핀에 대한 GPIO 설정을 위해 다음 그림과 같이 Device Configuration 창에서 Pinout View 탭의 CPU Pin 중 PA5를GPIO_OUTPUT로 변경후, Pinout & Configuration 탭의 PA5 Configuration의 GPIO Mode가 Output Push Pull인지 확인한다.

  ![](./img/gpio_config.png)

  
  
  지금까지의 설정을 반영한 코드 생성을 위해 Project 메뉴의 Generate Code 항목을 선택한다. 
  

Open Associated Perspective 대화창에서 Yes 버튼을 클릭한다. 

![](./img/open_associated_perspective.png)



<img width="520" height="350" alt="LED_Blinks_001" src="https://github.com/user-attachments/assets/3aa18d2c-5bc8-4d24-ae1f-898272f96bf9" />
<br>
<img width="1359" height="805" alt="LED_Blinks_002" src="https://github.com/user-attachments/assets/d2634652-6e5b-46be-bbb8-027a9d0f3c9b" />
<br>
<img width="402" height="51" alt="LED_Blinks_004" src="https://github.com/user-attachments/assets/6cf1078d-0d6a-4d3f-88b3-b3e46151b983" />
<br>
<img width="492" height="558" alt="LED_Blinks_005" src="https://github.com/user-attachments/assets/3197a10b-59cd-43e6-baa2-4d5f46a0e8a1" />
<br>
<img width="486" height="129" alt="LED_Blinks_006" src="https://github.com/user-attachments/assets/4d967a98-f423-4f2f-ac96-e863f1115848" />
<br>
<img width="1433" height="908" alt="LED_Blinks_007" src="https://github.com/user-attachments/assets/3df11327-4bef-41f6-8e77-d99d4d80d467" />
<br>
<img width="1433" height="908" alt="LED_Blinks_008" src="https://github.com/user-attachments/assets/d3133bc5-09d9-4b51-baec-290a8c229c6f" />
<br>
<img width="1433" height="908" alt="LED_Blinks_009" src="https://github.com/user-attachments/assets/401ce922-6baf-4321-a3eb-87b8bffff721" />
<br>
<img width="1433" height="908" alt="LED_Blinks_010" src="https://github.com/user-attachments/assets/f4b312b8-fa49-4413-bb46-4125ad84b26b" />
<br>
<img width="242" height="297" alt="LED_Blinks_011" src="https://github.com/user-attachments/assets/7bf0e956-97db-48b5-9856-d4a119fc7538" />
<br>
<img width="486" height="165" alt="LED_Blinks_012" src="https://github.com/user-attachments/assets/ca3e7a4a-8acc-4cfd-9905-e01ccbeb556c" />
<br>
<img width="1137" height="545" alt="LED_Blinks_013" src="https://github.com/user-attachments/assets/c4d741b9-39ba-41f5-9f5a-405e0d6c1157" />
<br>
<img width="1137" height="545" alt="LED_Blinks_014" src="https://github.com/user-attachments/assets/e0c09683-07a4-4a1b-8dd1-3e2f91cba259" /><br>


* main.c
```c
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	  HAL_Delay(1000);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	  HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```
<img src="https://github.com/gotree94/STM32/blob/main/NUCLEO_F103RB/01.LED_Blink/led2.gif?raw=true" width="400" alt="LED Blink Animation">
<br>

<img width="583" height="360" alt="shield-101" src="https://github.com/user-attachments/assets/b614372a-7016-45c8-9bda-873a4b3c9944" />
<br>

* main.c
```c
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
	  HAL_Delay(1000);
	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
	  HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```
