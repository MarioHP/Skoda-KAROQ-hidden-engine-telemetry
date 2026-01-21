# 🏎️ Monitor telemetrie motoru (OBD II)

Monitor pro zobrazování skrytých údajů motoru v reálném čase pomocí ESP32 a OBD II diagnostiky.

---

## 🚗 Podporované motory
Seznam ověřených motorizací, na kterých byl monitor úspěšně testován. Seznam budu postupně doplňovat na základě zpětné vazby.

* **1.5 TSI Evo 2** (kód motoru: `DXD`)
* *Další budou doplněny...*

> [!TIP]
> Pokud jste monitor vyzkoušeli na jiném motoru, dejte mi prosím vědět (např. přes Issues), abych mohl seznam rozšířit!

---
<p align="center">
  <ins><b>🎥 Ukázka systému v praxi</b></ins><br>
  <a href="https://www.youtube.com/watch?v=KxJALGIXn9g">
    <img src="https://img.youtube.com/vi/KxJALGIXn9g/maxresdefault.jpg" width="600">
  </a>
</p>

---

## 🛠️ Hardware
Pro zprovoznění systému budete potřebovat následující komponenty:

* **Diagnostika:** [Vgate iCar Pro OBD II Bluetooth](https://www.diags.cz/diagnostiky-pro-android-ios-pc/diagnostika-vgate-icar-pro-obd-ii-s-bluetooth-pro-android--pc/) (nebo jiný kompatibilní ELM327 BT modul).
* **Mikrokontrolér:** [LilyGO TTGO T-Display ESP32 1.14 TFT](https://www.laskakit.cz/lilygo-ttgo-t-displej-esp32-1-14-tft-wifi-modul/).

---

## 💻 Software & Konfigurace

> [!CAUTION]
> **Důležité upozornění:** Pro správnou funkčnost a připojení k OBD je nutné v Arduino IDE použít konkrétní verze knihoven a desek:
> - **Board:** `esp32` ver. 2.0.17 (výběr: ESP32 Dev Module)
> - **Knihovna ELMDuino:** ver. 3.3.0
> - **Knihovna TFT_eSPI:** ver. 2.5.43

### Výchozí nastavení (`settings.h`)
V konfiguračním souboru lze upravit tyto parametry:
- `BT_NAME = "V-LINK";` – Název BT modulu, ke kterému se ESP32 připojuje.
- `BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW;` – Jas spořiče displeje (25 %).

---

## 📊 Funkční přehled zobrazovaných dat

Systém monitoruje 5 klíčových parametrů s dynamickým uzpůsobením barev.

### 1. Teplota motorového oleje (`OLEJ`)
| Barva | Stav | Význam |
| :--- | :--- | :--- |
| 🟦 **Modrá** | Studený motor | Nepoužívat plný výkon |
| 🟧 **Oranžová** | Zahřívání | Motor se blíží k provozní teplotě |
| 🟩 **Limetková** | Ideální stav | Motor je plně prohřátý |
| 🟥 **Červená** | Zvýšená zátěž | Např. dálnice nebo táhlé stoupání |
| ⬜ **Alarm** | Kritický stav | Červený text na bílém pozadí - doporučeno zvolnit |

### 2. Teplota chladicí kapaliny (`KAPALINA`)
| Barva | Stav |
| :--- | :--- |
| 🟦 **Modrá** | Studená voda |
| 🟧 **Oranžová** | Zahřívání |
| 🟩 **Zelená** | Provozní teplota |
| 🟥 **Červená** | Možné přetížení chladicího systému |

### 3. Reálná rychlost (`RYCHLOST`)
Digitální údaj přímo z řídící jednotky (přesnější než tachometr). Implementována **chytrá hystereze 2 km/h** proti problikávání barev.
- ⬜ **Šedá:** Běžná rychlost.
- 🟧 **Oranžová (Limity):** Signalizace pro rychlosti:
  - 54–60 km/h (Limit obec)
  - 94–100 km/h (Limit mimo obec)
  - 134–140 km/h (Limit dálnice)

### 4. Palubní napětí (`NAPETI`)
Měřeno na OBD zásuvce. Klíčové pro vozy s **inteligentním dobíjením** (rekuperací).
- 🟥 **Červená:** Baterie se vybíjí (motor vypnutý/slabý alternátor).
- 🟩 **Limetková:** Standardní dobíjení za jízdy.
- 🟨 **Zlatá:** Aktivní rekuperace (brzdění motorem, intenzivní dobíjení).

### 5. Ujetá vzdálenost (`UJETO`)
- Zobrazuje celkovou ujetou vzdálenost s přesností na **0.1 km**.

---

## 💡 Chytré funkce systému

* **Hystereze:** Zabraňuje zběsilému přepínání barev při jízdě na hranici limitu (např. stabilní barva při kolísání 53-54 km/h).
* **Smoothing (Vyhlazování):** Optimalizované čtení dat z OBD sběrnice, které nezatěžuje procesor a nezpůsobuje záseky vykreslování.
* **Periferní scannability:** Barevné schéma je navrženo tak, aby řidič nemusel číst čísla – stačí vnímat barvu v zorném poli.

