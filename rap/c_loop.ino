  
void loop()
{

  // Steppers will move back and forth based on their MAX_RANGE so ensure this is correct & that they're centered
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

    // Follows this pattern: "X;0;520;!"
    // X indicates the start of a new cmd
    // 0 indicates the motor to be moved
    // 520 the speed
    // ; an arbitrary char used to tokenize
    // ! ends the string

    tmp = strtok(input, ";");
    if (!strcmp(tmp, "X")) {

      tmp = strtok(NULL, ";");
      int channel = atoi(tmp); // stepper i plays notes from a midi channel i

      tmp = strtok(NULL, ";");
      steppers[channel].speed = getSafeSpeed(atoi(tmp));
      
      steppers[channel].driver.setMaxSpeed(steppers[channel].speed/2); // Added to make it sound better
      steppers[channel].driver.setMaxSpeed(steppers[channel].speed);

    } else if (!strcmp(tmp, "ARDUINO?")) {
      Serial.print("ARM!");
    }
  } // end of serial available

}

// Ensure speeds fall between 100 - 1050 range (G2 - C6)
// Note: lowest note may not be G2 but probably higher/different depending on the motor/load/etc
float getSafeSpeed(float speed) {
      if (speed < 100)
        speed = 0;
      while (speed > 1050) // Halving == going down an octave; so C6 -> C5
        speed /= 2;
  return speed;
}