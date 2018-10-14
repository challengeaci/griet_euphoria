/*THIS TUTORIAL USED GSM SIM900A MINI V3.9.2
 
  Connect 5VT to D9 and 5VR to D10
  Feed GSM SIM900A with Arduino's 5V

  Code by IDAYU SABRI - MYBOTIC
*/
#include<SoftwareSerial.h>
int bluetoothTx = 11;  
int bluetoothRx = 10;  
String data;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
RTC_DS1307 rtc;
SoftwareSerial mySerial(9, 10);
File dataFile;
char msg;
char call;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const int chipSelect = 4;
int i=0;
String dat="";
String phoneno="";
String com="";
String nop="";
String timein="";
String tnop="";

int t,d;
void setup()
{
  bluetooth.begin(9600); 
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
//  Serial.println("GSM SIM900A BEGIN");
//  Serial.println("Enter character for control option:");
//  Serial.println("h : to disconnect a call");
//  Serial.println("i : to receive a call");
//  Serial.println("s : to send message");
//  Serial.println("c : to make a call");  
//  Serial.println("e : to redial");
//  Serial.println();
//  delay(1000);


Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  File dataFile=SD.open("phoneno.txt",FILE_WRITE);
  if (dataFile){
    dataFile.println(phoneno);
    dataFile.close();
    Serial.println("done");
  }
  else{
    Serial.println("error");
  }
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("phoneno.txt");
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
 Serial.print("phoneno");
 i++;
    }
    else if(i==1)
    {
com=save();
Serial.print(com);
Serial.print("com");
i++;
    }
 else if(i==2)
    {
nop=save();
Serial.print(nop);
Serial.print("nop");
i++;
    }
 else if(i==3)
    {
timein=save();
Serial.print(timein);
Serial.print("ti");
i++;
    }
     else if(i==4)
    {
tnop=save();
Serial.print(tnop);
Serial.print("tnop");
i++;
    }
            
 }
  
   ///   Serial.print(data);
      delay(500);
      dat="";

    DateTime now = rtc.now();
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.println("   ");

    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(" ,");
    Serial.print(now.day());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.println(now.year());
    delay(3000); 
    t=now.hour();  
Serial.println(t);
Serial.println(d);
    
    if(t==d){
      MakeCall();
      Serial.println("we are done");
      delay(30000);
    }



  
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'c':
      MakeCall();
      break;
    case 'h':
      HangupCall();
      break;
    case 'e':
      RedialCall();
      break;
    case 'i':
      ReceiveCall();
      break;
  }
 if (mySerial.available()>0)
 Serial.write(mySerial.read());
}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919640112005\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("sim900a sms");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (mySerial.available()>0)
  {
    msg=mySerial.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  mySerial.println("ATD"+data+";"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  mySerial.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  mySerial.println("ATA");
  delay(1000);
  {
    call=mySerial.read();
    Serial.print(call);
  }
}

void RedialCall()
{
  mySerial.println("ATDL");
  Serial.println("Redialing");
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

