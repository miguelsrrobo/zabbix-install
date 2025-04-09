## Visão geral:

Sera construir um sistema de monitoramento de bateria de 12V baseado em IoT usando sensor de corrente CC ESP8266 e INA226. Este sistema é projetado especificamente para monitorar baterias de chumbo-ácido, que são amplamente utilizadas em aplicações automotivas, modulos solares e outras aplicações de alta capacidade. O objetivo principal deste sistema é garantir o desempenho ideal e a longevidade da bateria, evitando sobrecarga ou descarga excessiva, o que pode levar a danos na bateria ou falha do sistema.

Nesta configuração, estamos usando o sensor INA226, um monitor de corrente e energia de alta precisão, para fornecer leituras precisas da tensão, tensão de carga, corrente e energia da bateria. Essas leituras são cruciais para manter a saúde e a eficiência da bateria. O ESP8266, um microchip Wi-Fi de baixo custo com capacidade completa de pilha TCP / IP e microcontrolador, é usado para enviar esses dados para o servidor ThingSpeak.

O servidor ThingSpeak, um aplicativo e API de Internet das Coisas (IoT) de código aberto, é usado para coletar e armazenar dados de sensores na nuvem e desenvolver aplicativos de IoT. Isso permite que os usuários monitorem o status da bateria remotamente de qualquer lugar do mundo através de seus smartphones ou painéis de computadores. O servidor exibe a tensão da bateria, a tensão de carga, a corrente e a energia, fornecendo uma visão abrangente da condição da bateria nos estados de carregamento e descarga.
<p align = "center">
  <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/IoT-Based-12V-Battery-Monitoring-System-with-ESP8266-448x360.png" alt="Rinha logo" width="200%" />
</p>
