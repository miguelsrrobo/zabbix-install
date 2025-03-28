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

```sh
mysql -uroot -p
```
Digite a senha do root do MySQL e execute:

```sql
CREATE DATABASE zabbix CHARACTER SET utf8mb4 COLLATE utf8mb4_bin;
CREATE USER zabbix@localhost IDENTIFIED BY 'senha_segura';
GRANT ALL PRIVILEGES ON zabbix.* TO zabbix@localhost;
SET GLOBAL log_bin_trust_function_creators = 1;
QUIT;
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
http://<seu_servidor>/zabbix
```

---

### 2.2. Instalação do Zabbix com PostgreSQL e Apache

Repita os passos `a` e `b`.

#### c) Instalar Pacotes

```sh
apt install zabbix-server-pgsql zabbix-frontend-php php8.3-pgsql zabbix-apache-conf zabbix-sql-scripts zabbix-agent
```

#### d) Configurar o Banco de Dados PostgreSQL

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
http://<seu_servidor>/zabbix
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
http://seu_dominio.com:8080/zabbix
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
http://seu_dominio.com:8080/zabbix
```
## 3 Comece a usar o Zabbix

Leia em documentação: [Guia de início rápido](https://www.zabbix.com/documentation/7.2/en/manual/quickstart/login)

---

## Conclusão

Agora você tem o Zabbix instalado e configurado com diferentes combinações de banco de dados e servidores web. Acesse a interface web para finalizar a configuração e comece a monitorar seus dispositivos!

---

## Referências

Para mais detalhes sobre a instalação do Zabbix, consulte a documentação oficial: [Zabbix Download](https://www.zabbix.com/download?zabbix=7.2&os_distribution=ubuntu&os_version=24.04&components=server_frontend_agent&db=mysql&ws=apache)
ou [Zabbix manuals](https://www.zabbix.com/manuals)

