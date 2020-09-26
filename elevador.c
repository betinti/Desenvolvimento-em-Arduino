#include <Adafruit_NeoPixel.h>

//		PORTAS 
//	LED
int ledOperante = 12;
int ledPorta = 13;
int ledEmergencia = 11;

//	FITA LED
int numeroLED = 12;
int portaLED = 4;
Adafruit_NeoPixel fitaLED = Adafruit_NeoPixel(numeroLED, portaLED, NEO_GRB + NEO_KHZ800);

//	GLOBAIS
int nAdares = 10;							//	Numero de Andares
int operante = 0;
int tempoDelay = 250;

int andares [10] = { };	//	Lista de requisições
int andarAtual = 0; 						// 	Andar atual em que o elevador está
int proxAndar = 0;							// 	Proximo andar que o elevador irá
int estado = 0; 							// 	1 = subindo	||	0 = parado	|| 	-1 = descendo
int porta = 0;								//	1 = Aberta || 0 = Fechada
int portaAberta = 1;
int statusEmergencia = 0;

void setup(){

	Serial.begin(9600);	// Iniciando o terminal
	fitaLED.begin();	// Iniciando a biblioteca NeoPixel
	pinMode( ledOperante, OUTPUT );	
	pinMode( ledPorta, OUTPUT );
	pinMode( ledEmergencia, OUTPUT );
	pinMode(3, INPUT_PULLUP);
	attachInterrupt(1, chamada, FALLING);

	digitalWrite(ledPorta, HIGH);	
	Serial.println("OFF");
	Serial.println("PORTA ABERTA"); porta = 1;
}

int testar_led(int num){
    for(int i=0;i<num;i++){
        fitaLED.setPixelColor(i, fitaLED.Color(0,255,0 ));
        fitaLED.show();
        delay(tempoDelay);
        fitaLED.setPixelColor(i, 0,0,0);
        delay(tempoDelay);
        Serial.print("Abrindo Andar ");
        Serial.println(i);
        delay(2000);
        Serial.print("Fechando Andar ");
        Serial.println(i);
        delay(2000);
        Serial.print("indo para andar ");
        if(i+1 == 10){
            Serial.println(0);
        }
        else{
            Serial.println(i+1);
        }
        delay(2000);
        fitaLED.setPixelColor(i+1, fitaLED.Color(0,255,0 ));
        fitaLED.show();
        delay(tempoDelay);
        fitaLED.setPixelColor(i+1, 0,0,0);
        delay(tempoDelay);
    }
}

void chamada(){
	int bot = analogRead(A5);
	//Serial.println(bot);
	switch (bot){
		//ANDARES
		case 25:  chamadaFIxa(0); break;	//T
		case 50:  chamadaFIxa(1); break;	//1
		case 73:  chamadaFIxa(2); break;	//2
		case 95:  chamadaFIxa(3); break;	//3
		case 116: chamadaFIxa(4); break;	//4
		case 136: chamadaFIxa(5); break;	//5
		case 155: chamadaFIxa(6); break;	//6
		case 173: chamadaFIxa(7); break;	//7
		case 191: chamadaFIxa(8); break;	//8
		case 208: chamadaFIxa(9); break;	//9
		case 240: botaoPorta();   break;	//Porta
		case 224: emergencia();   break;	//Emergencia
		case 269: opera(1); 	  break;	//ON
		case 255: opera(0);		  break;	//OFF
		//SOBE
		case 283: chamadaFIxa(0); break;	//T
		case 296: chamadaFIxa(1); break;	//1
		case 309: chamadaFIxa(2); break;	//2
		case 322: chamadaFIxa(3); break;	//3
		case 334: chamadaFIxa(4); break;	//4
		case 346: chamadaFIxa(5); break;	//5
		case 357: chamadaFIxa(6); break;	//6
		case 368: chamadaFIxa(7); break;	//7
		case 378: chamadaFIxa(8); break;	//8
		//DESCE
		case 388: chamadaFIxa(1); break;	//1
		case 398: chamadaFIxa(2); break;	//2
		case 408: chamadaFIxa(3); break;	//3
		case 417: chamadaFIxa(4); break;	//4
		case 426: chamadaFIxa(5); break;	//5
		case 435: chamadaFIxa(6); break;	//6
		case 443: chamadaFIxa(7); break;	//7
		case 452: chamadaFIxa(8); break;	//8
		case 460: chamadaFIxa(9); break;	//9
	}

}

