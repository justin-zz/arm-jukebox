
void setup()
{  
  Serial.begin(115200);
  
  for (int i = WP ; i < LAST ; i++) {
    steppers[i].driver.setMaxSpeed(steppers[i].speed);
    steppers[i].driver.setAcceleration(steppers[i].acceleration);
    steppers[i].driver.moveTo(steppers[i].MAX_RANGE);
    pinMode(steppers[i].BOOT_PIN, OUTPUT);
    digitalWrite(steppers[i].BOOT_PIN, HIGH);
  }

}