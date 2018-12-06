/*
LIGHT SENSETIVE ALARM CLOCK  
____________________________

PINS THAT HAS BEEN USED: 

LCD:
rs = 0
E = 2
d4 = 4
d5 = A0 
d6 = A1
d7 = A2

RTC:
SCL = A5
SDL = A4

PUSHBUTOONS:
P1 = 7 "THE MENU BUTTON IS THE MAIN BUTTON, PRESS ONCE TO BLINK. PRESS TWICE TO SET ALARM HOURS. PRESS ONE MORE TIME TO SET ALARM MINUTES"
P2 = 5(OPTIONAL) "WHEN SETTING THE HOURS OR MINUTES YOU NEED TO PRESS IT TO CHANGE THE NUMBER"

 KEYPAD "MAIN USE":
 the pin used 3, 8, 9, 10, 11, 12, 13
 * for the numbers to be correct and in order you have to connect the keypad in the arduino in this order
 * rows:
 * pin 4 (keypad) > pin 3 arduino
 * pin 3 (keypad) > pin 8 arduino
 * pin 2 (keypad) > pin 9 arduino
 * pin 1 (keypad) > pin 10 arduino
 * cols:
 * pin 7 (keypad) > pin 11 arduino
 * pin 6 (keypad) > pin 12 arduino
 * pin 5 (keypad) > pin 13 arduino
 * 
 * if you choose another digital pin is okay just keep the order
 * if you don't keep the order the numbers will mixup
 * press 1 on keypad > the lcd will show 7 or some other number not 1
 * 
 * NOTICE:
 * you can only use pin 0 or 1 on the arduino if you are not using serial.
 * serial uses communication pins 0, and 1, so if you are not using serial 
 * the pins 0, and 1 can be used as digital

ALARM SOUND:
SPEAKER = 11

LIGHT SENSOR:
PHOTORESISTOR = A3

 */
 
 #include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include "RTClib.h"
#include <Wire.h>

RTC_DS1307 RTC;
#include <Keypad.h>
LiquidCrystal lcd(0, 2, 4, A0, A1, A2); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

// ALARM SOUND  ==========================================

int speakerOut = 6;

// Start by defining the relationship between 
//       note, period, &  frequency. 
#define  c     3830    // 261 Hz 
#define  d     3400    // 294 Hz 
#define  e     3038    // 329 Hz 
#define  f     2864    // 349 Hz 
#define  g     2550    // 392 Hz 
#define  a     2272    // 440 Hz 
#define  b     2028    // 493 Hz 
#define  C     1912    // 523 Hz 
// Define a special note, 'R', to represent a rest
#define  R     0
// Set up speaker on a PWM pin (digital 9, 10 or 11)

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[], 
//  which sets each note's relative length (higher #, longer note) 
int melody[] = {  C,  b,  g,  C,  b,   e,  R,  C,  c,  g, a, C };
int beats[]  = { 16, 16, 16,  8,  8,  16, 32, 16, 16, 16, 8, 8 }; 
int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.

// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

// ============================================

//************keypad*****************//

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[ROWS] = {10, 9, 8, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 11}; //connect to the column pinouts of the keypad


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//************Button*****************//


 int P1= 7;
 int P2= 5;

  int menu;
/*
 * the following line is declaring the variable of unassigned 
 * to hold the alarm minute and hour
 */
int alarmHours, alarmMinutes;  // Holds the current alarm time
char key1 = ' ', key2 = ' ', key3 = ' ', key4 = ' '; // Display the alarm time on the screen

/*
 * Putting the pins as outputs 
 * Note Menu the variable counting the times the button is pushed
 * 
 */
void setup() {

  pinMode(P1,INPUT_PULLUP);
  pinMode(P2,INPUT_PULLUP);
  int menu=0;

 Wire.begin();
 RTC.begin();
 lcd.begin(16,2);// Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
 pinMode(speakerOut, OUTPUT);


}


/*THE functions are below the loop function
 * timenn: is a function that display the time adn date on the LCD
 * Alarm: is the function that control compare the current time with the alarb time, then write wake up on the screen
 * SetHourAlarm is the function that contrls the hours of the alarm
 * SetMinuteAlarm is the function that contrls the minute of the alarm
 * Menu:
 * depends on how many times you click on menu will take you to a different case
 * 
 */

void loop() { 

  timenn();
  Alarm();
  int sensor_value = analogRead(A3);


  if(digitalRead(P1)== LOW)
  {
   menu=menu+1;
  }
  
  if (menu == 1){

    lcd.clear();
    delay(1000);
    menu=menu+1;

  }

  if(menu==2){

    SetHourAlarm();

  }

  if (menu == 3){
    SetMinuteAlarm();
    
    }


  if (menu == 4)
  {
    menu = 0;
   
  }
   
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm");
    lcd.setCursor(0,1);
    //lcd.print(alarmHours, DEC);
    lcd.print(key1);
    lcd.print(key2);
    lcd.print(":");
    lcd.print(key3);
    lcd.print(key4);
    //lcd.print(alarmMinutes, DEC);
    //delay(1000);
    //lcd.clear();
    
    

    
}




