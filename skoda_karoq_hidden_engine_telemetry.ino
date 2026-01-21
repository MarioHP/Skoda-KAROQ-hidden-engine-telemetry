// SKODA KAROQ hidden engine telemetry
// SETTINGS:
// Open C:\Users\xxxx\Documents\Arduino\libraries\TFT_eSPI\User_Setup_select.h
// Comment the line #include <User_Setup.h>
// Uncomment the ine #include <User_Setups/Setup25_TTGO_T_Display.h>
// Board: esp32 ver. 2.0.17 (ESP32 Dev Module), Library: ELMDuino ver. 3.3.0, TFT_eSPI ver. 2.5.43

#include <TFT_eSPI.h>
#include <SPI.h>
#include "skoda.h"
#include "settings.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
TFT_eSPI tft = TFT_eSPI(135, 240);

bool isScreensaverOn = false; 
unsigned long lastActivity = 0;

void setupELM327();
void updateDisplay(float oil, float coolant, float volt, float km);
float getOBDValue(String command, String header, float offset, float multiplier, int bytes);
void setBrightness(int level);

int errorCount = 0; 
const int MAX_ERRORS = 10; 

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);

  pinMode(BACKLIGHT_PIN, OUTPUT);
  analogWrite(BACKLIGHT_PIN, BACKLIGHT_BRIGHTNESS); 

  tft.pushImage(0, 0, 240, 135, skoda);
  delay(3000);

  SerialBT.begin("ArduHUD", true);

  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);

  bool connected = false;
  int pokus = 1;

  while (!connected) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_CAPRI);
    tft.drawString("PRIPOJOVANI BT...", 120, 50, 4);
    tft.setTextColor(TFT_SKODAGREY);
    tft.drawString("POKUS: " + String(pokus), 120, 90, 4);

    if (SerialBT.connect(BT_NAME)) {
      connected = true;
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_LIMEGREEN);
      tft.drawString("PRIPOJENO!", 120, 50, 4); 
      tft.setTextColor(TFT_SKODAGREY);
      tft.drawString("Nastavuji ECU...", 120, 90, 4);
      delay(500); 
      setupELM327(); 
    } else {
      tft.fillScreen(TFT_BLACK); 
      tft.setTextColor(TFT_RED);
      tft.drawString("V-LINK NENALEZEN", 120, 50, 4);
      tft.setTextColor(TFT_SKODAGREY);
      tft.drawString("Zkousim znovu...", 120, 90, 4); 
      pokus++;
      delay(3000); 
    }
  }
  
  tft.fillScreen(TFT_BLACK);
  drawStaticLabels();
  lastActivity = millis(); 
}

// Kresleni popisku
void drawStaticLabels() {
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_SKODAGREY);
  tft.drawString("OLEJ", 5, 9, 4);
  tft.drawString("KAPALINA", 5, 42, 4);
  tft.drawString("RYCHLOST", 5, 75, 4);
  // tft.drawString("NAPETI", 5, 75, 4);
  tft.drawString("UJETO", 5, 108, 4);
}

void setBrightness(int level) {
  analogWrite(BACKLIGHT_PIN, level); 
}


void loop() {
  // --- 1. NAPĚTÍ ---
  while(SerialBT.available()) SerialBT.read();
  SerialBT.print("ATRV\r");
  delay(150);
  String vResp = "";
  while(SerialBT.available()) {
    char c = SerialBT.read();
    if (isDigit(c) || c == '.') vResp += c;
  }
  float obdVoltage = vResp.toFloat();

  float oilTemp = getOBDValue("015C1", "41 5C", -40.0, 1.0, 1);
  float coolantTemp = getOBDValue("01051", "41 05", -40.0, 1.0, 1);
  // float coolantTemp = getOBDValue("01671", "41 67", -40.0, 1.0, 1);
  float totalKm = getOBDValue("01A61", "41 A6", 0.0, 0.1, 4);
  float speed = getOBDValue("010D1", "41 0D", 0.0, 1.0, 1);

  if (oilTemp < -45.0 || coolantTemp < -45.0 || obdVoltage < 5.0) {
    errorCount++;
    Serial.println("Chyba dat! Pocet: " + String(errorCount));
  } else {
    errorCount = 0; 
  }

    if (errorCount >= 15) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("ZTRATA DAT", 120, 50, 4);
    tft.setTextColor(TFT_SKODAGREY);
    tft.drawString("Restartuji...", 120, 90, 4);
    delay(3000);
    ESP.restart();
  }

  updateDisplay(oilTemp, coolantTemp, speed, obdVoltage, totalKm);
  
  delay(1000); 
}

void setupELM327() {
  Serial.println("Konfigurace ELM327...");
  
  while(SerialBT.available()) SerialBT.read();
  
  SerialBT.print("ATZ\r");     delay(500); 
  SerialBT.print("ATE0\r");    delay(50); 
  SerialBT.print("ATH0\r");    delay(50); 
  SerialBT.print("ATSP6\r");   delay(50); 
  SerialBT.print("ATSH7E0\r"); delay(50); 
  
    SerialBT.print("01001\r");
  delay(500);
  
    while(SerialBT.available()) SerialBT.read();
}


