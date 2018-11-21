int pushButton =2;
int led = 9;           // the PWM pin the LED is attached to

int brightness = 20;
int fadeAmount = 5;    // how many points to fade the LED by

bool hasPressed = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(pushButton, INPUT);
  pinMode(led, OUTPUT);

  
}

void loop() {
  // put your main code here, to run repeatedly:

int buttonState = digitalRead(pushButton);

if (buttonState == 1 && hasPressed == false)
{
  brightness = brightness + fadeAmount;
  
  if (brightness >= 255) 
  {
    brightness = 20;
  }

  hasPressed = true;
  analogWrite(led, brightness);
}
else if (buttonState == 0)
{
  hasPressed = false;
  // analogWrite(led, 0);
}

Serial.println(buttonState);
delay(1);

}
