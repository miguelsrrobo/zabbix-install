#include "ESP8266ZabbixSender.h"
#include <Base64.h>
#include <Wire.h>
#include <INA226_WE.h>   
#define I2C_ADDRESS 0x40
#include <math.h>
#include <limits.h>
#include <time.h>

// Inputs
float tensao = 0; // Tensão obtida do painel solar
float corrente = 0; // Corrente obtida do painel solar
//float random = 0;

// Outputs
float pwm = 0; // Output Principal
float D = 0.5;// Resultado do algoritimo P&O
int Contador = 0;
float Speed = 0;
int p_atual = 0; // particula atual sendo considerada na simulação

// Definições
#define ExecTime 0.0000001 // Tempo de execução, 1*10^-7 s
#define MAXwave 1 // Valor maximo que a onda triangular vai obter
#define MINwave 0 // Valor minimo que a onda triangular vai obter
#define NUM_INTERACAO 10 // Valor que representa o numero de interações
#define NUM_PARTICULAS 20 // Numero de particulas
#define INERTIA_WEIGHT 0.5 // Peso da inércia
#define MAXD 0.95 // Valor maximo que o D poderá obter
#define MIND 0.05 // Valor minimo que o D poderá obter
#define Ciclo 12500 // Ciclos de analise para cada particula

// Particula //
struct Particula{
	float posicao; // posição da particula
	float velocidade; // velocida da particula
	float best_posicao; // melhor posição da particula
	float best_potencia; // melhor potencia da particula
	float potencia; // potencia da particula
};

struct Particula particula[NUM_PARTICULAS];

// PSO
float potencia = 0; // Tensão x Corrente
float C1 = 0.6; // Peso cognitivo
float C2 = 0.95; // Peso cognitivo

int inic = 0;
float gbestPSO = 0;
float gbestPST = 0;

// TriangularWave
float frequencia = 31250; // Frequência desejada para a onda triangular
float periodo = 0; // 1 / Frequência
float valorWave = 0; // Valor atual da onda triangular
float delta = 0; // Valor a se somar ou subtrair a cada periodo de execução 1 / passos por periodo da onda, versão estatica
float somador = 0; // Valor que usa o delta porém é alterado a cada ciclo da onda
float passos = 0.1; // Valor de passos que irá dar por periodo, Periodo / tempo de execução
int espera = 1; // Variavel que guarda a informação para liberar a inversão do valor delta
int AcumulodeCiclos = 0; // Acumulo de ciclos do sistemas, 
int AcumulodeCiclos2 = 0; // Acumulo de ciclos do sistemas2, 



ESP8266ZabbixSender zSender;
INA226_WE ina226 = INA226_WE(I2C_ADDRESS);

/* WiFi settings */
String ssid = "";
String pass = "";

/* Zabbix server setting */
#define SERVERADDR 127,0,0,1 // IP Address example 192.168.0.123
#define ZABBIXPORT 10050			
#define ZABBIXAGHOST "tensao"
#define ZABBIX_KEY "000"

// DS18B20
//OneWire oneWire(2); // GPIO2 or D4 pin on ESP device
//DallasTemperature DS18B20(&oneWire);

/////////////////////////////////
// Função da onda triangular ///
///////////////////////////////
void TriangularWave()
{   
   if(valorWave >= MAXwave && espera == 1)
   {
   	espera = 0;
   	valorWave = MAXwave;
   	somador = -delta;
   }
   else if(valorWave <= MINwave && espera == 0)
   {
   	espera = 1;
   	valorWave = MINwave;
   	somador = delta;
   }
   
 	valorWave += somador;
}

///////////////////////////////
// Inicializar a Particulas //
/////////////////////////////
void InicializarParticulas()
{
	inic++;
	float multi = ((MAXD - MIND) / NUM_PARTICULAS);
	
	for (int i = 0; i < NUM_PARTICULAS; i++) {
		particula[i].posicao = MIND + (i * multi);
		particula[i].velocidade = 0.00001;
		particula[i].best_posicao = particula[i].posicao;
		particula[i].best_potencia = particula[i].potencia;
	}
}

/////////////////
//Função reset//
///////////////
void reset() {
	if (AcumulodeCiclos2 == 10000000) 
	{
		gbestPSO = 0;
		gbestPST = 0;			
	float multi = ((MAXD - MIND) / NUM_PARTICULAS);
	for (int i = 0; i < NUM_PARTICULAS; i++) 
	{
		particula[i].posicao = MIND + (i * multi);
		particula[i].velocidade = 0.00001;
		particula[i].best_posicao = 0;
		particula[i].best_potencia = 0;
		particula[i].potencia = 0;}
		AcumulodeCiclos2=0;
		p_atual = 0;
		D = 0.5;// Resultado do algoritimo P&O
	} 
}


