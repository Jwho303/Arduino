
#include <Servo.h>

Servo horizontalServo;
Servo verticalServo;

struct Vector2
{
    int x;
    int y;
};

enum TurretMode { SetupArea, Sleep, Move, Wait, Wiggle, Circle };
TurretMode mode = SetupArea;

int laserPin = 4;

Vector2 minValue = { 60,60 };
Vector2 maxValue = { 120,120 };

Vector2 currentValue = { 0,0 };
Vector2 previousValue = { 0,0 };
Vector2 targetValue = { 0,0 };
Vector2 startValue = { 0,0 };

int moveDelay = 0;
bool invertX = true;
bool invertY = true;
bool laserOn = true;

int waitTimer = 0;
int waitDuration = 0;
int modeLoop = 0;
bool wiggleReturn = false;

bool isPaused;
int pauseTimer = 0;
int pauseDuration = 0;

int circleResolution = 20;
Vector2 circlePositions[20];

void setup() {
    //Serial.begin(9600);
    horizontalServo.attach(5);
    verticalServo.attach(3);
    pinMode(laserPin, OUTPUT);

    //bottom left
    currentValue = minValue;
    //bottom right
    targetValue = { maxValue.x,minValue.y };

    DriveServo();

    randomSeed(303);

    Pause(1000);

    //gameSpeed = 1;
    //StartRandomMove();
}

void loop() {

    if (!isPaused)
    {

        switch (mode)
        {
            case Sleep:

                break;
            case SetupArea:
                SettingUp();
                break;
            case Move:
                Moving();
                break;
            case Wait:
                Waiting();
                break;
            case Wiggle:
                Wiggling();
                break;
            case Circle:
                DrawCircle();
                break;
            default:
                break;
        }
    }
    else
    {
        PauseMode();
    }

    LaserControl();

    delay(1);
}

void PauseMode()
{
    pauseTimer++;

    if (pauseTimer >= pauseDuration)
    {
        pauseTimer = 0;
        isPaused = false;
    }
}

void Pause(int wait)
{
    isPaused = true;
    pauseDuration = wait;
    pauseTimer = 0;
}

void RandomMode()
{
    int weights[] =
    {
        100,//ShortMove
        30,//pause
        50,//wiggle
        10, //move to ground
        50//circle
    };

    int weightMax = 0;
    int arrayLength = 5;

    for (int i = 0; i < arrayLength; i++)
    {
        weightMax += weights[i];
    }

    int randomMode = random(0, weightMax);
    int weightCounter = 0;
    int selectedMode = 0;

    for (int i = 0; i < arrayLength; i++)
    {
        if (randomMode < (weightCounter + weights[i]))
        {
            selectedMode = i;
            break;
        }

        weightCounter += weights[i];
    }


    switch (selectedMode)
    {
    case 0:
        Pause(2000);
        StartRandomShortMove();
      //  Serial.println("Short Move");
        break;
    case 1:
        Pause(5000);
        RandomMode();
       // Serial.println("Wait");
        break;
    case 2:
        Pause(2000);
        StartWiggle();
        //Serial.println("Wiggle");
        break;
    case 3:
        MoveToGround();
       // Serial.println("Ground");
        break;
    case 4:
        Pause(2000);
        StartCircle();
       // Serial.println("Circle");
        break;
    default:
        break;
    }

}

void StartWaiting()
{
    Pause(3000);
    mode = Wait;
    waitTimer = 0;
    waitDuration = random(5, 30);
    //Serial.println("Wait");
}

void StartRandomShortMove()
{
    mode = Move;
    SetRandomTarget(10, true);
    //Serial.println("Move");
}

void MoveToGround()
{
    mode = Move;
    targetValue = { round((minValue.x + maxValue.x) / 2),minValue.y };
    //Serial.println("Move");
}

void Waiting()
{

    waitTimer += 1;
    if (waitTimer >= waitDuration)
    {
        RandomMode();
    }

    //Serial.print(waitTimer);
    //Serial.print("/");
    //Serial.println(waitDuration);
}

void Moving()
{
    if (MoveToTarget(currentValue, targetValue, 50))
    {
        RandomMode();
    }

    DriveServo();
}

//Wiggle
//=====================================
void StartWiggle()
{
    mode = Wiggle;
    modeLoop = 0;
    wiggleReturn = false;

    startValue = currentValue;

    targetValue = RandomWigglePosition();
    //Serial.println("Wiggle");
}

Vector2 RandomWigglePosition()
{
    int randWig = (int)random(0, 8);
    Vector2 distance = { 6,3 };
    Vector2 offset = { 0,0 };
    switch (randWig)
    {
    case 0:
        offset = { -distance.x,-distance.y };
        break;
    case 1:
        offset = { 0,-distance.y };
        break;
    case 2:
        offset = { distance.x,-distance.y };
        break;
    case 3:
        offset = { -distance.x,0 };
        break;
    case 4:
        offset = { distance.x,0 };
        break;
    case 5:
        offset = { -distance.x,distance.y };
        break;
    case 6:
        offset = { 0,distance.y };
        break;
    case 7:
        offset = { distance.x,distance.y };
        break;

    default:
        offset = { -distance.x,-distance.y };
        break;
    }
    return { offset.x + currentValue.x,offset.y + currentValue.y };
}

void Wiggling()
{
    if (MoveToTarget(currentValue, targetValue, 5))
    {
        if (!wiggleReturn)
        {
            targetValue = startValue;
            wiggleReturn = true;
            //Pause(1000);
            //      Serial.print("Out ");
        }
        else
        {
            targetValue = RandomWigglePosition();
            wiggleReturn = false;
            //Pause(1000);
            //Serial.print("In ");
            modeLoop++;
            //  Serial.print(modeLoop);
            if (modeLoop > 30)
            {
                RandomMode();
                //  Serial.println("End");
            }
        }
    }

    DriveServo();
}

