#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal.h>

// DECLARACOES ----------------------------------------------------------------------

//  Plugs LCD
const int db7 = 8;
const int db6 = 9;
const int db5 = 10;
const int db4 = 11;
const int e = 12;
const int rs = 13;
//  Instância LCD
LiquidCrystal lcd(rs, e, db4, db5, db6, db7);

//  Keyboard
char customKey;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'C','0','=','/'}
};
byte rowPins[ROWS] = {7,6,5,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3,A3,1,0}; //connect to the column pinouts of the keypad
//  Instância Keyboard
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
//Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

//  GLOBAIS

int modos = 0;      // modo da operacao

long first = 0;     // primeiro numero decimal
long second = 0;    // segundo numero Decimal
double total = 0;   // total calculadora decimal
int dec = 0;

long decimal = 0; 

int protecao = 0;

//  POSICOES
//Posicao Numero 1 (X,Y)
const int PNumreo1X = 0;    const int PNumreo1Y = 1;
//Posicao Operador 1 (X,Y)
const int POperador1X = 3;  const int POperador1Y = 1;
//Posicao Nunemro 2 (X,Y)
const int PNumreo2X = 5;    const int PNumreo2Y = 1;
//Posicao Operador 2 (=) (X,Y)
const int POperador2X = 8;  const int POperador2Y = 1;
//Posicao Resultado (X,Y)
const int PResultadoX = 10; const int PResultadoY = 1;

//  1100011 -> 99.00
//  ________________
//  0123456789012345
const int PS = 6;
const int PN1 = 0;
const int PN2 = 9;

const int PN1B = 0;
const int PSB = 8;
const int PN2B = 11;

// MODE && SETUP --------------------------------------------------------------------

void mode(){
  modos = modos + 1;
  if (modos > 7) { modos = 0; }
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Changing ...");
  delay(500);
  lcd.clear();
}


void setup(){ 
  attachInterrupt(0, mode, RISING); // iniciando o botao
  lcd.begin(16, 2);                 // iniciando o lcd
  // print inicial LCD
  lcd.setCursor(1,0);
    lcd.print("Bernardo Tinti");
  lcd.setCursor(2,1);
    lcd.print("Trabalho  14");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
}

//  Calculadora Decimal ---------------------------------------------------------------

void calcDec(){
  lcd.setCursor(0,0); lcd.print("Calculadora Dec");

  char key = keypad.getKey();
  switch(key) 
  {
  case '0' ... '9':
    if(key >= '0' && key <= '9' && protecao == 0)
    {
      first = first * 10 + (key - '0');
    }
      lcd.setCursor(PNumreo1X,PNumreo1Y);
      lcd.print(first);
    if ( first > 1000 ){ 
      lcd.clear(); 
      lcd.setCursor(0,0); lcd.print("ERROR"); 
      lcd.setCursor(0,1); lcd.print("Num > 999");
      delay(1500);
      clear();
    }
    break;

  case '+':
  protecao++;
    // printar operador
    lcd.setCursor(POperador1X,POperador1Y); lcd.print("+");
    first = (total != 0 ? total : first);   // primeiro numero
    second = segNumDec();        // segundo numero
    // printar operador de igualdade
    lcd.setCursor(POperador2X,POperador2Y); lcd.print("=");
    total = first + second;         // total
    // printar resultado
    lcd.setCursor(PResultadoX,PResultadoY); lcd.print(total);
    first = 0, second = 0; // reset values back to zero for next use
    break;

  case '-':
  protecao++;
    // printar operador
    lcd.setCursor(POperador1X,POperador1Y); lcd.print("-");
    first = (total != 0 ? total : first);   // primeiro numero
    second = segNumDec();        // segundo numero
    // printar operador de igualdade
    lcd.setCursor(POperador2X,POperador2Y); lcd.print("=");
    total = first - second;         // total
    // printar resultado
    lcd.setCursor(PResultadoX,PResultadoY); lcd.print(total);
    first = 0, second = 0; // reset values back to zero for next use
    break;

  case '*':
  protecao++;
    // printar operador
    lcd.setCursor(POperador1X,POperador1Y); lcd.print("*");
    first = (total != 0 ? total : first);   // primeiro numero
    second = segNumDec();        // segundo numero
    // printar operador de igualdade
    lcd.setCursor(POperador2X,POperador2Y); lcd.print("=");
    total = first * second;         // total
    // printar resultado
    lcd.setCursor(PResultadoX,PResultadoY); lcd.print(total);
    first = 0, second = 0; // reset values back to zero for next use
    break;

  case '/':
  protecao++;
    // printar operador
    lcd.setCursor(POperador1X,POperador1Y); lcd.print("/");
    first = (total != 0 ? total : first); // primeiro numero
    second = segNumDec();        // segundo numero
    second == 0 ? Invalid() : total = (float)first / (float)second;
    // printar operador de igualdade
    lcd.setCursor(POperador2X,POperador2Y); lcd.print("=");
    total = first / second;         // total
    // printar resultado
    lcd.setCursor(PResultadoX,PResultadoY); lcd.print(total);
    first = 0, second = 0; // reset values back to zero for next use
    break;

  case 'C':
  protecao = 0;
    first = 0;  second = 0; total = 0;
    clear();
    calcDec();
    break;
  }
}
long segNumDec(){
  while( 1 )
  {
    customKey = keypad.getKey();
    if(customKey >= '0' && customKey <= '9')
    {
        second = second * 10 + (customKey - '0');
      // printar segundo numero
    lcd.setCursor(PNumreo2X,PNumreo2Y);   
    lcd.print(second);
    }
    if ( second > 1000 ){ 
      lcd.clear(); 
      lcd.setCursor(0,0); lcd.print("ERROR"); 
      lcd.setCursor(0,1); lcd.print("Num > 999");
      delay(1500);  clear();  break;
    }
    if(customKey == '=') break;
    if (customKey == 'C'){ 
      first = 0;  second = 0; total = 0;
      clear();  calcDec(); break;
    }
  }
 return second; 
}

