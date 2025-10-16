# 3. EXTI
---
## 프로그램 목표

파란색 버튼을 외부 인터럽트로 받아서 LD2를 키고 끄는 인터럽트를 구현한다.

<br>

* 구현 영상
 

https://github.com/user-attachments/assets/5ad0c667-d5e5-444b-8c83-0beb6ff4b30c



---

### 개발환경
<details>
<summary>펼치기/접기 **개발환경** </summary>

**OS** MS-Windows 10(64bit)

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

2. **GPIO 설정**  
B1 SW가 연결된 PC13은 외부 인터럽트 입력 (EXTI)으로 설정한다.  
B1 스위치는 풀업 스위치이므로 스위치가 눌리지 않았을 때 HIGH가 입력되다가 스위치가 눌리면 LOW로 떨어지고, 스위치에서 손을 떼면 HIGH로 올라간다.  
즉 누르는 순간에는 Falling Edge가 발생하고, 스위치에서 손을 떼는 순간에는 Rising Edge가 발생하므로 이 SW 를 땔 때 인터럽트를 발생시키기 위해 System Core-GPIO-GPIO Mode and Configuration의 GPIO탭의 PC13 설정을 Interrupt Mode with Rising Edge triger Detection으로 변경한다.  
<img width="1899" height="739" alt="image" src="https://github.com/user-attachments/assets/46e1df09-2aae-486e-8f0e-27026c667254" />
<br>
<br>

3.  **NVIC** 설정
<br>
 이제 PC13 입력으로 인터럽트를 발생시키기 위해 GPIOMode and Configuration의 NVIC 탭에서 NVIC Interrupt Table항의 EXTI line[15:10] interrupts Enabled 에 체크한다.

<img width="1890" height="766" alt="image" src="https://github.com/user-attachments/assets/6a5d033c-0007-4658-99f2-8587546eed66" />
<br>
NVIC 탭에서 인터럽트 사용 설정할 경우 반드시 System Core의 NVIC에서 확인을 해봐야 한다. NVIC Mode and Configuration의 NVIC 탭의 EXTI line interrupts[15:10]에 체크하여 선택 후, NVIC Mode and Configuration의 탭의 EXTI line interrupts[15:10]
<img width="1892" height="749" alt="image" src="https://github.com/user-attachments/assets/1a8f8e23-0945-477f-a41a-2fbe1dc5d774" />  
<br>
VIC Mode and Configuration의 Code Generation 탭의 EXTI line[15:10]항목의 Generate IRQ Handler 및 Call HAL handler에 체크.  
<img width="986" height="739" alt="image" src="https://github.com/user-attachments/assets/085579bf-b479-4235-a904-3105427881dc" />  



</details>

---

### 코드 작성 (기능 구현 코드)
<details>
<summary>펼치기/접기 코드</summary>

생성된 코드에서 다음 부분을 수정한다.

```c
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case B1_Pin:
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		break;

	default:
		;
	}
}
/* USER CODE END 0 */
```

</details>

