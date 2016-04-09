#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_Simple_AHRS.h>

// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>

// XBee's DOUT (TX) is connected to pin 3 (Arduino's Software RX) as 3 pin on shield
// XBee's DIN (RX) is connected to pin 4 (Arduino's Software TX) as 4 pin on shield
SoftwareSerial XBee(3, 4); // RX, TX

// Create sensor instances.
Adafruit_LSM303_Accel_Unified accel(30301);
Adafruit_LSM303_Mag_Unified   mag(30302);
Adafruit_BMP085_Unified       bmp(18001);

// Create simple AHRS algorithm using the above sensors.
Adafruit_Simple_AHRS          ahrs(&accel, &mag);

// Update this with the correct SLP for accurate altitude measurements
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

//send buffer for the XBee
char sendBuffer[40];
String tempBuffer;

void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  
  Serial.begin(9600);
  Serial.println(F("Adafruit 10 DOF Board AHRS Example")); Serial.println("");
  
  // Initialize the sensors.
  accel.begin();
  mag.begin();
  bmp.begin();
  
}

void loop(void)
{
  sensors_vec_t   orientation;

  // Use the simple AHRS function to get the current orientation.
  if (ahrs.getOrientation(&orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
	
	//Creation of the send buffer 
    tempBuffer = "Orientation: ";
    tempBuffer += String(orientation.roll) + " ";
    tempBuffer += String(orientation.pitch) + " ";
    tempBuffer += String(orientation.heading) + char(10);
    tempBuffer.toCharArray(sendBuffer,40);
    
	//Send data
	XBee.write(sendBuffer);

	//Print to serial monitor for testing
    //Serial.print(sendBuffer);

  }
  
  //delay(100);
}