long Invalid(){
  protecao = 0;
  first = 0;  second = 0; total = 0;
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Operacao");
  lcd.setCursor(0,1); lcd.print("Invalida");
  delay(1500);
  clear();
  calcDec();
}

//  Calculadora Binaria --------------------------------------------------------------

int cont1 = 0;       //contador Binario 1
int cont2 = 0;       //contador Binario 2
int b1 [3] = { };   //Binario 1
int b2 [3] = { };   //Binario 2
int contD = 0;
int result = 0;
int dec1 = 0;
int dec2 = 0;
int resultB = 0;
int bb1 [ 4 ] = { };
int bb2 [ 4 ] = { };

void calcBin(){
  lcd.setCursor(0,0); lcd.print("Calculadora Bin");

  char key = keypad.getKey();
  switch(key){

    case '0':
      if (cont1 < 4 && protecao == 0){
        bb1[cont1] = 0;
        lcd.setCursor(PNumreo1X+cont1,PNumreo1Y);
        lcd.print(bb1[cont1]);
        cont1++;
      }
      break;

    case '1':
      if (cont1 < 4 && protecao == 0){
        bb1[cont1] = 1;
        lcd.setCursor(PNumreo1X+cont1,PNumreo1Y);
        lcd.print(bb1[cont1]);
        cont1++;
      }
    break;  

    case '+':
    protecao++;
      //printa o operador
      lcd.setCursor(POperador1X+1,POperador1Y);   lcd.print("+");
      //segundo numero binario
      segBin();
      //printa operador de iguladade
      lcd.setCursor(POperador2X+1,POperador2Y);   lcd.print('=');
      trans1(); trans2();
      result = (1+dec1) + (1+dec2);
      transR(result);
      //printar o resultado
      lcd.setCursor(PResultadoX, PResultadoY);  lcd.print(resultB);
    break;

    case '-':
    protecao++;
      //printa o operador
      lcd.setCursor(POperador1X+1,POperador1Y);   lcd.print("-");
      //segundo numero binario
      segBin();
      //printa operador de iguladade
      lcd.setCursor(POperador2X+1,POperador2Y);   lcd.print('=');
      trans1(); trans2();
      result = dec1 - dec2;
      transR(result);
      //printar o resultado
      lcd.setCursor(PResultadoX, PResultadoY);  lcd.print(resultB); 
    break;

    case '*':
    protecao++;
      //printa o operador
      lcd.setCursor(POperador1X+1,POperador1Y);   lcd.print("*");
      //segundo numero binario
      segBin();
      //printa operador de iguladade
      lcd.setCursor(POperador2X+1,POperador2Y);   lcd.print('=');
      trans1(); trans2();
      result = dec1 * dec2;
      transR(result);
      //printar o resultado
      lcd.setCursor(PResultadoX, PResultadoY);  lcd.print(resultB);
      break;

    case '/':
    protecao++;
      //printa o operador
      lcd.setCursor(POperador1X+1,POperador1Y);   lcd.print("/");
      //segundo numero binario
      segBin();
      //printa operador de iguladade
      lcd.setCursor(POperador2X+1,POperador2Y);   lcd.print('=');
      trans1(); trans2();
      if (dec2 == 0)
      {
        result = 0;
        Invalido();
      } else {
        result = dec1 / dec2;
      }
      transR(result);
      //printar o resultado
      lcd.setCursor(PResultadoX+1, PResultadoY);  lcd.print(resultB);
      break;

    case 'C':
      protecao = 0;
      cont1 = 0;  cont2 = 0;
      bb1[0] = 0; bb1[1] = 0; bb1[2] = 0; bb1[3] = 0;
      bb2[0] = 0; bb2[1] = 0; bb2[2] = 0; bb2[3] = 0; 
      dec1 = 0; dec2 = 0; result = 0; resultB = 0;
      clear();
      calcBin();
    break;

  }
}

