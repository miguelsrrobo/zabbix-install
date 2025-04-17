/***************************************************
  Leitura do INA226 e envio via MQTT em formato JSON
 ***************************************************/
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <INA226_WE.h>

#define I2C_ADDRESS 0x40
INA226_WE ina226 = INA226_WE(I2C_ADDRESS);

// Configurações de WiFi
const char* ssid       = "Nelson 2G";
const char* password   = "nelson180599";

// Configurações do MQTT
const char* mqtt_server = "127.0.0.1"; 
const int   mqtt_port   = 10050;
const char* mqtt_topic  = "sensor/potencia";

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

  // Inicializa o sensor INA226
  Wire.begin();
  ina226.init();
  ina226.setResistorRange(0.1, 1.3); // resistor de 0.1 Ohm, até 1.3A
  ina226.setCorrectionFactor(0.93);
  ina226.waitUntilConversionCompleted();

  Serial.println("INA226 configurado!");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura do sensor INA226
  ina226.readAndClearFlags();
  float busVoltage_V = ina226.getBusVoltage_V();
  float current_mA   = ina226.getCurrent_mA();
  float power_mW     = ina226.getBusPower();

  // Monta JSON com os três parâmetros
  String payload = "{";
  payload += "\"busVoltage_V\":" + String(busVoltage_V, 3) + ",";
  payload += "\"current_mA\":"   + String(current_mA, 2)   + ",";
  payload += "\"power_mW\":"     + String(power_mW, 2);
  payload += "}";

  Serial.print("Publicando no tópico ");
  Serial.print(mqtt_topic);
  Serial.print(": ");
  Serial.println(payload);

  // Publica no tópico MQTT
  client.publish(mqtt_topic, payload.c_str());

  delay(3000); // Aguarda 3 segundos
}

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
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("INA226Client")) {
      Serial.println("conectado!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}
