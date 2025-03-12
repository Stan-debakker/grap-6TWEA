#define knop_rood  3
#define knop_zwart 2

void setup() {
  Serial.begin(115200);
  pinMode(knop_zwart,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(knop_zwart),zwarte_knop,CHANGE);
  pinMode(knop_rood,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(knop_rood),rode_knop,CHANGE);
}

void loop() {}

void zwarte_knop(){
  if (!digitalRead(knop_zwart)){
    Serial.println("de zwarte knop is ingedrukt");
  }else{
    Serial.println("de zwarte knop is niet ingedrukt");
  }
}
void rode_knop(){
  if (!digitalRead(knop_rood)){
    Serial.println("de rode knop is ingedrukt");
  }else{
    Serial.println("de rode knop is niet ingedrukt");
  }
}