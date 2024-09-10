///////////////////////////////////111111111111111111111111111111111111111111111111111111111111111
#include <HX711.h>
#include <LiquidCrystal.h>

#define MOTORSPEED 6
#define MOTORA A1
#define MOTORB A2

#define BUTTON A3

const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;

#define PULSEPERROTATE 400

unsigned long PulseCount=0;
unsigned long TimeSave=0;
unsigned long TimeCount=0;
unsigned long Speed;

unsigned long CheckTime=0;

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal LCDDisplay(rs, en, d4, d5, d6, d7);

HX711 MyScale;

 unsigned int WeightValue;

byte Mode=0; 

void Stop()
{
  analogWrite(MOTORSPEED,0);
  digitalWrite(MOTORA,LOW);
  digitalWrite(MOTORB,LOW);
  Mode=0;
}

void Run1()
{
  analogWrite(MOTORSPEED,100);
  digitalWrite(MOTORA,HIGH);
  digitalWrite(MOTORB,LOW);
  Mode=1;
}
void Run2()
{
  analogWrite(MOTORSPEED,150);
  digitalWrite(MOTORA,HIGH);
  digitalWrite(MOTORB,LOW);
  Mode=2;
}
void Run3()
{
  analogWrite(MOTORSPEED,200);
  digitalWrite(MOTORA,HIGH);
  digitalWrite(MOTORB,LOW);
  Mode=3;
}
void Run4()
{
  analogWrite(MOTORSPEED,255);
  digitalWrite(MOTORA,HIGH);
  digitalWrite(MOTORB,LOW);
  Mode=4;
}


void CountPulse()
{
  PulseCount++;
}

void ScaleStartUp()
{
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(MyScale.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(MyScale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(MyScale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(MyScale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)

}

void DisplayMain()
{
  LCDDisplay.clear();
  LCDDisplay.print("TD:    v/p   M: ");
  LCDDisplay.setCursor(0,1);
  LCDDisplay.print("CAN NANG:    g");
}

void DisplayMode()
{
  LCDDisplay.setCursor(15,0);
  LCDDisplay.print(Mode);
}

void DisplaySpeed()
{
  LCDDisplay.setCursor(3,0);
  if(Speed<1000) LCDDisplay.print(" ");
  if(Speed<100) LCDDisplay.print(" ");
  if(Speed<10) LCDDisplay.print(" ");
  LCDDisplay.print(Speed);
}

void DisplayWeight()
{
  LCDDisplay.setCursor(9,1);
  if(WeightValue<1000) LCDDisplay.print(" ");
  if(WeightValue<100) LCDDisplay.print(" ");
  if(WeightValue<10) LCDDisplay.print(" ");
  LCDDisplay.print(WeightValue);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(2, INPUT_PULLUP); 
  pinMode(MOTORSPEED,OUTPUT);
  pinMode(MOTORA,OUTPUT);
  pinMode(MOTORB,OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);
  
  Stop();
  LCDDisplay.begin(16,2);
  DisplayMain();
  DisplayMode();
  attachInterrupt(0, CountPulse, RISING);

  MyScale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  ScaleStartUp();
  MyScale.set_scale(1700.f); //5kg
  MyScale.tare();                // reset the scale to 0
  
  TimeSave=millis();
  CheckTime=millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:
   TimeCount=millis()-TimeSave;
   if(TimeCount>2000)
   {
    Speed=PulseCount*1000*60/400/TimeCount;
    Serial.print("TOC DO:");
    Serial.println(Speed);
    PulseCount=0;
    TimeSave=millis();
    DisplaySpeed();
   }

   if(millis()-CheckTime>500)
   {
     WeightValue=MyScale.get_units(10);
     if(WeightValue>1000) WeightValue=0;
     Serial.print("Gia tri: ");
     Serial.println(WeightValue);
     CheckTime=millis();
     DisplayWeight();
   }

   if(digitalRead(BUTTON)==LOW)
   {
    delay(100);
    while(digitalRead(BUTTON)==LOW);
    if(Mode==0)
    {
      Mode=1;
      Run1();
    }
    else if(Mode==1)
    {
      Mode=2;
      Run2();
    }
    else if(Mode==2)
    {
      Mode=3;
      Run3();
    }
    else if(Mode==3)
    {
      Mode=4;
      Run4();
    }
    else if(Mode==4)
    {
      Mode=0;
      Stop();
    }
    DisplayMode();
   }
}
