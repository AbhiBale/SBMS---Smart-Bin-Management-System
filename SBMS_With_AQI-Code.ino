#include <Servo.h>   //including servo library

//for appOpenClose()
//Components used Bluetooth Module,SBMS App, Servo motor
String voice;

//for aqivalue()
int sensorvalue;

//for binStatus()
//Components used Ultrasonic Sensor 1,Bluetooth Module,SBMS App
int Limit = 25.5; //height limit of dust-bin in cm
const int trigPin1 = 7;
const int echoPin1 = 8;
long duration, percentage, range;

//for lidOpen()
//Components used Ultrasonic Sensor 2,Servo Motor
Servo myservo;
int cm = 0;
int trigPin2 = 9;
int echoPin2 = 10;
long distance;



void lidOpen()
{
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);  
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    distance = pulseIn(echoPin2, HIGH);
    cm = 0.01723 * distance;
    delay(100);
    if(cm<=20)
    {
        myservo.write(180);
        delay(2000);
    }
    else
    {
        myservo.write(0);
        delay(10);
    }
}

void binStatus()
{
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);  
    digitalWrite(trigPin1, HIGH);  
    delayMicroseconds(10);  
    digitalWrite(trigPin1, LOW);
    duration = pulseIn(echoPin1, HIGH);
    percentage = (duration / 2) / 29.1 / Limit * 100;
    range = 100 - percentage;

    if ((range >= 0) && (range <= 100))
    {
        if (range > 70) 
        {
            Serial.print("F ");
        }
        if ((range >= 30) && (range <= 70))
        {
            Serial.print("M ");
        }
        if (range < 30)
        {
            Serial.print("E ");
        }
        Serial.print(range);
        Serial.println();
        delay(700);
    }
    else
    {
        Serial.print("F 100");
        Serial.println();
        delay(500);
    }
}
void appOpenClose()
{
    while (Serial.available())
    { 
        delay(10); 
        char c = Serial.read(); 
        voice += c; 
    }  
    if (voice.length() > 0)
    { 
        if(voice == "O")
        {
            myservo.write(180);
            delay(2000);
        }  
        if(voice == "C")
        {
            myservo.write(0);
            delay(10);
        }
        voice="";
    }
}

void aqivalue()
{
sensorValue = analogRead(0); // read analog input pin 0
  if(sensorValue<370)
  {
   Serial.print("G ");
   Serial.println(sensorValue, DEC);
  }
  else if(sensorValue<450)
  {
   Serial.print("D ");
   Serial.println(sensorValue, DEC);
  }
  else
  {
   Serial.print("P ");
   Serial.println(sensorValue, DEC);
  }
  delay(500); // wait 100ms for next reading
}

void setup()
{
    //for appOpenClose
    Serial.begin(9600);

    //Status of Bin
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    
    //Analog Pin for MQ-135 Sensor
    pinMode(A0, INPUT);

    //Opening Lid using Ultrasonic Sensor
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    myservo.attach(6);
}
void loop() 
{
    lidOpen();     //Opening Lid using Ultrasonic Sensor
    
    binStatus();   //Status of Bin
     
    appOpenClose();//AppOpenClose
     
    aqivalue();    //AQI Value
    
}