/////////////////////////////////
// Função Atualizar Paticulas //
///////////////////////////////
void AtualizarPaticulas()
{

	AcumulodeCiclos++;
	AcumulodeCiclos2++;
	reset();
	
	if(AcumulodeCiclos >= Ciclo)
	{
			
		particula[p_atual].potencia = particula[p_atual].potencia / Ciclo; // potencia da particula atua recebe a potencia tual dividida pelo ciclos de analise para cada particula, serve para ajuste fino
		
		AcumulodeCiclos = 0;// zera os ciclo do sistema
		
		// Calculo do PSO //
		if(particula[p_atual].potencia > particula[p_atual].best_potencia) // se a potencia da particula atual for maior ou igual a melhor posição da particula atual faça
		{
			particula[p_atual].best_posicao = particula[p_atual].posicao; // a melhor posção recebe a posição
			particula[p_atual].best_potencia = particula[p_atual].potencia; // a melhor potencia recebe a melhor potencia
		}
		if(particula[p_atual].best_potencia > gbestPST) // se a melhor potencia da da particula for maior que melhor potencia global faça
		{
			gbestPSO = particula[p_atual].best_posicao; // guarda a melhor posição da particula numa vairiavel global
			gbestPST = particula[p_atual].best_potencia; // guarda a melhor potencia da particula numa vairiavel global
		}
				
		//	Calculo da velocidade da particula
		particula[p_atual].velocidade = INERTIA_WEIGHT * particula[p_atual].velocidade + C1 * 0 * (particula[p_atual].best_posicao - particula[p_atual].posicao) + C2 * 1 * (gbestPSO - particula[p_atual].posicao);
		
		//	Calculo da posição da particula
		particula[p_atual].posicao = particula[p_atual].posicao + particula[p_atual].velocidade;
		// Calculo do PSO //

		//	Incremento das particulas
		p_atual++;
		if(p_atual >= NUM_PARTICULAS)
		{
			p_atual = 0;
		}
	}	
	else
	{
		particula[p_atual].potencia += potencia;
	}
	Contador++;
}

/////////////////////
// Função do PSO ///
///////////////////
void PSO()
{
	if(inic == 0)
	{
		InicializarParticulas();	
	}
	else
	{
		 AtualizarPaticulas();
	}
	
	D = particula[p_atual].posicao;

	if(D > MAXD)
	{
		D = MAXD;
	}
	else if(D < MIND)
	{
		D = MIND;
	}
}


///////////////////
// Função Main ///
/////////////////
void IA_MPPT()
{
   TriangularWave();  
	PSO();
	// comparação para output
   if(D >= valorWave)
	{
		pwm = MAXwave; 
	} 
	else 
	{ 	
		pwm = MINwave;	
	}
}

////////////////////////////////
// WiFi connectivity checker///
//////////////////////////////
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

//////////
//Setup//
////////
void setup() {

  Serial.begin(115200);
  while (!Serial); // wait until serial comes up on Arduino Leonardo or MKR WiFi 1010
  Wire.begin();
  ina226.init();
  //Serial.println();

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
  ina226.setResistorRange(0.1, 1.3); // choose resistor 0.1 Ohm and gain range up to 1.3A
 
  /* If the current values delivered by the INA226 differ by a constant factor
     from values obtained with calibrated equipment you can define a correction factor.
     Correction factor = current delivered from calibrated equipment / current delivered by INA226*/
 
  ina226.setCorrectionFactor(0.93);
 
  Serial.println("INA226 Current Sensor Example Sketch - Continuous");
 
  ina226.waitUntilConversionCompleted(); //if you comment this line the first data might be zero

}

//////////////////////////////////
//Função Loop, Função principal//
////////////////////////////////
void loop() {
  
  // Get temperature from DS18B20
  //DS18B20.requestTemperatures(); 
  float temp =0;// DS18B20.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temp);

  // Check connectivity
  checkConnection();

  // If temperature is good - send it to Zabbix
 /* if (! (temp == 85.0 || temp == (-127.0))) {
    zSender.ClearItem();              
    zSender.AddItem(ZABBIX_KEY, temp); 
    if (zSender.Send() == EXIT_SUCCESS) {
      Serial.println("ZABBIX SEND: OK");
    } else {
      Serial.println("ZABBIX SEND: Not Good");
    }
  }*/

  //delay(1000); 

  float shuntVoltage_mV = 0.0;
  float loadVoltage_V = 0.0;
  float busVoltage_V = 0.0;
  float current_mA = 0.0;
  float power_mW = 0.0;
 
  ina226.readAndClearFlags();
  shuntVoltage_mV = ina226.getShuntVoltage_mV();
  busVoltage_V = ina226.getBusVoltage_V();
  current_mA = ina226.getCurrent_mA();
  power_mW = ina226.getBusPower();
  loadVoltage_V  = busVoltage_V + (shuntVoltage_mV / 1000);
 
  zSender.ClearItem();              
  zSender.AddItem(ZABBIX_KEY, power_mW);

  Serial.print("Shunt Voltage [mV]: "); Serial.println(shuntVoltage_mV);
  Serial.print("Bus Voltage [V]: "); Serial.println(busVoltage_V);
  Serial.print("Load Voltage [V]: "); Serial.println(loadVoltage_V);
  Serial.print("Current[mA]: "); Serial.println(current_mA);
  Serial.print("Bus Power [mW]: "); Serial.println(power_mW);

  if (!ina226.overflow)
  {
    Serial.println("Values OK - no overflow");
  }
  else
  {
    Serial.println("Overflow! Choose higher current range");
  }
  Serial.println();
 
  delay(3000);
}

