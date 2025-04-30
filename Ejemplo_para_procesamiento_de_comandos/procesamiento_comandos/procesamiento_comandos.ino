
#define SET   1
#define RESET 0

String help_menu = "\n=================================COMANDOS===================================\n"
                   "///INSERTAR EL COMANDO CON LA SIGUIENTE ESTRUCTURA QUE SE ENCUENTRA ABAJO///\n"
                   "///               Y TERMINARLA CON EL SIGUIENTE CARACTER @               ///\n"
                   "///             1)  led #1-> ON, 0-> OFF                                 ///\n"
                   "///             2)  message msg                                          ///\n"
                   "///             3)  set_pwm  #0->255                                     ///\n||";

int pin_led = LED_BUILTIN;
int pin_analog = 11;



// Variables para comandos 
String bufferreception;

void readDatatoSend(void);
void processCommand(String command);


// initialize the stepper library
void setup(){
  // initialize the serial port
  Serial.begin(9600);

  Serial.print(help_menu);  

  delay(3000);


  pinMode(pin_led, OUTPUT);
  pinMode(pin_analog, OUTPUT);

  // Despues de inicializar todo imprimimos el menu de instrucciones
}

void loop(){

  // En esta parte leeremos la comunicacion serial que se envia desde python
  readDatatoSend();

}


// Function to process the command
void processCommand(String input) {

    //Serial.print(input);

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

      //Serial.print(task);
      //Serial.print(Sparam1);
      
      if (task == "message"){
         Serial.print("Message received: Arduino Speaking: ");
         Serial.print(Sparam1);
         Serial.print("\n");
      }else{
        space_index = Sparam1.indexOf(' ');

        if (space_index == -1){
          param1 = Sparam1.toInt();

        }else{
          param1 = Sparam1.substring(0,space_index).toInt();
          param2 = Sparam1.substring(space_index+1).toInt();
        }
      }

    }
    //Serial.print(task);
    //Serial.print(param1);
    // Process based on task
    if (task == "led") {

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
    }else {
        Serial.println("Non a command");
    }

}

void readDatatoSend(void){
  String c = "";
 while (Serial.available()){ 

    c = Serial.readString();

    if (c.indexOf('@') == -1){

    }else{
      c.replace("@","");
      processCommand(c); 
    } 
    // Procesamos el comando ingresado
    //Serial.print(c);
    //processCommand(c);   
  }// Fin del while  
}