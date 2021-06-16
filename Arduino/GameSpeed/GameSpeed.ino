uint8_t potValue = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  uint8_t newPotValue = analogRead(2)/16;
  if (newPotValue != potValue) {
    potValue = newPotValue;
    Serial.write(potValue);
    Serial.flush();
    }
}
