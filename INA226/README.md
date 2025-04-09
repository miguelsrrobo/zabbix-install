## Visão geral:

Sera construir um sistema de monitoramento de bateria de 12V baseado em IoT usando sensor de corrente CC ESP8266 e INA226. Este sistema é projetado especificamente para monitorar baterias de chumbo-ácido, que são amplamente utilizadas em aplicações automotivas, modulos solares e outras aplicações de alta capacidade. O objetivo principal deste sistema é garantir o desempenho ideal e a longevidade da bateria, evitando sobrecarga ou descarga excessiva, o que pode levar a danos na bateria ou falha do sistema.

Nesta configuração, estamos usando o sensor INA226, um monitor de corrente e energia de alta precisão, para fornecer leituras precisas da tensão, tensão de carga, corrente e energia da bateria. Essas leituras são cruciais para manter a saúde e a eficiência da bateria. O ESP8266, um microchip Wi-Fi de baixo custo com capacidade completa de pilha TCP / IP e microcontrolador, é usado para enviar esses dados para o servidor ThingSpeak.

O servidor ThingSpeak, um aplicativo e API de Internet das Coisas (IoT) de código aberto, é usado para coletar e armazenar dados de sensores na nuvem e desenvolver aplicativos de IoT. Isso permite que os usuários monitorem o status da bateria remotamente de qualquer lugar do mundo através de seus smartphones ou painéis de computadores. O servidor exibe a tensão da bateria, a tensão de carga, a corrente e a energia, fornecendo uma visão abrangente da condição da bateria nos estados de carregamento e descarga.
<p align = "center">
  <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/INA226-board.jpg" alt="Rinha logo" width="50%" />
</p>
O módulo INA226 é construído com um chip INA226, alguns resistores e um capacitor que ajuda a reduzir o ruído ou sinais elétricos indesejados.

<p align = "center">
  <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/IoT-Based-12V-Battery-Monitoring-System-with-ESP8266-448x360.png" alt="Rinha logo" width="50%" />
</p>
<p align = "center">
  <img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/Screenshot%202025-04-09%20at%2010-15-57%20INA226%2036V%2016-Bit%20Ultra-Precise%20I2C%20Output%20Current%20Voltage%20and%20Power%20Monitor%20With%20Alert%20datasheet%20(Rev.%20B)%20-%20ina226.pdf.png" alt="Rinha logo" width="50%" />
</p>
<p align = "center">
<img src="https://github.com/miguelsrrobo/zabbix-install/blob/main/INA226/Screenshot%202025-04-09%20at%2010-16-41%20INA226%2036V%2016-Bit%20Ultra-Precise%20I2C%20Output%20Current%20Voltage%20and%20Power%20Monitor%20With%20Alert%20datasheet%20(Rev.%20B)%20-%20ina226.pdf.png" width="50%"/>
</p>
