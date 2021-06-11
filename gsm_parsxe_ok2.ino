#include <SoftwareSerial.h> //Incluimos la libreria Software Serial

String Numero = "5511223344"; //Incamos el numero que controlara el dispositivo

SoftwareSerial SIM800L(2,3); //Instanciamos el objeto SIM800L y le pasamos los parametros de los pines TX y RX
String valor;  //Declaramos la variable de tipo String valor.
void setup() {
  pinMode(13, OUTPUT); //Declarar el pin 13 como salida.
  Serial.begin(9600); //Inicializamos la primera comunicacion Serial.
  SIM800L.begin(9600); //Inicializamos la segunda comunicacion Serial.
  SIM800L.println("AT+CMGF=1"); //Vamos utilizar los SMS.
  delay(100); //Delay de 0.1 sec
  SIM800L.println("AT+CNMI=1,2,0,0,0"); //Configurar el SIM800L p/ que muestre msm por com. serie.
}

void loop() {
  //Lograr que nos muestre lo que nos llega de mensaje por el monitor serial.
  if(SIM800L.available()){
    valor = SIM800L.readString(); //Guardar en la var valor el sms que recibe el Arduino
    Serial.println("Nuevo SMS: "+ valor); //Imprime ese SMS en el monitor Serial
    validar();
  } 
}
/*Esta funcion "Validar" verificara que el numero que envio el mensaje coincida con el numero definido en la variable Numero*/
void validar(){
  if(valor.indexOf(Numero)>=0){//Si coincide con el numero controlador hara lo siguiente:
    Serial.println("EL numero concuerda"); //Mostrar un mensaje que diga "el numero concuerda"
    
    if(valor.indexOf("On")>=0){  //Si la var valor tiene la palabra ON hace esto:
      digitalWrite(13, HIGH);    //Se enciende el pin 13.
      Serial.println("Led encendido");  //Immprime el mensaje
      enviarMensaje("Led encendido");
      delay(2000);
    }
    else if(valor.indexOf("Off")>=0){//Si la var valor tiene la palabra Off hace esto:
      digitalWrite(13, LOW);    //Se apaga el pin 13.
      Serial.println("Led apagado");  //Immprime el mensaje
      enviarMensaje("Led apagado");
      delay(2000);
    }
    else{//Si es cualquier otro mensaje
      Serial.println("Es otro mensaje");
    }
  }
  else{//Esto pasara si el numero es diferente al establecido en la variable Numero
    Serial.println("El numero no coincide");
  }
}

void enviarMensaje(String msj){
  String comando_numero = "AT+CMGS=\"+52" + Numero + "\"\r\n";
  Serial.println(comando_numero);
  SIM800L.print(comando_numero);
  delay(1000);
  SIM800L.print(msj);
  delay(1000);
  //SIM800L.write(char(0x1A));
  SIM800L.write(char(26));
  delay(1000);
}
