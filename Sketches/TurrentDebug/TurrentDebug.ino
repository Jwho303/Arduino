#include <Servo.h>
Servo horizontalServo;
Servo verticalServo;

int xPin = 0;
int yPin = 1;
int laserPin = 4;
int rotationX = 0;
bool testFlip = false;

void setup() {
  // put your setup code here, to run once:
  
  horizontalServo.attach(3);
  verticalServo.attach(5);
  pinMode(laserPin, OUTPUT);
}

void loop() {
    if (testFlip)
    {
        rotationX++;
        if (rotationX >= 180)
        {
            testFlip = false;
        }
    }
    else
    {
        rotationX--;
        if (rotationX <= 0)
        {
            testFlip = true;
        }
    }
    verticalServo.write(rotationX);
  ////ManualDrive();
  //LaserControl();

  delay(1);
  //delay(1);

}
