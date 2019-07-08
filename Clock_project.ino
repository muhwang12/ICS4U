/* Title: Arduino Weather Alarm Clock
   Course: ICS4U
   Name: Matthew Wang
   Teacher: Mr. Creelman */   

#include <LiquidCrystal.h>
#include <Wire.h>
#include "DHT.h"
#include <RTClib.h>

//LCD display
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

RTC_DS3231 RTC;

//Button variables
const int P1 = 8; //Alarm On/ Off
bool alarm; //true = On, false = Off
bool clicked_p1;

const int P2 = 9; //12 hour/ 24 hour format
bool format; //false = 12 hour format, true = 24 hour format
bool clicked_p2;

const int P3 = 10; //Changes bottom display information
int option; //0 = Date, 1 = Alarm, 2 = Temperature, 3 = Humidity

//Button on A0 = changes alarm values
//Button on A1 = confirms alarm values
int process; /*Stage in alarm setting process 0 = default, 1 = hours, 2 = minutes (tens), 
               3 = minutes (ones)*/

bool selectAlarm;

int alarmHour, alarmMin;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


#define buzz 6
#define LED 13

//Temperature/ Humidity Sensor variables
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#define TEMPTYPE 0 //Temperature °Celsius

void setup() {

  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  pinMode(P1, INPUT_PULLUP);
  pinMode(P2, INPUT_PULLUP);
  pinMode(P3, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(buzz, OUTPUT);
  
  
  lcd.begin(16, 2);
  RTC.adjust(DateTime(__DATE__, __TIME__));

  option = 0;
  format = false;
  clicked_p1 = false;
  clicked_p2 = false;
  selectAlarm = false;
  alarmHour = 0;
  alarmMin = 0;
  process = 0;
}

void loop() {

  DateTime now = RTC.now();
  
  lcd.setCursor(0, 0);
  DisplayTime();

  lcd.setCursor(0, 1);
  changeDisplay();
  
  checkButtons();

  //If Alarm is on
  if(digitalRead(P1) == LOW)
    Alarm();
  
}

void DisplayTime()
{
  DateTime now = RTC.now();
  
  lcd.setCursor(0, 0);
  lcd.print("Time: ");

  twentyfourTotwelve(now.hour());
  
  lcd.print(":");
  
  print2digits(now.minute());

  lcd.print(":");
  
  print2digits(now.second());

  period(now.hour());
}

void DisplayDate()
{
   DateTime now = RTC.now();
  
   lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
   lcd.print(" ");
   print2digits(now.day());
   lcd.print("/");
   print2digits(now.month());
   lcd.print("/");
   lcd.print(now.year(), DEC);
}

//Shows the temperature
void DisplayTemperature()
{
  int t = dht.readTemperature(TEMPTYPE); // reads temperature
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print((char)223);
  lcd.print ("C");
}

//Shows the humidity
void DisplayHumidity()
{
  int h = dht.readHumidity(); // reads humidity
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print("% ");
}

//Shows the time set for the alarm
void DisplayAlarm()
{
  lcd.print("Alrm: ");
  twentyfourTotwelve(alarmHour);
  lcd.print(":");
  print2digits(alarmMin);
  
  period(alarmHour);

  if(alarm){
    lcd.setCursor(14, 1);
    lcd.print("ON");
    delay(100);
  }
}

//Alarm system that checks current time
void Alarm()
{
  DateTime now = RTC.now();
  if(now.hour() == alarmHour && now.minute() == alarmMin){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("ALARM ON");

    //If the alarm is on
    while(digitalRead(P1) == LOW){
      tone(buzz,1000);
      digitalWrite(LED, HIGH);
      delay(300);
      digitalWrite(LED, LOW);
      //If the minute has passed
      if(!(now.hour() == alarmHour && now.minute() == alarmMin))
        break;
    }
    noTone(buzz);
    digitalWrite(LED, LOW);
  }
}
//Larger function that sets the alarm time with the other functions relevant to it
void setAlarm()
{
  int alarmTensMin = (alarmMin-(alarmMin % 10))/ 10;
  int alarmOnesMin = alarmMin % 10;
  
  while(process < 4){
  setAlarmHour();
  delay(100);
  setAlarmMinute(alarmTensMin);
  delay(100);
  setAlarmMinute(alarmOnesMin);
  alarmMin = alarmTensMin * 10 + alarmOnesMin;
  }
}
//For setting the hours of the alarm
void setAlarmHour()
{
  lcd.clear();
  while(process == 1){
    if(digitalRead(A0) == LOW){
      if(alarmHour == 23)
        alarmHour = 0;
      else
        alarmHour++;
       delay(200);
    }
      lcd.setCursor(0, 0);
      lcd.print ("Set HOURS: ");
      lcd.setCursor(0, 1);

      twentyfourTotwelve(alarmHour);
      period(alarmHour);
    if(digitalRead(P3) == LOW)
      break;
  }
  process++;
  delay(500);
}
//For setting both the minutes (tens) and the minutes (ones) for the alarm
void setAlarmMinute(int &minutes)
{
  lcd.clear();
  while(process > 1){
    if(digitalRead(A0) == LOW){
      if(process == 2 && minutes == 5){
          minutes = 0;
      }
      else if(process == 3 && minutes == 9)
        minutes = 0;
      else
        minutes++;
      delay(200);
    }
      lcd.setCursor(0, 0);
      lcd.print("Set MINS");
      if(process == 2)
        lcd.print(" (TENS):");
      else if(process == 3)
        lcd.print(" (ONES):");
      
      lcd.setCursor(0, 1);
      lcd.print(minutes, DEC);
      
      if(digitalRead(P3) == LOW)
        break; 
  }
  process++;
  if(process == 4){
      lcd.clear();
      selectAlarm = false;
  }
  delay(500);
}
//Checks whether buttons have been pressed
void checkButtons()
{
  //Set alarm time
  if(digitalRead(A0) == LOW && digitalRead(P3) == LOW){
    lcd.clear();
    selectAlarm = true;
    process++;
    delay(500);
    setAlarm();
  }
  if(!selectAlarm){
    process = 0;
    selectAlarm = false;
    //Alarm On/Off
    if(digitalRead(P1) == LOW){
      alarm = true;
      clicked_p1 = true;
    }
    //Refresher for smooth removal of text
    else if(digitalRead(P1) == HIGH && clicked_p1){
      lcd.clear();
      alarm = false;
      clicked_p1 = false;
    }
    else{
      alarm = false;
    }
    //Refresher for smooth removal of text
    if(digitalRead(P2) == LOW && clicked_p2){
      format = true;
    }
    //Switch between 12-hour and 24-hour clock
    else if(digitalRead(P2) == LOW){
      format = true;
      clicked_p2 = true;
      lcd.clear();
    }
    else{
      format = false;
      clicked_p2 = false;
    }

    //Display information changer
    if(digitalRead(P3) == LOW){
      if(option == 3)
        option = 0;
      else
        option++;
      delay(200);
      lcd.clear();
    }
  }
}

//Changes display information based on if its respective button was pressed
void changeDisplay()
{
  switch(option){

    case 1: DisplayAlarm();
            break;

    case 2: DisplayTemperature();
            break;

    case 3: DisplayHumidity();
            break;

    default: DisplayDate();
             break;
  }
}

//Converts hour formats whenever it is changed
void twentyfourTotwelve(int hours)
{
  if(format)
    print2digits(hours);
  else{
    if(hours == 0 || hours == 12 || hours==24)
      lcd.print("12");
    else
      print2digits(hours % 12);
  }
}

//Detects whether hours are in AM or PM in 12 hour format
void period(int hours)
{
  if(!format){
    if(hours > 12 && hours != 24)
      lcd.print("PM");
    else
      lcd.print("AM"); 
  }
}

//Prints a 0 before the number if the number is < than 10
void print2digits(int number) {
  if (number < 10) {
    lcd.print("0"); 
  }
  
 lcd.print(number, DEC);
}


