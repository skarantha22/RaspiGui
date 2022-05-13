/*****
 
 All the resources for this project:
 https://randomnerdtutorials.com/
 
*****/

// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//////////////////////////////////////////////////////////
int stepsPerTurn = 200;
//int RPM = 200;
//float stepsPerSecond = (stepsPerTurn*RPM)/60;
//float waitInMilliseconds = (1/stepsPerSecond)*1000;

//int stepsPerTurn;
int RPM;
float stepsPerSecond;
float waitInMilliseconds;
////////////////////////////////////////////////////////////

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "NETGEAR06";
const char* password = "appupallu0824";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.1.40";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// Connect an LED to each GPIO of your ESP8266
const int motor2_pin5 = 5;
const int motor1_pin4 = 4;
int pin_status0;
int pin_status1;

// Don't change the function below. This functions connects your ESP8266 to your router
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
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2, you check if the message is either 1 or 0. Turns the ESP GPIO according to the message
  if(topic=="motor1_onoff"){
      Serial.print("Changing M1PULGPIO 4 to ");
      if(messageTemp == "1"){
         pin_status0=1;
         //Serial.print(pin_status0);
        Serial.print("On");
      }
      else if(messageTemp == "0"){
         pin_status0=0;
        digitalWrite(motor1_pin4, LOW);
        Serial.print("Off");
      }
  }
  if(topic=="motor2_onoff"){
      Serial.print("Changing M2PULGPIO 5 to ");
      if(messageTemp == "1"){
        pin_status1=1;
        //digitalWrite(ledGPIO5, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "0"){
        pin_status1=0;
        digitalWrite(motor2_pin5, LOW);
        Serial.print("Off");
      }
  }
  if(topic=="motorspeed"){
      Serial.print("Changing motor speed to ");
      if(messageTemp == "1"){
        //int stepsPerTurn = 200;
       RPM = 160;
       stepsPerSecond = (stepsPerTurn*RPM)/60;
       waitInMilliseconds = (1/stepsPerSecond)*1000;
        //digitalWrite(ledGPIO5, HIGH);
        Serial.print("HIGH");
      }
      else if(messageTemp == "0"){
       // int stepsPerTurn = 200;
       RPM = 60;
       stepsPerSecond = (stepsPerTurn*RPM)/60;
       waitInMilliseconds = (1/stepsPerSecond)*1000;
        //digitalWrite(motor2_pin5, LOW);
        Serial.print("LOW");
      }
  }
  
  Serial.println();

  
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
     /*
     YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a unique name to the ESP8266.
     Here's how it looks like now:
       if (client.connect("ESP8266Client")) {
     If you want more devices connected to the MQTT broker, you can do it like this:
       if (client.connect("ESPOffice")) {
     Then, for the other ESP:
       if (client.connect("ESPGarage")) {
      That should solve your MQTT multiple connections problem

     THE SECTION IN loop() function should match your device name
    */
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("motor1_onoff");
      client.subscribe("motor2_onoff");
      client.subscribe("motor1_dir");
      client.subscribe("motor2_dir");
      client.subscribe("motorspeed");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(motor1_pin4, OUTPUT);
  pinMode(motor2_pin5, OUTPUT);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())

  


if (pin_status1==1)
{
    digitalWrite(motor2_pin5, HIGH);
    digitalWrite(motor1_pin4, HIGH);
    delay(waitInMilliseconds);
    digitalWrite(motor2_pin5,LOW);
    digitalWrite(motor1_pin4,LOW);
    delay(waitInMilliseconds);
      
}

if (pin_status0==1 && pin_status1==0 )
{
    digitalWrite(motor1_pin4, LOW);
    delay(waitInMilliseconds);
    digitalWrite(motor1_pin4,HIGH);
    delay(waitInMilliseconds);
      
}
else if (pin_status1==1 && pin_status0==0 )
{
    digitalWrite(motor2_pin5, HIGH);
    delay(waitInMilliseconds);
    digitalWrite(motor2_pin5,LOW);
    delay(waitInMilliseconds);
      
}

else if (pin_status1==1 && pin_status0==1 )
{
    digitalWrite(motor1_pin4, HIGH); 
    digitalWrite(motor2_pin5, HIGH);
    delay(waitInMilliseconds);
    digitalWrite(motor1_pin4, LOW);
    digitalWrite(motor2_pin5,LOW);
    delay(waitInMilliseconds);
      
}

     /*
     YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a unique name to the ESP8266.
     Here's how it looks like now:
       client.connect("ESP8266Client");
     If you want more devices connected to the MQTT broker, you can do it like this:
       client.connect("ESPOffice");
     Then, for the other ESP:
       client.connect("ESPGarage");
      That should solve your MQTT multiple connections problem

     THE SECTION IN recionnect() function should match your device name
    */
    client.connect("ESP8266Client");
}