//Setup
//=====================================
void SettingUp()
{
    if (MoveToTarget(currentValue, targetValue, 0))
    {
        modeLoop++;

        if (modeLoop == 1)
        {
            targetValue = maxValue;
            Pause(1000);
        }
        else if (modeLoop == 2)
        {
            targetValue = { minValue.x,maxValue.y };
            Pause(1000);
        }
        else if (modeLoop == 3)
        {
            targetValue = minValue;
            Pause(1000);
        }
        else if (modeLoop == 4)
        {
            targetValue = { round((minValue.x + maxValue.x) / 2),round((minValue.y + maxValue.y) / 2) };
            Pause(1000);
        }
        else
        {
            RandomMode();
            //  StartRandomShortMove();
              // MoveToGround();
            modeLoop = 0;
        }
    }
    DriveServo();
}

//Moving
//=====================================
bool MoveToTarget(Vector2 &current, Vector2 target, int delay)
{
    Pause(delay);

    if (current.x < target.x)
        current = { current.x + 1,current.y };
    else if (current.x > target.x)
        current = { current.x - 1,current.y };

    if (current.y < target.y)
        current = { current.x ,current.y + 1 };
    else if (current.y > target.y)
        current = { current.x,current.y - 1 };

    return IsSameCoordinate(current, target);
}

void DriveServo()
{
    if (currentValue.x != previousValue.x)
    {
        if (currentValue.x >= minValue.x && currentValue.x <= maxValue.x)
        {
            if (invertX)
            {
                horizontalServo.write(180 - currentValue.x);
            }
            else
            {
                horizontalServo.write(currentValue.x);
            }
        }
        previousValue = { currentValue.x, previousValue.y };
    }

    if (currentValue.y != previousValue.y)
    {
        if (currentValue.y >= minValue.y && currentValue.y <= maxValue.y)
        {
            if (invertY)
            {
                verticalServo.write(180 - currentValue.y);
            }
            else
            {
                verticalServo.write(currentValue.y);
            }
        }
        previousValue = { previousValue.x, currentValue.y };
    }
}

bool IsSameCoordinate(Vector2 vec1, Vector2 vec2)
{
    if (vec1.x == vec2.x && vec1.y == vec2.y)
        return true;
    else
        return false;
}

void SetRandomTarget(int range, bool within)
{
    //Serial.println("StartMove");
    int distance = 99;
    int randomX = 0;
    int randomY = 0;

    bool pass = false;
    //Serial.print("Current ");
    //Serial.print(rotationX);
    //Serial.print(" x ");
    //Serial.println(rotationY);

    while (!pass)
    {
        pass = false;

        randomX = (int)random(minValue.x, maxValue.x);
        randomY = (int)random(minValue.y, maxValue.y);

        if (WithinBounds({ randomX, randomY }))
        {
            if (DistanceCheck(range, randomX, randomY, within))
            {
                pass = true;
            }
        }

        //Serial.print("Target ");
        //Serial.print(randomX);
        //Serial.print(" x ");
        //Serial.println(randomY);

        //Serial.print("Distance ");
        //Serial.println(distance);
    }



    targetValue = { randomX, randomY };

    //Serial.print("Target ");
    //Serial.print(targetRotationX);
    //Serial.print(" x ");
    //Serial.println(targetRotationY);

    //Serial.print("Distance ");
    //Serial.println(distance);
}

bool WithinBounds(Vector2 vec)
{
    bool pass = true;

    if (vec.x < minValue.x || vec.x > maxValue.x)
    {
        pass = false;
    }

    if (vec.y < minValue.y || vec.y > maxValue.y)
    {
        pass = false;
    }

    return pass;
}

bool DistanceCheck(float range, int randomX, int randomY, bool within)
{

    int XX = randomX - currentValue.x;
    int YY = randomY - currentValue.y;
    int distance = sqrt((XX*XX) + (YY*YY));

    if (within)
    {
        return distance <= range;
    }
    else
    {
        return distance > range;
    }
}

//Circle
//======================================

void StartCircle()
{
    CreateEllipse(10, currentValue);
    mode = Circle;
    modeLoop = 0;
    targetValue = circlePositions[0];
}

void DrawCircle()
{
    if (MoveToTarget(currentValue, targetValue, 50))
    {
        modeLoop++;


        if (modeLoop > circleResolution-1)
        {
            RandomMode();
        }
        else
        {
            targetValue = circlePositions[modeLoop];
        }

    }
    DriveServo();
}

void CreateEllipse(float radius, Vector2 center)
{
    for (int i = 0; i <= circleResolution; i++)
    {
        float angle = (float)i / (float)circleResolution * 2.0f * M_PI;
        circlePositions[i] = Vector2{ radius * cos(angle), radius * sin(angle) };
        circlePositions[i] = ClampBounds(AddVector(circlePositions[i], center));
    }
}

Vector2 AddVector(Vector2 vec1, Vector2 vec2)
{
    return{ vec1.x + vec2.x, vec1.y + vec2.y };
}

Vector2 ClampBounds(Vector2 position)
{
    if (position.x > maxValue.x)
    {
        position = { maxValue.x, position.y };
    }
    else if (position.x < minValue.x)
    {
        position = { minValue.x, position.y };
    }

    if (position.y > maxValue.y)
    {
        position = { position.x,maxValue.y };
    }
    else if (position.y < minValue.y)
    {
        position = { position.x,minValue.y };
    }

    return position;
}

//Laser
//=====================================

void LaserControl()
{
    if (laserOn)
        digitalWrite(laserPin, HIGH);
    else
        digitalWrite(laserPin, LOW);
}




