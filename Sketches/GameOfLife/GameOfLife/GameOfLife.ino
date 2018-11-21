#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

#define	GREEN 0x07E0
#define RED 0xF800
#define PINK 0xF19C

#define __CS 10
#define __DC 9

TFT_ILI9163C tft = TFT_ILI9163C(__CS, 8, __DC);

int gridSize = 25;
bool grid [28][28];
bool calculatedGrid [28][28];

bool showGrid = true;
int gridThickness = 1;

int scale = 1;

void setup() {
  tft.begin();
  tft.fillScreen();
  unsigned long start = micros();
  randomSeed(analogRead(0));

  RandomGrid();
  //SetUpGrid();
  DrawGrid();
}

void loop() {
  CalcualteTurn();
  DrawGrid();
  delay(5);
}

void CalcualteTurn()
{

  for (int i = 0; i < gridSize; i++)
  {
    for (int ii = 0; ii < gridSize; ii++)
    {
      calculatedGrid[i][ii] = ApplyRules(i, ii, CountNeighbours(i, ii), grid[i][ii]);
    }
  }

  for (int i = 0; i < gridSize; i++)
  {
    for (int ii = 0; ii < gridSize; ii++)
    {
      grid[i][ii] = calculatedGrid[i][ii];
    }
  }

}

bool IsValid(int num)
{

  return num >= 0 && num <= gridSize;
}

int CountNeighbours(int x, int y)
{
  int count = 0;
  for (int i = -1; i <= 1; i++)
  {
    for (int ii = -1; ii <= 1; ii++)
    {
      if (i == 0 && ii == 0)
      {

      }
      else if (IsValid(x + i) && IsValid(y + ii))
      {
        count = count + grid[x + i][y + ii];
      }
    }
  }

  return count;
}

int ApplyRules(int x, int y, int count, bool alive)
{
  int life = 0;

  if (alive)
  {
    if (count < 2)
    {
      life = 0;

    }

    if (count > 3)
    {
      life = 0;
    }

    if (count == 2 || count == 3)
    {
      life = 1;
    }

  }
  else
  {

    if (count == 3 )
    {
      life = 1;

    }
  }
  return life;
}

void RandomGrid()
{
  for (int i = 0; i < gridSize; i++)
  {
    for (int ii = 0; ii < gridSize; ii++)
    {
      grid[i][ii] = random(0, 2);
    }
  }
}

void SetUpGrid()
{
  grid[4][3] = 1;
  grid[3][3] = 1;
  grid[2][3] = 1;
  // grid[1][3] = 1;
}


void DrawGrid()
{


  for (int x = 0; x < gridSize; x++)
  {
    for (int y = 0; y < gridSize; y++)
    {
      int col;
      if (grid[x][y] == 1)
      {
        col = PINK;
      }
      else
      {
        col = BLACK;
      }

      for (int xscale = 0; xscale < scale; xscale++)
        for (int yscale = 0; yscale < scale; yscale++)
        {
          tft.drawPixel(((x * scale) + xscale) + (gridThickness*x), ((y * scale) + yscale) + (gridThickness*y), col);
        }
    }

  }
}

