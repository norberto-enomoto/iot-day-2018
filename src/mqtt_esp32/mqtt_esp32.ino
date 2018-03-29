/*
* Date: 29/03/2018
* Author: Norberto Hideaki Enomoto - norberto.enomoto@gmail.com
*/
#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid          = "";
const char* password      = "";

const char* MQTT_SERVER   = "";
const int MQTT_PORT       = 0;
const char* MQTT_USER     = "";
const char* MQTT_PASSWORD = "";
const char* MQTT_CLIENT   = "ESP-32-01";

const char* SUBSCRIBE_TOPIC  = "entrance/subscribe";
const char* PUBLISH_TOPIC    = "entrance/publish";

// const int RELAY_PIN = 0; //Equivalente ao D3 no NodeMCU

const int RELAY_PIN1 = 14;
const int RELAY_PIN2 = 2;

/*
const int RELAY_PIN1 = 16;
const int RELAY_PIN2 = 14;
*/

#define FALSE 0      
#define TRUE  1 

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String msg = String((char)payload[0]);     
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
    //digitalWrite(BUILTIN_LED, LOW); 
    //digitalWrite(RELAY_PIN1, LOW);
   
    digitalWrite(RELAY_PIN2, HIGH);
    Serial.println("HIGH");
  } 
  else if ((char)payload[0] == '0') {
    // Turn the LED off by making the voltage HIGH
    //digitalWrite(BUILTIN_LED, HIGH);
    //digitalWrite(RELAY_PIN1, HIGH);
    digitalWrite(RELAY_PIN2, LOW);        
    Serial.println("LOW");
  } 
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD, "WillTopic", 0, false, "Desconectado")) {    
      Serial.println("connected");
      client.subscribe(SUBSCRIBE_TOPIC);
      Serial.println(String(SUBSCRIBE_TOPIC) + " subscribed");
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
  // Initialize the BUILTIN_LED pin as an output
  // pinMode(BUILTIN_LED, OUTPUT);  
  // pinMode(RELAY_PIN1, OUTPUT);
  
  Serial.begin(115200);
  
  pinMode(RELAY_PIN2, OUTPUT);
  digitalWrite(RELAY_PIN2, LOW); 
  
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }  
  client.loop();
}

