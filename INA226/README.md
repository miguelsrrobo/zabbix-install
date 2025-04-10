# Sistema de monitoramento de bateria 12V baseado em IoT com ESP8266
  Sera construir um sistema de monitoramento de bateria de 12V baseado em IoT usando sensor de corrente CC ESP8266 e INA226. Este sistema é projetado especificamente para monitorar baterias de chumbo-ácido, que são amplamente utilizadas em aplicações automotivas, modulos solares e outras aplicações de alta capacidade. O objetivo principal deste sistema é garantir o desempenho ideal e a longevidade da bateria, evitando sobrecarga ou descarga excessiva, o que pode levar a danos na bateria ou falha do sistema.

  Nesta configuração, estamos usando o sensor INA226, um monitor de corrente e energia de alta precisão, para fornecer leituras precisas da tensão, tensão de carga, corrente e energia da bateria. Essas leituras são cruciais para manter a saúde e a eficiência da bateria. O ESP8266, um microchip Wi-Fi de baixo custo com capacidade completa de pilha TCP / IP e microcontrolador, é usado para enviar esses dados para o servidor ThingSpeak.

  O servidor ThingSpeak, um aplicativo e API de Internet das Coisas (IoT) de código aberto, é usado para coletar e armazenar dados de sensores na nuvem e desenvolver aplicativos de IoT. Isso permite que os usuários monitorem o status da bateria remotamente de qualquer lugar do mundo através de seus smartphones ou painéis de computadores. O servidor exibe a tensão da bateria, a tensão de carga, a corrente e a energia, fornecendo uma visão abrangente da condição da bateria nos estados de carregamento e descarga.

---
  
## Descrição do INA226 :shipit:

O INA226 é um monitor de corrente por shunt e potência com uma interface compatível com I2C™ ou SMBUS.
O dispositivo monitora tanto a queda de tensão no shunt quanto a tensão da linha de alimentação (bus).

Um valor de calibração programável, tempos de conversão e média (averaging), combinados com um multiplicador interno, permitem leituras diretas de corrente em amperes e potência em watts.

O INA226 mede a corrente em tensões de barramento (common-mode) que podem variar de 0V a 36V, independentemente da tensão de alimentação.
O dispositivo opera com uma única fonte de 2,7V a 5,5V, consumindo corrente típica de 330μA.

Ele é especificado para funcionar em uma faixa de temperatura de operação entre –40°C e 125°C e conta com até 16 endereços programáveis na interface compatível com I2C.

<p align = "center">
  <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/Screenshot%202025-04-09%20at%2010-15-57%20INA226%2036V%2016-Bit%20Ultra-Precise%20I2C%20Output%20Current%20Voltage%20and%20Power%20Monitor%20With%20Alert%20datasheet%20(Rev.%20B)%20-%20ina226.pdf.png" alt="Rinha logo" width="90%" />
</p>

---

## Circuito & esquemático da placa INA226

<p align = "center">
  <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/INA226-board.jpg" alt="Rinha logo" width="50%" />
</p>

O módulo INA226 é construído com um chip INA226, alguns resistores e um capacitor que ajuda a reduzir o ruído ou sinais elétricos indesejados.

<p align = "center">
 <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/INA226-board-sch-546x360.jpg" alt="Rinha logo" width="50%" />
</p>

---

## Características & especificações do INA226

- Tensão Operacional (2,7 – 5,5 Volts): O INA226 opera entre 2,7 a 5,5 volts. Isso amplia sua compatibilidade com sistemas que funcionam em diferentes níveis de tensão. Esta característica é benéfica para acomodar uma extensa gama de projetos de circuitos.
- Gama de tensão de ônibus (0 – 36 volts): O INA226 pode monitorar fontes de alimentação de até 36 volts, expandindo a aplicabilidade do dispositivo a várias fontes de alimentação.
- Faixa de detecção atual (500mA a 50A): Dependendo do valor do resistor de derivação, o INA226 pode monitorar uma ampla gama de correntes, atendendo a uma infinidade de aplicações, incluindo gerenciamento de energia, carregadores de bateria e controle de motor DC.
- Consumo de potência: Modo contínuo: 0,35 mA; Modo de parada de energia: 2.3.
- Modos de medição: contínuos ou sob demanda (“desencadeados”)
- Média de 1, 4, 64, 128, 256, 512 ou 1024 medições individuais
- Tempo de conversão A/D ajustável em oito níveis: 0,14 a 8,2 ms
- Precisão mais alta: O INA226 fornece maior precisão através de seu ADC de 16 bits, resultando em medições mais precisas.
- Pin de alarme programável para violações de limite e dados disponíveis

