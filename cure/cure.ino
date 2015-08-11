// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT11.h"

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  pinMode(3, OUTPUT);// output for controlling the fridge compressor
  pinMode(4, OUTPUT);// output for controlling the humidifier
  dht.begin();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  digitalWrite(3, LOW);   // set fridge off
  digitalWrite(4, LOW);   // set humidifier off
  if (h < 55)
    digitalWrite(3, HIGH);   // turn the humidifier on for 5 minutes

  if (t > 17)
    digitalWrite(4, HIGH);   // turn the fridge on for 5 minutes
    
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
  }
  
  int mistTime = 30000;
  int totalLoopTime = 300000;
  int remainingCoolingTime = totalLoopTime - mistTime;
  
  // half a minute of mist max
  delay(mistTime);
  digitalWrite(3, LOW);   // turn the humidifier on for 5 minutes
  
  // 5 minutes is 5*60sec = 300
  delay(remainingCoolingTime);
}
