import json
import subprocess
import paho.mqtt.client as mqtt

# Configurações do Zabbix
ZABBIX_SERVER = "regis-nitro-an515-51"  # ou o IP correspondente, como "192.168.254.104"
ZABBIX_PORT = "10051"  # Porta padrão para trapper
HOSTNAME = "SensorHost"               # Nome do host cadastrado no Zabbix
ZABBIX_KEY = "sensor.acelerometro"    # Key definida no item do Zabbix

# Função chamada ao conectar no broker MQTT
def on_connect(client, userdata, flags, rc):
    print("Conectado ao MQTT com código " + str(rc))
    client.subscribe("sensor/acelerometro")  # Inscreve-se no tópico

# Função chamada ao receber uma mensagem MQTT
def on_message(client, userdata, msg):
    try:
        # Converte o payload recebido de JSON para dicionário
        data = json.loads(msg.payload.decode())
        # Envia o JSON original para o Zabbix
        json_data = json.dumps(data)  # Exemplo: {"x":1.2, "y":-0.5, "z":0.8}
        cmd = f'zabbix_sender -z {ZABBIX_SERVER} -p {ZABBIX_PORT} -s "{HOSTNAME}" -k {ZABBIX_KEY} -o \'{json_data}\''
        # Executa o comando
        subprocess.run(cmd, shell=True)
    except Exception as e:
        print("Erro ao processar mensagem:", e)

# Configura o cliente MQTT  
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Conecta no broker MQTT (ajuste o endereço se necessário)
client.connect("localhost", 1883, 60)

# Mantém o loop de escuta
client.loop_forever()