long Invalido(){
  protecao = 0;
  cont1 = 0;  cont2 = 0;
  bb1[0] = 0; bb1[1] = 0; bb1[2] = 0; bb1[3] = 0;
  bb2[0] = 0; bb2[1] = 0; bb2[2] = 0; bb2[3] = 0; 
  dec1 = 0; dec2 = 0; result = 0; resultB = 0;
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("Operacao");
  lcd.setCursor(0,1); lcd.print("Invalida");
  delay(1500);
  clear();
  calcBin();
}

void segBin(){

  while(1){
    char key = keypad.getKey();

    if (key == '0' && cont2 < 4)
        {
          bb2[cont2] = 0;
          lcd.setCursor(PNumreo2X+cont2,PNumreo2Y);
          lcd.print(bb2[cont2]);
          cont2++;
        }
    if (key == '1' && cont2 < 4)
        {
          bb2[cont2] = 1;
          lcd.setCursor(PNumreo2X+cont2,PNumreo2Y);
          lcd.print(bb2[cont2]);
          cont2++;
        }
    if (key == '=') break;
    if (key == 'C')
    {
      protecao = 0;
      cont1 = 0;  cont2 = 0;
      bb1[0] = 0; bb1[1] = 0; bb1[2] = 0; bb1[3] = 0;
      bb2[0] = 0; bb2[1] = 0; bb2[2] = 0; bb2[3] = 0; 
      dec1 = 0; dec2 = 0; result = 0; resultB = 0;
      clear();
      calcBin();
    }
  }
}

void trans1() {
  dec1 = 0;
  for (int i = 0; i < cont1; ++i)
  {
    dec1 += bb1[cont1 - i -1] * pow(2,i);
  }
}

void trans2() {
  dec2 = 0;
  for (int i = 0; i < cont2; ++i)
  {
    dec2 += bb2[cont2 - i -1] * pow(2,i);
  }
}

void transR(int n){
    if(n==0){ }
    else{
        transR(n/2);
        resultB = (resultB * 10)  + n%2;
    }
}

