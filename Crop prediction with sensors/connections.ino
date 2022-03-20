// Code for connection between NodeMcu, DHT11 and Moisture sensor.
// Import required libraries

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DHT.h>


// Replace with your network credentials
const char* ssid = "YOUR WIFI/Router NAME eg.Pratiksha";
const char* password = "#Wifi password";
const char* host = "IPv4 address of the connection made. eg of wifi";

#define DHTPIN 5     // Digital pin D1 connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  // Serial port for debugging purposes
  pinMode(A0,INPUT);
//  Serial.begin(115200);
  Serial.begin(115200);
  Serial.println("DHT11 Output");
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  // Print ESP8266 Local IP Address
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

 
void loop(){  
  //get sensor values and print it to serial monitor
  int moisture = analogRead(A0); //read the state of button 
//  Serial.println(moisture);
//  delay(50000);
    float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();  
  if(isnan(temperature) || isnan(humidity) || isnan(moisture)){
    Serial.println("Failed to read DHT11");
  }else{
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C");
    Serial.print(" \t");
    Serial.print("Moisture: ");
    Serial.println(moisture);
    delay(10000);
  }
   
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }


 
    // This will send the request to the server
 client.print(String("GET http://localhost/crop_recommendation/dht11.php?") + 
                          ("&temperature=") + temperature +
                          ("&humidity=") + humidity +
                          ("&moisture=") + moisture +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }

    Serial.println();
    Serial.println("closing connection");
}
