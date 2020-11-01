#include <M5StickC.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Camera Shutter M5StcickC");

void setup()
{
    M5.begin();
    delay(50);
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
    } else {
        M5.Lcd.printf("Wait...");
    }

    delay(50);
}

void takeSwingVideo()
{
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);

    coundDown(7);

    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
}

void coundDown(int waitSec)
{
    String time = (String)(waitMilliSec / 1000);
    M5.Lcd.printf(time);

    for (int i = waitSec; i >= 0; i--)
    {
        delay(waitMilliSec / 5);
        M5.Lcd.printf(i);
        M5.Lcd.fillScreen(BLACK);
    }
}