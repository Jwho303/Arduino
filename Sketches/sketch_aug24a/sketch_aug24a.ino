/*
 Name:    Sketch1.ino
 Created: 8/22/2017 7:27:25 PM
 Author:  J
*/

struct light
{
  int pin;
  int value;
};

light lights[] = { {3,LOW},{ 4,LOW },{ 5,LOW },{ 6,LOW },{ 7,LOW },{ 10,LOW },{ 11,LOW },{ 12,LOW },{ 13,LOW } };
light colorLights[] = { 7,8,9 };
int middleLightIndex = 4;
int lightIndex =-1;
int buttonPin = 2;

int tonePin = 4;
int toneFrequency = 262;

int timeBetweenLights = 100;
int timer = 0;

int buttonState = 0;
bool buttonPressed = false;

bool forwardDirection = true;

enum GameState { Play, Win, Lose };
GameState gameState = Play;

int flashCount = 0;
bool flashLightOn = false;

// the setup function runs once when you press reset or power the board
void setup()
{
  Serial.begin(9600);

  SetLightPins();
  pinMode(buttonPin, INPUT);
}

void SetLightPins()
{
  for (int i = 0; i < sizeof(lights); i++)
  {
    pinMode(lights[i].pin, OUTPUT);
  }


}

#define numlights (sizeof(lights)/sizeof(int))

void loop()
{

  if (gameState == Play)
  {
    LightsInPlay();
    ButtonCheck();
  }
  else if (gameState == Win)
  {
    WinLights();
  }
  else
  {
    LoseLights();
  }

  DisplayLights();
  delay(1);
}

void LightsInPlay()
{
  if (timer > timeBetweenLights)
  {
    Serial.println(lightIndex);
    Serial.println("||");
    //LoopIndex();
    PingPongIndex();
    SetLights();
    timer = 0;
  }
  else
  {
    timer++;
  }
}

void WinLights()
{
  if (timer > timeBetweenLights/2)
  {
    //Serial.println(gameState);

    if (!flashLightOn)
    {
      flashLightOn = true;
      flashCount++;

    }
    else
    {
      flashLightOn = false;

      if (flashCount >= 10)
      {
        flashCount = 0;
        gameState = Play;
      }
    }

    timer = 0;
  }
  else
  {
    timer++;
  }

  if (flashLightOn)
  {
    lights[lightIndex].value = HIGH;
  }
  else
  {
    lights[lightIndex].value = LOW;
  }

}

void LoseLights()
{
  if (timer > timeBetweenLights * 2)
  {
    

    if (!flashLightOn)
    {
      flashLightOn = true;
      flashCount++;

    }
    else
    {
      flashLightOn = false;

      if (flashCount >= 5)
      {
        flashCount = 0;
        gameState = Play;
      }
    }

    timer = 0;
  }
  else
  {
    timer++;
  }

  if (flashLightOn)
  {
    lights[lightIndex].value = HIGH;
  }
  else
  {
    lights[lightIndex].value = LOW;
  }

}

void ButtonCheck()
{
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && !buttonPressed)
  {
    buttonPressed = true;
    WinCheck();
  }
  else if (buttonState == LOW && buttonPressed)
  {
    buttonPressed = false;
  }
}

void WinCheck()
{
  if (gameState == Play)
  {
    if (lightIndex == middleLightIndex)
    {
      gameState = Win;
    }
    else
    {
      gameState = Lose;
    }

    SetLights(LOW);
    timer = 0;
  }
}

void SetLights()
{
  for (int i = 0; i < numlights; i++)
  {
    if (i == lightIndex)
    {
      lights[i].value = HIGH;
    }
    else
    {
      lights[i].value = LOW;
    }
  }
}

void SetLights(int v)
{
  for (int i = 0; i < numlights; i++)
  {
    lights[i].value = v;
  }
}

void DisplayLights()
{
  for (int i = 0; i < numlights; i++)
  {
    digitalWrite(lights[i].pin, lights[i].value);
  }

  //digitalWrite(colorLights[0].pin, colorLights[0].value);
  //digitalWrite(colorLights[1].pin, colorLights[1].value);
  //digitalWrite(colorLights[2].pin, colorLights[2].value);
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
  if (forwardDirection)
  {
    lightIndex++;
    Serial.println("Plus");
  }
  else
  {
    lightIndex--;
    Serial.println("Minus");
  }

  if (lightIndex > numlights - 1)
  {
    forwardDirection = false;
    Serial.println("Reached Max");
  }
  else if (lightIndex < 0)
  {
    forwardDirection = true;
    lightIndex = 0;
    Serial.println("Reached Min");
  }


}

