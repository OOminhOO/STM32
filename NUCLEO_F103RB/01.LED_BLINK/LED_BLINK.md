# 1. LED_Blink
---
## 프로그램 목표
GPIO 출력 기능을 이용한 LED 제어  
1초마다 LED를 깜박이게 구현
<br>
- 구현 영상
  
https://github.com/user-attachments/assets/7815f03f-7688-4da4-b877-ecaa8652235e

---

### 개발환경

**OS** MS-Windows 10(64bit)

**Target** STM32 NUCLEO F103RB

**IDE** STM32 Cube IDE

**참고문헌** STM32CubeIDE를 이용한 STM32 따라하기(주)북랩 김남수 ∙ 이진형 지음 

### 프로젝트 생성

**STM32CubeIDE** 실행 후, 아래와 같이 File - New - STM32 Project 선택 
<img width="875" height="573" alt="image" src="https://github.com/user-attachments/assets/6c8c3c53-fd43-4eff-85dc-6697a09c49b9" />
<br>



### Target 선택

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

---

### Device Configuration Tool
<details>
<summary> ### Device Configuration Tool
</summary>

- 1. **RCC 설정**

RCC 설정을 위해 다음 그림과 같이 Device Configuration 창에서 Pinout & Configuration 탭의 System Core 항목 중 RCC를 선택 후 우측의 RCC Mode and Configuration 의 Mode의 High Speed Clock(HSE), Low Speed Clock(LSE) 모두 Disable로 변경한다.  
<img width="1688" height="741" alt="image" src="https://github.com/user-attachments/assets/a1c05663-3c5f-492b-9852-5b6cd6911ad8" />  
<br>
<br>
**클럭 설정 확인**

Clock Configuration 화면에서

SYS_CLK이 64(MHz)로 설정되었는 지 확인한다.  
<img width="1293" height="717" alt="image" src="https://github.com/user-attachments/assets/4f6b6678-ee13-44b7-9ba9-9f1061700c1f" />
<br>


- **GPIO 설정**

  PA5 핀에 대한 GPIO 설정을 위해 다음 그림과 같이 Device Configuration 창에서 Pinout View 탭의 CPU Pin 중 PA5를GPIO_OUTPUT로 변경후, Pinout & Configuration 탭의 PA5 Configuration의 GPIO Mode가 Output Push Pull인지 확인한다.

  <img width="1469" height="735" alt="image" src="https://github.com/user-attachments/assets/53c3b8de-a4f2-4e5f-a1dd-a76717a03d9d" />

<br>
  지금까지의 설정을 반영한 코드 생성을 위해 Project 메뉴의 Generate Code 항목을 선택한다. 
  <img width="421" height="347" alt="image" src="https://github.com/user-attachments/assets/c1461db2-aaa1-49cf-aced-1fabe9235b2a" />

<br>
  
Open Associated Perspective 대화창에서 Yes 버튼을 클릭한다. 
<img width="605" height="210" alt="open_associated_perspective (1)" src="https://github.com/user-attachments/assets/c23d7ae2-93b9-46b7-8e3e-e09bdbad62d1" />
<br>

</details>




* 이 과정을 완료하면 다음과 같은 코드가 만들어진
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
