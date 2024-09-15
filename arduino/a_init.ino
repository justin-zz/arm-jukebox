// Libraries (make sure to install them if using the IDE)
#include <AccelStepper.h>

//Received serial command variables
const int INPUT_SIZE = 20;
char input[INPUT_SIZE + 1];
char* tmp;     //stores chars as we tokenize

//Enumeration for motors
enum Stepper_ID {
    WR,
    WP,
    EE,
    LAST//not a motor; makes iteration more intuitive
};

//Define what variables to use for our motor
struct Stepper {
  AccelStepper driver;
  char*         NAME;
  long          MAX_RANGE; // Steppers will move back and forth based on the MAX_RANGE
  int           BOOT_PIN;  // Used to enable the dip switch pins for stepper dmodes
  float         speed;     // A stepper's current speed
  float         acceleration;
};

//Create the motor objects
Stepper wr = {AccelStepper (AccelStepper::DRIVER, 10,  11), "Roll",  100000, 4, 0.0, 10000};
Stepper wp = {AccelStepper (AccelStepper::DRIVER, 8,    9), "Pitch", 500, 16, 0.0, 10000};
Stepper ee = {AccelStepper (AccelStepper::DRIVER, 12,  13), "EE",    400, 5, 0.0, 10000};

//Pack into an array for iterability
Stepper steppers[] = {wr, wp, ee};
