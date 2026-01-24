#ifndef SETTINGS_H
#define SETTINGS_H

// Definice názvu VGate
const char* BT_NAME = "V-LINK";

// Definice MAC adresy VGate
uint8_t BT_MAC[6] = {0x00, 0x1D, 0x43, 0x12, 0x34, 0x56};

// --- Definice barev ---
#define TFT_CAPRI     0x05FF
#define TFT_LIMEGREEN 0x3666
#define TFT_SKODAGREY 0xCE79
#define TFT_RED       0xF800
#define TFT_BLACK     0x0000
#define TFT_ORANGE    0xFDA0
#define TFT_GOLD      0xFEA0

// --- Konfigurace jasu ---
#define BACKLIGHT_PIN 4 // pin podsviceni
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

// Jas displeje (rozsah 0–255)
#define BRIGHTNESS_OFF     0
#define BRIGHTNESS_MIN     26    // 10 %
#define BRIGHTNESS_LOW     64    // 25 %
#define BRIGHTNESS_MEDIUM  127   // 50 %
#define BRIGHTNESS_HIGH    191   // 75 %
#define BRIGHTNESS_MAX     255   // 100 %

const int BACKLIGHT_BRIGHTNESS = BRIGHTNESS_LOW;  // Nastav jas spořiče

#endif
