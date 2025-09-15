
#define SET   1
#define RESET 0

String help_menu = "\n=================================COMANDOS===================================\n"
                   "///INSERTAR EL COMANDO CON LA SIGUIENTE ESTRUCTURA QUE SE ENCUENTRA ABAJO///\n"
                   "///               Y TERMINARLA CON EL SIGUIENTE CARACTER @               ///\n"
                   "///             1)  led #1-> ON, 0-> OFF                                 ///\n"
                   "///             2)  message msg                                          ///\n"
                   "///             3)  set_pwm  #0->255                                     ///\n";

int pin_led = 12;
int pin_analog = 11;



// Variables para comandos 
String bufferreception = "";

void readDatatoSend(void);
void processCommand(String command);


// initialize the stepper library
void setup(){
  // initialize the serial port
  Serial.begin(115200);

  Serial.print(help_menu);  

  delay(3000);



  // Despues de inicializar todo imprimimos el menu de instrucciones
}

void loop(){

  // En esta parte leeremos la comunicacion serial que se envia desde python
  readDatatoSend();

}


// Function to process the command
void processCommand(String input) {

    String task = "";
    String Sparam1 = "";
    int param1 = 0;
    int param2 = 0;
    int space_index;
    
    space_index = input.indexOf(' ');

    if (space_index == -1){
      task = input;
    }else{
      task = input.substring(0,space_index);
      Sparam1 = input.substring(space_index + 1);

      space_index = Sparam1.indexOf(' ');

      if (space_index == -1){

      }else{
        param1 = Sparam1.substring(0,space_index).toInt();
        param2 = Sparam1.substring(space_index+1).toInt();
      }

    }
    
    if (task == "message"){
         Serial.print("Message received: Arduino Speaking: ");
         Serial.print(Sparam1);
         Serial.print("\n");
      
    }else if (task == "led") {

      if (param1 == 1){
        digitalWrite(pin_led, HIGH);
        Serial.print("LED ON\n");
      }else{
        digitalWrite(pin_led, LOW);
        Serial.print("LED OFF\n");
      }
     
    }else if (task == "set_pwm"){
      analogWrite(pin_analog, param1);
      Serial.print("Dutty changed\n");
    }else{
        Serial.println("Non a command");
    }
    bufferreception = "";
}

void readDatatoSend(void){
  while (Serial.available()){ 

    bufferreception += Serial.readString();

    if (bufferreception.indexOf('@') == -1){

    }else{
      bufferreception.replace("@","");
      processCommand(bufferreception); 
    } 
    // Procesamos el comando ingresado
    //Serial.print(c);
    //processCommand(c);   
  }// Fin del while  
}