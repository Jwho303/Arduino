/*
 Name:    Sketch1.ino
 Created: 8/22/2017 7:27:25 PM
 Author:  J
*/

int lights[] = { 7,8,9,10,11,12,13 };
int middleLight = 6;
int lightIndex = 0;
int buttonPin = 3;

int timeBetweenLights = 10;
int timer = 0;

int buttonState = 0;
bool buttonPressed = false;

bool forwardDirection = true;

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);

  SetLightPins();
  digitalWrite(lights[0], HIGH);
  pinMode(buttonPin,INPUT);
}

void SetLightPins()
{
  for (int i = 0; i < sizeof(lights); i++)
  {
    pinMode(lights[i], OUTPUT);
  }


}

#define numlights (sizeof(lights)/sizeof(int))

void loop()
{
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && !buttonPressed)
  {
    buttonPressed = true;
    forwardDirection = !forwardDirection;
  }
  else if (buttonState == LOW && buttonPressed)
  {
    buttonPressed = false;
  }

  if (timer > timeBetweenLights)
  {
    LoopIndex();
    Serial.println(lightIndex);
    SetLights();
    timer = 0;
  }
  else
  {
    timer++;
  }

  delay(10);
}

void SetLights()
{
  for (int i = 0; i < numlights; i++)
  {
    if (i == lightIndex)
    {
      digitalWrite(lights[i], HIGH);
    }
    else
    {
      digitalWrite(lights[i], LOW);
    }
  }
}

void LoopIndex()
{
  if (forwardDirection)
    lightIndex++;
  else
    lightIndex--;

  if (lightIndex < 0)
  {
    lightIndex = (numlights - 1);
  }
  else if (lightIndex >= numlights)
  {
    lightIndex = 0;
  }
}

void PingPongIndex()
{
  //if (forwardDirection)
  lightIndex++;
  //else
  //  lightIndex--;

  if (lightIndex >= numlights)
  {
    lightIndex = 0;
  }

  //if (lightIndex < 0)
  //{
  //  lightIndex = (numlights - 1);
  //}
}

