# Guia de Instalação do Zabbix

## 1. Pré-requisitos

Escolha a sua plataforma de trabalho:

| Banco de Dados | Servidor Web |
|--------------|--------------|
| MySQL       | Apache       |
| PostgreSQL  | Nginx        |

Instale os pacotes necessários conforme sua escolha:

| Ferramenta  | Comando de Instalação |
|------------|-----------------------|
| MySQL      | `apt install mysql-server` |
| PostgreSQL | `apt install postgresql postgresql-contrib` |
| Apache     | `apt install apache2` |
| Nginx      | `apt install nginx` |

Certifique-se de instalar os pacotes antes de iniciar a instalação do Zabbix.

---

## 2. Instalação e Configuração do Zabbix

### 2.1. Instalação do Zabbix com MySQL e Apache

#### a) Acessar como Root

```sh
sudo -s
```

#### b) Adicionar o Repositório do Zabbix

```sh
wget https://repo.zabbix.com/zabbix/7.2/release/ubuntu/pool/main/z/zabbix-release/zabbix-release_latest_7.2+ubuntu24.04_all.deb

dpkg -i zabbix-release_latest_7.2+ubuntu24.04_all.deb

apt update
```

#### c) Instalar Pacotes do Zabbix

```sh
apt install zabbix-server-mysql zabbix-frontend-php zabbix-apache-conf zabbix-sql-scripts zabbix-agent
```

#### d) Configurar o Banco de Dados MySQL
Obs.: ao configurar o banco de dados, seja MySQL ou PostgreSQL, em alguns computadores pode demorar muito (alguns até 1h!) então se você achar que seu terminal travou, NÃO FAÇA NADA, se cancelar essa parte poderá dar problemas ao tentar de novo.

```sh
mysql -uroot -p
```
Digite a senha do root do MySQL e execute:

```sql
mysql> create database zabbix character set utf8mb4 collate utf8mb4_bin;
mysql> create user zabbix@localhost identified by 'password';
mysql> grant all privileges on zabbix.* to zabbix@localhost;
mysql> set global log_bin_trust_function_creators = 1;
mysql> quit; 
```

Importar esquema inicial:

```sh
zcat /usr/share/zabbix/sql-scripts/mysql/server.sql.gz | mysql --default-character-set=utf8mb4 -uzabbix -p zabbix
```

Desabilitar `log_bin_trust_function_creators`:

```sh
mysql -uroot -p
SET GLOBAL log_bin_trust_function_creators = 0;
QUIT;
```

#### e) Configurar o Zabbix Server

Edite o arquivo:

```sh
nano /etc/zabbix/zabbix_server.conf
```

Ajuste a linha:

```sh
DBPassword=senha_segura
```

#### f) Reiniciar e Habilitar os Serviços

```sh
systemctl restart zabbix-server zabbix-agent apache2
systemctl enable zabbix-server zabbix-agent apache2
```

#### g) Acessar a Interface Web

```
http://host/zabbix
```

---

### 2.2. Instalação do Zabbix com PostgreSQL e Apache

Repita os passos `a` e `b`.

#### c) Instalar Pacotes

```sh
apt install zabbix-server-pgsql zabbix-frontend-php php8.3-pgsql zabbix-apache-conf zabbix-sql-scripts zabbix-agent
```

#### d) Configurar o Banco de Dados PostgreSQL
Obs.: ao configurar o banco de dados, seja MySQL ou PostgreSQL, em alguns computadores pode demorar muito (alguns até 1h!) então se você achar que seu terminal travou, NÃO FAÇA NADA, se cancelar essa parte poderá dar problemas ao tentar de novo.

```sh
sudo -u postgres createuser --pwprompt zabbix
sudo -u postgres createdb -O zabbix zabbix
```

Importar esquema inicial:

```sh
zcat /usr/share/zabbix/sql-scripts/postgresql/server.sql.gz | sudo -u zabbix psql zabbix
```

#### e) Configurar o Zabbix Server

Edite o arquivo:

```sh
nano /etc/zabbix/zabbix_server.conf
```

Ajuste a linha:

