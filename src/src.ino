#include "freertos/task.h"
#include "time.h"
#include <Adafruit_NeoPixel.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define PIN_CHEST 25
#define PIN_LEFT 26
#define PIN_RIGHT 32
#define PIN_CENTER 27

#define NUM_CHEST 16
#define NUM_RIGHT 8
#define NUM_LEFT 8
#define NUM_CENTER 14

TaskHandle_t taskHandle_main, taskHandle_bl;

char state = 'N', buf = '0';

Adafruit_NeoPixel strip_chest = Adafruit_NeoPixel(NUM_CHEST, PIN_CHEST, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip_left = Adafruit_NeoPixel(NUM_LEFT, PIN_LEFT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_right = Adafruit_NeoPixel(NUM_RIGHT, PIN_RIGHT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_center = Adafruit_NeoPixel(NUM_CENTER, PIN_CENTER, NEO_GRB + NEO_KHZ800);

BluetoothSerial SerialBT;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  SerialBT.begin("ESP32"); //Bluetooth device name
  Serial.printf("SerialBT runs\n");

  strip_chest.begin();
  strip_left.begin();
  strip_right.begin();
  strip_center.begin();

  strip_chest.show();
  strip_left.show();
  strip_center.show();
  strip_right.show();

  xTaskCreatePinnedToCore(mainloop, "task_main", 4096, NULL, 10, &taskHandle_main, 1);
  xTaskCreatePinnedToCore(task, "task_bl", 4096, NULL, 5, &taskHandle_bl, 0);
  Serial.printf("%s\n", "done");
}
void loop() {}

void setBack(int r, int g, int b)
{
  Serial.printf("%s %d, %d, %d\n", "back set as", r, g, b);

  setcenter(r, g, b);
  setright(r, g, b);
  setleft(r, g, b);
}

void setleft(int r, int g, int b)
{
  for (size_t i = 0; i < NUM_LEFT; i++)
  {
    strip_left.setPixelColor(i, r, g, b);
  }
  strip_left.show();
}

void setright(int r, int g, int b)
{
  for (size_t i = 0; i < NUM_RIGHT; i++)
  {
    strip_right.setPixelColor(i, r, g, b);
  }
  strip_right.show();
}

void setcenter(int r, int g, int b)
{
  for (size_t i = 0; i < NUM_CENTER; i++)
  {
    strip_center.setPixelColor(i, r, g, b);
  }
  strip_center.show();
}

void mainloop(void *arg)
{
  while (1)
  {
    delay(1);
    switch (state)
    {
    case '1':
    {
      rainbowCycle(1);
      break;
    }

    case '2':
    {
      rainbow(5);
      break;
    }

    case '3':
    {
      int cnt = 0;
      while (cnt <= 255)
      {
        for (int i = 0; i < NUM_CHEST; i++)
        {
          strip_chest.setPixelColor(i, cnt, 0, 0);
        }
        strip_chest.show();
        cnt += 4;
        delay(1.47);
      }
      delay(310);

      while (cnt >= 0)
      {
        for (int i = 0; i < NUM_CHEST; i++)
        {
          strip_chest.setPixelColor(i, cnt, 0, 0);
        }
        strip_chest.show();
        cnt -= 4;
        delay(2);
      }
      cnt = 0;
      for (int i = 0; i < NUM_CHEST; i++)
      {
        strip_chest.setPixelColor(i, 0, 0, 0);
      }
      strip_chest.show();
      delay(500);
      break;
    }

    case '4':
    {
      strip_chest.setPixelColor(NUM_CHEST - 1, 0, 255, 0);
      for (int i = 0; i < NUM_CHEST; i++)
      {
        for (size_t i = 0; i < NUM_CHEST; i++)
        {
          strip_chest.setPixelColor(i, 0, 0, 0);
        }
        strip_chest.setPixelColor(i + 14 % NUM_CHEST, 0, 255, 0);
        strip_chest.setPixelColor(i + 15 % NUM_CHEST, 0, 255, 0);
        strip_chest.setPixelColor(i + 16 % NUM_CHEST, 0, 255, 0);
        strip_chest.setPixelColor(i + 17 % NUM_CHEST, 0, 255, 0);
        strip_chest.setPixelColor(i + 18 % NUM_CHEST, 0, 255, 0);
        strip_chest.setPixelColor(NUM_CHEST - 1, 0, 255, 0);
        strip_chest.setPixelColor((i + 13 % NUM_CHEST) - 2, 0, 0, 0);
        strip_chest.show();
        delay(15);
      }
      break;
    }

    case 'W':
    {
      for (int i = 0; i < NUM_CHEST; i++)
      {
        strip_chest.setPixelColor(i, 255, 255, 255);
      }
      strip_chest.show();
      state = '0';
      break;
    }

    case 'N':
    {
      for (int i = 0; i < NUM_CHEST; i++)
      {
        strip_chest.setPixelColor(i, 0, 0, 0);
      }
      strip_chest.show();
      state = '0';
      break;
    }

    case 'G':
    {
      for (int i = 0; i < NUM_CHEST; i++)
      {
        strip_chest.setPixelColor(i, 0, 255, 0);
      }
      strip_chest.show();
      state = '0';
      break;
    }

    case 'R':
    {
      for (int i = 0; i < NUM_CHEST; i++)
      {
        strip_chest.setPixelColor(i, 255, 0, 0);
      }
      strip_chest.show();
      state = '0';
      break;
    }

    case 'B':
    {
      for (int i = 0; i < NUM_CHEST; i++)
      {
        strip_chest.setPixelColor(i, 0, 0, 255);
      }
      strip_chest.show();
      state = '0';
      break;
    }

    case 'Y':
    {
      for (int i = 0; i < NUM_CHEST; i++)
      {
        strip_chest.setPixelColor(i, 255, 255, 0);
      }
      strip_chest.show();
      state = '0';
      break;
    }

    case 'S':
    {
      stoprainbow();
      state = '0';
      break;
    }

    case 'w':
    {
      setBack(255, 255, 255);
      state = buf;
      break;
    }

    case 'r':
    {
      setBack(255, 0, 0);
      state = buf;
      break;
    }

    case 'g':
    {
      setBack(0, 255, 0);
      state = buf;
      break;
    }

    case 'b':
    {
      setBack(0, 0, 255);
      state = buf;
      break;
    }

    case 'y':
    {
      setBack(255, 255, 0);
      state = buf;
      break;
    }

    case 'v':
    {
      setBack(255, 0, 255);
      state = buf;
      break;
    }

    case 'n':
    {
      setBack(0, 0, 0);
      state = buf;
      break;
    }

    case 'd':
    {
      ESP.restart();
      break;
    }

    default:
    {
      break;
    }
    }
  }
}

void task(void *arg)
{
  Serial.printf("%s\n", "into task");
  while (1)
  {
    delay(1);
    int BTAvailavle, SerialAvailable;
    if ((BTAvailavle = SerialBT.available()) || (SerialAvailable = Serial.available()))
    {
      buf = state;
      if (BTAvailavle)
        state = SerialBT.read();
      else if (SerialAvailable)
        state = Serial.read();
    }
  }
}

void stoprainbow()
{
  uint16_t i;

  for (i = 0; i < NUM_CHEST - 1; i++)
    {
      strip_chest.setPixelColor(i, Wheel((i * 256 / NUM_CHEST) & 255));
    }
    strip_chest.setPixelColor(15, 255, 255, 255);
    strip_chest.show();
}

void rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < NUM_CHEST; i++)
    {
      strip_chest.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip_chest.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_CHEST - 1; i++)
    {
      strip_chest.setPixelColor(i, Wheel(((i * 256 / NUM_CHEST) + j) & 255));
    }
    strip_chest.setPixelColor(15, 255, 255, 255);
    strip_chest.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip_chest.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip_chest.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip_chest.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
