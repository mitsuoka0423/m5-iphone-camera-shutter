#include <M5StickC.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Camera Shutter C Ver0.8");
 
// バッテリー更新用
unsigned long nextVbatCheck = 0;
 
// タイマー用
unsigned long startTimer = 0;

// 初回接続フラグ
bool isFirstConnected = true;

// バッテリー残量取得
int getVlevel() {
  float vbat = M5.Axp.GetBatVoltage();
  int vlevel = ( vbat - 3.2 ) / 0.8 * 100;
  if ( vlevel < 0 ) {
    vlevel = 0;
  }
  if ( 100 < vlevel ) {
    vlevel = 100;
  }
 
  return vlevel;
}

void takeVideo() {
  int waitingSec = 5;
  int videoLengthSec = 7;

  M5.Lcd.fillScreen(BLACK);

  for (int i = waitingSec; i > 0; i--)
  {
    M5.Lcd.setCursor(50, 16);
    M5.Lcd.setTextSize(4);
    M5.Lcd.println(i);
    delay(1000);
  }

  bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);

  for (int i = videoLengthSec; i > 0; i--)
  {
    M5.Lcd.setCursor(50, 16);
    M5.Lcd.setTextSize(4);
    M5.Lcd.println(i);
    delay(1000);
  }

  bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);

  isFirstConnected = true;
}

void showVatteryVolume() {
  M5.Lcd.setCursor(112, 0);
  M5.Lcd.printf("%3d%%", getVlevel());
}

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(9);
  setCpuFrequencyMhz(80);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 16);
  M5.Lcd.println("BLE Shutter Ver0.8");
  M5.Lcd.println("Connecting...");

  pinMode(32, INPUT);
  pinMode(33, INPUT);
 
  // 起動時にしかバッテリー残量は設定できない
  bleKeyboard.setBatteryLevel(getVlevel());
  bleKeyboard.begin();
}
 
void loop() {
  // ボタン状態更新
  M5.update();
 
  // キー操作
  if (bleKeyboard.isConnected()) {
    if (isFirstConnected) {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 16);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("BLE Shutter");
      M5.Lcd.println(" Press Btn ->");
      showVatteryVolume();
      isFirstConnected = false;
    }

    if ( M5.BtnA.wasPressed() ) {
      takeVideo();
    }
    Serial.println(analogRead(32));
    Serial.println(analogRead(33));
    if (analogRead(32) > 4000 || analogRead(33) > 4000) {
      takeVideo();
    }
    if ( M5.BtnB.wasPressed() ) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
    if ( M5.Axp.GetBtnPress() != 0 ) {
      // 電源ボタンでタイマーリセット
      startTimer = millis();
    }
  } else {
    // 接続していない場合にはタイマーを進めない
    startTimer = millis();

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 16);
    M5.Lcd.println("BLE Shutter");
    M5.Lcd.println("Connecting...");
  }
 
  // バッテリー残量更新(1分毎)
  if (nextVbatCheck < millis()) {
    showVatteryVolume();
    nextVbatCheck = millis() + 60000;
  }
 
  // Wait
  delay(100);
}