String somaBin (){ 
  String result = " "; 
  int carry = 0; 
  int dif = 0; 
  int tamanho = 4; 
  int i = 0; 
  int maior = 0; 
    int r = 0;


  if(cont1 > cont2){ 
    maior = 1; 
    tamanho = cont1; 
    dif = cont1 - cont2; 
  } 
  else if(cont1 <= cont2){ 
    maior = 2; 
    tamanho = cont2; 
    dif =  cont2 - cont1; 
  } 
  r = maior;
  if (maior == 1){ 
    while(i < tamanho || carry == 1){ 
      if(i > tamanho - dif){ 
        result += b2[i]; 
      } 
      else if(b1[i] == '0' && b2[i] == '0' && carry == 0){ 
        result += '0'; 
        carry = 0; 
        r = r * 10 + 0;
      } 
      else if(b1[i] == '0' && b2[i] == '0' && carry == 1){ 
        result += '1'; 
        r = r * 10 + 1;
        carry = 0; 
      } 
      else if(b1[i] == '1' && b2[i] == '0' && carry == 0){ 
        result += '1';
        r = r * 10 + 1; 
        carry == 0; 
      } 
      else if(b1[i] == '1' && b2[i] == '0' && carry == 1){ 
        result += '0';
        r = r * 10 + 0; 
        carry = 1; 
      } 
      else if(b1[i] == '0' && b2[i] == '1' && carry == 0){ 
        result += '1';
        r = r * 10 + 0; 
        carry = 0; 
      } 
      else if(b1[i] == '0' && b2[i] == '1' && carry == 1){ 
        result += '0'; 
        r = r * 10 + 0;
        carry = 1; 
      } 
      else if(b1[i] == '1' && b2[i] == '1' && carry == 0){ 
        result += '0'; 
        r = r * 10 + 0;
        carry = 1; 
      } 
      else if(b1[i] == '1' && b2[i] == '1' && carry == 1){ 
        result += '1'; 
        r = r * 10 + 1;
        carry = 1; 
      } 
      else if (carry == 1){ 
        result+='1'; 
        r = r * 10 + 1;
        carry = 0; 
      } 
      i +=1; 
    } 
  } 
    else if (maior == 2){ 
    while(i < tamanho || carry == 1){ 
      if(i > tamanho - dif){ 
        result += b2[i]; 
      } 
      else if(b1[i] == '0' && b2[i] == '0' && carry == 0){ 
        result += '0'; 
        r = r * 10 + 0;
        carry = 0; 
      } 
      else if(b1[i] == '0' && b2[i] == '0' && carry == 1){ 
        result += '1';
        r = r * 10 + 1; 
        carry = 0; 
      } 
      else if(b1[i] == '1' && b2[i] == '0' && carry == 0){ 
        result += '1';
        r = r * 10 + 1; 
        carry == 0; 
      } 
      else if(b1[i] == '1' && b2[i] == '0' && carry == 1){ 
        result += '0';
        r = r * 10 + 0; 
        carry = 1; 
      } 
      else if(b1[i] == '0' && b2[i] == '1' && carry == 0){ 
        result += '1'; 
        r = r * 10 + 1;
        carry = 0; 
      } 
      else if(b1[i] == '0' && b2[i] == '1' && carry == 1){ 
        result += '0';
        r = r * 10 + 0; 
        carry = 1; 
      } 
      else if(b1[i] == '1' && b2[i] == '1' && carry == 0){ 
        result += '0'; 
        r = r * 10 + 0;
        carry = 1; 
      } 
      else if(b1[i] == '1' && b2[i] == '1' && carry == 1){ 
        result += '1'; 
        r = r * 10 + 1;
        carry = 1; 
      } 
      else if (carry == 1 && b1[i]!= '0' && b1[i] != '1' && b2[i] != '0' && b2[i] != '1'){ 
        result+='1'; 
        r = r * 10 + 1;
        carry = 0; 
      } 
      i +=1; 
    } 
      i = 0; 
  } 
    return result; 
}

void subBin(){ 
  String result = " "; 
  int carry = 0; 
  int dif = 0; 
  int tamanho = 4; 
  int i = 0; 
  int maior = 0; 
  int r = 0;

  if(cont1 > cont2){ 
    maior = 1; 
    tamanho = cont1; 
    dif = cont1 - cont2; 
  } 
  else if(cont1 <= cont2){ 
    maior = 2; 
    tamanho = cont2; 
    dif =  cont2 - cont1; 
  } 
  if (maior == 1){ 
    for(int i = 0; i < dif; i++){ 
      result += b1[i];} 
    
    } 
  else if (maior == 2){ 
    for(int i = 0; i < dif; i++){ 
      result += b2[i]; 
    } 
    i = cont1 - dif-1; 
    while(i !=-1) { 
      if(b1[i+dif] == 0 && b2[i+dif] == 0 && carry == 0){ 
        r = r * 10 + 0;
        result += '0'; 
        carry = 0; 
      } 
      else if(b1[i+dif] == 1 && b2[i+dif] == 0 && carry == 0){ 
        r = r * 10 + 1;
        result +=  '1'; 
        carry = 0; 
      } 
      else if(b1[i+dif] == 0 && b2[i+dif] == 1 && carry == 0){ 
        r = r * 10 + 1;
        result +=  '1'; 
        carry = 1; 
      } 
      else if(b1[i+dif] == 1 && b2[i+dif] == 1 && carry == 0){ 
        r = r * 10 + 0;
        result +=  '0'; 
        carry = 0; 
      } 
      else if(b1[i+dif] == 0 && b2[i+dif] == 0 && carry == 1){ 
        r = r * 10 + 1;
        result += '1'; 
        carry = 0; 
      } 
      else if(b1[i+dif] == 1 && b2[i+dif] == 0 && carry == 1){ 
        r = r * 10 + 0;
        result +=  '0'; 
        carry = 0; 
      } 
      else if(b1[i+dif] == 0 && b2[i+dif] == 1 && carry == 1){ 
        r = r * 10 + 0;
        result +=  '0'; 
        carry = 0; 
      } 
      else if(b1[i+dif] == 1 && b2[i+dif] == 1 && carry == 1){ 
        r = r * 10 + 1;
        result +=  '1'; 
        carry = 1; 
      } 
      i--; 
    } 
    } 
} 

