/*
 *  This sketch sends data via HTTP GET requests to a server on your local network.
 *
 *  You need to enter your network SSID and Password below
 */

#include <ESP8266WiFi.h>

const char* ssid     = "not yours"; //Enter your local SSID here
const char* password = "thenerdherd"; //Enter your network password here

const char* host = "192.168.1.121"; //Enter the IP address of where you want to send the Pin State Data here

void setup() {
  Serial.begin(115200); //Baud rate at which the ESP8266 communicates with the Arduino Terminal
  delay(10);

  // We start by connecting to a WiFi network

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
  Serial.println(WiFi.localIP());

  pinMode(2, INPUT); //GPIO2 pin of the ESP8266
  pinMode(5, INPUT_PULLUP);
}

int value = 0;
bool lastState = 0;
bool PIRState = 0;
int buttonPin = 5;

void loop() {
  delay(1);

  PIRState = digitalRead(2);
  ++value;


  if (digitalRead(buttonPin) == 0) {
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 5000;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // We now create a URI for the request
    String url = "/master_bath"; 
    url += "?button_pressed";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10);

    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
  
    delay(200);
  }


  //Connection to server is made and Pin State transmitted upon Pin State change
  if (PIRState != lastState) {
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 5000;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // We now create a URI for the request
    String url = "/master_bath"; 
    url += "?pir_state=";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + PIRState + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10);

    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
  }
  lastState = PIRState;
}

