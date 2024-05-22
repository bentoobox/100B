#include "fsl_device_registers.h"
#include <stdint.h>

// Simple software delay function
void delay(unsigned long cycles) {
    while (cycles > 0) 
        cycles--;  // Simple decrement
}

int main(void) {
    // Enable clock for GPIO ports B, C, and D
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // Clock for Port D
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // Clock for Port C
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // Clock for Port B
    
    // Setup GPIO on ports
    PORTC_GPCLR = 0x01BF0100; // Setup PORTC PINS 0-5 and 7-8
    PORTD_GPCLR = 0x00FF0100; // Setup PORTD PINS 0-7
    PORTB_GPCLR = 0x00060100; // Setup PORTB PINS 1-2
    
    // Set pin directions
    GPIOC_PDDR = 0x000001BF; // Outputs on PORTC
    GPIOD_PDDR = 0x000000FF; // Outputs on PORTD
    GPIOB_PDDR = 0x00000000; // Inputs on PORTB
    
    // Initialize PORT outputs
    GPIOC_PDOR = 0x0001; // Initial state for PORT C
    GPIOD_PDOR = 0x0001; // Initial state for PORT D
    
    // Constants for operations
    unsigned long cycleDelay = 0x200000;
    uint32_t portCValue = 0;
    uint32_t portDValue = 0;
    
    // Main loop
    while (1) {
        delay(cycleDelay); // Apply software delay
        
        // Read input from PORT B
        uint32_t dirInputC = GPIOB_PDIR & 0x02;
        uint32_t dirInputD = GPIOB_PDIR & 0x04;
        
        // Handle PORT C operations
        if (dirInputC == 0) {
            if (portCValue < 0xFF) portCValue++;
        } else {
            if (portCValue > 0) portCValue--;
        }
        
        // Handle PORT D operations
        if (dirInputD == 0) {
            if (portDValue < 0x80) {
                portDValue <<= 1;
            } else {
                portDValue = 0x01;
            }
        } else {
            if (portDValue > 0x01) {
                portDValue >>= 1;
            } else {
                portDValue = 0x80;
            }
        }
        
        // Update output registers
        GPIOC_PDOR = (portCValue & 0x3F) | ((portCValue & 0xC0) << 1);
        GPIOD_PDOR = portDValue;
    }
    return 0;
}
