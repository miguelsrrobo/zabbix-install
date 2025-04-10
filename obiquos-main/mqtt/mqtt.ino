/***************************************************
  Exemplo: Ler MPU6050 (via Adafruit libraries)
  e enviar leituras via MQTT em formato JSON
 ***************************************************/
#include <Wire.h>
#include <ESP8266WiFi.h>    // Se for ESP8266; caso contrário, use <WiFi.h> para ESP32
#include <PubSubClient.h>   // Biblioteca MQTT
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Configurações de WiFi
const char* ssid       = "A535G";
const char* password   = "mlki6803";

// Configurações do MQTT
const char* mqtt_server = "192.168.254.104"; 
const int   mqtt_port   = 1883;
const char* mqtt_topic  = "sensor/acelerometro";

// Criação do cliente WiFi e do cliente MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(100);

  // Inicializa WiFi
  setup_wifi();

  // Configura o servidor MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Inicializa o MPU6050
  if (!mpu.begin()) {
    Serial.println("Falha ao encontrar o MPU6050");
    while (1) { delay(10); }
  }
  Serial.println("MPU6050 encontrado!");

  // Configurações semelhantes ao Código 2
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  Serial.println("MPU6050 configurado e pronto!");
}

void loop() {
  // Verifica se está conectado ao broker; se não, tenta reconectar
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lê dados do sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Monta JSON (usando 'String'; se quiser algo mais robusto, usar ArduinoJson)
  String payload = "{";
  payload += "\"x\":" + String(a.acceleration.x, 2) + ",";
  payload += "\"y\":" + String(a.acceleration.y, 2) + ",";
  payload += "\"z\":" + String(a.acceleration.z, 2) + ",";
  payload += "\"gyro_x\":"  + String(g.gyro.x, 2)        + ",";
  payload += "\"gyro_y\":"  + String(g.gyro.y, 2)        + ",";
  payload += "\"gyro_z\":"  + String(g.gyro.z, 2)        + ",";
  payload += "\"temp\":"    + String(temp.temperature, 2);
  payload += "}";

  // Exibe no Serial o payload que será enviado
  Serial.print("Publicando no tópico ");
  Serial.print(mqtt_topic);
  Serial.print(": ");
  Serial.println(payload);

  // Publica no tópico MQTT
  client.publish(mqtt_topic, payload.c_str());

  // Aguarda meio segundo
  delay(500);
}

//--------------------------------------------------
// Função para conectar no WiFi
//--------------------------------------------------
void setup_wifi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

//--------------------------------------------------
// Função para reconectar ao MQTT quando cair
//--------------------------------------------------
void reconnect() {
  // Enquanto não conectar
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    // Tenta conectar (nome do cliente MQTT pode ser qualquer)
    if (client.connect("MPU6050Client")) {
      Serial.println("conectado!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}
