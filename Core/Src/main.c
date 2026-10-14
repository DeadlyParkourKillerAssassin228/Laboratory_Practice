#include "init.h"
volatile uint8_t led_count = 0; 
volatile uint8_t swap_mode = 0; 

// Массивы битов регистра BSRR для управления PB0, PB7, PB14, PB1, PB2, PB15
static const uint32_t LED_SET[6] = {
    GPIO_BSRR_BS0,   // LED1 (PB0)
    GPIO_BSRR_BS7,   // LED2 (PB7)
    GPIO_BSRR_BS14,  // LED3 (PB14)
    GPIO_BSRR_BS1,   // LED4 (PB1)
    GPIO_BSRR_BS2,   // LED5 (PB2)
    GPIO_BSRR_BS15   // LED6 (PB15)
};

static const uint32_t LED_RESET[6] = {
    GPIO_BSRR_BR0,
    GPIO_BSRR_BR7,
    GPIO_BSRR_BR14,
    GPIO_BSRR_BR1,
    GPIO_BSRR_BR2,
    GPIO_BSRR_BR15
};

// Функция обновления состояния светодиодов
void update_leds(uint8_t count) {
    for (int i = 0; i < 6; i++) {
        if (i < count) {
            GPIOB->BSRR = LED_SET[i];   
        } else {
            GPIOB->BSRR = LED_RESET[i]; 
        }
    }
}

void delay_ms(volatile uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 4000; i++) {
        __NOP();
    }
}

int main(void) {
    GPIO_Ini();
    update_leds(led_count);

    uint8_t btn1_prev = 0;
    uint8_t btn2_prev = 0;
    uint8_t btn3_prev = 0;

    while (1) {
        uint8_t btn1_cur = (READ_BIT(GPIOC->IDR, GPIO_IDR_ID13) != 0); // Кнопка 1 (PC13)
        uint8_t btn2_cur = (READ_BIT(GPIOC->IDR, GPIO_IDR_ID0)  != 0); // Кнопка 2 (PC0)
        uint8_t btn3_cur = (READ_BIT(GPIOC->IDR, GPIO_IDR_ID1)  != 0); // Кнопка 3 (PC1)

        if (btn3_cur && !btn3_prev) {
            delay_ms(40); // Антидребезг
            if (READ_BIT(GPIOC->IDR, GPIO_IDR_ID1) != 0) {
                swap_mode = !swap_mode; // Инвертируем флаг
            }
        }
        btn3_prev = btn3_cur;

        uint8_t trigger_inc = swap_mode ? btn2_cur : btn1_cur;
        uint8_t trigger_dec = swap_mode ? btn1_cur : btn2_cur;
        
        static uint8_t inc_prev = 0;
        static uint8_t dec_prev = 0;

        if (trigger_inc && !inc_prev) {
            delay_ms(40);
            if (led_count < 6) {
                led_count++;
                update_leds(led_count);
            }
        }
        inc_prev = trigger_inc;

        if (trigger_dec && !dec_prev) {
            delay_ms(40);
            if (led_count > 0) {
                led_count--;
                update_leds(led_count);
            }
        }
        dec_prev = trigger_dec;
    }
}