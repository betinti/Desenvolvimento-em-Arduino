#include <Adafruit_NeoPixel.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define PIN 12
#define NUMPIXELS 12
#define rxPin 3
#define txPin 2

// Setup inicial dos estados do elevador //

enum
{
  SETUP,
  ESPERANDO,
  MARCANDOJOGADA,
  AVALIANDO_JOGADA,
  FIM
} mode = SETUP;

//  Vars GLOBAIS //

//  TRANFERENCIA
SoftwareSerial mySerial = SoftwareSerial(rxPin,txPin);

//  FITA LED
int numeroLED = 12;
int portaLED = 4;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// KEYPAD
char customKey;
const byte ROWS = 4;
const byte COLS = 4;

int keyWire1 = 4;
int keyWire2 = 5;
int keyWire3 = 6;
int keyWire4 = 7;
int keyWire5 = 8;
int keyWire6 = 9;
int keyWire7 = 10;
int keyWire8 = 11;

char keys[ROWS][COLS] = {
  {'1','2','3','_'},
  {'4','5','6','_'},
  {'7','8','9','_'},
  {'R','_','_','_'}
};
byte rowPins[ROWS] = {keyWire8,keyWire7,keyWire6,keyWire5};
byte colPins[COLS] = {keyWire4,keyWire3,keyWire2,keyWire1};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);



//  Gerais
int vez = 0;
char jogada;
int fita[12];
const int PLAYER = 2;
int device = 4;
int resultado;

// Funcoes operacionais //

void changeVez ()
{
    if (vez == 1)
    {
      vez = 2;
      if(PLAYER == vez)
      {
        ligaLed(11, 0);
      }
    }
    if (vez == 2)
    {
      vez = 1;
      if(PLAYER == vez)
      {
        ligaLed(11, 1);
    }
  }
}

void ligaLed(int porta, int cor)
{
  if(cor == 0)
  {
      pixels.setPixelColor(porta,pixels.Color(0,255,0)); pixels.show();
  }

  if(cor == 1)  //vermelho
  {
      pixels.setPixelColor(porta,pixels.Color(255,0,0)); pixels.show();
  }

  if(cor == 2)  //azul
  {
      pixels.setPixelColor(porta,pixels.Color(0,0,255)); pixels.show();
  }
  if (cor == -1)
  {
    pixels.setPixelColor(porta,pixels.Color(0,0,0)); pixels.show();
  }
  if (cor == -2)
  {
    pixels.setPixelColor(porta,pixels.Color(255,255,255)); pixels.show();
  }
}

bool cheio()
{
    int cont = 0;
    for(int i = 0; i < 9; i++)
    {
      if(fita[i] != 0)
      {
        cont++;
      }
    }
    if(cont == 9)
    {
      return true;
    }
    return false;
}

bool vermelhoVenceu()
{
  if(fita[0] == 1 && fita[1] == 1 && fita[2] == 1){
      return true;
  }
  else if (fita[3] == 1 && fita[4] == 1 && fita[5] == 1)
  {
      return true;
  }
  else if (fita[6] == 1 && fita[7] == 1 && fita[8] == 1)
  {
      return true;
  }
  else if (fita[0] == 1 && fita[3] == 1 && fita[6] == 1)
  {
      return true;
  }
  else if (fita[1] == 1 && fita[4] == 1 && fita[7] == 1)
  {
      return true;
  }
  else if (fita[2] == 1 && fita[5] == 1 && fita[8] == 1)
  {
      return true;
  }
  else if (fita[0] == 1 && fita[4] == 1 && fita[8] == 1)
  {
      return true;
  }
  else if (fita[2] == 1 && fita[4] == 1 && fita[6] == 1)
  {
      return true;
  }
  return false;
}

bool azulVenceu()
{
  if(fita[0] == 2 && fita[1] == 2 && fita[2] == 2)
  {
      return true;
  }
  else if (fita[3] == 2 && fita[4] == 2 && fita[5] == 2)
  {
      return true;
  }
  else if (fita[6] == 2 && fita[7] == 2 && fita[8] == 2)
  {
      return true;
  }
  else if (fita[0] == 2 && fita[3] == 2 && fita[6] == 2)
  {
      return true;
  }
  else if (fita[1] == 2 && fita[4] == 2 && fita[7] == 2)
  {
      return true;
  }
  else if (fita[2] == 2 && fita[5] == 2 && fita[8] == 2)
  {
      return true;
  }
  else if (fita[0] == 2 && fita[4] == 2 && fita[8] == 2)
  {
      return true;
  }
  else if (fita[2] == 2 && fita[4] == 2 && fita[6] == 2)
  {
      return true;
  }
  return false;
}

void escolhe_player()
{
  if(vez == 1)
  {
    vez = 2;
    ligaLed(11, vez);
  }
  else if (vez == 2)
  {
    vez = 1;
    ligaLed(11, vez);
  } else {
    vez = random(1,2);
    ligaLed(11, vez);
  }
}

void marcarJogada()
{
 switch (jogada){
  case '1':
    if(fita[0] == 0){ ligaLed(0, vez);    fita[0] = vez; }
    break;
  case '2':
    if(fita[1] == 0){ ligaLed(1, vez);    fita[1] = vez; }
    break;
  case '3':
    if(fita[2] == 0){ ligaLed(2, vez);    fita[2] = vez; }
    break;
  case '4':
    if(fita[3] == 0){ ligaLed(4, vez);    fita[3] = vez; }
    break;
  case '5':
    if(fita[4] == 0){ ligaLed(5, vez);    fita[4] = vez; }
    break;
  case '6':
    if(fita[5] == 0){ ligaLed(6, vez);    fita[5] = vez; }
    break;
  case '7':
    if(fita[6] == 0){ ligaLed(8, vez);    fita[6] = vez; }
    break;
  case '8':
    if(fita[7] == 0){ ligaLed(9, vez);    fita[7] = vez; }
    break;
  case '9':
    if(fita[8] == 0){ ligaLed(10, vez);   fita[8] = vez; }
    break;
  }
}


