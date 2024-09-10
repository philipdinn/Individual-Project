int cambien = 5;
int giatri;
int giatrimacdinh = 1;
int dem;

void setup() {
  Serial.begin(9600);

  pinMode(cambien, INPUT);

}

void loop() {
  giatri= digitalRead(cambien);
  if(giatri != giatrimacdinh)
  {
    if (giatri ==0)
    {
      dem++;
    }
    giatrimacdinh = giatri;
  }
  Serial.print("Gia tri cam bien là:   ");
  Serial.print(giatri);
  Serial.print("       ");
  Serial.print("so dem:   ");
  Serial.println(dem);
  Serial.println("       ");
  delay(200);

}
