// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// ADXL345
// This code is designed to work with the ADXL345_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=ADXL345_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// ADXL345 I2C address is 0x53(83)
#define Addr 0x53

int xAccl = 0, yAccl =  0, zAccl = 0;
void setup()
{   
  // Set variable
  Particle.variable("i2cdevice","ADXL345");
  Particle.variable("xAccl",xAccl);
  Particle.variable("yAccl",yAccl);
  Particle.variable("zAccl",zAccl);
  
  // Initialise I2C communication as MASTER 
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Begin transmission with given device on I2C bus
  Wire.beginTransmission(Addr);
  // Select bandwidth rate register
  Wire.write(0x2C);
  // Select output data rate = 100 Hz
  Wire.write(0x0A);
  // End transmission and release I2C bus
  Wire.endTransmission();

  // Begin transmission with given device on I2C bus
  Wire.beginTransmission(Addr);
  // Select power control register
  Wire.write(0x2D);
  // Select auto sleep disable
  Wire.write(0x08);
  // End transmission and release I2C bus
  Wire.endTransmission();

  // Begin transmission with given device on I2C bus
  Wire.beginTransmission(Addr);
  // Select data format register
  Wire.write(0x31);
  // Select full resolution, +/-2g
  Wire.write(0x08);
  // End transmission and release I2C bus
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  int data[6];
  for(int i = 0; i < 6; i++)
  {
    // Begin transmission with given device on I2C bus
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((50+i));
    // End transmission and release I2C bus
    Wire.endTransmission();

    // Request 1 byte of data from the device
    Wire.requestFrom(Addr,1);
    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if(Wire.available()==1)
    {
      data[i] = Wire.read();
    }
  delay(300);
  }

  // Convert the data
  xAccl = ((data[1] & 0xFF) * 256) + (data[0] & 0xFF);
  if (xAccl > 32767)
  {
    xAccl -= 65536;
  }

  yAccl = ((data[3] & 0xFF) * 256) + (data[2] & 0xFF);
  if (yAccl > 32767)
  {
    yAccl -= 65536;
  }

  zAccl = ((data[5] & 0xFF) * 256) + (data[4] & 0xFF);
  if (zAccl > 32767)
  {
    zAccl -= 65536;
  }

  // Output data to dashboard
  Particle.publish("Acceleration in X-Axis is :", String(xAccl));
  Particle.publish("Acceleration in Y-Axis is :", String(yAccl));
  Particle.publish("Acceleration in Z-Axis is :", String(zAccl));
}

