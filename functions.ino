// Функция установления соединения по WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(wifi_ssid);
  Serial.println(wifi_pass);
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(F("."));
    WiFi.printDiag(Serial);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

// Функция установления подключения к MQTT брокеру
void reconnect() { 
  while (!client.connected()) {
    Serial.println(F("Attempting MQTT connection..."));
    Serial.print(F("Client ID:"));
    Serial.print(clientId);
    Serial.print(F(" "));
    Serial.print(mqtt_login);
    Serial.print(F(" "));
    Serial.print(mqtt_pass);
    Serial.print(F(" "));
    if (client.connect(clientId.c_str(), mqtt_login, mqtt_pass)) {  
      Serial.println(F("connected"));

      // Публикация сообщения с идентификаторм клиента в топик, заданный значением 'mqtt_topic_status'
      client.publish(mqtt_topic_status, clientId.c_str());

      // Подписка на сообщения в топике, заданном значением mqtt_topic_in
      client.subscribe(mqtt_topic_lamp);
      // Если нужно подписаться на несколько топиков, то для каждого из них вызываем client.subscribe()
      client.subscribe(mqtt_topic_value);
      client.subscribe(mqtt_topic_mode);

      char buf[256];
      sprintf(buf, "{\"on\":\"false\", \"is_rgb\":\"false\", \"dimmer\":\"100\", \"rgb\":\"#ffcf00\" }");
      client.publish(mqtt_topic_init, buf);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F("try again in 5 seconds"));
      delay(5000);
    }
  }
}
