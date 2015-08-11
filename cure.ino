#include <DHT22.h>
// Only used for sprintf
#include <stdio.h>
#include <LiquidCrystal.h>

// Data wire is plugged into port 7 on the Arduino
// Connect a 4.7K resistor between VCC and the data pin (strong pullup)
#define DHT22_PIN 2


// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

unsigned long lastTimeGet = 0;
unsigned long lastTimeSet = 0;
unsigned long currTime = 0;
int setTemp = 13;

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/


 return btnNONE;  // when all others fail, return this...
}

void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
  pinMode(3, OUTPUT);// output for controlling the fridge compressor
  delay(3000);
  digitalWrite(3, LOW);   // set fridge off

   lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
// lcd.print("Push the buttons"); // print a simple message
}

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  currTime = millis()/1000;
  
  float h,t;

    lcd.setCursor(12,1);            // move to the begining of the second line      
    lcd.print(currTime - lastTimeSet);
    if (currTime - lastTimeSet >=100)
    {
      
    }
    else if (currTime - lastTimeSet >=10)
    {
      lcd.print(" ");
    }
    else
    {
      lcd.print("  ");
    }

    if (currTime - lastTimeGet > 5)
    {
      DHT22_ERROR_t errorCode;
      errorCode =  myDHT22.readData();
      lcd.setCursor(15,1);
      lcd.print("F");
      lastTimeGet = currTime;      

      switch(errorCode)
      {
        case DHT_ERROR_NONE:
          lcd.setCursor(15,1);          
          lcd.print("G");
          lcd.setCursor(0,0);            // move to the begining of the second line          
          t = myDHT22.getTemperatureC();
          h = myDHT22.getHumidity();          
          Serial.print(t);
          Serial.print("C everything working");
          if (isnan(t) || isnan(h)) {
            Serial.println("Failed to read from DHT");
          } 
          else
          {
                      
            lcd.print(t);
            lcd.print("C ");

            lcd.print(h);
            lcd.print("% ");
          
            lcd.setCursor(0,1);            // move to the begining of the second line        
            if (currTime - lastTimeSet > 100)
            {
                lastTimeSet = currTime;
                if (t > setTemp)
                {
                  lcd.print("cool on");
                  digitalWrite(3, HIGH);   // turn the fridge on for 5 minutes
                  Serial.println("cool on");
                }
                else
                {
                  lcd.print("cool off");
                  digitalWrite(3, LOW);   // turn the fridge on for 5 minutes
                  Serial.println("cool off");
                  
                }
                // check if returns are valid, if they are NaN (not a number) then something went wrong!
            }
          }
          break;
        case DHT_ERROR_CHECKSUM:
          Serial.print("check sum error ");
          Serial.print(myDHT22.getTemperatureC());
          Serial.print("C ");
          Serial.print(myDHT22.getHumidity());
          Serial.println("%");
          break;
        case DHT_BUS_HUNG:
          Serial.println("BUS Hung ");
          break;
        case DHT_ERROR_NOT_PRESENT:
          Serial.println("Not Present ");
          break;
        case DHT_ERROR_ACK_TOO_LONG:
          Serial.println("ACK time out ");
          break;
        case DHT_ERROR_SYNC_TIMEOUT:
          Serial.println("Sync Timeout ");
          break;
        case DHT_ERROR_DATA_TIMEOUT:
          Serial.println("Data Timeout ");
          break;
        case DHT_ERROR_TOOQUICK:
          Serial.println("Polled to quick ");
          break;
      }
    }

     lcd_key = read_LCD_buttons();  // read the buttons
     lcd.setCursor(7,1);            // move to the begining of the second line
     lcd.print(setTemp);
     lcd.setCursor(7,1);            // move to the begining of the second line
     switch (lcd_key)               // depending on which button was pushed, we perform an action
     {
       case btnRIGHT:
         {
         break;
         }
       case btnLEFT:
         {
         break;
         }
       case btnUP:
         {
           setTemp = setTemp + 1;
           lcd.print(setTemp);
          delay(1000);
         break;
         }
       case btnDOWN:
         {
           setTemp = setTemp - 1;
           lcd.print(setTemp);
          delay(1000);
         break;
         }
       case btnSELECT:
         {
         //lcd.print("SELECT");
          lcd.print(setTemp);
         delay(1000);
         break;
         }
         case btnNONE:
         {
         //lcd.print("NONE  ");
         break;
         }
       default:
         {
            lcd.print(setTemp);           
            break;
         }
         
     }
    
}
 