float getOBDValue(String command, String header, float offset, float multiplier, int bytes) {
  while(SerialBT.available()) SerialBT.read(); 
  SerialBT.print(command + "\r");

  String resp = "";
  unsigned long startWait = millis();
  bool timeout = false;

  while (resp.indexOf(">") == -1) {
    if (millis() - startWait > 1000) { 
      timeout = true;
      break;
    }
    while (SerialBT.available()) {
      resp += (char)SerialBT.read();
    }
  }

  if (!timeout) {
    int pos = resp.indexOf(header);
    if (pos != -1) {
      String dataPart = resp.substring(pos + header.length());
      dataPart.replace(" ", ""); 
      dataPart.replace(">", ""); 
      dataPart.replace("\r", "");
      dataPart.replace("\n", "");
      
      String hexVal = dataPart.substring(0, bytes * 2);
      if (hexVal.length() > 0) {
        long decimalVal = strtol(hexVal.c_str(), NULL, 16);
        return (float)decimalVal * multiplier + offset;
      }
    }
  }
  
  return -50.0; 
}

void updateDisplay(int oil, int coolant, int speed, float volt, float km) {

  tft.setTextDatum(TR_DATUM);
  int xPos = 238; 
  int standardPdd = 100;   

  // ===== 1. ŘÁDEK: OLEJ =====
  tft.setTextPadding(standardPdd); 
  if (oil > -50) {
    if (oil > 120) {
      tft.setTextColor(TFT_RED, TFT_WHITE); 
    } else if (oil <= 40) {
      tft.setTextColor(TFT_CAPRI, TFT_BLACK);
    } else if (oil <= 80) {
      tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    } else if (oil <= 105) {
      tft.setTextColor(TFT_LIMEGREEN, TFT_BLACK);
    } else {
      tft.setTextColor(TFT_RED, TFT_BLACK); 
    }
    tft.drawString(String(oil) + " `C", xPos, 9, 4);
  } else {
    tft.setTextColor(TFT_SKODAGREY, TFT_BLACK);
    tft.drawString("---", xPos, 9, 4);
  }

  // ===== 2. ŘÁDEK: KAPALINA =====
  tft.setTextPadding(standardPdd);
  if (coolant > -50) {
    if (coolant <= 40)       tft.setTextColor(TFT_CAPRI, TFT_BLACK);
    else if (coolant <= 70)  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    else if (coolant <= 100) tft.setTextColor(TFT_LIMEGREEN, TFT_BLACK);
    else                     tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString(String(coolant) + " `C", xPos, 42, 4);
  } else {
    tft.setTextColor(TFT_SKODAGREY, TFT_BLACK);
    tft.drawString("---", xPos, 42, 4);
  }

  // // ===== 3. ŘÁDEK: NAPĚTÍ =====
  // tft.setTextPadding(standardPdd);
  // // Barva podle stavu: pod 11.8V červená (vybíjí), nad 14.4V zlatá (rekuperuje)
  // if (volt < 11.8) tft.setTextColor(TFT_RED, TFT_BLACK);
  // else if (volt > 14.4) tft.setTextColor(TFT_GOLD, TFT_BLACK);
  // else tft.setTextColor(TFT_LIMEGREEN, TFT_BLACK);
  // tft.drawString(String(volt, 1) + " V", xPos, 80, 4);


 // ===== 3. ŘÁDEK: RYCHLOST (Minimalistický režim s oranžovými zónami) =====
  tft.setTextPadding(standardPdd);
  static uint16_t lastSpeedColor = TFT_SKODAGREY; 

  // Definice barev pro lepší čitelnost
  uint16_t orange = TFT_ORANGE;
  uint16_t grey = TFT_SKODAGREY;
 
  // ZÓNA 134-140 (Dálnice limit)
  if (speed >= 134 && speed <= 140) {
    lastSpeedColor = orange;
  }
  // ZÓNA 94-100 (Okreska limit)
  else if (speed >= 94 && speed <= 100) {
    lastSpeedColor = orange;
  }
  // ZÓNA 54-60 (Obec limit)
  else if (speed >= 54 && speed <= 60) {
    lastSpeedColor = orange;
  }
  // VŠE OSTATNÍ (Hystereze pro návrat do šedé)
  else {
    bool inHysteresis = (speed >= 52 && speed <= 62) || 
                        (speed >= 92 && speed <= 102) || 
                        (speed >= 132 && speed <= 142);

    if (lastSpeedColor == orange && inHysteresis) {
      lastSpeedColor = orange; 
    } else {
      lastSpeedColor = grey; 
    }
  }

  tft.setTextColor(lastSpeedColor, TFT_BLACK);
  tft.drawString(String((int)speed), xPos, 75, 4);

  // ===== 4. ŘÁDEK: UJETO =====
  tft.setTextPadding(140); 
  tft.setTextColor(TFT_SKODAGREY, TFT_BLACK);
  if (km > 0.0) {
    tft.drawString(String(km, 1), xPos, 108, 4); 
  } else {
    tft.drawString("---", xPos, 108, 4);
  }

}

