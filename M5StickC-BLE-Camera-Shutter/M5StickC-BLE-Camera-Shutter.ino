#include <M5StickC.h>
#include <BleKeyboard.h>
 
BleKeyboard bleKeyboard("Camera Shutter C Ver0.3");
 
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
 
void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(9);
  setCpuFrequencyMhz(80);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 16);
  M5.Lcd.println("BLE Shutter");
  M5.Lcd.println("Connecting...");
 
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
      M5.Lcd.println("BLE Shutter");
      M5.Lcd.println(" BtnA :Shttr");
      M5.Lcd.println(" BtnB :Shttr");
      M5.Lcd.println(" BtnP :Reset");
      M5.Lcd.setCursor(112, 0);
      M5.Lcd.printf("%3d%%", getVlevel());
      isFirstConnected = false;
    }

    if ( M5.BtnA.wasPressed() ) {
      // ホームボタンでスライドを進める
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
    if ( M5.BtnB.wasPressed() ) {
      // 右ボタンでスライドを戻す
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    }
    if ( M5.Axp.GetBtnPress() != 0 ) {
      // 電源ボタンでタイマーリセット
      startTimer = millis();
    }
  } else {
    // 接続していない場合にはタイマーを進めない
    startTimer = millis();
  }
 
  // バッテリー残量更新(1分毎)
  if (nextVbatCheck < millis()) {
    M5.Lcd.setCursor(112, 0);
    M5.Lcd.printf("%3d%%", getVlevel());
 
    nextVbatCheck = millis() + 60000;
  }
 
  // Wait
  delay(100);
}