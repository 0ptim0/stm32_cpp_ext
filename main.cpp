#include "stm32_conf.h"
#include "blah.h"

const rom_class b;
int const rom_glob_var = 1;
int ram_glob_var = 5;

const rom_class *ptr1;
const int *ptr2;
int *ptr3;

UART_HandleTypeDef UART1_InitStructure;

void init(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void usart(void) {
    __HAL_RCC_USART1_CLK_ENABLE();
    UART1_InitStructure.Instance = USART1;
    UART1_InitStructure.Init.BaudRate= 115200;
    UART1_InitStructure.Init.WordLength = UART_WORDLENGTH_8B;
    UART1_InitStructure.Init.Parity = UART_PARITY_NONE;
    UART1_InitStructure.Init.Mode = UART_MODE_TX_RX;
    UART1_InitStructure.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART1_InitStructure.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&UART1_InitStructure);
}

int main(void) {
    HAL_StatusTypeDef status;
    status = HAL_Init();
    init();
    int local1;
    int local2 = 5;
    static int local3;
    static int local4 = 1;
    static const int local5 = 1;
    int const *ptr[5] = {&local1, &local2, &local3, &local4, &local5};
    ptr1 = &b;
    ptr2 = &rom_glob_var;
    ptr3 = &ram_glob_var;
    NVIC_SetPriority(SysTick_IRQn, 0x0);
    HAL_Delay(100);
    T_RCC & RCC1 = *static_cast<T_RCC *>RCC;
    RCC1.PortOn<Gport::A, Gport::B, Gport::C>();
    b.foo();
    uint8_t data = 25;
    while(1){
        status = HAL_UART_Transmit(&UART1_InitStructure, &data, 1, 0);
        HAL_Delay(100);
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
