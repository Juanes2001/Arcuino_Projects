
double analog_value;
int analog_pin = A0;
double voltage_value;

int pin_led = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(pin_led, OUTPUT);
}



void loop() {
  // put your main code here, to run repeatedly:

  analog_value = analogRead(analog_pin);

  // Son 10 bits con Vref 0 5V.
  voltage_value = analog_value*5/1023;

  if (voltage_value >= 3.3){
    digitalWrite(pin_led, HIGH);
  }else{
    digitalWrite(pin_led, LOW);
  }

  Serial.println(voltage_value);
  delay(200);

}





