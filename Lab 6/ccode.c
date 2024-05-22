////MK64
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "FX1.h"
#include "GI2C1.h"
#include "WAIT1.h"
#include "CI2C1.h"
#include "CsIO1.h"
#include "IO1.h"
#include "MCUC1.h"

// Shared modules used across the project
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

// User includes
#include "MK64F12.h"  // Library to configure GPIO

// Macro definitions for clarity
#define GPIO_PORTB_PIN2_MASK 0x04
#define MAX_DELAY 300000
#define HIGH 0x04
#define LOW 0x00

// Global variables
unsigned char write[512];

// Function prototypes
void init_GPIO(void);
void perform_IO(void);
int send_data(LDD_TDeviceData *deviceData, char *message, int len);

int main(void) {
    PE_low_level_init(); // Initialize low level Processor Expert components
    init_GPIO(); // Initialize GPIO
    perform_IO(); // Main I/O loop
    return 0;
}

void init_GPIO(void) {
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; // Enable Port B Clock Gate Control
    PORTB_GPCLR = 0x00040100; // Configure PORTB[2] for GPIO
    GPIOB_PDDR = GPIO_PORTB_PIN2_MASK; // Configure PORTB[2] as output
    GPIOB_PDOR = LOW; // Initialize PORTB[2] = 0
}

void perform_IO(void) {
    LDD_TDeviceData *SM1_DeviceData = SM1_Init(NULL);
    uint32_t delay;
    uint8_t ret, who;
    int8_t temp;
    int16_t accX, accY, accZ, magX, magY, magZ;
    int len;

    FX1_Init(); // Initialize FX1 module
    for (;;) {
        // Get WHO AM I values
        if (FX1_WhoAmI(&who) != ERR_OK) return ERR_FAILED;
        len = sprintf(write, "Who Am I value in decimal \t: %4d\n", who);
        send_data(SM1_DeviceData, write, len);

        // Get raw temperature values
        if (FX1_GetTemperature(&temp) != ERR_OK) return ERR_FAILED;
        len = sprintf(write, "RAW Temperature value in decimal \t: %4d\n", temp);
        send_data(SM1_DeviceData, write, len);

        // Set up and get sensor readings
        if (FX1_WriteReg8(FX1_CTRL_REG_1, 0x0D) != ERR_OK) return ERR_FAILED;
        accX = FX1_GetX();
        accY = FX1_GetY();
        accZ = FX1_GetZ();
        len = sprintf(write, "Accelerometer value \tX: %4d\t Y: %4d\t Z: %4d\n", accX, accY, accZ);
        send_data(SM1_DeviceData, write, len);

        // Get magnetometer values
        if (FX1_GetMagX(&magX) != ERR_OK) return ERR_OK;
        if (FX1_GetMagY(&magY) != ERR_OK) return ERR_OK;
        if (FX1_GetMagZ(&magZ) != ERR_OK) return ERR_OK;
        len = sprintf(write, "Magnetometer value \tX: %4d\t Y: %4d\t Z: %4d\n", magX, magY, magZ);
        send_data(SM1_DeviceData, write, len);
    }
}

int send_data(LDD_TDeviceData *deviceData, char *message, int len) {
    GPIOB_PDOR = HIGH; // Set PORTB[2] to high before calling SM1_SendBlock
    SM1_SendBlock(deviceData, message, len);
    for (uint32_t delay = 0; delay < MAX_DELAY; delay++); // Delay
    GPIOB_PDOR = LOW; // Set PORTB[2] to low after sending SM1_SendBlock
    return 0;
}
