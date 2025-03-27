# Get Zabbix

## 1. Pré-requisitos para o Zabbix

Certifique-se de atender a todos os pré-requisitos antes de prosseguir com a instalação.
|Database| Web Server|
|--------|--------|
|MySQL|Apache|
|PostgreSQL|Nginx|

MySQL : apt install mysql-server

postgreSQL : apt install postgresql postgresql-contrib

Apache : apt install apache2

nginx : apt install nginx

## 2. Instale e configure o Zabbix para database MySQL com Web Server Apache

### a. Torne-se um usuário root

Inicie uma nova sessão de shell com privilégios de root:

```sh
sudo -s
```

### b. Instalar o repositório do Zabbix

```sh
wget https://repo.zabbix.com/zabbix/7.2/release/ubuntu/pool/main/z/zabbix-release/zabbix-release_latest_7.2+ubuntu24.04_all.deb

dpkg -i zabbix-release_latest_7.2+ubuntu24.04_all.deb

apt update
```

### c. Instalar o Zabbix Server, Frontend e Agente

```sh
apt install zabbix-server-mysql zabbix-frontend-php zabbix-apache-conf zabbix-sql-scripts zabbix-agent
```

### d. Configurar o banco de dados MySQL para o Zabbix

```sh
mysql -uroot -p
```

Digite a senha do root do MySQL e execute os seguintes comandos:

```sql
CREATE DATABASE zabbix CHARACTER SET utf8mb4 COLLATE utf8mb4_bin;
CREATE USER zabbix@localhost IDENTIFIED BY 'password';
GRANT ALL PRIVILEGES ON zabbix.* TO zabbix@localhost;
SET GLOBAL log_bin_trust_function_creators = 1;
QUIT;
```

No host do servidor Zabbix importe esquema e dados iniciais. Você será solicitado a digitar sua senha recém-criada.

```sh
zcat /usr/share/zabbix/sql-scripts/mysql/server.sql.gz | mysql --default-character-set=utf8mb4 -uzabbix -p zabbix
```
Desabilite a opção log_bin_trust_function_creators após importar esquema de banco de dados.

```sh
mysql -uroot -p
```

Digite a senha do root do MySQL e execute:

```sql
SET GLOBAL log_bin_trust_function_creators = 0;
QUIT;
```

### e. Configurar a senha do banco de dados no Zabbix Server

Edite o arquivo de configuração do Zabbix Server:

```sh
nano /etc/zabbix/zabbix_server.conf
```

Encontre a linha `DBPassword=` e defina:

```sh
DBPassword=password
```

### f. Reiniciar e habilitar os serviços do Zabbix

```sh
systemctl restart zabbix-server zabbix-agent apache2
systemctl enable zabbix-server zabbix-agent apache2
```

### g. Acessar a interface web do Zabbix

Abra um navegador e acesse a interface web do Zabbix:

```
http://host/zabbix
```
Substitua `host` pelo endereço IP ou hostname do seu servidor.

## 3 Instale e configure o Zabbix para database PostgreSQL com Web Server Apache

### c. Instalar o Servidor, Frontend e Agente do Zabbix

```sh
apt install zabbix-server-pgsql zabbix-frontend-php php8.3-pgsql zabbix-apache-conf zabbix-sql-scripts zabbix-agent
```

### d. Criar o Banco de Dados Inicial

Certifique-se de que você tem um servidor de banco de dados em execução. Execute o seguinte comando no host do banco de dados:

```sh
sudo -u postgres createuser --pwprompt zabbix
sudo -u postgres createdb -O zabbix zabbix
```

No host do servidor Zabbix, importe o esquema inicial e os dados. Você será solicitado a inserir a senha recém-criada:

```sh
zcat /usr/share/zabbix/sql-scripts/postgresql/server.sql.gz | sudo -u zabbix psql zabbix
```

### e. Configurar o Banco de Dados para o Servidor Zabbix

Edite o arquivo de configuração:

```sh
nano /etc/zabbix/zabbix_server.conf
```

Encontre a linha que contém `DBPassword=` e defina:

```sh
DBPassword=password
```

### f. Iniciar os Processos do Servidor e Agente do Zabbix

Inicie os processos do servidor e agente do Zabbix e habilite-os para iniciar na inicialização do sistema:

```sh
systemctl restart zabbix-server zabbix-agent apache2
systemctl enable zabbix-server zabbix-agent apache2
```

### g. Abrir a Página Web da Interface do Zabbix

A URL padrão para a interface web do Zabbix ao usar o servidor Apache é:

```
http://host/zabbix
```

