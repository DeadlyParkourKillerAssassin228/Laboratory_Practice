#include "init.h"

void GPIO_Ini(void) {
    // Включение тактирования портов GPIOB и GPIOC через CMSIS
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

    // 1. Настройка LED 1 (PB0) - ПРЯМЫМ ОБРАЩЕНИЕМ К ПАМЯТИ (стр. 50)
    // MODER0 = 01 (Output)
    *(volatile uint32_t*)(0x40020400UL + 0x00UL) |= (0x01UL << 0);
    // OTYPER0 = 0 (Push-Pull)
    *(volatile uint32_t*)(0x40020400UL + 0x04UL) &= ~(0x01UL << 0);
    // OSPEEDR0 = 01 (Medium Speed)
    *(volatile uint32_t*)(0x40020400UL + 0x08UL) |= (0x01UL << 0);
    // PUPDR0 = 00 (No pull-up, pull-down)
    *(volatile uint32_t*)(0x40020400UL + 0x0CUL) &= ~(0x03UL << 0);

  
    MY_SET_BIT(MY_GPIOB_MODER, MY_PIN7_MODE_OUT);
    MY_CLEAR_BIT(MY_GPIOB_OTYPER, (1UL << 7));
    MY_SET_BIT(MY_GPIOB_OSPEEDR, MY_PIN7_OSPEED_MID);
    MY_CLEAR_BIT(MY_GPIOB_PUPDR, (3UL << 14));

    //3. Настройка LED 3..6 (PB14, PB1, PB2, PB15) - ЧЕРЕЗ CMSIS
    // Настройка пинов 1, 2, 14, 15 на выход (General purpose output)
    GPIOB->MODER |= (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | 
                     GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0);

    // Push-pull для пинов 1, 2, 14, 15
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 | 
                       GPIO_OTYPER_OT14 | GPIO_OTYPER_OT15);

    // Средняя скорость (Medium speed)
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR1_0 | GPIO_OSPEEDER_OSPEEDR2_0 | 
                       GPIO_OSPEEDER_OSPEEDR14_0 | GPIO_OSPEEDER_OSPEEDR15_0);

    // Без подтяжек
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 | 
                      GPIO_PUPDR_PUPD14 | GPIO_PUPDR_PUPD15);

    //4. Настройка Кнопок: PC13 (на плате), PC0, PC1 (внешние)
    // Режим входа (Input mode, 00)
    GPIOC->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER13);

    // Включаем подтяжку к земле (Pull-Down, 10) для внешних кнопок PC0 и PC1
    // (PC13 уже имеет внешнюю подтяжку на плате)
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1);
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1);
}