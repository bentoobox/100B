 //Arduino Side 
// Define the pin where the LED is connected.
int ledPin = 3;

// Set up the environment.
void setup() {
  // Configure Timer 2 control register.
  // This changes the PWM frequency to 245 Hz, which is half of its default frequency.
  TCCR2B = (TCCR2B & 0b11111000) | 0x05;

  // Set the LED pin as an output pin.
  pinMode(ledPin, OUTPUT);
}

// Main program loop.
void loop() {
  // Increase the LED brightness from 0 to 255 in steps of 5.
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    // Write the current brightness level to the LED.
    analogWrite(ledPin, fadeValue);
    // Pause to observe the dimming effect.
    delay(30);
  }

  // Decrease the LED brightness from 255 to 0 in steps of 5.
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    // Write the current brightness level to the LED.
    analogWrite(ledPin, fadeValue);
    // Pause to observe the dimming effect.
    delay(30);
  }
}

//FRDM K64F side 
#include "fsl_device_registers.h"

// Global constants and variables
const unsigned short seg_decoder[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};
unsigned int nr_overflows = 0;

// Interrupt handler for FlexTimer Module 3
void FTM3_IRQHandler(void) {
    nr_overflows++;  // Increment overflow counter
    uint32_t SC_VAL = FTM3_SC;  // Read Status And Control register
    FTM3_SC &= ~0x80; // Clear Timer Overflow Flag (TOF)
}

// Main function
int main(void) {
    // Clock configuration for ports and FlexTimer
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK;  // Enable clocks for Port C and Port D
    SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;  // Enable clock for FTM3

    // Pin configuration
    PORTC_PCR10 = 0x300; // Set Port C Pin 10 as FTM3_CH6 (ALT3)
    PORTC_GPCLR = 0x01BF0100; // Configure PORTC[8:7] & PORTC[5:0] for GPIO
    PORTD_GPCLR = 0x00FF0100; // Configure PORTD[7:0] for GPIO

    // GPIO direction configuration
    GPIOC_PDDR = 0x01BF; // Set PORTC[8:7] & PORTC[5:0] as OUTPUTS
    GPIOD_PDDR = 0x00FF; // Set PORTD[7:0] as OUTPUTS

    // FlexTimer configuration
    FTM3_MODE = 0x5;  // Enable FTM3
    FTM3_MOD = 0xFFFF;  // Set the counter's max value
    FTM3_SC = 0x0D;  // Set up system clock / 32, enable overflow interrupt
    NVIC_EnableIRQ(FTM3_IRQn); // Enable FTM3 interrupts
    FTM3_SC |= 0x40; // Start the timer

    // Variables for timing and calculations
    unsigned int t1, t2, t3, pulse_width, period, duty_cycle;
    unsigned int portc_value, portd_value;

    while (1) {
        // Reset counter and overflow
        FTM3_CNT = 0;
        nr_overflows = 0;

        // Edge capture for timing analysis
        FTM3_C6SC = 0x4; // Set to capture on rising edge
        while (!(FTM3_C6SC & 0x80));  // Wait for capture event
        FTM3_C6SC &= ~0x80;
        t1 = FTM3_C6V; // Capture time of first edge

        FTM3_C6SC = 0x8; // Set to capture on falling edge
        while (!(FTM3_C6SC & 0x80));  // Wait for capture event
        FTM3_C6SC &= ~0x80;
        t2 = FTM3_C6V; // Capture time of second edge

        // Pulse width and period calculations
        pulse_width = (t2 >= t1) ? ((nr_overflows << 16) + (t2 - t1)) : ((nr_overflows-1) << 16) + (t2 - t1);
        period = t3 - t1;
        duty_cycle = (pulse_width * 100) / period;

        // Decode duty cycle for 7-segment display
        unsigned short ones = duty_cycle % 10;
        unsigned short tens = duty_cycle / 10;
        portc_value = seg_decoder[ones];
        portd_value = seg_decoder[tens];

        // Update PORTC and PORTD outputs
        GPIOC_PDOR = (portc_value & 0x3F) | ((portc_value & 0xC0) << 1);
        GPIOD_PDOR = portd_value;
    }

    return 0;
}
