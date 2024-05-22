#include "fsl_device_registers.h"
#include <stdint.h>

// Function Prototypes
void software_delay(unsigned long delay);
unsigned short ADC_read16b(void);
void PORTA_IRQHandler(void);

// Global Constants
const unsigned short seg_decoder[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
    // Segment codes for digits 0 to 9
};

// Function Definitions
void software_delay(unsigned long delay) {
    while (delay > 0) delay--;
}

unsigned short ADC_read16b(void) {
    ADC0_SC1A = 0x00; // Start conversion
    while (ADC0_SC2 & ADC_SC2_ADACT_MASK); // Wait for conversion to complete
    while (!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Check if conversion is complete
    return ADC0_RA;
}

void PORTA_IRQHandler(void) {
    unsigned short MODE_SW = GPIOB_PDIR & 0x04;
    unsigned short CNT_DIR = GPIOB_PDIR & 0x08;
    unsigned short ADC_VALUE = 0;
    float ADC_VOLTAGE = 0;
    unsigned short ADC_VOLTAGE_DISPLAY_VALUE = 0;
    static unsigned short COUNT = 0;

    if (MODE_SW == 0) { // ADC Mode
        ADC_VALUE = ADC_read16b();
        ADC_VOLTAGE = ADC_VALUE * (3.3 / 65535);
        ADC_VOLTAGE_DISPLAY_VALUE = ADC_VOLTAGE * 10;
    } else { // Counter Mode
        if (CNT_DIR == 0) {
            COUNT = (COUNT < 99) ? COUNT + 1 : 0;
        } else {
            COUNT = (COUNT > 0) ? COUNT - 1 : 99;
        }
    }

    // Display logic
    unsigned short PORTC_NUM_VALUE = MODE_SW == 0 ? ADC_VOLTAGE_DISPLAY_VALUE / 10 : COUNT / 10;
    unsigned short PORTD_NUM_VALUE = MODE_SW == 0 ? ADC_VOLTAGE_DISPLAY_VALUE % 10 : COUNT % 10;
    GPIOC_PDOR = seg_decoder[PORTC_NUM_VALUE];
    GPIOD_PDOR = seg_decoder[PORTD_NUM_VALUE];

    PORTA_ISFR = (1 << 1); // Clear interrupt flag
}

int main(void) {
    // Clock Gating for PORTS A, B, C, D
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK;
    // GPIO Configuration
    PORTB_GPCLR = 0x00C0100; // PORTB[10] and PORTB[3:2]
    PORTC_GPCLR = 0x01BF0100; // PORTC[8:7] & PORTC[5:0]
    PORTD_GPCLR = 0x00FF0100; // PORTD[7:0]
    PORTA_PCR1 = 0xA0100; // PORTA[1] for interrupt on falling edge
    GPIOB_PDDR = 0x0400; // Set PORTB[10] as output
    GPIOC_PDDR = 0x01BF; // PORTC as outputs
    GPIOD_PDDR = 0x00FF; // PORTD as outputs
    // Initial PORT states
    GPIOC_PDOR = seg_decoder[0];
    GPIOD_PDOR = seg_decoder[0];
    // ADC Initialization
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
    ADC0_CFG1 = 0x0C; // 16-bit ADC
    ADC0_SC1A = 0x1F; // Disable ADC module during initialization
    NVIC_EnableIRQ(PORTA_IRQn); // Enable PORTA interrupts

    unsigned long delay = 0x100000;
    while (1) {
        GPIOB_PTOR |= (1 << 10); // Toggle PORTB[10]
        software_delay(delay); // Software delay
    }

    return 0;
}