int avaliar()
{

  if(vermelhoVenceu())
  {
    ligaLed(3,1);
    Serial.println("Jogador Vermelho venceu.");
    return 0;
  }

  if(azulVenceu())
  {
    ligaLed(3,2);
    Serial.println("Jogador Azul venceu.");
    return 1;
  }
  if(cheio())
  {
    ligaLed(7,0);
    Serial.println("Deu Velha! Tentem novamente.");
    return 2;
  }
  return -1;
}

void clearPlayArea()
{
  ligaLed(0, -2); ligaLed(0, -1); fita[0] = 0;
  ligaLed(1, -2); ligaLed(1, -1); fita[1] = 0;
  ligaLed(2, -2); ligaLed(2, -1); fita[2] = 0;
  ligaLed(3, -2); ligaLed(3, -1); fita[3] = 0;
  ligaLed(4, -2); ligaLed(4, -1); fita[4] = 0;
  ligaLed(5, -2); ligaLed(5, -1); fita[5] = 0;
  ligaLed(6, -2); ligaLed(6, -1); fita[6] = 0;
  ligaLed(7, -2); ligaLed(7, -1); fita[7] = 0;
  ligaLed(8, -2); ligaLed(8, -1); fita[8] = 0;
  ligaLed(9, -2); ligaLed(9, -1); fita[9] = 0;
  ligaLed(10, -2); ligaLed(10, -1); fita[10] = 0;
  ligaLed(11, -2); ligaLed(11, -1); fita[11] = 0;

  Serial.println("Area de jogo limpa, se preparem para a proxima rodada.");
}

void envia()
{
  Serial.println("ENVIANDO");
  char key = keypad.getKey();
  while (key == NO_KEY){ key = keypad.getKey(); }
  if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9'){
      Serial.print("Enviado: ");
      Serial.print(key);
      Serial.println("");
      mySerial.write("lixo");
      mySerial.write(key);
      jogada = key;
    }
  else if (key == 'R') {
    Serial.println("");
    mySerial.write("lixo");
    mySerial.write(key);
    jogada = key;
   }
  else { Serial.println("ERROR KEY"); }
}

void enviaRandom(int ran)
{
  Serial.println("ENVIANDO RANDOM");
      Serial.print("Vez Random: ");
      Serial.print(ran);
      Serial.println("");
      mySerial.write("lixo");
      mySerial.write(ran);
      vez = ran;
}

void recebe()
{
  Serial.println("RECEBE");
  while (!(mySerial.available()) > 0) {}
  delay(1000);
  int cont = 0;
  while (cont < 5) {
    cont++;
    char c = mySerial.read();
    Serial.print('[');
    Serial.print(c);
    Serial.print(']');

   Serial.print(" (ainda disponivel: ");
   Serial.print(mySerial.available());
   Serial.println(')');
   jogada = c;
 }
}

void recebeRandom()
{
  Serial.println("RECEBE RANDOM");
  while (!(mySerial.available()) > 0) {}
  delay(1000);
  int cont = 0;
  while (cont < 5) {
    cont++;
    int c = mySerial.read();
    Serial.print('[');
    Serial.print(c);
    Serial.print(']');

   Serial.print(" (ainda disponivel: ");
   Serial.print(mySerial.available());
   Serial.println(')');
   vez = c;
 }
}

// Funcoes de estados //

void setupJogo()
{
  if (PLAYER == 1) { enviaRandom(random(0,3)); }
  if (PLAYER == 2) { recebeRandom(); }
  Serial.println("SETUP");
  escolhe_player();
  mode = ESPERANDO;
}

void esperando()
{
  Serial.println("ESPERANDO");
  if (vez == PLAYER)
  {
    envia();
    mode = MARCANDOJOGADA;
  }
  if (vez != PLAYER)
  {
    recebe();
    mode = MARCANDOJOGADA;
  }
}

void marcandoJogada()
{
  Serial.println("MARCANDO JOGADA");
  if (jogada == 'R') { fim(); }
  marcarJogada();
  mode = AVALIANDO_JOGADA;
}

void avaliandoJogada()
{
  Serial.println("AVALIANDO JOGADA");
  resultado = avaliar();

  if(resultado == -1)
  {
    Serial.println("JOGO CONTINUA");
    escolhe_player();
    mode = ESPERANDO;
  }
  else if(resultado == 0 || resultado == 1 || resultado == 2)
  {
    mode = FIM;
  }
}

void fim()
{
  Serial.println("FIM");
  delay(2500);
  vez = 0;
  clearPlayArea();
  mode = SETUP;
}

void setup()
{
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600); // Iniciando o terminal
  pixels.begin(); // Iniciando a biblioteca NeoPixel
  Wire.begin(device);

}

void loop()
{
  switch (mode)
  {
  case SETUP:
    setupJogo();
    break;
  case ESPERANDO:
    esperando();
    break;
  case MARCANDOJOGADA:
    marcandoJogada();
    break;
  case AVALIANDO_JOGADA:
    avaliandoJogada();
    break;
  case FIM:
    fim();
    break;
  }
}
