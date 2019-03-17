#include <Arduino.h>
// Network
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
// webthing-arduino
#include <Thing.h>
#include <WebThingAdapter.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
// OTA
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
// Others
#include <DebounceEvent.h>

#include "log.h"
#include "settings.h"

WebThingAdapter* adapter;
DebounceEvent* button;

const char* relayTypes[] = {"OnOffSwitch", nullptr};
ThingDevice relay("relay", DEVICE_NAME, relayTypes);
ThingProperty relayOn("on", "Whether the relay is turned on", BOOLEAN, "OnOffProperty");

bool lastOn = false;

// Prototypes
void toggleRelay(bool on);
void handleButton();
void startRemoteUpdate();

void setup() {
  delay(1000);
  Serial.begin(115200);
  INFO_PRINTLN("\n");

  // inputs
  pinMode(BUTTON_PIN, INPUT);

  // outputs
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);

  String deviceName(DEVICE_NAME);
  deviceName.concat("-");
  deviceName.concat(ESP.getChipId());
  deviceName.toLowerCase();

  INFO_PRINT(PROJECT_NAME);
  INFO_PRINT(" ");
  INFO_PRINT(PROJECT_VERSION);
  INFO_PRINT(" (");
  INFO_PRINT(PROJECT_BUILD_VERSION);
  INFO_PRINT(") / built on: ");
  INFO_PRINT(PROJECT_DATE);
  INFO_PRINT(" at ");
  INFO_PRINTLN(PROJECT_TIME);
  INFO_PRINT_VAR("Device name: ", deviceName);
  DEBUG_PRINTLN("");

  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceName.c_str());

  if (WiFi.SSID() == "") {
    WiFi.beginSmartConfig();
    DEBUG_PRINT("SmartConfig");

    bool blink = true;
    while (!WiFi.smartConfigDone()) {
      delay(1000);
      DEBUG_PRINT(".");

      digitalWrite(LED_PIN, blink ? LED_ON : LED_OFF);
      blink = !blink;
    }

    DEBUG_PRINTLN("OK");
  } else {
    DEBUG_PRINT_VAR("Reusing saved SSID: ", WiFi.SSID());
  }

  WiFi.begin();
  DEBUG_PRINT("Connecting to WiFi");

  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);

    DEBUG_PRINT(".");

    digitalWrite(LED_PIN, blink ? LED_ON : LED_OFF);
    blink = !blink;
  }

  DEBUG_PRINTLN("OK");
  DEBUG_PRINTLN("");

  WiFi.stopSmartConfig();
  digitalWrite(LED_PIN, LED_OFF);

  INFO_PRINT("IP address: ");
  INFO_PRINTLN(WiFi.localIP());

  adapter = new WebThingAdapter(deviceName, WiFi.localIP());
  relay.addProperty(&relayOn);
  adapter->addDevice(&relay);
  adapter->begin();

  INFO_PRINT("Thing URL: http://");
  INFO_PRINT(deviceName);
  INFO_PRINT(".local/things/");
  INFO_PRINTLN(relay.id);
  INFO_PRINTLN("");

  button = new DebounceEvent(
    BUTTON_PIN,
    BUTTON_PUSHBUTTON | BUTTON_SET_PULLUP | BUTTON_DEFAULT_HIGH
  );
}

void loop() {
  handleButton();

  MDNS.update();
  adapter->update();

  bool on = relayOn.getValue().boolean;
  if (on != lastOn) {
    DEBUG_PRINTLN("Adapter will change relay state");
    toggleRelay(on);
  }
  lastOn = on;
}

void handleButton() {
  if (unsigned int event = button->loop()) {
    if (event == EVENT_RELEASED) {
      switch (button->getEventCount()) {
        case 1:
          if (button->getEventLength() >= HARD_RESET_THRESHOLD_MS) {
            INFO_PRINTLN("Hard reset triggered");

            WiFi.disconnect();
            delay(3000);
            ESP.reset();
          } else {
            DEBUG_PRINTLN("Built-in button will change relay state");
            bool nextOn = !lastOn;

            toggleRelay(nextOn);

            ThingPropertyValue state;
            state.boolean = nextOn;
            relayOn.setValue(state);
          }
          break;
        case 10:
          startRemoteUpdate();
          break;
      }
    }
  }
}

void toggleRelay(bool enabled) {
  DEBUG_PRINT_VAR("Relay state has changed: ", enabled);
  digitalWrite(RELAY_PIN, enabled ? RELAY_ON : RELAY_OFF);
  digitalWrite(LED_PIN, enabled ? LED_ON : LED_OFF);
}

void startRemoteUpdate() {
#if ENABLE_OTA_REMOTE_UPDATE
  INFO_PRINTLN("Remote OTA update triggered");

  bool blink = true;
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN, blink ? LED_ON : LED_OFF);
    blink = !blink;
    delay(200);
  }

  ESPhttpUpdate.setLedPin(LED_PIN, LOW);

  DEBUG_PRINT_VAR("OTA_REMOTE_URL: ", OTA_REMOTE_URL);

  WiFiClient client;
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, OTA_REMOTE_URL);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      DEBUG_PRINT("HTTP_UPDATE_FAILED Error (");
      DEBUG_PRINT(ESPhttpUpdate.getLastError());
      DEBUG_PRINT("): ");
      DEBUG_PRINTLN(ESPhttpUpdate.getLastErrorString().c_str());
      INFO_PRINTLN("HTTP_UPDATE: Failed");
      break;
    case HTTP_UPDATE_NO_UPDATES:
      DEBUG_PRINTLN("HTTP_UPDATE_NO_UPDATES");
      break;
    case HTTP_UPDATE_OK:
      INFO_PRINTLN("HTTP_UPDATE: OK");
      break;
  }
#else
  INFO_PRINTLN("Remote OTA update is disabled");
#endif
}
