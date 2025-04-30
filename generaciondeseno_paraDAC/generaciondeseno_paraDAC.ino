int pinDAQ = 26;
double time_var = 0;
double sine = 0; 

long currentTime;
double loopDuration;
int lastTime;
double loopFrequency;


void setup() {
 
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(pinDAQ, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
/*
  currentTime = micros();                      // Get current time in microseconds
  loopDuration = currentTime - lastTime;       // Calculate time between loops
  lastTime = currentTime;

  loopFrequency = 1000000.0 / loopDuration;    // Frequency in Hz (since micros() is in µs)

  Serial.print("Loop duration (us): ");
  Serial.print(loopDuration);
  Serial.print(" | Frequency (Hz): ");
  Serial.println(loopFrequency);
*/
 
  sine = (((1 + 1*cos(2*PI*10*time_var))*0.5*cos(2*PI*100*time_var)) + 1.5)*(255/3.3);
  //sine = (sin(2*PI*20*time_var) + 1.5)*(255/3.3);

  time_var+= 0.0002;

  dacWrite(pinDAQ, sine);

  Serial.print(sine);
  Serial.print(" ");
  Serial.println(time_var);
}
