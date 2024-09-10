 int nutnhan =2;
int in1 = 7;
int in2 = 8;
int ena = 9;
int dem = 0;
int chedo = 0;
int pre = LOW;

void setup()
{
Serial.begin(9600);
pinMode(nutnhan,INPUT);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(ena,OUTPUT);
}

void chedo0()
{
  analogWrite(ena,0);
  chedo=0;
}
void chedo1()
{
  analogWrite(ena,150);
  chedo=1;
}
void chedo2()
{
  analogWrite(ena,200);
  chedo=2;
}
void chedo3()
{
  analogWrite(ena,200);
  chedo=3;
}
void chedo4()
{
  analogWrite(ena,255);
  chedo=4;
}
void chedo5()
{
  dem=0;
}

void loop()
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  Serial.print("Che do:   ");
  Serial.println(chedo);
  delay(200);
  
  int docnutnhan = digitalRead(nutnhan);
  if(docnutnhan==HIGH && pre==LOW)
  {
    dem=dem+1;
    delay(100);     //chống nhiễu
  }
  pre=docnutnhan;    //để chống giữ nút nhấn

  if(dem==0)
  {
    chedo0();
  }
  else if(dem==1)
  {
    chedo1();
  }
  else if(dem==2)
  {
    chedo2();
  }
    else if(dem==3)
  {
    chedo3();
  }
    else if(dem==4)
  {
    chedo4();
  }
    else if(dem==5)
  {
    chedo5();
  }
}