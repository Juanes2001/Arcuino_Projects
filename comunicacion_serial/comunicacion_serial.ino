#include <Stepper.h>
#include <Wire.h>
#include <BH1750.h>

#include <iostream>
#include <sstream>
#include <vector>

#define SET   1
#define RESET 0

// Mensajes a mandar

char* help_menu = "\n=========================MENU_COMMANDS=========================\n"
                  "\n///INSERT THE COMMAND YOU WANT BELOW FOLLOWING THE STRUCTURE///\n"
                  "\n///               AND ENDING IT WITH @                      ///\n"
                  "\n          1)  step_motor #steps #dir --> 1 CW, 0 CCW        ///\n" // Anti horario es con pasos negativos
                  "\n          2)  start_measure                                 ///\n";

int IN1 = 19;
int IN2 = 18;
int IN3 = 5;
int IN4 = 17;

// Cantidad de pasos por rev
const int stepsPerRevolution = 2048;
BH1750 lightMeter;
float lux;
int16_t steps = 0;
char c;

// Variables para comandos 
String bufferreception;
bool flag_start_measure = 0;


 // Creamos la clase stepper y un objeto llamado motor
Stepper motor(stepsPerRevolution, IN1,IN3,IN2,IN4);

// initialize the stepper library
void setup() {
    // set the speed at 5 rpm
    motor.setSpeed(5);
    // initialize the serial port
    Serial.begin(9600);
    // Initiacializa el bus I2C (la librer ́ıa BH1750 no lo hace automáticamente)
   while (!Wire.begin()){Serial.print("::"); delay(500);}

   lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2);
    
   while (!lightMeter.begin()) {
      Serial.print(":");
      delay(500);
   }
  // Si llego hasta aca es porque ya se logro inicializar la comunicacion serial.
    Serial.println("BH1750 inicializado");
    delay(1000);

  // Despues de inicializar todo imprimimos el menu de instrucciones
}

void loop() {

  // En esta parte leeremos la comunicacion serial que se envia desde python

    readDatatoSend();


  // En esta seccion se analizara que funcion dependiendo del comando tomado se ejecutará

  if (flag_start_measure){
    
    while (flag_start_measure){}
      do_measurements();
    }  
  }

  delay(500);
}


// Function to process the command
void processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string task;
    int number;
    std::vector<int> numbers;

    // Extract the task identifier
    iss >> task;

    // Extract the numbers following the task identifier
    while (iss >> number) {
        numbers.push_back(number);
    }

    // Process based on task
    if (task == "step_motor") {
      // Si llegamos aqui es porque simplemente queremos mover el motor cierta cantidad de pasos 
      // en sentido horario o sentido antihorario
        std::cout << task << std::endl; 
    } else if (task == "start_measure") {
      // Si llegamos aqui es porque simplemente queremos mover el motor cierta cantidad de pasos 
      // en sentido horario o sentido antihorario
        std::cout << task << std::endl; 
    } else {
        std::cout << "Unknown task." << std::endl;
    }
}


void readDatatoSend(void){

 delay(5);
 while (Serial.available()){ 

    c = Serial.read(); 

    if (c != '\0'){
      if (c != '@'){
        bufferreception += c;
      }else{ 
        // Procesamos el comando ingresado
        processCommand(std::string(bufferreception.c_str()));
      }
    }
  }// Fin del ciclo while  

}


void do_measurements (void){

  // En esta funcion se realizará las mediciones del fotodetector mientras corremos el motor cada 10 pasos
   motor.step(-10);
    lux = lightMeter.readLightLevel();
    Serial.print(lux);
    Serial.print(" ");
    Serial.println(steps);
    steps = steps + 10;
    if (steps == 990) {
      motor.step(990);

      steps = 0;
      flag_start_measure = RESET;
      delay(10000);
    }
}