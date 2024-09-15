  
void loop()
{

  // Steppers will move back and forth based on the MAX_RANGE
  for (int i = WR ; i < LAST ; i++) {
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
      
      steppers[channel].driver.setMaxSpeed(steppers[channel].speed/2);
      steppers[channel].driver.setMaxSpeed(steppers[channel].speed);
    } else if (!strcmp(tmp, "ARDUINO?")) {
      Serial.print("ARM!");
    }
  } // end of serial available

} // end of loop

// Ensure speeds fall between 100 - 1050 range (G2 - C6)
float getSafeSpeed(float speed) {
      if (speed < 100)
        speed = 0;
      while (speed > 1050)
        speed /= 2;
  return speed;
}