int pin_LED_blinky = LED_BUILTIN;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin_LED_blinky, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  digitalWrite(pin_LED_blinky, HIGH);
  Serial.println("LED ON");
  delay(500);
  digitalWrite(pin_LED_blinky, LOW);
  Serial.println("LED OFF");
}
