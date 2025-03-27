# Get Zabbix

## 1. Pré-requisitos para o Zabbix

Certifique-se de atender a todos os pré-requisitos antes de prosseguir com a instalação.
|Database| Web Server|
|--------|--------|
|MySQL|Apache|
|PostgreSQL|Nginx|
## 2. Instale e configure o Zabbix para sua plataforma

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

### e. Importar o esquema inicial do banco de dados

```sh
zcat /usr/share/zabbix/sql-scripts/mysql/server.sql.gz | mysql --default-character-set=utf8mb4 -uzabbix -p zabbix
```

### f. Desativar a configuração temporária no MySQL

```sh
mysql -uroot -p
```

Digite a senha do root do MySQL e execute:

```sql
SET GLOBAL log_bin_trust_function_creators = 0;
QUIT;
```

### g. Configurar a senha do banco de dados no Zabbix Server

Edite o arquivo de configuração do Zabbix Server:

```sh
nano /etc/zabbix/zabbix_server.conf
```

Encontre a linha `DBPassword=` e defina:

```sh
DBPassword=password
```

### h. Reiniciar e habilitar os serviços do Zabbix

```sh
systemctl restart zabbix-server zabbix-agent apache2
systemctl enable zabbix-server zabbix-agent apache2
```

### i. Acessar a interface web do Zabbix

Abra um navegador e acesse a interface web do Zabbix:

```
http://host/zabbix
```

Substitua `host` pelo endereço IP ou hostname do seu servidor.
