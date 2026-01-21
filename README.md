# **Monitor telemetrie motoru**
Monitor k zobrazování skrytých údajů motoru.

[![Ukázka ovládání a webserveru](https://img.youtube.com/vi/wReb6p8iHjs/0.jpg)](https://www.youtube.com/watch?v=wReb6p8iHjs)


## **Hardware:**

Diagnostika Vgate iCar pro OBD II s bluetooth: https://www.diags.cz/diagnostiky-pro-android-ios-pc/diagnostika-vgate-icar-pro-obd-ii-s-bluetooth-pro-android--pc/
LilyGO TTGO T-Displej ESP32 1.14 TFT: https://www.laskakit.cz/lilygo-ttgo-t-displej-esp32-1-14-tft-wifi-modul/

## **Software:**

Výchozí nastavení v **settings.h**

- BT_NAME = "V-LINK"; - název BT modulu Vgate, který je vidět při hledání BT zařízení v telefonu
- BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW; – jas spořiče displeje (25 %)

### **Displej**

Možnost zobrazit tyto položky, ale prakticky by mělo jít vše, co zobrazí Car scanner:

- Teplota oleje
- Teplota kapaliny
- Napětí
- Rychlost vozidla (reálná)
- Celkový stav ujetých kilometrů (odometr)

