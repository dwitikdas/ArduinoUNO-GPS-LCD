/*
 Global Positioning System (GPS) & Displaying the coordinates on LCD
  The circuit connections:
 * LCD RS pin to digital pin A2
 * LCD Enable pin to digital pin A4
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * GPS Pin TX (Data Transfer) to digital pin 6
 * GPS Pin RX (Data Receiver) to digital pin 7 
 * Ends to +5V and ground
 The Original Program can be found in Internet of Things Blog
 The Libraries were found in GitHub Repositories
 The Program is modified by DWITI KRUSHNA DAS
 */

#include <SoftwareSerial.h> //Including the SoftwareSerial Library
#include <LiquidCrystal.h>  //Including the LCD Library

#include <TinyGPS.h>       // Including the TinyGPS Library for the GPS from local folder

TinyGPS gps;  // defining a funtional varible

int unoRxPin = 6; // connected to TX pin of the GPS
int unoTxPin = 7; // connected to RX pin of the GPS
SoftwareSerial ss(unoRxPin, unoTxPin);    // Putting the RX & TX Pins to digital pins through library files

LiquidCrystal lcd(A2, A4, 9, 10, 11, 12);

long startMillis;     // Defining the time taken to Search the signal
long secondsToFirstLocation = 0; // Initializing the searching time to 0 sec

void setup()
{
  Serial.begin(9600);   // Initializing the Serial Monitor
  ss.begin(9600);   // Initializing the LCD
  lcd.begin(16,2); // Columns, Rows
  lcd.clear();  // start with a blank screen
  startMillis = millis();     // Initializing the Time taken to search Signal
}

void loop()
{
  bool newData = false;   // Getting the present status of the Data recieved
  unsigned long chars = 0;
  unsigned short sentences, failed;

  // For one second GPS data is parsed and some key values is reported
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())    // Checking if the data is received
    {
      int c = ss.read();    // Reading the data if available
      ++chars;
      if (gps.encode(c)) // reading & verifying the new data
        newData = true;
    }
  }

  if (newData)
  {
    // We have a locked location so output the Latitude / Longitude and time to acquire the data
    if(secondsToFirstLocation == 0){
      secondsToFirstLocation = (millis() - startMillis) / 1000;
    }
    
    lcd.clear();  // Start LCD with a blank screen
    
    float flat, flon;   // defining floating variable for isolating the Lat & Long from the Raw Data
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    lcd.setCursor(0,0);         // set cursor to column 0, row 0 (the first row)
    lcd.print("Lat=");
    lcd.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6); // Printing Latitude on LCD
    lcd.setCursor(0,1);         // set cursor to column 0, row 1 (the second row)
    lcd.print("Long=");
    lcd.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);  // Printing Longitude on LCD
  /*
    lcd.setCursor(0,2);
    lcd.print("Acquire Time=");
    lcd.print(secondsToFirstLocation); // Printing the Time elapsed to acquire the location on LCD
    lcd.print("s");
    */
  }
  
  if (chars == 0){
    // Error Message, in case no data is received or wrong connection
    lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
    lcd.print("No GPS: Check Wiring!");
  }
  else if(secondsToFirstLocation == 0){
    // If it has received some chars but not yet got a fix then it'll indicate searching and elapsed time
    lcd.clear();  // start with a blank screen
    long seconds = (millis() - startMillis) / 1000;     // counting the seconds elapsed
    lcd.setCursor(0,0);           // set cursor to column 0, row 0 (the first row)
    lcd.print("Searching ");
    for(int i = 0; i < seconds % 4; ++i){
      lcd.print(".");
    }
    
    lcd.setCursor(0,1);
    lcd.print("Elapsed time:");
    lcd.print(seconds);
    lcd.print("s");
  }
}

