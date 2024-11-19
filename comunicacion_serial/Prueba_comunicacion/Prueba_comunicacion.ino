String msg;
char c;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  Serial.println(F(">> Initialized System")); 
}

void loop() {
  // put your main code here, to run repeatedly:
  readDatatoSend();
}


void readDatatoSend(void){

 delay(10);
 while (Serial.available()){ 

    if (Serial.available()){
      c = Serial.read(); 
      msg += c;
    }
  }  

  if (msg.length() > 0){
    Serial.println(">> " + msg);
    msg = "";
  }

}
