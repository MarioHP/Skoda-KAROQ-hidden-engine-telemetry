# **Monitor telemetrie motoru**
Monitor k zobrazování skrytých údajů motoru.

[![Ukázka ovládání a webserveru](https://img.youtube.com/vi/dRTjhbWR-sU/0.jpg)](https://www.youtube.com/watch?v=dRTjhbWR-sU)


## **Hardware:**

Diagnostika Vgate iCar pro OBD II s bluetooth: https://www.diags.cz/diagnostiky-pro-android-ios-pc/diagnostika-vgate-icar-pro-obd-ii-s-bluetooth-pro-android--pc/
LilyGO TTGO T-Displej ESP32 1.14 TFT: https://www.laskakit.cz/lilygo-ttgo-t-displej-esp32-1-14-tft-wifi-modul/

## **Software:**

> [!NOTE]
Při programování je potřeba nainstalovat správné verze desek a knihoven uvedených v poznámce na začátku kódu, jinak se monitor nepřipojí!

> Board: esp32 ver. 2.0.17 (ESP32 Dev Module), Library: ELMDuino ver. 3.3.0, TFT_eSPI ver. 2.5.43

Výchozí nastavení v **settings.h**

- BT_NAME = "V-LINK"; - název BT modulu Vgate, který je vidět při hledání BT zařízení v telefonu
- BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW; – jas spořiče displeje (25 %)


### **Displej** - Funkční přehled HET systému

Možnost zobrazit následující položky:

- **1. Teplota motorového oleje (OLEJ)**

**Logika barev:**

* Modrá:  (Studený motor, nepoužívat plný výkon).
* Oranžová: – (Zahřívání, motor se blíží k provozní teplotě).
* Zelená (Lime): – (Ideální stav, motor je plně prohřátý).
* Červená: – (Zvýšená zátěž, např. dálnice nebo táhlý kopec).
* Alarm (Červená na bílé):  (Kritické přehřátí, doporučeno zvolnit).


- **2. Teplota chladicí kapaliny (KAPALINA)**

> Ukazuje stav chladicího okruhu.

** Logika barev:**

* Modrá:  (Studená voda).
* Oranžová: – (Zahřívání).
* Zelená: – (Provozní teplota).
* Červená:  (Možné přetížení chladicího systému).


- **3. Reálná rychlost (RYCHLOST)**

> Přesnější než tachometr (neřeší odchylku výrobce). Klíčový parametr s implementovanou chyytrou hysterezí (2 km/h), aby barvy neproblikávaly.

**Logika barev (Zóny):**

* Šedá: – (Normální rychlost).
  
* Oranžová: 
– ZÓNA 54-60 (obec limit)
– ZÓNA 94-100 (okreska limit)
– ZÓNA 134-140 (dálnice limit)


- **4. Palubní napětí (NAPETI)**

> Napětí na OBD zásuvce. Důležité kvůli inteligentnímu dobíjení (rekuperaci).

**Logika barev:**

* Červená:  – (Baterie se vybíjí, motor vypnutý nebo slabý alternátor).
* Zelená (Lime): – (Standardní dobíjení/provoz).
* Zlatá (Gold):  - (Rekuperace – auto intenzivně brzdí motorem a dobíjí baterii "zadarmo").


- **5. Ujetá vzdálenost (UJETO)**

Zobrazuje se s přesností na jedno desetinné místo (např. 12469.5 km).

---

## Chytré funkce systému

* Hystereze: Pokud jedeš přesně na hranici (např. 53 km/h), barva se nezmění při každém zakolísání o 1 km/h.
* Smoothing: Data z OBD jsou čtena v cyklech, které nezpomalují procesor ESP32.
* Scannability: Barvy jsou zvoleny tak, aby řidič věděl "vše je OK" (zelená) nebo "pozor" (oranžová/červená) pouhým periferním viděním.