Substitua `host` pelo endereço IP ou nome do host do seu servidor.

## 4 Instale e configure o Zabbix para database MySQL com Web Server Nginx


### c. Instalar o Servidor, Frontend e Agente do Zabbix

```sh
apt install zabbix-server-mysql zabbix-frontend-php zabbix-nginx-conf zabbix-sql-scripts zabbix-agent
```

### d. Criar o Banco de Dados Inicial

Certifique-se de que você tem um servidor de banco de dados em execução. Execute o seguinte comando no host do banco de dados:

```sh
mysql -uroot -p
```
Digite a senha do root do MySQL e execute os seguintes comandos:

```sql
create database zabbix character set utf8mb4 collate utf8mb4_bin;
create user zabbix@localhost identified by 'password';
grant all privileges on zabbix.* to zabbix@localhost;
set global log_bin_trust_function_creators = 1;
quit;
```

No host do servidor Zabbix, importe o esquema inicial e os dados. Você será solicitado a inserir a senha recém-criada:

```sh
zcat /usr/share/zabbix/sql-scripts/mysql/server.sql.gz | mysql --default-character-set=utf8mb4 -uzabbix -p zabbix
```

Desative a opção `log_bin_trust_function_creators` após importar o esquema do banco de dados:

```sh
mysql -uroot -p
```
Digite a senha do root do MySQL e execute:

```sql
set global log_bin_trust_function_creators = 0;
quit;
```

### e. Configurar o Banco de Dados para o Servidor Zabbix

Edite o arquivo de configuração:

```sh
nano /etc/zabbix/zabbix_server.conf
```

Encontre a linha que contém `DBPassword=` e defina:

```sh
DBPassword=password
```

### f. Configurar o PHP para o Frontend do Zabbix

Edite o arquivo `/etc/zabbix/nginx.conf`, descomente e configure as diretivas `listen` e `server_name`:

```sh
listen 8080;
server_name example.com;
```

### g. Iniciar os Processos do Servidor e Agente do Zabbix

Inicie os processos do servidor e agente do Zabbix e habilite-os para iniciar na inicialização do sistema:

```sh
systemctl restart zabbix-server zabbix-agent nginx php8.3-fpm
systemctl enable zabbix-server zabbix-agent nginx php8.3-fpm
```

### h. Abrir a Página Web da Interface do Zabbix

A URL da interface web do Zabbix ao usar o Nginx depende das configurações feitas no `nginx.conf`. Acesse:

```
http://example.com:8080/zabbix
```

Substitua `example.com` pelo endereço IP ou nome do host do seu servidor.

## 5 Instale e configure o Zabbix para database PostgreSQL com Web Server Nginx

### c. Instalar o Servidor, Frontend e Agente do Zabbix

```sh
apt install zabbix-server-pgsql zabbix-frontend-php php8.3-pgsql zabbix-nginx-conf zabbix-sql-scripts zabbix-agent
```

### d. Criar o Banco de Dados Inicial

Certifique-se de que você tem um servidor de banco de dados PostgreSQL em execução. Execute os seguintes comandos no host do banco de dados:

```sh
sudo -u postgres createuser --pwprompt zabbix
sudo -u postgres createdb -O zabbix zabbix
```

No host do servidor Zabbix, importe o esquema inicial e os dados. Você será solicitado a inserir a senha recém-criada:

```sh
zcat /usr/share/zabbix/sql-scripts/postgresql/server.sql.gz | sudo -u zabbix psql zabbix
```

### e. Configurar o Banco de Dados para o Servidor Zabbix

Edite o arquivo de configuração:

```sh
nano /etc/zabbix/zabbix_server.conf
```

Encontre a linha que contém `DBPassword=` e defina:

```sh
DBPassword=password
```

### f. Configurar o PHP para o Frontend do Zabbix

Edite o arquivo `/etc/zabbix/nginx.conf`, descomente e configure as diretivas `listen` e `server_name`:

```sh
listen 8080;
server_name example.com;
```

### g. Iniciar os Processos do Servidor e Agente do Zabbix

Inicie os processos do servidor e agente do Zabbix e habilite-os para iniciar na inicialização do sistema:

```sh
systemctl restart zabbix-server zabbix-agent nginx php8.3-fpm
systemctl enable zabbix-server zabbix-agent nginx php8.3-fpm
```

### h. Abrir a Página Web da Interface do Zabbix

A URL da interface web do Zabbix ao usar o Nginx depende das configurações feitas no `nginx.conf`. Acesse:

```
http://example.com:8080/zabbix
```

Substitua `example.com` pelo endereço IP ou nome do host do seu servidor.
