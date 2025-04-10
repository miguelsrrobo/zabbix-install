#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <MPU6050.h>

// Configurações de rede Wi‑Fi
const char* ssid = "A535G";       // Nome da sua rede Wi‑Fi
const char* password = "mlki6803";  // Senha da rede

// Configuração do broker MQTT
const char* mqtt_server = "192.168.254.104";  // IP do broker MQTT (pode ser o Mosquitto ou o Node‑RED se configurado como broker)

WiFiClient espClient;
PubSubClient client(espClient);

// Instancia o objeto MPU6050
MPU6050 mpu;

// Função para conectar ao Wi‑Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Função para reconectar ao MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" - Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Inicializa a comunicação com o sensor MPU6050
  Wire.begin();
  mpu.initialize();
  
  // Verifica se o sensor está conectado corretamente
  if (!mpu.testConnection()) {
    Serial.println("Erro ao conectar com o MPU6050");
    while (1);  // Trava o código se o sensor não for encontrado
  }
  Serial.println("MPU6050 conectado com sucesso");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lê os valores de aceleração (em g)
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Converte os valores de aceleração para valores em g (com 1g = 16384 no MPU6050)
  float x = ax / 16384.0;
  float y = ay / 16384.0;
  float z = az / 16384.0;

  // Cria uma mensagem JSON com as chaves "x", "y" e "z"
  String payload = "{\"x\": " + String(x, 3) + ", \"y\": " + String(y, 3) + ", \"z\": " + String(z, 3) + "}";
  Serial.println("Enviando: " + payload);

  // Publica a mensagem no tópico "sensor/acelerometro"
  client.publish("sensor/acelerometro", payload.c_str());
  delay(5000);  // Aguarda 5 segundos para enviar novamente
}
