#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

//Relays for switching appliances
#define Relay1            D6
#define Relay2            D2
#define Relay3            D1
#define Relay4            D5

//DHT11 for reading temperature and humidity value
#define DHTPIN            D6

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);

// Update these with values suitable for your network.

const char* ssid = "NIPTICT_OFFICIAL";
const char* password = "N!PT!CT@@WiFi";
const char* mqtt_server = "broker.hivemq.com"; 
const int mqttPort = 1883; //HiveMQ Port
const char* username = "testing221";
const char* pass = "11111111";


#define sub1 "myhome/control/relay1"
#define sub2 "myhome/control/relay2"
#define sub3 "myhome/control/relay3"
#define sub4 "myhome/control/relay4"

char str_hum_data[10];
char str_temp_data[10];



WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strstr(topic, sub1))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(Relay1, HIGH);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay1, LOW);  // Turn the LED off by making the voltage HIGH
    }
  }

  else if ( strstr(topic, sub2))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(Relay2, HIGH);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay2, LOW);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if ( strstr(topic, sub3))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(Relay3, HIGH);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay3, LOW);  // Turn the LED off by making the voltage HIGH
    }
  }
  else if ( strstr(topic, sub4))
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(Relay4, HIGH);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    } else {
      digitalWrite(Relay4, LOW);  // Turn the LED off by making the voltage HIGH
    }
  }

  else
  {
    Serial.println("unsubscribed topic");
  }



}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), username, pass) ) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(sub1);
      client.subscribe(sub2);
      client.subscribe(sub3);
      client.subscribe(sub4);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    float hum_data = dht.readHumidity();
    Serial.println(hum_data);
    /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
    dtostrf(hum_data, 4, 2, str_hum_data);
    float temp_data = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
    dtostrf(temp_data, 4, 2, str_temp_data);
    lastMsg = now;
    Serial.print("Publish message: ");
    Serial.print("Temperature - "); Serial.println(str_temp_data);
    client.publish("device2/temp", str_temp_data);
    Serial.print("Humidity - "); Serial.println(str_hum_data);
    client.publish("device2/hum", str_hum_data);
  }
}
