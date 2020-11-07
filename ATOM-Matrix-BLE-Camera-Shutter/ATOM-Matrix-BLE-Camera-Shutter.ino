#include <M5Atom.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Camera Shutter");

void setup()
{
    M5.begin(true, false, true);
    delay(50);
    M5.dis.drawpix(12, 0xf00000);
    bleKeyboard.begin();
}

void loop()
{
    if(bleKeyboard.isConnected()) {
        M5.dis.clear();
        
        if (M5.Btn.wasPressed()) {
            takeSwingVideo();
        }
        delay(50);
        M5.update();
    }
}

void takeSwingVideo() {
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);

    coundDown(5500, 0xf00000);

    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
}

void coundDown(int waitMilliSec, int color) {
    setAllLed(color);

    for (int i = 4; i >= 0; i--) {
        delay(waitMilliSec / 5);
        clearLedCol(i);
    }
}

void setAllLed(int color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        M5.dis.drawpix(i, color);     
    }
}

void clearLedCol(int col) {
    col = col % 5;

    for (int i = 0; i < 5; i++) {
        M5.dis.drawpix(i * 5 + col, 0x000000);
    }
}