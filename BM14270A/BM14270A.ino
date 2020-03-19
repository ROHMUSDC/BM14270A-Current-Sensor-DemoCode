// March 18, 2020 3:30 PM  Kris Bahar, Arun Kumar, Barry Tigner, Mitch Van Ochten
#include <Wire.h>

int CurrentSensorAddress = 0x0e; // I2c address for the current sensor
//Address is controlled by ADDR pin L = 0x0E, H = 0x0F

byte Data0,Data1;
int i;
float SensorData;
int Offset = 30;
int Gain = 120;
int myData = 0;
float myAvg = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  delay(100);

  Wire.beginTransmission(CurrentSensorAddress);
  Wire.write(0x1B); //CNTL1 = 0x80
  Wire.write(0x80); // Bit 7 = Power Control, 0 = Power Down, 1 = Active
  Wire.endTransmission();

  Wire.beginTransmission(CurrentSensorAddress);
  Wire.write(0x5C); //CNTL4 = 0x00
  Wire.write(0x00); // RSTB_LV[7:0] are Reserved bits
  Wire.endTransmission();

  Wire.beginTransmission(CurrentSensorAddress);
  Wire.write(0x5D); // CNTL4 = 0x00
  Wire.write(0x00); // RSTB_LV[15:8] Logic release control (don't understand)
  Wire.endTransmission();

  Wire.beginTransmission(CurrentSensorAddress);
  Wire.write(0x1C); // CNTL2 = 0x08
  Wire.write(0x08); // Bit 3 = ALERT_EN, if = 1 then Pin 16 = RD_DRDY signal
  Wire.endTransmission();

  Wire.beginTransmission(CurrentSensorAddress);
  Wire.write(0x1D); // CNTL3 = 0x40
  Wire.write(0x40); // Bit 6 = FORCE, set = 1 to Start Measurement, automatically clears
  Wire.endTransmission();
}

void loop() {
  // put your main code here, to run repeatedly:
myAvg = 0;
for (i=0;i<16;i++){
  Wire.beginTransmission(CurrentSensorAddress);
  Wire.write(0x10); //first address to read
  Wire.endTransmission();

  Wire.requestFrom(CurrentSensorAddress,2);
  myData = 0;
  if(Wire.available()>0) {
     Data0 = Wire.read();  
     Data1 = Wire.read();
     myData = Data1 << 8;
     myData = myData + Data0;
     } 
  myAvg = myAvg + myData ;  
}

delay(500);
SensorData =  myAvg/16 + Offset; //Correct zero reading
SensorData = SensorData/Gain;   // Scale gain as needed

String myresult = String(SensorData,1); //One decimal point precision
Serial.println(myresult);

}
