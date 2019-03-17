#include <MqttConnector.h>

extern uint16_t adc0, adc1, adc2, adc3;

extern int relayPinState;
extern MqttConnector* mqtt;

extern int relayPin;
extern int LED_PIN;

extern char myName[];

static void readSensor();

extern String DEVICE_NAME;
extern int PUBLISH_EVERY;

void register_publish_hooks() {
  strcpy(myName, DEVICE_NAME.c_str());
  mqtt->on_prepare_data_once([&](void) {
    Serial.println("initializing sensor...");
  });

  mqtt->on_before_prepare_data([&](void) {
    readSensor();
  });

  mqtt->on_prepare_data([&](JsonObject * root) {
    JsonObject& data = (*root)["d"];
    JsonObject& info = (*root)["info"];
    data["myName"] = myName;
    data["millis"] = millis();
    data["relayState"] = relayPinState;
    data["updateInterval"] = PUBLISH_EVERY;
    if(adc0 > 0) {
      data["adc0"] = adc0;  // ph1
    }
    if(adc1 > 0) {
      data["adc1"] = adc1;  // moisture 1
    }
    if(adc2 > 0) {
      data["adc2"] = adc2;  // ph2
    }
    if(adc3 > 0) {
      data["adc3"] = adc3;  // moisture 2
    }
    data["battery"] =  analogRead(A0) * (5.0 / 1023.0);
    
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(200);
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }, PUBLISH_EVERY);
  mqtt->on_after_prepare_data([&](JsonObject * root) {
    /**************
      JsonObject& data = (*root)["d"];
      data.remove("version");
      data.remove("subscription");
    **************/
  });

  mqtt->on_published([&](const MQTT::Publish & pub) {
    Serial.println("Published.");
  });
}

static void readSensor() {
  // perform reading sensor
  Serial.println("Perform reading sensor...");
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
}
