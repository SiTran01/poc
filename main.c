#include "stm32f407_base.h"
#include "system_manager.h"
#include "blind_led.h"
#include "mcal_systick.h"
#include "mcal_gpio.h"  // Thêm cái này d? config I2C pin
#include "mcal_i2c.h"   // Thêm I2C
#include "ecu_oled.h"   // Thêm OLED
#include "ecu_pedal.h"
#include "ecu_servo.h"

/* --- 1. BI?N TOÀN C?C CHO OLED --- */
I2C_Handle_t hI2C1;
OLED_Handle_t hOled;

/* --- 2. CÁC HÀM CONFIG I2C (L?y t? bài tru?c) --- */
void I2C1_GPIOInits(void)
{
    GPIO_Handle_t I2CPins;
    I2CPins.Port = GPIOB;
    I2CPins.Pinconfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.Pinconfig.GPIO_PinAltFunMode = 4; // AF4 = I2C1
    I2CPins.Pinconfig.GPIO_OutputType = GPIO_OUTPUT_TYPE_OD; 
    I2CPins.Pinconfig.GPIO_PUPD       = GPIO_PULL_UP;        
    I2CPins.Pinconfig.GPIO_PinSpeed   = GPIO_SPEED_HIGH;

    // SCL (PB6) & SDA (PB7)
    I2CPins.Pinconfig.GPIO_PinNumber = 6; MCAL_GPIO_Init(&I2CPins);
    I2CPins.Pinconfig.GPIO_PinNumber = 7; MCAL_GPIO_Init(&I2CPins);
}

void I2C1_Init(void)
{
    hI2C1.pI2Cx = I2C1;
    hI2C1.I2C_Config.I2C_AckControl    = I2C_ACK_ENABLE;
    hI2C1.I2C_Config.I2C_DeviceAddress = 0x61; 
    hI2C1.I2C_Config.I2C_FMDutyCycle   = I2C_FM_DUTY_2;
    hI2C1.I2C_Config.I2C_SCLSpeed      = I2C_SCL_SPEED_SM; 
    MCAL_I2C_Init(&hI2C1);
}

/* --- 3. LOGIC L?C NHI?U ADC (Gi? nguyên c?a ông) --- */
#define ADC_FILTER_SIZE  8
uint16_t Filter_ADC(uint16_t new_sample)
{
    static uint16_t buffer[ADC_FILTER_SIZE];
    static uint32_t sum = 0;
    static uint8_t index = 0;

    sum -= buffer[index];
    buffer[index] = new_sample;
    sum += new_sample;

    index++;
    if(index >= ADC_FILTER_SIZE) index = 0;

    return (uint16_t)(sum / ADC_FILTER_SIZE);
}

/* --- 4. LOGIC MAP PEDAL -> GÓC SERVO (Gi? nguyên) --- */
uint8_t Pedal_To_Angle(uint16_t adc_percent)
{
    if(adc_percent > 100) adc_percent = 100;
    return (uint8_t)((adc_percent * 180UL) / 100UL);
}

/* --- 5. LOGIC DEADZONE SERVO (Gi? nguyên) --- */
#define SERVO_DEAD_ZONE  2
void Servo_Write_With_Deadzone(Servo_Handle_t *hServo, uint8_t angle)
{
    static uint8_t last_angle = 0;
    if( (angle > last_angle + SERVO_DEAD_ZONE) || (angle + SERVO_DEAD_ZONE < last_angle) )
    {
        ECU_Servo_WriteAngle(hServo, angle);
        last_angle = angle;
    }
}

/* --- MAIN PROGRAM --- */
int main(void) {
    // 1. Kh?i t?o h? th?ng
    System_Init();
    MCAL_RCC_InitSystemClock(); // Ð?ng quên Clock h? th?ng nhé!

    // 2. Kh?i t?o LED (Debug choi thôi)
    LED_Handle_t led12 = {GPIOD, {12,0,0,0,0}}; 
    ECU_LED_Init(&led12); // Ch? demo 1 LED cho g?n code

    // 3. Kh?i t?o I2C & OLED
    I2C1_GPIOInits();
    I2C1_Init();
    MCAL_I2C_PeripheralControl(I2C1, 1); // B?t I2C
    ECU_OLED_Init(&hOled, &hI2C1);       // Init OLED

    // 4. Kh?i t?o Pedal (ADC)
    Pedal_Handle_t hGa;
    // PA1 là ADC1 Channel 1
    ECU_Pedal_Init(&hGa, ADC1, 1, GPIOA, 1); 

    // 5. Kh?i t?o Servo
    Servo_Handle_t servo0;
    // Timer 4, Channel 1 (Thu?ng là PD12 ho?c PB6 - Ki?m tra l?i schematic c?a ông nhé)
    // N?u PB6 dang dùng cho I2C thì Servo ph?i dùng chân khác (VD: PB8 - Tim4_Ch3 ho?c PD12 n?u remap)
    // Gi? s? ông dùng PD12 cho Servo (TIM4_CH1)
    ECU_Servo_Init(&servo0, TIM4, 1, GPIOD, 12, 2);

    // Bi?n luu tr?
    uint16_t adc_raw;
    uint16_t adc_flt;    // Giá tr? này s? là T?c d? (0-100)
    uint8_t servo_angle; // Giá tr? này là Góc quay (0-180)

    while(1)
    {
        // --- BU?C 1: Ð?C D? LI?U ---
        adc_raw = ECU_Pedal_GetPosition(&hGa); // Tr? v? 0-100%
        adc_flt = Filter_ADC(adc_raw);         // L?c cho mu?t

        // --- BU?C 2: TÍNH TOÁN ---
        // Servo c?n góc 0-180
        servo_angle = Pedal_To_Angle(adc_flt);
        
        // --- BU?C 3: ÐI?U KHI?N CO KHÍ (SERVO) ---
        Servo_Write_With_Deadzone(&servo0, servo_angle);

        // --- BU?C 4: HI?N TH? OLED ---
        ECU_OLED_Clear();
        
        // V? ch?
        ECU_OLED_PrintString_F5x7(56, 0, "KM/H");
        
        // V? d?ng h? (Truy?n tr?c ti?p giá tr? pedal 0-100 vào làm t?c d?)
        ECU_OLED_DrawSpeedometer((uint8_t)adc_flt);
        
        // Ð?y ra màn hình
        ECU_OLED_UpdateScreen(&hOled);

        // --- BU?C 5: DELAY ---
        // Delay 20-50ms là d?p cho c? m?t nhìn và servo
        MCAL_Delay_ms(20);
    }
}