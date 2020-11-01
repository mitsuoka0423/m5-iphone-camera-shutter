#include <M5StickC.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Camera Shutter M5StcickC");

void coundDown(int waitSec)
{
    for (int i = waitSec; i >= 0; i--)
    {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(30, 30);
        M5.Lcd.printf("%d", i);
        delay(1000);
    }
}

void takeSwingVideo()
{
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);

    coundDown(7);

    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
}

void setup()
{
    M5.begin();
    delay(50);
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(3);
    bleKeyboard.begin();
}

void loop()
{
    M5.update();
    M5.Lcd.fillScreen(BLACK);

    if (bleKeyboard.isConnected())
    {
        if (M5.BtnA.wasPressed())
        {
            takeSwingVideo();
        }
    }

    delay(50);
}
