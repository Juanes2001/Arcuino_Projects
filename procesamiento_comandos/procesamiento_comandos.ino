
#define SET   1
#define RESET 0

String help_menu = "\n=================================COMANDOS===================================\n"
                   "///INSERTAR EL COMANDO CON LA SIGUIENTE ESTRUCTURA QUE SE ENCUENTRA ABAJO///\n"
                   "///               Y TERMINARLA CON EL SIGUIENTE CARACTER @               ///\n"
                   "///                           1)  Do_sweep                               ///\n";


const int analogPinA0 = A0;
const int analogPinA1 = A1; 
const int sampleDelay = 20; 
const int time_waiting = 100;
const uint8_t pin_signal1 = 3;
const uint8_t pin_signal2 = 5;

uint8_t val = 0;
uint8_t wait = 0;

long sum = 0;          // variable to accumulate samples
float Volt_base_avg = 0; 
double Base_current = 0 ;
float Voltage_CE_avg = 0;
double colector_Current = 0; 

const long 1M_res = 994000;
const int 220_res = 214;





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

  pinMode(pin_signal1,OUTPUT);
  pinMode(pin_signal2,OUTPUT);
  
  analogReference(EXTERNAL);



  // Despues de inicializar todo imprimimos el menu de instrucciones
}

void loop(){

  // En esta parte leeremos la comunicacion serial que se envia desde python
  readDatatoSend();

}

// Esta opción permite crear un mini osciloscopio usando la interfaz del computador.
/*void Osciloscope (void){

  for (int i = 0; i < 400; i++) { 
    
    int value = analogRead(analogPin); 
    
    // Convert to voltage 
    
    float voltage = value * (5.0 / 1023.0); 
    
    Serial.println(voltage); 

    Serial.print(voltage);
    Serial.print(" ");
    Serial.print(0);    // lower limit
    Serial.print(" ");
    Serial.println(5);  // upper limit

    
    delayMicroseconds(sampleDelay); 
  } 

}*/

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
      
    }else if (task == "Do_sweep"){
        
        /* Si se llego hasta aca es porque se hará un sweep para 5 curvas distintas del transistor 2N2222A, en donde se medira con A0 el voltaje en la base del transistor, conociendo el dutty
            cycle para el signal que esta ligado a A0, podremos conocer el voltaje que se encuentra en la salida del amplificador operacional, (existe un limite superior en donde el voltaje ya no sube mas, 
             para evitar equivocaciones averiguar bien cual es el threshold superior e inferior para cada caso y hacer un sweep adecuado para los valores en donde existe cambio.). 
             COn esto se pueden conocer con buena precision la corriente de base.

             Comenzar con un valor fijo de voltaje a la entrada (en donde a partir de si cambia el voltaje a la salida del OPAM) Con esto podemos hacer una primera medida para medir la corriente de base y tener referenciada
             la curva que se le va  a hacer un sweep.

             Luego comenzar a aumentar la señal de la otra salida signal de tal forma que por cambio se mida la corriente del colector con el mismo metodo, con una sola medida se podra saber tanto la corriente de colector como el voltaje colector
             emisor. Finalmente se hace el sweep hasta el valor threshold en donde no se puede aumentar mas el voltaje  del signal relacionado con A1 (determinar el signal y la entrada analoga relacionada.)
        
        */

        for (uint8_t val_sig_3 = 0 /*Recordar cambiar a un valor en donde a partir del dutty siguiente, cambie en el OPAM*/; val_sig_3 <255; val_sig_3 += 255/5){

            // Primera medicion, medimos la corriente de base

            for (int i = 0; i < 100; i++) {
              sum += analogRead(analogPinA0) * (3.3/1023);   // read ADC and accumulate
            }

            Volt_base_avg = sum / 100;

            Base_current = ((5.0 * val_sig_3/255) - (Volt_base_avg))/1M_res

            for (uint8_t val_sig_5 = 0; val_sig_5 <255; val_sig_5++){
                
                sum = 0;

                // Primera medicion, medimos la corriente de base

                for (int i = 0; i < 100; i++) {
                  sum += analogRead(analogPinA1) * (3.3/1023);   // read ADC and accumulate
                }

                Voltage_CE_avg = sum / 100;

                colector_Current = ((5.0 * val_sig_5/255) - (Voltage_CE_avg))/220_res

                pair = [Base_current,Voltage_CE_avg,colector_Current]

                Serial.println(pair);
          
        }


        }



    }
    
    
    /*else if (task == "Osciloscope"){
      if (param1 == 1){
        flag_osci = SET;
        while(flag_osci){
          Osciloscope();
          readDatatoSend();  
        }
      }else{
        flag_osci = RESET;
      }
    }*/
    
    
    
    else{
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