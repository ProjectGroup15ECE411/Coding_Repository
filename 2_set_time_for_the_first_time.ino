//make sure to put SCL to A5 on Arduino
//make sure to put SDA to A4 on Arduino
/*
 * this is a code to set the time to a real time for the first time
 * you have to enter the current time from the code down
 * this code only sets the RTC, no LCD dislay
 */
#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68
 
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
 
void setDateDs1307(
byte second,        // 0-59
byte minute,        // 0-59
byte hour,          // 1-23
byte dayOfWeek,     // 1-7 1=Mon, 7=Sun
byte dayOfMonth,    // 1-28/29/30/31
byte month,         // 1-12
byte year           // 0-99
)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}
 
void setup()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  Wire.begin();
  pinMode(13, OUTPUT);
 
  // Change these values to what you want to set your clock to.
  // It is best to add 30 seconds to a minute to allow time for your computer to compile and upload the current time.
  // Only run this script once, as running it again will overwrite the time set in the RTC chip!
  // Hours are in 24 hour format
  // Day of week starts with Monday = 1 up to Sunday = 7
  // Year is in YY format, so only use the last 2 digits of the year
  //
  // Once you have run the program, the LED on pin 13 will flash to say it has finished, DO NOT UNPLUG OR RESET.
  // Simply follow the tutorial and upload the LCD code to avoid overwriting the correct time with this time again.
  //
  second = 0;
  minute = 34;
  hour = 12;
  dayOfWeek = 4;
  dayOfMonth = 6;
  month = 12;
  year = 18;
  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  //*/
}
 
void loop()
{
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}