int checagem(){
    int x = andarAtual;
	if (operante == 1 && estado != 0)
	{
		if (x == 0) estado = 1;
		if (x == 9) estado = -1;
		while(x < nAdares && x > -1 && proxAndar == andarAtual){
			x = x + (1 * estado);
			if (andares[x] == estado)
			{
				proxAndar = x;
				movimento();
              	andares[x] = 0;
			} else if (proxAndar == andarAtual)
			{
				andares[x] = 0;
			}
		}
		andarAtual = proxAndar; 
      	estado = 0; operante = 1;
	}
}

void movimento(){
	operaPorta(0);
	while(andarAtual != proxAndar){
		andarAtual = andarAtual + (1 * estado);	
			fitaLED.setPixelColor(andarAtual - (1 * estado), fitaLED.Color(0, 0, 0)); fitaLED.show();			//OFF	
			fitaLED.setPixelColor(andarAtual, fitaLED.Color(0, 0, 255)); fitaLED.show();						//ON	
			delay(tempoDelay);
	} andarAtual = proxAndar; 
	operaPorta(1); delay(1000); operaPorta(0);
}

void inverteSentido(){
	if (estado == 1) estado = -1;
	if (estado == -1) estado = 1;
	if (estado == 0) estado = 0;
}


void chamadaFIxa(int floor){
	if (operante == 1)
	{
		if (andarAtual > floor) { andares[floor] = -1; estado = -1; Serial.println(floor); }
		if (andarAtual < floor) { andares[floor] = 1;  estado = 1; Serial.println(floor); }
	}
}

void compFloor(int floor){
  		if (andarAtual > floor) { andares[floor] = -1; Serial.println(floor); }
		if (andarAtual < floor) { andares[floor] = 1;  Serial.println(floor); }

}

void chamadaSobe(int floor){ if(operante == 1) {andares[floor] = 1; estado = 1; Serial.println(floor); } } 

void chamadaDesce(int floor){ if(operante == 1) {andares[floor] = -1; estado = -1; Serial.println(floor); } }

void botaoPorta(){
	if (porta == 0){
		porta = 1;	//Porta aberta
		digitalWrite(ledPorta, HIGH);	
		Serial.println("PORTA ABERTA");
	} else if (porta == 1){
		porta = 0;	//Porta Fechada
		digitalWrite(ledPorta, LOW);	
		Serial.println("PORTA FECHADA");
	} 
}

void operaPorta(int estado){
	if (estado == 1){ porta = 1; digitalWrite(ledPorta, HIGH); Serial.println("PORTA ABERTA"); }
	if (estado == 0){ porta = 0; digitalWrite(ledPorta, LOW); Serial.println("PORTA FECHADA"); }
}

void emergencia(){
	if (statusEmergencia == 0)
	{
		statusEmergencia = 1;
		digitalWrite(ledEmergencia, HIGH);	
		Serial.println("EMERGENCIA ACIONADA");
		operante = 0;	//elevador OFF
	} else if (statusEmergencia == 1)
	{
		statusEmergencia = 0;
		digitalWrite(ledEmergencia, LOW);	
		Serial.println("EMERGENCIA DESATIVADA");
		operante = 1;	//elevador ON
	}
	
}

void opera(int o){
	if (o == 1 && operante == 0){
		operante = 1;	//Elevador ON
		digitalWrite(ledOperante, HIGH);	
		Serial.println("ON");
		operaPorta(1);
     	fitaLED.setPixelColor(0, fitaLED.Color(0, 0, 255)); fitaLED.show();		//ON	
	} else if (o == 0 && operante == 1){
		operante = 0;	//Elevador OFF
		digitalWrite(ledOperante, LOW);	
		Serial.println("OFF");
		operaPorta(0);
	} 
	estado = 0;	andarAtual = 0;	proxAndar = 0;	statusEmergencia = 0;
	andares[0] = 0;	andares[1] = 0;	andares[2] = 0;	andares[3] = 0;	andares[4] = 0;
	andares[5] = 0;	andares[6] = 0;	andares[7] = 0;	andares[8] = 0;	andares[9] = 0;
}

void loop(){ checagem(); }
