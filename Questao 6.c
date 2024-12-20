#include <EEPROM.h>

// Variáveis
int tempoLED = 5;        
int intervalo = 10;      
unsigned long ultimaAcao = 0; 
unsigned long inicioLED = 0;  
bool ledAtivo = false;        
bool ledLigado = false;    

// Pinos
const int pinLED = 8;      
const int pinBotao1 = 2;    
const int pinBotao2 = 3;    
const int pinBotao3 = 4;    
const int pinBotao4 = 5;    
const int pinPiezo = 9;     

// Endereços EEPROM
const int addrTempoLED = 0;
const int addrIntervalo = 1;

void setup() {
  Serial.begin(9600);

  pinMode(pinLED, OUTPUT);
  pinMode(pinPiezo, OUTPUT);
  pinMode(pinBotao1, INPUT_PULLUP);
  pinMode(pinBotao2, INPUT_PULLUP);
  pinMode(pinBotao3, INPUT_PULLUP);
  pinMode(pinBotao4, INPUT_PULLUP);

  tempoLED = EEPROM.read(addrTempoLED);
  intervalo = EEPROM.read(addrIntervalo);

  if (tempoLED == 255 || intervalo == 255) { 
    tempoLED = 5;
    intervalo = 10;
  }

  Serial.println("Configurações iniciais:");
  imprimeValores();

  attachInterrupt(digitalPinToInterrupt(pinBotao1), aumentaTempoLED, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBotao2), aumentaIntervalo, FALLING);
}

void loop() {
  verificaSerial();        
  verificaBotoesManuais(); 

  unsigned long tempoAtual = millis();

  if (!ledLigado && intervalo > 0 && tempoLED > 0 && (tempoAtual - ultimaAcao >= intervalo * 1000)) {
    digitalWrite(pinLED, HIGH);
    tone(pinPiezo, 1000);
    ledLigado = true;
    inicioLED = tempoAtual;
  }

  if (ledLigado && (tempoAtual - inicioLED >= tempoLED * 1000)) {
    digitalWrite(pinLED, LOW);
    noTone(pinPiezo);
    ledLigado = false;
    ultimaAcao = tempoAtual;
  }
}

void aumentaTempoLED() {
  tempoLED++;
  if (tempoLED > 10) tempoLED = 10;
  EEPROM.write(addrTempoLED, tempoLED);
  Serial.print("Tempo LED ajustado para: ");
  Serial.println(tempoLED);
}

void aumentaIntervalo() {
  intervalo += 5;
  if (intervalo > 40) intervalo = 40;
  EEPROM.write(addrIntervalo, intervalo);
  Serial.print("Intervalo ajustado para: ");
  Serial.println(intervalo);
}

void verificaBotoesManuais() {
  if (digitalRead(pinBotao3) == LOW) {
    diminuiTempoLED();
    delay(300);
  }
  if (digitalRead(pinBotao4) == LOW) {
    diminuiIntervalo();
    delay(300);
  }
}

void diminuiTempoLED() {
  tempoLED--;
  if (tempoLED < 0) tempoLED = 0; 
  EEPROM.write(addrTempoLED, tempoLED);
  Serial.print("Tempo LED ajustado para: ");
  Serial.println(tempoLED);
}

void diminuiIntervalo() {
  intervalo -= 5;
  if (intervalo < 0) intervalo = 0;
  EEPROM.write(addrIntervalo, intervalo);
  Serial.print("Intervalo ajustado para: ");
  Serial.println(intervalo);
}

void verificaSerial() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando.startsWith("TEMPO")) {
      int valor = comando.substring(6).toInt();
      if (valor >= 0 && valor <= 10) { // Permite valor 0
        tempoLED = valor;
        EEPROM.write(addrTempoLED, tempoLED);
        Serial.print("Tempo LED ajustado para: ");
        Serial.println(tempoLED);
      } else {
        Serial.println("Valor inválido! Informe entre 0 e 10 segundos.");
      }
    }

    if (comando.startsWith("INTERVALO")) {
      int valor = comando.substring(10).toInt();
      if (valor >= 0 && valor <= 40) { 
        intervalo = valor;
        EEPROM.write(addrIntervalo, intervalo);
        Serial.print("Intervalo ajustado para: ");
        Serial.println(intervalo);
      } else {
        Serial.println("Valor inválido! Informe entre 0 e 40 segundos.");
      }
    }
    imprimeValores();
  }
}

void imprimeValores() {
  Serial.print("Tempo LED: ");
  Serial.print(tempoLED);
  Serial.print("s | Intervalo: ");
  Serial.print(intervalo);
  Serial.println("s");
}
