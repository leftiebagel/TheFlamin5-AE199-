#include <SPI.h> // Library for communicating with SPI devices a.k.a. the SD board
#include <SD.h>  // Library for easy writing/reading from SD card
#include <Adafruit_Sensor.h> // Adafruit unified sensor library https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_BMP280.h> // BMP280 sensor library https://github.com/adafruit/Adafruit_BMP280_Library
#include <Wire.h> // Library for communicating with I2C devices (I2C is a protocol for sending data over two wires between many devices)

const int chipSelect = 4; // define which pin the CS pin is connected to
Adafruit_BMP280 bme; 
/*
 * Initializes a BMP280 object (Creates a data structure representing the sensor so the program can keep
 * track of important information about the sensor as well as establish an initial connection to, kind of
 * like saying hello to the BMP280 chip.
 */
unsigned long time;

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");
  Serial.println(F("BMP280 test"));

  if (!bme.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("New Recording");

    /********Change the Header to Reflect all the data you are logging!*********/
    String header = "Miliseconds, Temperature, Pressure, Alt 1, Alt 2, Alt 3";
    /***************************************************************************/
    
    dataFile.println(header);
    Serial.println(header);
    dataFile.close();
  }
}

void loop() {


  // make a string for assembling the data to log:
  
  String dataString = "";

  time = millis();

  // This adds the time to the data String to be recorded.
    // To Read the Temperature: bme.readTemperature();
  // To Read the Pressure:    bme.readPressure();

  /*****Your Code Here! Add the temperature and pressure to the datalog******/
  float pressure = bme.readPressure(); 
  float temperature = bme.readTemperature(); 
  
  /**************************************************************************/


  /*****add values to the dataString*****/
  dataString = dataString + String(time) + ", "; //time 
  dataString = dataString + bme.readTemperature() + ", "; //temp
  dataString = dataString + bme.readPressure() + ", "; //pressure
  dataString = dataString + alt_pres_special(pressure) + ", "; // alt 1 - 3 
  dataString = dataString + alt_pres(pressure)+ ", ";
  dataString = dataString + alt_temp(temperature); 
  
  /*************************************************************************/
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
    delay(100);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}


//Helpers 
float alt_temp (float temp) {
    return (-154.0832 * temp) + 2317.411; 
}

float alt_pres(float pressure) { 
    return (-0.087284 * pressure) + 8863.654;
}

float alt_pres_special(float pressure) {
    float expo = -44383.67 * pow(pressure / 101290, 1 / 5.265);
    return 44405.24 + expo; 
} 
