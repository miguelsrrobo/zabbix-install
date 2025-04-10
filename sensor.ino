#include "ESP8266ZabbixSender.h"
#include <Base64.h>
#include <Wire.h>
#include <INA226_WE.h>   
#define I2C_ADDRESS 0x40

ESP8266ZabbixSender zSender;
INA226_WE ina226 = INA226_WE(I2C_ADDRESS);

/* WiFi settings */
String ssid = "AndroidAP";
String pass = "12345678";

/* Zabbix server setting */
#define SERVERADDR 192, 168, 0, 123 // IP Address example 192.168.0.123
#define ZABBIXPORT 10050			
#define ZABBIXAGHOST "tensao"
#define ZABBIX_KEY "000"





// DS18B20
//OneWire oneWire(2); // GPIO2 or D4 pin on ESP device
//DallasTemperature DS18B20(&oneWire);

// WiFi connectivity checker
boolean checkConnection() {
  Serial.print("Checking Wifi");
  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Wifi connected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("Timed out.");
  return false;
}

void setup() {

  Serial.begin(115200);
  while (!Serial); // wait until serial comes up on Arduino Leonardo or MKR WiFi 1010
  Wire.begin();
  ina226.init();
//  Serial.println();

  // Configure WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid.c_str(), pass.c_str());
  
  // Wait for connectivity
  while (!checkConnection());

  // Initialize Zabbix sender
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST);

  // sensor de corrente e tensão
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

void loop() {
  
  // Get temperature from DS18B20
  //DS18B20.requestTemperatures(); 
  float temp =0;// DS18B20.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temp);

  // Check connectivity
  checkConnection();

  // If temperature is good - send it to Zabbix
  if (! (temp == 85.0 || temp == (-127.0))) {
    zSender.ClearItem();              
    zSender.AddItem(ZABBIX_KEY, temp); 
    if (zSender.Send() == EXIT_SUCCESS) {
      Serial.println("ZABBIX SEND: OK");
    } else {
      Serial.println("ZABBIX SEND: Not Good");
    }
  }

  
  delay(1000); 
}