```sh
DBPassword=senha_segura
```

#### f) Reiniciar e Habilitar os Serviços

```sh
systemctl restart zabbix-server zabbix-agent apache2
systemctl enable zabbix-server zabbix-agent apache2
```

#### g) Acessar a Interface Web

```
http://host/zabbix
```

---

### 2.3. Instalação do Zabbix com MySQL e Nginx

Siga os passos `a` e `b`, depois:

#### c) Instalar Pacotes

```sh
apt install zabbix-server-mysql zabbix-frontend-php zabbix-nginx-conf zabbix-sql-scripts zabbix-agent
```

#### d) Configurar Banco de Dados e Zabbix (mesmos passos do MySQL acima)

#### e) Configurar o Nginx

Edite:

```sh
nano /etc/zabbix/nginx.conf
```

Ajuste:

```sh
listen 8080;
server_name seu_dominio.com;
```

#### f) Reiniciar e Habilitar os Serviços

```sh
systemctl restart zabbix-server zabbix-agent nginx php8.3-fpm
systemctl enable zabbix-server zabbix-agent nginx php8.3-fpm
```

#### g) Acessar a Interface Web

```
http://host/zabbix
```

---

### 2.4. Instalação do Zabbix com PostgreSQL e Nginx

Repita os passos `a` e `b`, depois:

#### c) Instalar Pacotes

```sh
apt install zabbix-server-pgsql zabbix-frontend-php php8.3-pgsql zabbix-nginx-conf zabbix-sql-scripts zabbix-agent
```

#### d) Configurar Banco de Dados e Zabbix (mesmos passos do PostgreSQL acima)

#### e) Configurar o Nginx (mesmos passos acima)

#### f) Reiniciar e Habilitar os Serviços

```sh
systemctl restart zabbix-server zabbix-agent nginx php8.3-fpm
systemctl enable zabbix-server zabbix-agent nginx php8.3-fpm
```

#### g) Acessar a Interface Web

```
http://host/zabbix
```
---
## 3 Comece a usar o Zabbix

Leia em documentação: [Guia de início rápido](https://www.zabbix.com/documentation/7.2/en/manual/quickstart/login)

---
## 4 Para wsl

toda a instalação permanecerá a mesma conforme acima, mas o acesso a interface web muda:
```
http://IP/zabbix
```
Para consegui o IP da maquina linux no WSL, usase este comando:

```
ip addr show eth0 | grap "inet "
```
---
## 5 ESP8266 Zabbix Sender
Biblioteca para realizar o zabbix-sender em ESP8266-Arduino [ESP8266ZabbixSender](https://github.com/zaphodus/ESP8266ZabbixSender/tree/master)

---

## 6 ESP8266 Zabbix Sensor de temperatura
Repositorio para realizar a conexão do Zabbix com o ESP8266 e um sensor de termperatura [ESP8266 Zabbix Temperature sensor](https://github.com/EimantasRebzdys/ESP_zabbix_temperature/tree/master)

## 7 Conexão com MQTT e MUSQUITTO com ESP

[obiquos](https://github.com/RegisBloemer/obiquos)
---

## Conclusão

Agora você tem o Zabbix instalado e configurado com diferentes combinações de banco de dados e servidores web. Acesse a interface web para finalizar a configuração e comece a monitorar seus dispositivos!

---

## Referências

Para mais detalhes sobre a instalação do Zabbix, consulte a documentação oficial: [Zabbix Download](https://www.zabbix.com/download?zabbix=7.2&os_distribution=ubuntu&os_version=24.04&components=server_frontend_agent&db=mysql&ws=apache)
ou [Zabbix manuals](https://www.zabbix.com/manuals)

Assista a palestra completa de Marcilio Ramos, Administrador de Sistemas da Hostdime Brasil, na Zabbix Conference Brazil 2022,[Zabbix Conference Brazil 2022 | Palestra Marcilio Ramos](https://www.youtube.com/watch?v=bYX9So4B1HY) slide da conferencia [conference_brazil_2022](https://assets.zabbix.com/files/events/2022/conference_brazil_2022/9MarcilioRamos.pdf)

