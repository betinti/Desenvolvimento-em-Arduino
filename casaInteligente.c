//  BIBLIOTECAS
#include <SoftwareSerial.h>
#include <Wire.h>

// COMUNICAÇÃO
#define rxPin 5
#define txPin 4
SoftwareSerial master = SoftwareSerial(rxPin,txPin);

// LAMPADA
const int pL = 6;
int estadoLampada = 0;

// MOTOR
const int pM = 13;
int vel = 0;

// GLOBAIS
int estado = 0;

enum
{
  INICIALIZANDO,
  RECEBE,
  OPERA_MOTOR,
  OPERA_LUZ
} mode = INICIALIZANDO;

void setup()
{
  //Iniciando a comunicação
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    master.begin(9600);
//    Wire.begin(device);
  //Iniciando leitura da LAMPADA
    pinMode(pL, OUTPUT);
  //Iniciando leitura do MOOTR
    pinMode(pM, OUTPUT);
  //Iniciando o terminal
    Serial.begin(9600);
    digitalWrite(pL, estadoLampada);
}

// Funcoes operacionais //

char intTOchar(int a){
  switch (a){
      case 1:
          return '1';

      case 2:
          return '2';

      case 3:
          return '3';

      case 4:
          return '4';

      case 5:
          return '5';

      case 6:
          return '6';

      case 7:
          return '7';

      case 8:
          return '8';

      case 9:
          return '9';

      case 0:
          return '0';
  }
}

int charTOint(char a){
    switch (a){
        case '1':
            return 1;

        case '2':
            return 2;

        case '3':
            return 3;

        case '4':
            return 4;

        case '5':
            return 5;

        case '6':
            return 6;

        case '7':
            return 7;

        case '8':
            return 8;

        case '9':
            return 9;

        case '0':
            return 0;
    }
}

void confere(){
  switch (estado) {
    case 0:
      estadoLampada = 0;
      vel = 0;
      break;
    case 2:
      estadoLampada = 1;
      vel = 0;
      break;
    case 3:
      estadoLampada = 0;
      vel = 100;
      break;
    case 4:
      estadoLampada = 1;
      vel = 100;
      break;
  }
}


void recebe()
{
  Serial.println("RECEBE");

  master.listen();
  while (!master.isListening())
    { Serial.print(':'); delay(100); }

  while (!(master.available()) > 0)
    { Serial.print('#'); delay(100); }
	Serial.println(" ");
  delay(1000);
  int cont = 0;
  while (cont < 5) {
    cont++;
    char c = master.read();
    Serial.print('[');
    Serial.print(c);
    Serial.print(']');

   Serial.print(" (ainda disponivel: ");
   Serial.print(master.available());
   Serial.println(')');
   estado = charTOint(c);

  }
  Serial.println(estado);
}

void operaLampada(){
    Serial.println("Opera Lampada");
    digitalWrite(pL, estadoLampada);
}

void OperaMotor(){
  Serial.println("Opera Motor");
  digitalWrite(pM, vel);
}

void trata(){
  //if (estado != 0|| estado != 2 || estado != 3 || estado != 4) {estado = 0;}
}

// Funcoes de estados //

void begin(){
  Serial.print("INICIALIZANDO");
  delay(5000);
  mode = RECEBE;
}

void recebendo(){
  Serial.println("RECEBE");
  recebe();
  trata();
  confere();
  mode = OPERA_MOTOR;
}

void operaMotor(){
  Serial.println("OPERA_MOTOR");
  OperaMotor();
  mode = OPERA_LUZ;
}

void operaLuz(){
  Serial.println("OPERA");
  operaLampada();
  mode = RECEBE;
}

void loop()
{
  switch (mode)
  {
  case INICIALIZANDO:
    begin();
    break;
  case RECEBE:
    recebendo();
    break;
  case OPERA_MOTOR:
    operaMotor();
    break;
  case OPERA_LUZ:
    operaLuz();
    break;
  }
}
