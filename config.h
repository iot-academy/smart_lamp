// Настройки WiFi
#define wifi_ssid     "WIFI_NETWORK_NAME"
#define wifi_pass     "WIFI_PASSWORD"

// Сервер MQTT
#define mqtt_server   "SERVER"
#define mqtt_port     1883
#define mqtt_login    "MQTT_LOGIN"
#define mqtt_pass     "MQTT_PASSWORD"

// MQTT топики
#define mqtt_topic_base         "summer_eecs/718099ea"
#define mqtt_topic_status       mqtt_topic_base "/status"
#define mqtt_topic_heartbeat    mqtt_topic_base "/heartbeat"
#define mqtt_topic_wave         mqtt_topic_base "/wave"
#define mqtt_topic_lamp         mqtt_topic_base "/lamp"
#define mqtt_topic_init         mqtt_topic_base "/lamp/init"
#define mqtt_topic_value        mqtt_topic_base "/lamp/value"
#define mqtt_topic_mode         mqtt_topic_base "/lamp/mode"

// Период отправки сообщений
#define DELAY_MS                10000
