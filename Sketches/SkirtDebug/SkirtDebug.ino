#include "Wire.h"
#include "FastLED.h"
#define NUM_LEDS 15
#define DATA_PIN 3
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
int lightIndex = 0;

struct  Color
{
    int r;
    int g;
    int b;
};

struct Light
{
    int life;
    Color col;
};

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

int lifeMax = 5;

Light lights[NUM_LEDS];

int boostTimer = 0;
int boostWait = 2;

void setup() {

    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        SetLightLife(i, random(0, lifeMax), RandomColor());
    }

      Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

// the loop function runs over and over again until power down or reset
void loop() {
    // Turn the LED on, then pause
   // SetLight(lightIndex, { 255,0,0 });
   // FastLED.show();
   // delay(100);
    // Now turn the LED off, then pause
   // SetLight(lightIndex, { 0,0,0 });
GetWire();

    if (boostTimer > boostWait)
    {
        boostTimer = 0;
        SetLightLife(random(0, NUM_LEDS - 1), lifeMax, RandomColor());
    }
    else
    {
        boostTimer++;
    }

    ProcessLifeLight();
    FastLED.show();
    delay(100);
    //  lightIndex = GetIndex(lightIndex - 1);
}

void GetWire()
{
    Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  // print out data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
    Serial.println();
}

void SetLight(int index, Color col)
{
    leds[index] = CRGB{ col.r,col.g,col.b };
}

int GetIndex(int index)
{
    if (index > NUM_LEDS - 1)
    {
        return 0;
    }
    else if (index < 0)
    {
        return NUM_LEDS - 1;
    }
    else
    {
        return index;
    }
}


Color LifeColor(Light light)
{
    float factor = (float)light.life / (float)lifeMax;

    int r = round(light.col.r * factor);
    int g = round(light.col.g * factor);
    int b = round(light.col.b * factor);

    return{ r,g,b };
}

void ProcessLifeLight()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (lights[i].life > 0)
        {
            lights[i].life--;

            SetLight(i, LifeColor(lights[i]));
        }
    }
}

void SetLightLife(int index, int life, Color col)
{
    lights[index] = { life, col };
}

Color RandomColor()
{
    int r = random(0, 256);
    int g = random(0, 256);
    int b = random(0, 256);

    return{ r,g,b };
}