//  Conversor Decimal para Binario ---------------------------------------------------

int convDecBin = 0;
int r = 0;

void decToBin(){
  lcd.setCursor(0,0); lcd.print("Conv. Dec p/ Bin");

  char key = keypad.getKey();
  switch(key){
  case '0' ... '9':
    if(key >= '0' && key <= '9' && contD < 3 && protecao == 0)
    {
      dec = dec * 10 + (key - '0');
      contD++;
      lcd.setCursor(PN1,1);
      lcd.print(dec);
    }
  break;

  case '=':
    protecao++;
    // printar simbolo
    lcd.setCursor(PS - 3, 1);   lcd.print("->");
    convDecToBin(dec);
    lcd.setCursor(PN2 - 3, 1);  lcd.print(convDecBin);
    contD = 0; dec = 0;
  break;

  case 'C':
    protecao = 0;
    convDecBin = 0; contD = 0; 
    dec = 0;
    clear();
    decToBin();
    break;
  }
}

void convDecToBin(int n){
    if(n==0){ }
    else{
        convDecToBin(n/2);
        convDecBin = (convDecBin * 10)  + n%2;
    }
}

//  Conversor Decimal para Octadecimal ------------------------------------------------

void decToOct(){
  lcd.setCursor(0,0); lcd.print("Conv. Dec p/ Oct");

  char key = keypad.getKey();
  switch(key){
  case '0' ... '9':
    if(key >= '0' && key <= '9' && contD < 3 && protecao == 0)
    {
      dec = dec * 10 + (key - '0');
      contD++; 
    }
      lcd.setCursor(PN1,1);
      lcd.print(dec);
  break;

  case '=':
    protecao++;
    lcd.setCursor(PS, 1);   lcd.print("->");
    lcd.setCursor(PN2, 1);  lcd.print(String(dec,OCT));
    contD = 0; dec = 0;
  break;

  case 'C':
    protecao = 0; dec = 0;
    contD = 0;
    clear();
    decToOct();
    break;
  }
}

//  Conversor Decimal para Hexadecimal ------------------------------------------------

void decToHex(){
  lcd.setCursor(0,0); lcd.print("Conv. Dec p/ Hex");

  char key = keypad.getKey();
  switch(key){
  case '0' ... '9':
    if(key >= '0' && key <= '9' && contD < 3 && protecao == 0)
    {
      dec = dec * 10 + (key - '0');
      contD++;
    }
      lcd.setCursor(PN1,1);
      lcd.print(dec);
  break;

  case '=':
    protecao++;
    lcd.setCursor(PS, 1);   lcd.print("->");
    lcd.setCursor(PN2, 1);  lcd.print(String(dec,HEX));
    dec = 0;  contD = 0; 
  break;

  case 'C':
    protecao = 0;
    contD = 0; 
    dec = 0;
    clear();
    decToHex();
    break;
  }
}

//  Conversor Binario para Decimal ----------------------------------------------------

int contB = 0;
int maxbD = 7;
int bD [7] = { };

double convBinDec = 0;

