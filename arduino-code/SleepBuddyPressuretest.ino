#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX

const int P1 = A0;    //Piezo
const int P2 = A1;    //Piezo
const int P3 = A2;    //Pressure sensor
const int vari = 100; 
int PinA[vari] = {0};  //Array to store Piezo P1 measurements
int PinB[vari] = {0};  //Array to store Piezo P2 measurements
int counter = 0;
float SumA = 0;
float SumB = 0;
float AvgA = 0.0;
float AvgB = 0.0;
float MinA = 0.0;
float MaxA = 0.0;
float MinB = 0.0;
float MaxB = 0.0;
int A = 0;
int B = 0;
float DifA = 0.0;
float DifB = 0.0;
float pre = 0.0; //placeholder for pressure-sensor
volatile bool con = false;
bool Start = false;

unsigned long previousMillis=0;
unsigned long currentMillis=0;
unsigned long interval = 0;
float m = 0;
float h = 0;

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(P1, INPUT);
  pinMode(P2, INPUT);
  pinMode(P3, INPUT);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), concheck, CHANGE);
  concheck();
}
void concheck()
{
  con = !con;
  int val = digitalRead(3);
  if(con == false && val == HIGH)
  {
    con = true;
  }
}
void calc()
{
  SumA = 0;
  AvgA = 0;
  SumB = 0;
  AvgB = 0;
  for(int j=0; j<vari; j++)
  {
    SumA = SumA + PinA[j];
    SumB = SumB + PinB[j];
  }
  AvgA = (float)SumA/vari;
  AvgB = (float)SumB/vari;
  DifA = MaxA-MinA;
  DifB = MaxB-MinB;
  if(DifA > 100)
  {
    DifA = 100;
  }
  if(DifB > 100)
  {
    DifB = 100;
  }
}
void Header()
{
  Serial.print("Time(ms)");
  Serial.print(" TS(ms)");
  Serial.print(" Time(min)");
  Serial.print(" Time(h)");
  Serial.print(" MinA");
  Serial.print(" MaxA");
  Serial.print(" MinB");
  Serial.print(" MaxB");
  Serial.print(" DifA");
  Serial.print(" DifB");
  Serial.print(" AvgA");
  Serial.println(" AvgB");
}
void piezo()
{
  for(int i=0; i<vari; i++)
  {
    A = analogRead(P1);
    B = analogRead(P2);
    if(i == 0)
    {
      MaxA = A;
      MinA = A;
      MaxB = B;
      MinB = B;
    }
    PinA[i] = A;
    PinB[i] = B;
    MinA = min(MinA, PinA[i]);
    MaxA = max(MaxA, PinA[i]);
    MinB = min(MinB, PinB[i]);
    MaxB = max(MaxB, PinB[i]);
    delay(10);
  }
}
void serial()
{
  Serial.print(previousMillis);
  Serial.print(" ");
  Serial.print(interval);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.print(h,5);
  Serial.print(" ");
  Serial.print(MinA);
  Serial.print(" ");
  Serial.print(MaxA);
  Serial.print(" ");
  Serial.print(MinB);
  Serial.print(" ");
  Serial.print(MaxB);
  Serial.print(" ");
  Serial.print(DifA);
  Serial.print(" ");
  Serial.print(DifB);
  Serial.print(" ");
  Serial.print(AvgA);
  Serial.print(" ");
  Serial.println(AvgB);
}
void Pres()
{
  pre = analogRead(P3);
}
void BTserial()
{
  BTSerial.print(pre);
  BTSerial.print(" ");
  BTSerial.print(DifA);
  BTSerial.print(" ");
  BTSerial.println(DifB);
}
void Begin()
{
  if (BTSerial.available())
  {
    String line = BTSerial.readStringUntil('\r');
    if(line == "Start")
    {
      Start = true;
    }
    else if(line == "Stop")
    {
      Start = false;
    }
  }
}
void Time()
{
  interval = currentMillis-previousMillis;
  previousMillis = currentMillis;
  m = (float)previousMillis/1000/60;
  h = (float)m / 60;
}
void loop() 
{
  while(Start == false)
  {
    Begin();
  }
  while(con == true && Start == true)
  {
    
    if (counter < 1)
    {
      Header();
      counter++; 
      currentMillis = millis();
    }
    Time();    
    piezo();
    calc();
    Pres();
    BTserial();
    serial();
    currentMillis = millis();
    Begin();
  }
}
