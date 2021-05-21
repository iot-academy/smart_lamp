#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

WiFiClient espClient;
PubSubClient client(espClient);
String clientId = String(ESP.getChipId());

int  delayMS = DELAY_MS;  // Задаержка в мс между публикацией сообщений
long lastMsg = 0;         // Время публикации предыдущегосообщения  (мс) 
int  heart = 0;           // Переменная для формирования публикуемого сообщения
int  brightness = 255;    // Яркость свечения


boolean wrong_mode = false;
boolean switch_on_state = false;

// Функция обработки входящих сообщений
void callback(char* topic, byte* payload, unsigned int length) {
  // Печать информации о полученном сообщенийй
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Если получено сообщение и у нас подписка на несколько топиков, то определяем в каком топике сообщение опубликовано
  if (strcmp(topic, mqtt_topic_lamp) == 0) {
    // Определяем поведение MCU при различных значениях сообщения (payload) этого топика
    if (strncmp((char*)payload, "on", length) == NULL) {
      switch_on_state = true;
      if (!wrong_mode)
        analogWrite(BUILTIN_LED, brightness);
    }
    if (strncmp((char*)payload, "off", length) == NULL) {
      switch_on_state = false;
      if (!wrong_mode)
        digitalWrite(BUILTIN_LED, HIGH);
    }
  }
  if (strcmp(topic, mqtt_topic_value) == 0) {
    // Определяем поведение MCU при различных значениях сообщения (payload) этого топика
    payload[length] = '\0';

    long percent = strtol((char*)payload, NULL, 10);
    if (isdigit(payload[0])) { // Меняем значение только если как минимум один элемент (первый) в payload - это цифра
      brightness = 255 - int(255 * float(constrain(percent, 0, 100)/100.0));
    }
    if (!wrong_mode && switch_on_state) {
      analogWrite(BUILTIN_LED, brightness);
    }
  }
  if (strcmp(topic, mqtt_topic_mode) == 0) {
    if (strncmp((char*)payload, "dimmer", length) == NULL) {
      wrong_mode = false;
      delayMS = DELAY_MS;
    } else {
      wrong_mode = true;
      delayMS = 100;
      digitalWrite(BUILTIN_LED, HIGH);
    }
  }
}

// Функция настройки MCU
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Установка BUILTIN_LED как порт вывода
  digitalWrite(BUILTIN_LED, HIGH);  // BUILTIN_LED имеет подтягивающий резистор, HIGH = OFF, LOW = ON
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// Основная функция - вызывается на каждой итерации цикла работы MCU
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Публикация сообщения с заданной периодичностью
  long now = millis();
  if (now - lastMsg > delayMS) {
    lastMsg = now;
    if (!wrong_mode) {
      // Формирование сообщения и его публикация 
      char msg[200];
      snprintf (msg, sizeof(msg), "heartbeat #%ld", ++heart);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish(mqtt_topic_heartbeat, msg);
      // Формируем сообщение для графика
      snprintf (msg, sizeof(msg), "%7.3f", 10*sin(0.09*heart)+13*sin(0.3*heart));
      client.publish(mqtt_topic_wave, msg);
    } else {
      digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
    }
  }
}
