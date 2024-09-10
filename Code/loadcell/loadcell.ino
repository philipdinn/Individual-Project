#include <Q2HX711.h>
const byte hx711_data_pin = 2;
const byte hx711_clock_pin = 3;
long zero = 0;
long kl;

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

void setup() {
  Serial.begin(9600);
  Serial.println("Hieu chinh zero");
  Serial.println("Vui long khong di chuyen can");
  for(int i=0; i<100; i++){
    zero = zero + (hx711.read()/100);
  }
  zero=zero/100;
  Serial.println("Hieu chinh thanh cong");
  Serial.println("Tien hanh can");
  Serial.println(zero);
  delay(500);

}

void loop() {
  kl = ((hx711.read()/100)-zero)/3.8;
  Serial.println(kl);
 Serial.println("g");

}
//0g 84172
//10g 84210

