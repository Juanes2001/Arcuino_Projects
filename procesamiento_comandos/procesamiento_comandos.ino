
#define SET   1
#define RESET 0

/*String help_menu = "\n=================================COMANDOS===================================\n"
                   "///HAZ CLICK EN EL BOTON CON EL NOMBRE DEL COMANDO PARA MANDAR LA ORDEN CON LA SIGUIENTE ESTRUCTURA QUE SE ENCUENTRA ABAJO///\n"
                   "///                              1)  Do_sweep                               ///\n";
*/

const int analogPin_base = A0;
const int analogPin_colector = A1; 
const int sampleDelay = 20; 
const int time_waiting = 100;
const uint8_t pin_signal_base = 3;
const uint8_t pin_signal_colector = 5;

uint8_t val = 0;
uint8_t wait = 0;

double sum = 0;          // variable to accumulate samples
float Volt_base_avg = 0; 
double Base_current = 0 ;
float Voltage_CE_avg = 0;
double colector_Current = 0; 

const long _1M_res = 948000;
const int _220_res = 214;

const float max_volt = 3.3; // Maximo posible por medición del ADC
const int max_dutty = 173;
const float ref_volt = 4.7;






// Variables para comandos 
String bufferreception = "";

void readDatatoSend(void);
void processCommand(String command);
void Do_sweep(void);


// initialize the stepper library
void setup(){
  // initialize the serial port
  Serial.begin(115200);

  //Serial.print(help_menu);  

  delay(3000);

  pinMode(pin_signal_base,OUTPUT);
  pinMode(pin_signal_colector,OUTPUT);
  
  //analogReference(EXTERNAL);

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

        Do_sweep();



    }
    
    
    
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


void Do_sweep(void){

    for (uint8_t val_sig_base = 0 /*Recordar cambiar a un valor en donde a partir del dutty siguiente, cambie en el OPAM*/; val_sig_base <max_dutty; val_sig_base += int(max_dutty/5)){

            sum = 0;
            // Primera medicion, medimos la corriente de base
            analogWrite(pin_signal_base, val_sig_base);  // la señal en la base

            for (int i = 0; i < 100; i++) {
              sum += analogRead(analogPin_base) * (4.7/1023);   // read ADC and accumulate
            }

            Volt_base_avg = sum / 100;

            Base_current = ((ref_volt * val_sig_base/255) - (Volt_base_avg))*1000000/_1M_res; // uA

            for (uint8_t val_sig_colector = 0; val_sig_colector < max_dutty; val_sig_colector++){
                
                sum = 0;

                analogWrite(pin_signal_colector, val_sig_colector); // la señal en el colector

                // Primera medicion, medimos la corriente de base

                for (int i = 0; i < 100; i++) {
                  sum += analogRead(analogPin_colector) * (4.7/1023);   // read ADC and accumulate
                }

                Voltage_CE_avg = sum / 100;

                colector_Current = ((ref_volt * val_sig_colector/255) - (Voltage_CE_avg))*1000/_220_res; //mA

                Serial.print(Volt_base_avg,5); //uA
                Serial.print(" ");
                Serial.print(Voltage_CE_avg,5); //V
                Serial.print(" ");
                Serial.print(Base_current,5); //uA
                Serial.print(" ");
                Serial.print(Voltage_CE_avg,5); //V
                Serial.print(" ");
                Serial.println(colector_Current,5); //mA

                delay(200);
          
            }

            Serial.println("||");  


        }



}