---

## Pinos do INA226
O módulo de sensor INA226 normalmente tem 8 pinos, que são os seguintes:

<p align = "center">
<img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/Screenshot%202025-04-09%20at%2010-16-41%20INA226%2036V%2016-Bit%20Ultra-Precise%20I2C%20Output%20Current%20Voltage%20and%20Power%20Monitor%20With%20Alert%20datasheet%20(Rev.%20B)%20-%20ina226.pdf.png" width="90%"/>
</p>

Serão utilizados no projeto somentre estes pinos:
- VS: Aceita uma tensão de entrada de 2.7V a 5.5V.
- GND: Pão de aterramento, conectado ao solo da fonte de alimentação.
- SDA: Linha de dados de série para a interface I2C. É usado para a transferência bidirecional de dados.
- SCL: Linha de Relógio de Série para a interface I2C. É usado para sincronização durante a transferência de dados.
- ALE(ALERT): Este é o pino de alerta. É uma saída de drenagem aberta que requer um resistor pull-up. Este pino pode ser usado para vários alertas ou como um sinal de ----conversão-preensível.
- VBUS(VBS): Este pino é usado para medir a tensão de alimentação. Pode medir a tensão de alimentação até 36V.
- IN-: Este pino conecta-se à carga. É aqui que o resistor de shunt é colocado para detecção atual.
- IN+: Este pino se conecta à Fonte de Alimentação.

Alem destes pinos, existe o A0 e A1 que não serão utilizados.

---

## Como usar o sensor de corrente INA226 DC com ESP8266
Agora vamos interagir com o Módulo de Sensor de Corrente DC INA226 com ESP8266. A interface de hardware é muito simples.

Aqui está um diagrama de conexão simples que pode ser usado para conectar o sensor de corrente INA226 com o ESP8266, juntamente com a carga(LOAD) externa e a fonte de energia.

<p align = "center">
  <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/IoT-Based-12V-Battery-Monitoring-System-with-ESP8266-448x360.png" alt="Rinha logo" width="50%" />
</p>
Os pinos SDA e SCL do INA226 estão conectados aos pinos D1 e D2 do ESP8266. O pino In+ deve ser conectado a uma fonte de energia e o In- à carga, como mostrado no esquema. O sensor INA226 possui Pino VBus, que é usado para medir a tensão de carga. Portanto, precisamos unir os o pinos VBus e In-, o pino D3 esta conectado ao pino ALE.

---

## Biblioteca INA226 Arduino

---

## Código fonte / Programa

Copie o código a seguir e cole-o na janela do editor Arduino IDE. Antes de fazer o upload do código, talvez seja necessário fazer algumas modificações.

