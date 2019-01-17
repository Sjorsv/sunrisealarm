/*
 Sjors Platjouw
 Arduino ITTT
 Sunrise Alarm clock with LCD Display and Bluetooth app
 */
#include <Wire.h> //De library voor I2C
#include <RTClib.h> //De library voor de klok
#include <LiquidCrystal_I2C.h> //De library voor het I2C scherm
 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#include <DS3231.h> //Include the clock library

// Changable Vars
int fadeTime = 1; // How long the light will fade to max
int setHour = 02; // Set hours to wake (military time)
int setMin = 49; // Set minute to wake 
int uled = 9; // Set pinout with PWM

// Set up Vars
RTC_DS1307 RTC; //Definieer de klok als DS1307

DS3231  rtc(SDA, SCL);
Time t;
void start();
 void start()
{
  // Fix for SB LED
  analogWrite(uled, 1);
  delay((fadeTime * 60000)/50); // fade timers
  analogWrite(uled, 2);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 3);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 4);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 4);
  delay((fadeTime * 60000)/50);
  analogWrite(uled, 5);
  delay((fadeTime * 60000)/50);


  // Fade script for the LED
  for (int i = 6 ; i <= 255; i++)
  {
    analogWrite(uled, i);
    delay(((fadeTime * 60000)/306));
//    Serial.print(" mil sec ");
//    Serial.print(((fadeTime * 60000)/306));
//    Serial.print(" PWM " );
//    Serial.print(i);


  }
  
  delay(20000); // Stay Bright
  analogWrite(uled, 0); // Turn off
  
}
void setup()
{
  Wire.begin();
  pinMode(uled, OUTPUT); // Set pinmode
  Serial.begin(9600); // Match to serial monitor
  RTC.begin();
  rtc.begin();
  
  lcd.backlight();
  lcd.begin(16, 2);
// uncomment these lines to set time, then reupload again without them.
//  rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
//  rtc.setTime(02, 10, 0);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(17, 1, 2019);   // Set the date to January 1st, 2014
}

void loop()
{
  String input = "";
  while(Serial.available())
  {
    input += (char)Serial.read();
    delay(5);
  }
  DateTime now = RTC.now();
  lcd.setCursor(13,0); // Set text at 3,1 on display
  char dateBuffer[12]; // Set databuffer
  t = rtc.getTime(); // Make a time class called 't'
  
  // Send Day-of-Week
//  Serial.print(rtc.getDOWStr());
  lcd.print(rtc.getDOWStr());
  
  // Send date
//  Serial.print(rtc.getDateStr());
 lcd.setCursor(0,2);
  lcd.print(rtc.getDateStr());
 
  //sprintf(dateBuffer,"%02u-%02u-%02u ",now.day(),now.month(),now.year());

  // Send time
//  Serial.println(rtc.getTimeStr());
lcd.setCursor(0,0);
lcd.print(rtc.getTimeStr());
  
    delay(1000); //Wacht 1 seconde (1000 milliseconden)

lcd.setCursor(3,0);
  String firstHalf = getValue(input, ':', 0); // check first input untill ":"
  String secondHalf = getValue(input, ':', 1); // check second input after ":"

// use this code if you're not using the bluetooth module
//  //if (t.hour == setHour && t.min == setMin) // Check if it's time to wake up!
//  {
//    start();
//  }

// check first 2 digits inputs, then check second 2 digits of input
  if (t.hour == firstHalf.toInt() && t.min == secondHalf.toInt())
  {
    start();
  }
  
}

// logic to seperate strings
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
