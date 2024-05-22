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
