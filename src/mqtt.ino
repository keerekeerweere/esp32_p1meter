void sendMQTTMessage(const char *topic, const char *payload) {
  bool result = mqttClient.publish(topic, payload, false);
}

void makeSureMqttConnected() {
  uint _retries = MQTT_MAX_RECONNECT_TRIES;

  while (!mqttClient.connected() && _retries > 0) {
    if (mqttClient.connect(String(HOSTNAME).c_str(), MQTT_USER, MQTT_PASS)) {
      char *message = new char[16 + strlen(HOSTNAME) + 1];
      strcpy(message, "p1 meter alive: ");
      strcat(message, HOSTNAME);
      mqttClient.publish(String(MQTT_STATUS_TOPIC).c_str(), message);
    }
    else {
      logger.debug("MQTT Connection Failed! Retries left: %u", _retries);
      _retries--;
      delay(5000);
    }
  }

  if (!mqttClient.connected()) {
    blinkLed(20, 200); // Blink moderately fast to indicate failed connection
    logger.error("Connection to MQTT Failed! Rebooting...");
    ESP.restart();
  }
}

void sendMetric(String name, String metric) {
  String topic = String(MQTT_ROOT_TOPIC) + "/" + name;
#ifdef DEBUG
  Serial.println(topic);
#endif
  sendMQTTMessage(topic.c_str(), metric.c_str());
}

void sendDataToBroker() {
  for (int i = 0; i < telegramObjects.size(); i++) {
    if (telegramObjects[i].sendData) {
      logger.debug("Sending: %s value: %s" , telegramObjects[i].name.c_str() , telegramObjects[i].value.c_str());
      sendMetric(telegramObjects[i].name, telegramObjects[i].value);
      telegramObjects[i].sendData = false;
    }
  }
}
