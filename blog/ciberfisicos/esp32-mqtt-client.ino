
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
