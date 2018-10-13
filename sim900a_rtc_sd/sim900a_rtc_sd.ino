#include<SoftwareSerial.h>
int bluetoothTx = 9;  
int bluetoothRx = 10;  
String data;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
RTC_DS1307 rtc;
SoftwareSerial mySerial(7,8);
File dataFile;
char msg;
char call;
const int chipSelect = 4;
int i=0;
String dat="";
String phoneno="";
String com="";
String nop="";
String timein="";
String tnop="";
int phoneno1="";
int com1="";
int nop1="";
int timein1="";
int tnop1="";
int t;
int d;
void setup()
{
  analogReference( DEFAULT);
  bluetooth.begin(9600); 
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

    if (! rtc.begin()) 
  {
    Serial.print("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) 
  {
    Serial.print("RTC is NOT running!");
  }
  
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));// to set the time manualy 
}

void loop()
{
 

     while(bluetooth.available()>0)
  {
    delay(500);
    
    if(i==0)
    {
 phoneno=save();
 Serial.print(phoneno);
 filewrite("phoneno.txt",phoneno);
 i++;
    }
    else if(i==1)
    {
com=save();
Serial.print(com);
filewrite("com.txt",com);
i++;
    }
 else if(i==2)
    {
nop=save();
Serial.print(nop);
filewrite("nop.txt",nop);
i++;
    }
 else if(i==3)
    {
timein=save();
Serial.print(timein);
filewrite("timein.txt",timein);
i++;
    }
     else if(i==4)
    {
tnop=save();
Serial.print(tnop);
filewrite("tnop.txt",tnop);
i++;
    }
            
 }
int phoneno1=fileread("phoneno.txt");
int com1=fileread("com.txt");
int nop1=fileread("nop.txt");
int timein1=fileread("timein.txt");
int tnop1=fileread("tnop.txt");
   ///   Serial.print(data);
      delay(500);
      dat="";
    DateTime now = rtc.now();
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.println("   ");
    t=now.hour();  
Serial.println(t);
//Serial.println(d);
    
    if(timein1==t){
      MakeCall();
      Serial.println("we are done");
      delay(30000);
      if(com==1)
      {
        Serial.println("com1 entered");
        digitalWrite(2,HIGH);
        delay(3000);
        digitalWrite(2,LOW);
      }     
    }
}
void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"phoneno\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("sim900a sms");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
void MakeCall()
{
  mySerial.println("ATD"+phoneno+";"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  mySerial.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}
String save()
{
  dat="";
  while(bluetooth.available()>0) 
  {
  
 delay(500);
    char x=bluetooth.read();
     dat=dat+x;
  }  
  return dat; 
}
void filewrite(String nostring,String readata)
{
  File dataFile=SD.open(nostring,FILE_WRITE);
  if (dataFile){
    dataFile.println(readata);
    dataFile.close();
    Serial.println("done");
  }
  else{
    Serial.println("error");
  }
}
 int fileread(String nostring1)
  {
    dataFile = SD.open(nostring1);
  
  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
      d=dataFile.parseInt();
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
return d;
  }
