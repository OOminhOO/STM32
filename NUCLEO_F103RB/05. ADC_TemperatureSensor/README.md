
# 5. ADC_TemperatureSensor

---
## 프로그램 목표

타이머 인터럽트를 이용해 일정 주기마다 LED를 깜박이게 하는 프로그램

<br>

* 구현 영상

https://github.com/user-attachments/assets/a65d65a7-6fb4-472a-b059-ff930371934b



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

2.**TIM3 설정**  
Prescaler와 Counter Period 설정에 따라 타이머 인터럽트 주기가 결정된다.  
64 MHz 클럭을 Prescaler(64-1)로 분주하면 1 MHz가 되고, Counter Period(1000-1) 설정 시 1 ms마다 인터럽트가 발생한다.  
<img width="1435" height="739" alt="image" src="https://github.com/user-attachments/assets/f5664ae2-7c45-4481-9309-8649c5ea9115" />
<br>

TIM3 Configuration의 NVIC Setting 탭에서 TIM3 global interrupt Enabled에 체크.  
<img width="877" height="595" alt="image" src="https://github.com/user-attachments/assets/47ce6863-f125-41dd-adbd-9de8042d27fb" />


<br>
System Core 항목에서 NVIC을 선택하고, Configuration 의 NVIC탭에 NVIC Interrupt Table에 TIM3global interript가 등록되었는 지 확인한다.  
<img width="850" height="725" alt="image" src="https://github.com/user-attachments/assets/4e75b10a-ad34-47b8-b4cf-5e2ffbcc33ce" />
<br>
<img width="867" height="522" alt="image" src="https://github.com/user-attachments/assets/e3ced715-4881-4e26-ba9f-3a01228654a9" />

<br>
<br>
지금까지의 설정을 반영한 코드 생성하기 위해 ctrl + k 하여 Generate Code 실행

</details>

---

### 코드 작성 (기능 구현 코드)
<details>
<summary>펼치기/접기 코드</summary>

생성된 코드에서 다음 부분을 수정한다.

```c
/* USER CODE BEGIN PV */
// 타이머 인터럽트 발생 횟수를 저장할 전역 변수
// volatile : 인터럽트 내에서 값이 변경되므로 최적화 방지
volatile int gTimerCnt;
/* USER CODE END PV */
```


```c
/* USER CODE BEGIN 2 */
// TIM3 타이머를 인터럽트 모드로 시작
// 설정된 주기마다 HAL_TIM_PeriodElapsedCallback() 함수가 자동 호출됨
if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
{
  	  Error_Handler();   // 타이머 시작 실패 시 에러 처리
}
/* USER CODE END 2 */
```

```c
/* USER CODE BEGIN 0 */
// 타이머 주기마다 자동으로 호출되는 콜백 함수 (인터럽트 서비스 루틴)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	gTimerCnt++;   // 인터럽트 발생 시마다 카운트 증가

	// 인터럽트가 1000번 발생하면 (1ms × 1000 = 1초)
	if(gTimerCnt == 1000)
	{
		gTimerCnt = 0;   // 카운트 초기화
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);   // LED 상태 반전 (켜짐↔꺼짐)
	}
}
/* USER CODE END 0 */
```


</details>