void binToDec(){
  lcd.setCursor(0,0); lcd.print("Conv. Bin p/ Dec.");

  char key = keypad.getKey();
  switch(key){
  case '0':
    if (contB < maxbD && protecao == 0);
    {
      bD[contB] = 0;
      lcd.setCursor(PN1+contB, 1); lcd.print(bD[contB]);
      contB++;
    }
  break;

  case '1':
    if (contB < maxbD && protecao == 0);
    {
      bD[contB] = 1;
      lcd.setCursor(PN1+contB, 1); lcd.print(bD[contB]);
      contB++;
    }
  break;

  case '=':
    protecao++;
    lcd.setCursor(PSB, 1);   lcd.print("->");
    convBinToDec();
    lcd.setCursor(PN2B, 1);  lcd.print(convBinDec);
    contB = 0; 
  break;

  case 'C':
    protecao = 0;
    bD[0] = 0;  bD[1] = 0;  bD[2] = 0;  bD[3] = 0;
    bD[4] = 0;  bD[5] = 0;  bD[6] = 0;
    convBinDec = 0; contB = 0;
    clear();
    binToDec();
    break;
  }
}

void convBinToDec(){
  convBinDec = 0;
  for (int i = 0; i < contB; ++i)
  {
    convBinDec += bD[contB - i - 1] * pow(2,i);
  }
}

//  Conversor Binario para Octadecimal ------------------------------------------------

int bO [7] = { };
double convBinOct = 0;
int kbO = 0;


void binToOct(){
  lcd.setCursor(0,0); lcd.print("Conv. Bin p/ Oct");

  char key = keypad.getKey();
  switch(key){
  case '0':
    if (contB < maxbD && protecao == 0);
    {
      bO[contB] = 0;
      lcd.setCursor(PN1+contB, 1); lcd.print(bO[contB]);
      contB++;
    }
  break;

  case '1':
    if (contB < maxbD && protecao == 0);
    {
      bO[contB] = 1;
      lcd.setCursor(PN1+contB, 1); lcd.print(bO[contB]);
      contB++;
    }
  break;

  case '=':
    protecao++;
    lcd.setCursor(PSB, 1);   lcd.print("->");
    convBinToOct();
    kbO = convBinOct;
    lcd.setCursor(PN2B, 1);  lcd.print(String(kbO,OCT));
  break;

  case 'C':
    protecao = 0;
    bO[0] = 0;  bO[1] = 0;  bO[2] = 0;  bO[3] = 0;
    bO[4] = 0;  bO[5] = 0;  bO[6] = 0;
    contB = 0;  convBinOct = 0;   kbO = 0;
    clear();
    binToOct();
    break;
  }
}

void convBinToOct(){
  convBinOct = 0;
  for (int i = 0; i < contB; ++i)
  {
    convBinOct += bO[contB - i - 1] * pow(2,i);
  }
}

//  Conversor Binario para Hexadecimal ------------------------------------------------

int bH [7] = { };
double convBinHex = 0;
int kbH = 0;

void binToHex(){
  lcd.setCursor(0,0); lcd.print("Conv. Bin p/ Hex");

  char key = keypad.getKey();
  switch(key){
  case '0':
    if (contB < maxbD && protecao == 0);
    {
      bH[contB] = 0;
      lcd.setCursor(PN1+contB, 1); lcd.print(bH[contB]);
      contB++;
    }
  break;

  case '1':
    if (contB < maxbD && protecao == 0);
    {
      bH[contB] = 1;
      lcd.setCursor(PN1+contB, 1); lcd.print(bH[contB],OCT);
      contB++;
    }
  break;

  case '=':
    protecao++;
    lcd.setCursor(PS, 1);   lcd.print("->");
    convBinToHex();
    kbH = convBinHex + 1;
    lcd.setCursor(PN2, 1);  lcd.print(String(kbH,HEX));
  break;

  case 'C':
    protecao = 0;
    bH[0] = 0;  bH[1] = 0;  bH[2] = 0;  bH[3] = 0;
    bH[4] = 0;  bH[5] = 0;  bH[6] = 0;
    contB = 0;  convBinHex = 0; kbH = 0;
    clear();
    binToHex();
    break;
  }
}

void convBinToHex(){
  convBinHex = 0;
  for (int i = 0; i < contB; ++i)
  {
    convBinHex += bH[contB - i - 1] * pow(2,i);
  }
}

// CLEAR && LOOP ----------------------------------------------------------------------

void clear(){
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Cleaning ...");
    delay(500);
    lcd.clear();
}


void loop()
{
  switch(modos){
    case 0: calcDec();  break;
    case 1: calcBin();  break;
    case 2: decToBin(); break;
    case 3: decToOct(); break;
    case 4: decToHex(); break;
    case 5: binToDec(); break;
    case 6: binToOct(); break;
    case 7: binToHex();
  }
}