```
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <INA226_WE.h>
#define I2C_ADDRESS 0x40
 
String apiKey = "**********";
const char* ssid = "**********";          // Enter your WiFi Network's SSID
const char* pass = "**********";  // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";
 
INA226_WE ina226 = INA226_WE(I2C_ADDRESS);
 
WiFiClient client;
 
void setup()
{
  Serial.begin(115200);
  while (!Serial); // wait until serial comes up on Arduino Leonardo or MKR WiFi 1010
  Wire.begin();
  ina226.init();
 
  /* Set Resistor and Current Range
     if resistor is 5.0 mOhm, current range is up to 10.0 A
     default is 100 mOhm and about 1.3 A*/
 
  ina226.setResistorRange(0.1, 1.3); // choose resistor 0.1 Ohm and gain range up to 1.3A
 
  /* If the current values delivered by the INA226 differ by a constant factor
     from values obtained with calibrated equipment you can define a correction factor.
     Correction factor = current delivered from calibrated equipment / current delivered by INA226*/
 
  ina226.setCorrectionFactor(0.93);
 
  Serial.println("INA226 Current Sensor Example Sketch - Continuous");
 
  ina226.waitUntilConversionCompleted(); //if you comment this line the first data might be zero
 
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("*");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
 
void loop()
{
  float shuntVoltage_mV = 0.0;
  float loadVoltage_V = 0.0;
  float batteryVoltage_V = 0.0;
  float current_mA = 0.0;
  float power_mW = 0.0;
 
  ina226.readAndClearFlags();
  shuntVoltage_mV = ina226.getShuntVoltage_mV();
  batteryVoltage_V = ina226.getBusVoltage_V();
  current_mA = ina226.getCurrent_mA();
  power_mW = ina226.getBusPower();
  loadVoltage_V  = batteryVoltage_V + (shuntVoltage_mV / 1000);
 
 
  Serial.print("Battery Voltage");
  Serial.print(batteryVoltage_V);
  Serial.println("V");
 
  Serial.print("Load Voltage: ");
  Serial.print(loadVoltage_V);
  Serial.println("V");
 
  Serial.print("Current: ");
  Serial.print(current_mA);
  Serial.println("mA");
 
  Serial.print("Power: ");
  Serial.print(power_mW);
  Serial.println("mW");
 
  if (!ina226.overflow)
  {
    Serial.println("Values OK - no overflow");
  }
  else
  {
    Serial.println("Overflow! Choose higher current range");
  }
 
  if (client.connect(server, 80)) {
 
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(batteryVoltage_V);
    postStr += "&field2=";
    postStr += String(loadVoltage_V);
    postStr += "&field3=";
    postStr += String(current_mA);
    postStr += "&field4=";
    postStr += String(power_mW);
    postStr += "\r\n\r\n\r\n\r\n";
 
    client.print("POST /update HTTP/1.1\n");
    delay(100);
    client.print("Host: api.thingspeak.com\n");
    delay(100);
    client.print("Connection: close\n");
    delay(100);
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    delay(100);
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    delay(100);
    client.print("Content-Length: ");
    delay(100);
    client.print(postStr.length());
    delay(100);
    client.print("\n\n\n\n");
    delay(100);
    client.print(postStr);
    delay(100);
  }
  client.stop();
  Serial.println("Sent Successfully :)");
  Serial.println();
  delay(3000);
}

```
---
## Monitor + Disply OLED turotrial com Arduino nano
Este vídeo demonstra como programar um módulo de monitoramento de energia INA226 e um display OLED utilizando Arduino. O tutorial aborda o cabeamento, a instalação de bibliotecas e a escrita de código para exibir leituras de corrente, tensão e potência. O apresentador fornece uma explicação clara do processo e inclui uma demonstração para ilustrar a funcionalidade.
[INA226 Power Monitor + OLED Display - Arduino Tutorial ](https://www.youtube.com/watch?v=BscbVAANlDA)

---

## Adafruit INA219 Sensor de Corrente e Tensão
A placa de expansão INA219B e a FeatherWing INA219 resolverão todos os seus problemas de monitoramento de energia. Em vez de lutar com dois multímetros, você pode usar esta placa para medir tanto a tensão no lado alto quanto o consumo de corrente contínua via I2C com 1% de precisão. 
[Adafruit INA219 Current Sensor Breakout](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ina219-current-sensor-breakout.pdf)

---

## INA219 Tutorial for Arduino, ESP8266 and ESP32

[INA219 Tutorial for Arduino, ESP8266 and ESP32](https://diyi0t.com/ina219-tutorial-for-arduino-and-esp/)

## Referencias
[Zabbix Download](https://www.ti.com/lit/ds/symlink/ina226.pdf)

[IoT Based 12V Battery Monitoring System with ESP8266](https://how2electronics.com/iot-based-12v-battery-monitoring-system-with-esp8266/#google_vignette)

[How to use INA226 DC Current Sensor with Arduino](https://how2electronics.com/how-to-use-ina226-dc-current-sensor-with-arduino/#google_vignette)

[Solar power monitor w INA226 and ESP8266 V1](https://forum.arduino.cc/t/solar-power-monitor-w-ina226-and-esp8266-v1/649695)

[Ina226 voltage and current sensor with esp8266 sending by MQTT](https://1technophile.blogspot.com/2017/03/ina226-voltage-and-current-sensor-mqtt.html)
