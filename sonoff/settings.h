#ifndef SONOFF_SETTINGS_H
#define SONOFF_SETTINGS_H

#define PROJECT_VERSION "1.0.0"
#define PROJECT_DATE __DATE__
#define PROJECT_TIME __TIME__
#ifndef PROJECT_NAME
  #define PROJECT_NAME "sonoff-webthing"
#endif
#ifndef GIT_VERSION
  #define GIT_VERSION "unknown"
#endif
#define PROJECT_BUILD_VERSION GIT_VERSION

#define DEVICE_NAME "sonoff-basic"

#define HARD_RESET_THRESHOLD_MS 5000

#define ENABLE_OTA_REMOTE_UPDATE 1
#ifndef SIGNED_BIN_NAME
  #define SIGNED_BIN_NAME "sonoff-webthing-signed.bin"
#endif
#define OTA_REMOTE_URL "http://williamdurand.fr/sonoff-webthing/" SIGNED_BIN_NAME

#if defined(ARDUINO_ESP8266_WEMOS_D1MINI)
  #define BUTTON_PIN D5

  #define RELAY_PIN D6
  #define RELAY_OFF LOW
  #define RELAY_ON HIGH

  // Built-in LED is active LOW
  #define LED_PIN LED_BUILTIN
  #define LED_OFF HIGH
  #define LED_ON LOW
#else
  #define BUTTON_PIN 0

  #define RELAY_PIN 12
  #define RELAY_OFF HIGH
  #define RELAY_ON LOW

  #define LED_PIN 13
  #define LED_OFF HIGH
  #define LED_ON LOW
#endif

#endif