void timenn(){
  
    DateTime now = RTC.now();

    lcd.setCursor(6,0);
    lcd.print(now.day(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);

    lcd.setCursor(7, 1);
    
    if (now.hour()<10)
    lcd.print("0");
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    if (now.minute()<10)
    lcd.print("0");
    lcd.print(now.minute(), DEC);
    lcd.print(":");
    if (now.second()<10)
    lcd.print("0");
    lcd.print(now.second(), DEC);
    lcd.setCursor(12, 0);
    delay(1000);

  
  
  }



void SetHourAlarm()// Setting the alarm minutes
{
  lcd.clear();
  int z = 1;
  while(digitalRead(P1)==HIGH) {
    
    lcd.setCursor(0,0);
    lcd.print("Set HOUR Alarm:");
    while (z == 1){
      char key = keypad.getKey();
      if (key){
        lcd.setCursor(0,1);  
        lcd.print(key);
        alarmHours = (key - '0') * 10;
        key1 = key;
        delay(200);
        z = 0;
}
  while (z == 0){
    char key = keypad.getKey();
    if (key){
      lcd.setCursor(1,1);
      lcd.print(key);
      alarmHours = alarmHours + (key - '0');
      key2 = key;
      delay(200);
      if ((alarmHours > 23)||(key1 == '*')||(key2 == '*')||(key1 == '#')||(key2 == '#')){
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Error!");
       lcd.setCursor(0,1);
       lcd.print("Enter valid hour");
       delay(1500); 
       z = 1;
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Set HOUR Alarm:");
       
       }
      else{ 
       z = 2;
       menu = 3;
      }
      
      
}
}
    }
char key = keypad.getKey();
    if (key == '*'){
      z = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set HOUR Alarm:");
    }
     
 
 
}


 }



  
void SetMinuteAlarm()// Setting the alarm minutes
{
  lcd.clear();
  int z = 1;
  while(digitalRead(P1)==HIGH) {
    lcd.setCursor(0,0);
    lcd.print("Set MINUTE Alarm:");
    while (z == 1){
      char key = keypad.getKey();
      if (key){
        lcd.setCursor(0,1);  
        lcd.print(key);
        alarmMinutes = (key - '0') *10;
        key3 = key;
        delay(200);
        z = 0;
}
  while (z == 0){
    char key = keypad.getKey();
    if (key){
      lcd.setCursor(1,1);
      lcd.print(key);
      alarmMinutes = alarmMinutes + (key - '0' );
      key4 = key;
      delay(200);
      if ((alarmMinutes > 59)||(key3 == '*')||(key4 == '*')||(key3 == '#')||(key4 == '#')){
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Error!");
       lcd.setCursor(0,1);
       lcd.print("Enter valid MINs");
       delay(1500); 
       z = 1;
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Set MINUTE Alarm:");
       
       }
      else{ 
       z = 2;
       menu = 4;
      }
      
}
}
    }
char key = keypad.getKey();
    if (key == '*'){
 z = 1;
 lcd.clear()
 ;
    lcd.setCursor(0,0);
    lcd.print("Set MINUTE Alarm:");
       
    }
 
 
}


 }



void Alarm(){

     DateTime now = RTC.now();
     lcd.clear(); 
     if ( now.hour() == alarmHours && now.minute() == alarmMinutes )
        {
          lcd.print("WAKE UP ASSHOLE...");
         // delay(1000);
          DateTime now = RTC.now();
        

          
      // Set up a counter to pull from melody[] and beats[]
      for (int i=0; i<MAX_COUNT; i++) {
      tone_ = melody[i];
      beat = beats[i];

    duration = beat * tempo; // Set up timing

    playTone(); 
    // A pause between notes...
    delayMicroseconds(pause);         

        }


//THE FOLLOWING CODE TO MAKE THE ALARM STOP IF THE LIGHTS ARE ON

int sensor_value = analogRead(A3);
          if (sensor_value > 350)// the point at which the state of LEDs change 
    { 
              alarmHours = 0;
              alarmMinutes = 0;

     
    }
    


//THE FOLLOWING CODE TO MAKE THE ALARM STOP IF P2 IS PRESSED

  if(digitalRead(P2)==LOW)
    { 

      alarmHours = 0;
      alarmMinutes = 0;
    }

}
}

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration


void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
}
