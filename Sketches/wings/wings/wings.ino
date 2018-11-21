#include <Servo.h>
Servo leftServo;  // create servo object to control a servo
Servo rightServo;

bool wingsActive = false;
bool buttonDown = false;

int potpin = 0;  // analog pin used to connect the potentiometer
int buttonPin = 2;
int buttonState = 0;
int val;    // variable to read the value from the analog pin
int longPressDelay = 100;
int longPressTimer = 0;

void setup() {
    Serial.begin(9600);
    leftServo.attach(9);  // attaches the servo on pin 9 to the servo object
    rightServo.attach(10);  // attaches the servo on pin 9 to the servo object

    leftServo.write(90);
    rightServo.write(90);

    pinMode(buttonPin, INPUT);
    Serial.println("Start");
}

void loop() {
    val = analogRead(potpin);
    val = map(val, 0, 1023, 60, 120);
    buttonState = digitalRead(buttonPin);
    //Button down
    if (buttonState == HIGH && !buttonDown)
    {
    buttonDown = true;
Serial.println("Button down");
    }
    //Button up
    else if (buttonState == LOW && buttonDown)
    {
        buttonDown = false;
        longPressTimer = 0;
        Serial.println("Button up");
    }
    //Long press
    else if (buttonState == HIGH && buttonDown)
    {
        longPressTimer++;
        if (longPressTimer > longPressDelay)
        {
            wingsActive =!wingsActive;
            longPressTimer = 0;
            Serial.println("Button long press");
        }
    }

    if (wingsActive)
    {
        Wings(val);
        Serial.print("Dial ");
        Serial.println(val);
    }
    delay(15);                           // waits for the servo to get there
}

void Wings(int position)
{
    leftServo.write(position);
    rightServo.write(180 - position);
}

