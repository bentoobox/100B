#include <SPI.h>


// Constants
#define PIN_6 6
#define BUFFER_SIZE 255


// Globals
char buff[BUFFER_SIZE];
volatile byte indx;
volatile boolean process;


void setup() {
  Serial.begin(115200);
 
  // Set MISO pin as output for SPI communication
  pinMode(MISO, OUTPUT);
 
  // Enable SPI in slave mode
  SPCR |= _BV(SPE);
 
  // Initialize variables
  indx = 0;
  process = false;
 
  // Attach SPI interrupt for handling communication
  SPI.attachInterrupt();
 
  // Setup additional pin for signaling
  pinMode(PIN_6, OUTPUT);
  digitalWrite(PIN_6, LOW);
}


// SPI interrupt service routine
ISR(SPI_STC_vect) {
  byte c = SPDR;  // Read byte from SPI Data Register
 
  if (indx < BUFFER_SIZE) {
    buff[indx++] = c;  // Store received data
   
    if (c == '\n') {  // Check for end of transmission
      process = true;
      signalProcessing();  // Signal that processing can occur
    }
  }
}


// Function to handle post-reception processing signaling
void signalProcessing() {
  digitalWrite(PIN_6, HIGH);
  delay(100);  // Short delay to ensure signal is noticed
  digitalWrite(PIN_6, LOW);
 
  // Reset index for next message
  indx = 0;
}


void loop() {
  if (process) {
    processBuffer();  // Process the buffer if a full message has been received
    process = false;
  }
}


// Function to process the buffer content
void processBuffer() {
  // Debug output to Serial Monitor
  Serial.print("Received: ");
  Serial.println(buff);


  // Clear buffer after processing
  memset(buff, 0, BUFFER_SIZE);
}
