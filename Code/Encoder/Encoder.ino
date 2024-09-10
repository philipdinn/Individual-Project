// int count = 0;
// int interrupt0 = 0;
// int phaseA = 2;
// int phaseB = 3;


// void setup() {
//   pinMode (phaseA, INPUT_PULLUP);
//   pinMode (phaseB, INPUT_PULLUP);


// }

// void loop() {
//   // put your main code here, to run repeatedly:

// }

/////////////////////////////////

volatile unsigned int temp, counter = 0; 
volatile unsigned int xung = 0;   

void setup() {
  Serial.begin (9600);
  pinMode(2, INPUT_PULLUP); // pin 2 
  pinMode(3, INPUT_PULLUP); //  pin 3
  attachInterrupt(0, ai0, RISING);
  attachInterrupt(1, ai1, RISING);
  }   
  void loop() 
  {
    if( counter != temp )
    {
    Serial.println (counter);
    temp = counter;
    }
  }  
  
  void ai0() {
  if(digitalRead(3)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {

  if(digitalRead(2)==LOW) {
  counter--;
  xung--;
  }else{
  counter++;
  }
  }
