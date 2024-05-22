#include "fsl_device_registers.h"

int main(void) {
    // Configure Clock Gating for PORT B, D
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // Enable Port B Clock Gate Control
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // Enable Port D Clock Gate Control

    // Configure PORTB[3:2] for GPIO
    PORTB_GPCLR = 0x000C0100;

    // Configure PORTD[7:0] for GPIO
    PORTD_GPCLR = 0x00FF0100;

    // Configure PORTB as INPUT
    GPIOB_PDDR = 0x0;

    // Configure PORTD as OUTPUT
    GPIOD_PDDR = 0x00FF;

    // PD0 = A1
    // PD1 = A2
    // PD2 = B1
    // PD3 = B2
    // PD4 = ENA
    // PD5 = ENB

    unsigned long i = 0;

    while (1) {
        // Read PORTB
        unsigned short ROT_DIR = GPIOB_PDIR & 0x04;
        unsigned short ROT_SPD = GPIOB_PDIR & 0x08;

        if (ROT_DIR == 0 && ROT_SPD == 0) {
            // Rotate CW 22.5 degrees
            // Delay is 4x longer because steps are 22.5 degrees instead of 90 degrees

            // Step 0
            GPIOD_PDOR = 0x36;
            for (i = 0; i < 40000; i++);

            // Step 1
            GPIOD_PDOR = 0x3A;
            for (i = 0; i < 40000; i++);

            // Step 2
            GPIOD_PDOR = 0x39;
            for (i = 0; i < 40000; i++);

            // Step 3
            GPIOD_PDOR = 0x35;
            for (i = 0; i < 40000; i++);
        } 
        else if (ROT_DIR == 0 && ROT_SPD) {
            // Rotate CW 90 degrees

            // Step 0
            GPIOD_PDOR = 0x36;
            for (i = 0; i < 10000; i++);

            // Step 1
            GPIOD_PDOR = 0x3A;
            for (i = 0; i < 10000; i++);

            // Step 2
            GPIOD_PDOR = 0x39;
            for (i = 0; i < 10000; i++);

            // Step 3
            GPIOD_PDOR = 0x35;
            for (i = 0; i < 10000; i++);
        } 
        else if (ROT_DIR && ROT_SPD == 0) {
            // Rotate CCW 22.5 degrees
            // Delay is 4x longer because steps are 22.5 degrees instead of 90 degrees

            // Step 0
            GPIOD_PDOR = 0x36;
            for (i = 0; i < 40000; i++);

            // Step 1
            GPIOD_PDOR = 0x35;
            for (i = 0; i < 40000; i++);

            // Step 2
            GPIOD_PDOR = 0x39;
            for (i = 0; i < 40000; i++);

            // Step 3
            GPIOD_PDOR = 0x3A;
            for (i = 0; i < 40000; i++);
        } 
        else if (ROT_DIR && ROT_SPD) {
            // Rotate CCW 90 degrees

            // Step 0
            GPIOD_PDOR = 0x36;
            for (i = 0; i < 10000; i++);

            // Step 1
            GPIOD_PDOR = 0x35;
            for (i = 0; i < 10000; i++);

            // Step 2
            GPIOD_PDOR = 0x39;
            for (i = 0; i < 10000; i++);

            // Step 3
            GPIOD_PDOR = 0x3A;
            for (i = 0; i < 10000; i++);
        }
    }

    return 0;
}
