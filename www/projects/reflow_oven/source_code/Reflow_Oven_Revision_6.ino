#include "Adafruit_MAX31855.h"
#include <LiquidCrystal.h>

int thermoDO = 3;
int thermoCS = 4;
int thermoCLK = 5;
const int relay = 6;   //assigns the relay control to pin 6.
const int start = 7;   //assigns start to pin 7.
//const int cancel = 8;  //assigns cancel to pin 8.
int startState = 0;    // assigns startState to 0, will change if push button is pressed.
int cancelState = 0;   // assigns cancelState to 0, will change if push button is pressed.
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // initailizes LCD pins. 
int buzzer = 2;

void setup()
{
  Serial.begin(9600); // begins the serial monitor readings.
  pinMode (6, OUTPUT); // sets pin 6 as an output value for the relay.
  pinMode (7, INPUT); // sets pin 7 as an input pin for the start push button.
  //pinMode (8, INPUT); // sets pin 8 as an input pin for the cancel push button.
  lcd.begin(16, 2); // sets up the LCD's number of columns and rows.
  Serial.println ("MAX31855 chip iniztallizing"); // waits for chip to initialize.
  delay(500); // delays for .5 seconds.
  pinMode(2, OUTPUT);
}

void preheat()
{
    for (int i=0;i<40;) 
    {
      if (thermocouple.readCelsius() < 120) 
      {
       // Serial.print("Internal Temp = "); 
        //Serial.println(thermocouple.readInternal());
  
        double c = thermocouple.readCelsius();
        if(isnan(c))
        {
          Serial.println("Something is wrong with the thermocouple!");
        }
        else
        {
         // Serial.print("C = ");
          Serial.println(c); // prints C in serial monitor.
        }    
       // Serial.print("F = ");
       // Serial.println(thermocouple.readFarenheit()); // prints F in serial monitor.
        lcd.setCursor(0,0); // sets begining point of LCD screen at 0,0
        lcd.print("C = ");  // prints "C =" on LCD screen. 
        lcd.print(c);     // prints Celsius reading on LCD screen.
        lcd.setCursor(0,2);
        lcd.print ("Status: Preheat");
        delay(200);
        digitalWrite (relay, HIGH);
        delay (500);
      }
      else
       {
        // Serial.print("Internal Temp = ");
        // Serial.println(thermocouple.readInternal());
         double c = thermocouple.readCelsius();
         if(isnan(c))
         {
          Serial.println("Something is wrong with the thermocouple!");
         }      
         else
         {
        //  Serial.print("C = ");
          Serial.println(c); // prints C in serial monitor.
         }    
        //  Serial.print("F = ");
         // Serial.println(thermocouple.readFarenheit()); // prints F in serial monitor.
          delay(200);
          digitalWrite (relay, LOW); 
          i++;
          delay (500);
       }
   }
} 

void peak()
{

  for (int i=0;i<5;) //Ramp-to-Peak stage, set to 45 seconds 
  {
    if (thermocouple.readCelsius() < 175) // If reading is less than 175 C, turn relay on.
    {
      //Serial.print("Internal Temp = ");
      //Serial.println(thermocouple.readInternal());
      double c = thermocouple.readCelsius();
      if(isnan(c))
      {
        Serial.println("Something is wrong with the thermocouple!");
      }
      else
      {
       // Serial.print("C = ");
        Serial.println(c); // prints C in serial monitor.
      }    
      //Serial.print("F = ");
     // Serial.println(thermocouple.readFarenheit()); // prints F in serial monitor.
      lcd.setCursor(0,0);
      lcd.print("C = ");
      lcd.print(c);
      lcd.setCursor(0,2);
      lcd.print ("Status: R-T-S  ");
      
      delay(200);
      digitalWrite (relay, HIGH);
      delay (500);
    }
    else
    {
      //Serial.print("Internal Temp = ");
      //Serial.println(thermocouple.readInternal());
      double c = thermocouple.readCelsius();
      if(isnan(c))
      {
        Serial.println("Something is wrong with the thermocouple!");
      }
      else
      {
       // Serial.print("C = ");
        Serial.println(c); // prints C in serial monitor.
      }    
   // Serial.print("F = ");
   // Serial.println(thermocouple.readFarenheit()); // prints F in serial monitor.
    delay(200);
    digitalWrite (relay, LOW); // If reading is more than 175 C, turn relay off.
    i++;
    delay (500);
    }
  }
}

void cool()
{
  for (int i=0;i<240;) //Ramp-to-Peak stage, set to 45 seconds 
  {
      //Serial.print("Internal Temp = ");
      //Serial.println(thermocouple.readInternal());
      double c = thermocouple.readCelsius();
      if(isnan(c))
      {
        Serial.println("Something is wrong with the thermocouple!");
      }
      else
      {
       // Serial.print("C = ");
        Serial.println(c); // prints C in serial monitor.
      }    
      //Serial.print("F = ");
     // Serial.println(thermocouple.readFarenheit()); // prints F in serial monitor.
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("C = ");
      lcd.print(c);
      lcd.setCursor(0,2);
      lcd.print ("Status: Cooling");
      delay(500);
      i++;
  }
 }

void loop()
{
  startState = digitalRead(start);   // starts reading pin 7.
  //cancelState = digitalRead(cancel); // starts reading pin 8.

  //Serial.print("Internal Temp = ");
 // Serial.println(thermocouple.readInternal());
  
  double c = thermocouple.readCelsius();
  if(isnan(c))
  {
    Serial.println("Something is wrong with the thermocouple!");
  }
  else
  {
   // Serial.print("C = ");
    Serial.println(c); // prints C in serial monitor.
  }    
 // Serial.print("F = ");
  //Serial.println(thermocouple.readFarenheit()); // prints F in serial monitor.
  
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("C = ");
  lcd.print(c);
  lcd.setCursor(0,2);
  lcd.print ("Status: Idle");
  
  if (startState == HIGH) // If start button is pressed, start reflow process.
  {
    preheat();
   // lcd.setCursor(0,2);
   // lcd.print ("Preheating");
    delay (500);
    peak();
   // lcd.setCursor(0,2);
   // lcd.print ("Ramp-To-Peak");
    delay (500);
    cool();
    digitalWrite (2,HIGH);
    delay (1500);
    digitalWrite (2, LOW);
    //lcd.setCursor(0,2);
   // lcd.print ("Cooling");
   }
  else if(cancelState == HIGH) // If cancel button is pressed, stop reflow process.
  {
    digitalWrite (relay, LOW);
  }
}
