#include<SoftwareSerial.h>
int bluetoothTx = 11;  
int bluetoothRx = 10;  
 
String data;
 
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
 
void setup()
{
  Serial.begin(9600);  
  bluetooth.begin(9600);  
}
 
void loop()
{
 
  while(bluetooth.available()>0) 
  {
  
 delay(500);
    char x=bluetooth.read();
     data=data+x;
  }
      Serial.print(data);
      delay(500);
      data="";
    
}
  
  

      
    
  
 


