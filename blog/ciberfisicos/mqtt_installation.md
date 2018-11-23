* Buscar la raspberry en la red
```
sudo nmap -sP 192.168.0.0/24
```

* Acceder a la raspberry
```
ssh pi@192.168.0.100
pi@192.168.0.100's password: raspberry
```

* Instalar mosquito en la raspberry
Referencia: https://www.instructables.com/id/Installing-MQTT-BrokerMosquitto-on-Raspberry-Pi/
```
wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
sudo apt-key add mosquitto-repo.gpg.key
```
```
cd /etc/apt/sources.list.d/
```
Lo siguiente si es "stretch" or "wheezy" la version
```
sudo wget http://repo.mosquitto.org/debian/mosquitto-wheezy.list
#o
sudo wget http://repo.mosquitto.org/debian/mosquitto-stretch.list
#o jessie
sudo wget http://repo.mosquitto.org/debian/mosquitto-jessie.list
```
```
sudo -i
apt-get update
apt-get install mosquitto mosquitto-clients
```
Creando el topico o añadir subscripción
```
$ mosquitto_sub -d -t sensores/temperatura
Client mosqsub|1200-raspberry sending CONNECT
Client mosqsub|1200-raspberry received CONNACK (0)
Client mosqsub|1200-raspberry sending SUBSCRIBE (Mid: 1, Topic: sensores/temperatura, QoS: 0)
Client mosqsub|1200-raspberry received SUBACK
Subscribed (mid: 1): 0
```


## Desde tu pc:

Te instalas mosquitto y haces

```
$ mosquitto_pub -t sensores/temperature -m "{value: 32.0, id: mypc}"
```

## Añadir authenticacion basica

Referencia: http://www.steves-internet-guide.com/mqtt-username-password-example/

create mosquito/passwords file
```
14A4AE30:mysecretkey
mypc:mysecretkey
otrousuario:mysecretkey
```

Encriptar los passwords:
```
mosquitto_passwd -U passwords
```


edit /etc/mosquitto/mosquitto.conf
```
allow_anonymous false
password_file /home/pi/mosquito/passwords
```
## Añadir SSL

Referencia: https://mcuoneclipse.com/2017/04/14/enable-secure-communication-with-tls-and-the-mosquitto-broker/

## Clientes


* Desde un cliente, i.e python

pip3.6 install paho-mqtt

```
import paho.mqtt.client as paho
import socket

# https://os.mbed.com/teams/mqtt/wiki/Using-MQTT#python-client

# MQTT broker hosted on local machine
mqttc = paho.Client()

# Settings for connection
host = "192.168.0.101"
topic= "sensores/"

# Callbacks
def on_connect(self, mosq, obj, rc):
    print("Connected rc: " + str(rc))

def on_message(mosq, obj, msg):
    print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n");

def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed OK")

def on_unsubscribe(mosq, obj, mid, granted_qos):
    print("Unsubscribed OK")

# Set callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe
mqttc.on_unsubscribe = on_unsubscribe

# Connect and subscribe
print("Connecting to " + host + "/" + topic)
mqttc.connect(host, port=1883, keepalive=60)
mqttc.subscribe(topic+"#", 0)
mqttc.publish(topic+"temperature", "{ value: 45.00 , id: python-test}")

# Loop forever, receiving messages
mqttc.loop_forever()

print("rc: " + str(rc))
```

* Client ESP32

```

#include <WiFi.h>
#include <PubSubClient.h>

/* Wifi router */
const char* ssid     = "TP-Link_CA4C";
const char* password = "95079165";

/* mqtt server */
const char* mqtt_server = "192.168.0.101";

/* topics */
#define TEMPERATURE_TOPIC    "sensores/temperature"

/* secret key */
const char* secretKey  = "mysecretkey";

/* values */
float temperature = 20.0;
uint64_t chipid;
char * deviceId;
String localIp = "";
long lastMsg = 0;
char msg[100];

WiFiClient wifi;
PubSubClient client(wifi);

void callbackOnReceive(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

char* getDeviceId(){
  char* deviceid;
  deviceid = (char*)malloc (sizeof (20));
  snprintf (deviceid, 20, "%08X", (uint32_t) chipid);
  return deviceid;
}

void connect() {
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    String peer = "esp32_client_" + String(deviceId);
    if (client.connect(peer.c_str(), deviceId ,secretKey)) {
      Serial.println("connected");
      /* subscribe topic with default QoS 0*/
      client.subscribe(TEMPERATURE_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setupMqtt()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callbackOnReceive);
}

void setupWifi()
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");

    localIp = String(WiFi.localIP());
    Serial.println(WiFi.localIP());
}

void setupDevice()
{
  chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X\n",(uint16_t)(chipid>>32));//print High 2 bytes

  deviceId = getDeviceId();
  Serial.printf("Device id %s\n", deviceId); 
}

void setup()
{
    Serial.begin(115200);
    delay(10);
    setupDevice();
    delay(10);
    setupWifi();
    delay(10);
    setupMqtt();
}

int value = 0;

void loop()
{
  if (!client.connected()) {
    connect();
  }
  
  client.loop();
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++temperature;
    if (!isnan(temperature)) {
      Serial.println("Sending temperature update...");
      snprintf (msg, 100, "{ value: %lf, id: %08X }", temperature, (uint32_t)chipid);
      client.publish(TEMPERATURE_TOPIC, msg);
    }
  }
}

```



