#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char *ssid =  "OnePlus Nord CE 2";     // Enter your WiFi Name
const char *pass =  "180725"; // Enter your WiFi Password

String msg;
char msg1[20];
WiFiClient client;

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "choudharyas"
#define MQTT_PASS "988c4e045ef64c1b9bc8b5bb7ef5f2d9"

void send_event(const char *event);
const char *host = "maker.ifttt.com";
const char *privateKey = "hUAAAz0AVvc6-NW1UmqWXXv6VQWmpiGFxx3sV5rnaM9";


FPS_GT511C3 fps(D6, D5); 
#define relay D1
#define buzzer D2

Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
//Set up the feed you'r publishing to
Adafruit_MQTT_Publish Fingerprint = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/Fingerprint");


void setup()
{
  Serial.begin(9600); //set up Arduino's hardware serial UART
  delay(100);
  fps.Open();         //send serial command to initialize fps
  fps.SetLED(true);   //turn on LED so fps can see fingerprint
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(relay, LOW); // keep motor off initally

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{
  MQTT_connect();
  // Identify fingerprint test
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    
     if (id ==7) //<- change id value depending model you are using
     {
      Serial.print("Ashish Entered");
      Buzzer();
      digitalWrite(relay, HIGH);
      delay(6000);
      digitalWrite(relay, LOW);
      msg = "Ashish Entered";
      msg.toCharArray(msg1, 20);
      if (! Fingerprint.publish(msg1))
        {                     
         //delay(7000);   
          }
         }
     else if (id ==8)
       {
        Serial.print("Manoj Entered");
        Buzzer();
        digitalWrite(relay, HIGH);
        delay(6000);
        digitalWrite(relay, LOW);
        msg = "Manoj Entered";
        msg.toCharArray(msg1, 20);
        if (! Fingerprint.publish(msg1))
        {                     
         //delay(7000);   
          }
       }

     else if (id ==9)
       {
        Serial.print("Aswinth Entered");
        Buzzer();
        digitalWrite(relay, HIGH);
        delay(6000);
        digitalWrite(relay, LOW);
        msg = "Aswinth Entered";
        msg.toCharArray(msg1, 20);
        if (! Fingerprint.publish(msg1))
        {                     
         //delay(7000);   
          }
       }
      
    
     else
      {//if unable to recognize
       send_event("login_event");
       Serial.println("Finger not found");
       for(int i = 7; i > 0; i--){
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
        delay(100);
       }

    }
  }
  else
  {
    Serial.println("Please press finger");
  }
  delay(100);

 
}

void send_event(const char *event)
{
  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      // No data yet, wait a bit
      delay(50);
    };
  }
  
  Serial.println();
  Serial.println("closing connection");
  client.stop();
}

void MQTT_connect() 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}

void Buzzer()
{
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
}
