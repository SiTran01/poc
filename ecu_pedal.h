#ifndef ECU_PEDAL_H
#define ECU_PEDAL_H

#include "stm32f407_base.h"
#include "mcal_adc.h" 
#include "stdio.h"
#include "mcal_gpio.h"

/* * 1. C?U HÌNH NGU?NG (CALIBRATION)
 * Ông ch?nh 2 s? này d?a trên th?c t? do du?c khi nh? ga và d?p lút sàn.
 */
#define PEDAL_MIN_ADC       50    // Giá tr? ADC khi nh? h?t ga (Deadzone du?i)
#define PEDAL_MAX_ADC       4000  // Giá tr? ADC khi d?p k?ch sàn (Deadzone trên)

/* * 2. STRUCT QU?N LÝ PEDAL
 */
typedef struct {
    ADC_Handle_t ADCHandle;
    uint8_t PedalPosition;
} Pedal_Handle_t;

/* * 3. FUNCTION PROTOTYPES
 */

/**
 * @brief  Kh?i t?o bàn d?p ga
 * @param  pPedal: Con tr? d?n bi?n qu?n lý Pedal
 * @param  ADCx: B? ADC mu?n dùng (ADC1, ADC2, ADC3...)
 * @param  Channel: Kênh ADC tuong ?ng v?i chân c?m (Tra Datasheet, ví d? PA1 là Ch1)
 * @param  Port: Port GPIO ch?a chân c?m (GPIOA, GPIOB...)
 * @param  Pin: S? th? t? chân (PIN_0, PIN_1...)
 */
void ECU_Pedal_Init(Pedal_Handle_t *pPedal, ADC_Type_Def *ADCx, uint8_t Channel, GPIO_Type_Def *Port, uint8_t PinNumber);

/**
 * @brief  Ð?c v? trí bàn d?p hi?n t?i, có x? lý ch?ng rung
 * @return Giá tr? % (0 - 100)
 */
uint8_t ECU_Pedal_GetPosition(Pedal_Handle_t *pPedal);

#endif
