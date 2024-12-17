#include <Stepper.h>
#include <Wire.h>
#include <BH1750.h>

#include <iostream>
#include <sstream>
#include <vector>

#define SET   1
#define RESET 0

const char* help_menu = "\n=================================COMANDOS===================================\n"
                        "\n///INSERTAR EL COMANDO CON LA SIGUIENTE ESTRUCTURA QUE SE ENCUENTRA ABAJO///\n"
                        "\n///               Y TERMINARLA CON EL SIGUIENTE CARACTER @               ///\n"
                        "\n///             1)  step_motor #steps #dir --> 1 CW, 0 CCW               ///\n" // Anti horario es con pasos negativos
                        "\n///             2)  start_measurements                                   ///\n"
                        "\n///             3)  on_lux                                               ///\n";

int cuentas = 3;

// Mensajes a mandar
int IN1 = 19;
int IN2 = 18;
int IN3 = 5;
int IN4 = 17;

// Cantidad de pasos por rev
const int stepsPerRevolution = 2048;

BH1750 lightMeter;
float lux;
float lux_medida;


int counter = 0;
int16_t steps = 0;
int pasos_a_mover = 10;
int pasos_max = 1500;
char c;

// Variables para comandos 
String bufferreception;

int pasos_a_dar = 0;
bool direccion = RESET;

//banderas varias
bool flag_step_motor = RESET;
bool flag_start_measure = RESET;
bool flag_on_lux = RESET;
bool received_by_python = RESET;

void readDatatoSend(void);
void do_measurements (void);
void processCommand(const std::string& command);
void show_lux(bool flag);

 // Creamos la clase stepper y un objeto llamado motor
Stepper motor(stepsPerRevolution, IN1,IN3,IN2,IN4);

// initialize the stepper library
void setup() {
  // initialize the serial port
  Serial.begin(115200);
  // set the speed at 5 rpm
  motor.setSpeed(5);

    
    // Initiacializa el bus I2C (la librer ́ıa BH1750 no lo hace automáticamente)
  
   while (!Wire.begin()){}

   lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2);
    
   while (!lightMeter.begin()) {}
   
  // Si llego hasta aca es porque ya se logro inicializar la comunicacion serial.
  delay(3000);

  Serial.print(">> SISTEMA INICIALIZADO\n");


  // Despues de inicializar todo imprimimos el menu de instrucciones
}

void loop() {

  // En esta parte leeremos la comunicacion serial que se envia desde python
  lux_medida = lightMeter.readLightLevel();
  readDatatoSend();


  // En esta seccion se analizara que funcion dependiendo del comando tomado se ejecutará
  
  if (flag_start_measure){
    while (flag_start_measure){do_measurements();delay(500);}

    std::cout << "\nFin del comando... start_measurements\n" << "--||--\n";
    received_by_python = RESET;

  }else if (flag_on_lux){

    while (counter < cuentas ){
      show_lux(flag_on_lux);
      received_by_python = RESET;
      while (!received_by_python){readDatatoSend();delay(500);} 
      
    }
    std::cout << "\nFin del comando... on_lux\n" << help_menu << "\nMANDADO" << "--||--\n";
    received_by_python = RESET;
    flag_on_lux = RESET;
    counter = 0;
  }
  else if (flag_step_motor){
    if (direccion == 1){
      motor.step(pasos_a_dar);
    }else{
      motor.step((-1) * pasos_a_dar);
    }
    flag_step_motor = RESET;
    std::cout << "\nFin del comando... step_motor\n";
    std::cout << help_menu << "--||--\n" << std::endl;
    received_by_python = RESET;
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
        flag_step_motor = SET; 
        pasos_a_dar = numbers[0]; 
        direccion = numbers[1];

        std::cout << "\nComando procesado..."<< task << "\n" ;
        std::cout << "Pasos a dar: " << pasos_a_dar << "\n";
        if(direccion == 1){
          std::cout << "Dirección: " << "Horario\n"<< "\n--||--\n"  <<std::endl;
        }else{
          std::cout << "Dirección: " << "Anti Horario\n" << "\n--||--\n"<< std::endl; 
        }
        bufferreception = "";
        while (!received_by_python){readDatatoSend();delay(500);}
    }else if (task == "start_measurements") {

      // Aqui se ejecutara la rutina para hacer la medicion en tiempo real y la graficacion de los datos.
      std::cout << "\nComando procesado..."<< task << "\n" ;
      std::cout << "||||MODO DE LECTURA CONTINUA ACTIVADO|||||"<< "--||--\n"  << std::endl;
      flag_start_measure = SET; 
      bufferreception = "";
      while (!received_by_python){readDatatoSend();delay(500);}


    }else if (task == "on_lux") {
      // Si estamos aqui se busca que el luxometro me regale cierta cantidad de datos cada cierto tiempo
      std::cout << "\nComando procesado..."<< task << "\n" ;
      std::cout << "|||||MOSTRANDO LUX, "<< cuentas << " MUESTRAS|||||"<< "--||--\n"  << std::endl;
      flag_on_lux = SET;
      bufferreception = "";
      while (!received_by_python){readDatatoSend();delay(500);}

    }else if (task == "INICIO") {
      // Este es el comando de inicializacion, con esto podemos saber que la comunicacion serial se realizo correctamente y que el
      // Python y el ESP32 estan conectados correctamente. Se enviara el menu de opciones.
      std::cout << help_menu << std::endl; 
      bufferreception = "";
    }else if (task == "RECIBIDO_step_motor" || 
              task == "RECIBIDO_start_measurements" || 
              task == "RECIBIDO_on_lux"){
        if (task == "RECIBIDO_step_motor") {received_by_python = SET;}
        else if (task == "RECIBIDO_start_measurements"){received_by_python = SET;}
        else if (task == "RECIBIDO_on_lux"){received_by_python = SET;}

        bufferreception = "";
    }else {
        std::cout << "\nTarea desconocida...Corrija su comando\n" << "--||--\n" << std::endl;
        bufferreception = "";
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
  steps = steps + pasos_a_mover;
  motor.step((-1) * pasos_a_mover);
  lux = lightMeter.readLightLevel();
  Serial.print(lux);
  Serial.print(" ");
  Serial.println(steps);
  if (steps == pasos_max) {
    std::cout << "\nTERMINADO\n"  << std::endl;
    motor.step(pasos_max);
    steps = 0;
    flag_start_measure = RESET;
  }
}


void show_lux(bool flag){
  //   Con esta funcion solo mostraremos el estado de la lux real
  if (flag){
    lux_medida = lightMeter.readLightLevel();
    std::cout << lux_medida << " lux\n" << "MANDADO" << std::endl;
    counter++;
  }
}