int lightIndex = 13;
bool flashLightOn = true;
int flashCount = 0;

int timer = 0;
int timeBetweenLights = 100;

void setup() {
  // put your setup code here, to run once:
pinMode(lightIndex, OUTPUT);
}

void loop() {
if (timer > timeBetweenLights*2)
  {


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
      //  gameState = Play;
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
    digitalWrite(lightIndex, HIGH);
  }
  else
  {
    digitalWrite(lightIndex, LOW);
  }
  
 delay (1);
}
