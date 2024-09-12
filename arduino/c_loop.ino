  
void loop()
{

  // Steppers will move back and forth based on the MAX_RANGE
  for (int i = WP ; i < LAST ; i++) {
    if (steppers[i].driver.distanceToGo() == 0)
      steppers[i].driver.moveTo(-steppers[i].driver.currentPosition());
    steppers[i].driver.run();
  }

  if (Serial.available()) {
    
    // Use memset to 'empty' input buffer; may not be necessary,
    // but ensures that input is in a well-defined state
    memset(input, 0, sizeof(input));

    // Characters from the serial input are read into the input array
    // readBytesUntil returns the number of characters read to the size variable
    byte size = Serial.readBytesUntil('!', input, INPUT_SIZE);

    // Add the final 0 to end the C string
    input[size] = 0;

    // Command example:
    // "X;0x90;520!" move stepper at a speed of 520 steps/s (520 Hz note frequency)

    tmp = strtok(input, ";");

    if (!strcmp(tmp, "X")) {

      tmp = strtok(NULL, ";");
      int channel = atoi(tmp); // channel/stepper

      tmp = strtok(NULL, ";");
      steppers[channel].speed = getSafeSpeed(atoi(tmp));
      
      steppers[channel].driver.setMaxSpeed(steppers[channel].speed);
      
      if (verbose) {
        Serial.print(steppers[channel].NAME);
        if (steppers[channel].speed > 0) {
          Serial.print(" @ ");
          Serial.print(steppers[channel].speed);
          Serial.print("Hz, ");
        } else {
          Serial.print(" off, ");
        }
      }

    } else if (!strcmp(tmp, "T")) {
      verbose = true;
      Serial.println("Verbose on!");
    } else if (!strcmp(tmp, "F")) {
      verbose = false;
      Serial.println("Verbose off!");
    } else {
      Serial.println("Invalid command received!");
    }
  } // end of serial available

} // end of loop

// Ensure speeds fall between 100 - 1000 range (G2 - C6)
float getSafeSpeed(float speed) {
      if (speed < 100)
        speed = 0;
      while (speed > 1000)
        speed /= 2;
  return speed;
}