#include <Wire.h>

byte pixelTempL;
//byte pixelTempH;
char addr = 0x68;
float celsius;
float aveTemp;
float AvgTemp;
  int i = 0;
  int serialInit = 0;
  int serialint = 0;

//int irqPin = 2;
//int pwmPin = 3;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  //pinMode(irqPin, INPUT);
  //pinMode(pwmPin, OUTPUT);

}

void loop() {

  while(serialInit == 0) 
  {
    if(Serial.available()){
      serialint = Serial.parseInt();
      serialInit = 1;
      //Check for start of Message
      if(serialint == 12345){       
        Serial.print("54321");
        //Clear Message bytes
        serialint = 0;
        //Let the PC know we are ready for more data
        Serial.println("-READY TO RECEIVE");
      }
    }
  }

  while(!(Serial.available())); //wait until computer sends us a signal to send again
  int garbage = Serial.read();

    //Serial.print("Grid-EYE:\r\n");
    
    pixelTempL = 0x80;
    aveTemp = 0;
    AvgTemp = 0;
    
    for(int pixel = 0; pixel < 64; pixel++){
      Wire.beginTransmission(addr);
      Wire.write(pixelTempL);
      Wire.endTransmission();
      Wire.requestFrom(addr, 2);
      byte lowerLevel = Wire.read();
      byte upperLevel = Wire.read();
  
  /*    
      Wire.beginTransmission(addr);
      Wire.write(pixelTempH);
      Wire.endTransmission();
      Wire.requestFrom(addr, 1);
      byte upperLevel = Wire.read();
  */
  
      int temperature = ((upperLevel << 8) | lowerLevel);
  //    int temperature = temp - 2048;
      if (temperature > 2047){
        temperature = temperature - 4096;
  //      temperature = -(2048 - temperature);
      }
      
      celsius = temperature * 0.25; //.38;
      
      Serial.print(celsius);
      Serial.print("\n");
      
      AvgTemp += celsius;
      
      pixelTempL = pixelTempL + 2;
    }
    
    Wire.beginTransmission(addr);
    Wire.write(0x0E);
    Wire.endTransmission();
    Wire.requestFrom(addr, 2);
    byte upperLevelTherm = Wire.read();
    byte lowerLevelTherm = Wire.read();
    
  /*
    Wire.beginTransmission(addr);
    Wire.write(0x0F);
    Wire.endTransmission();
    Wire.requestFrom(addr, 1);
    byte lowerLevelTherm = Wire.read();
  */
  
    int temperatureTherm = ((lowerLevelTherm << 8) | upperLevelTherm);
    
    float celsiusTherm = temperatureTherm * 0.0625;
    
    //Serial.print(celsiusTherm);
   // Serial.print("\r\n");
    
    aveTemp *= 0.25;
    //Serial.print(aveTemp);
    
    delay(2000);
}
