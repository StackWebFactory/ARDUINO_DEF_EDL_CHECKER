#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1);

int keyIndex = 0;    
//initialisation LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//initialisation cla
#include <Keypad.h>
const byte ROWS = 4; // 4 rows
const byte COLS = 4; // 3 columns
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}};
byte rowPins[ROWS] = {2, 3, 4, 5}; // connecté aux pins de sorties des rows clavier
byte colPins[COLS] = {6, 7, 8, 9}; // connecté aux pins de sorties des column clavier
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS );

//Initialisation des variable 
int ALIM ;
int ATT ;
int SECU ;
int ALIMOLD;
int ATTOLD;
int SECUOLD;
int MENUOLD ;
int MENUBACK ;
int MENU = 1;
int SATTENTE;
int SSECURITE;
String ATTENTE;
String ATTENTEOLD;
String SECURITE;
String SECURITEOLD;
int key = NO_KEY;
//int statledrouge = HIGH;//initialisation de l'etat led rouge
//int statledvert = HIGH; //initialisation de l'etat led vert
int Rk_3 = 10; //Valeur résistance de comparaison 
//initialisation de l'interval de rafreshisement 
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long stoprelay = 0;
const long seconde1 = 1000;
long mseconde500 = 500;
const long seconde2 = 2000;
int buzz;
//initialisation caractere special fleche haut et bas
byte FBAS[] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

byte FHAUT[] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000
};

byte Check [] = {
 B00000,
 B00001,
 B00011,
 B10110,
 B11100,
 B01000,
 B00000,
 B00000
};


byte ohm [] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000
};

float sinVal;        // crée une variable de type "float" appelée "SinVal" et qui contient la valeur sinusoïdale qui fera monter et descendre la tonalité.
int toneVal;         // crée une variable de type "int" appelée "toneVal" et qui représente la fréquence de la note produite en hertz (Hz).
char mystr[50]; //Initialized variable to store recieved data
String SENDERALIM;
String SENDERATT;
String SENDERSECU;
int relay = 10;
int ledrouge = 13;
int ledvert = 11;
int buzzer= 12;
int countledstatvert=0;
int blinkledvert=0;
int ledvertfix = 0;
unsigned long previousMillisledvert = 0;
int statledvert=HIGH;
int countledstatrouge=0;
int blinkledrouge=0;
int ledrougefix = 0;
unsigned long previousMillisledrouge = 0;
int statledrouge=HIGH;
int buzzerrouge;
int buzzervert;
void setup() {

Serial.begin(9600) ; // start serial com
mySerial.begin(9600);
lcd.begin();//start lcd
lcd.backlight();// allumage du retro-éclairage
pinMode(ledrouge,OUTPUT);// def pin led rouge
pinMode(ledvert,OUTPUT);// def pin led VERT
pinMode(buzzer, OUTPUT);//def pin buzzer
pinMode(relay, OUTPUT);//def pin relay
lcd.createChar(0, FBAS);//import fleche haut
lcd.createChar(1, FHAUT);//import fleche bas
lcd.createChar(2, Check);//import fleche bas
lcd.createChar(3, ohm);//import fleche bas
lcd.setCursor(0,0);
lcd.print("SWF_DEF_EDL");
lcd.setCursor(0,1);
lcd.print("CHECKER V_2.1.0");
digitalWrite(ledvert, HIGH);
digitalWrite(ledrouge, HIGH);
delay(2000);
}
void loop() {
LCD(); // bloucle MENU LCD
}

/************************************************************************************************************************/
void OHM(){
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= seconde1) {
previousMillis = currentMillis;
float value = analogRead(0);
delay(5);
value = analogRead(0);
ALIM = (Rk_3 * value / (1024 - value)*1000);
float value1 = analogRead(1);
delay(5);
value1 = analogRead(1);
ATT = (Rk_3 * value1 / (1024 - value1)*1000);
float value2 = analogRead(2);
delay(5);
value2 = analogRead(2);
SECU = (Rk_3 * value2 / (1024 - value2)*1000);
if (MENU > 10){
if (ATT < 0 or ATT > 4800){ATTENTE = "ATT: LINE OPEN";SSECURITE= 999;digitalWrite(ledvert, HIGH);noTone(buzzer);}
if (SECU < 0 or SECU > 4800){SECURITE = "SECU: LINE OPEN";SATTENTE= 999;digitalWrite(ledrouge, HIGH);noTone(buzzer);}
if (ATT > 2500 && ATT < 3000 ) {ATTENTE = "1 EN SECU";SSECURITE=1;digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(200);}
if (ATT > 3100 && ATT < 3500 ) {ATTENTE = "2 EN SECU";SSECURITE=2;digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(50);digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(200);}
if (ATT > 3600 && ATT < 4000 ) {ATTENTE = "3 EN SECU";SSECURITE=3;digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(50);digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(50);digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(200);}
if (ATT > 4100 && ATT < 4800 ) {ATTENTE = "4 EN SECU";SSECURITE=4;digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(50);digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(50);digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(50);digitalWrite(ledvert, LOW); tone(buzzer,2000); delay(50);digitalWrite(ledvert, HIGH); noTone(buzzer);delay(200);}
if (SECU > 2500 && SECU < 3000 ) {SECURITE = "1 EN ATT";SATTENTE=1;digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(200);}
if (SECU > 3100 && SECU < 3500 ) {SECURITE = "2 EN ATT";SATTENTE=2;digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(50);digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(200);}
if (SECU > 3600 && SECU < 4000 ) {SECURITE = "3 EN ATT";SATTENTE=3;digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(50);digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(50);digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(200);}
if (SECU > 4100 && SECU < 4800 ) {SECURITE = "4 EN ATT";SATTENTE=4;digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(50);digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(50);digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(50);digitalWrite(ledrouge, LOW); tone(buzzer,800); delay(50);digitalWrite(ledrouge, HIGH); noTone(buzzer);delay(200);}
if (ATT > 2100 && ATT < 2300 ) {ATTENTE = "0 EN SECU FL OK";SSECURITE=5;digitalWrite(ledvert, LOW);tone(buzzer,2000);}
if (SECU > 2100 && SECU < 2300 ) {SECURITE = "0 EN ATT FL OK";SATTENTE=5;digitalWrite(ledrouge, LOW);tone(buzzer,800);}
if (ATT == 0 or (ATT > 0 & ATT < 150)){ATTENTE = "ATT: COURT CIRCUIT";SSECURITE= 100; digitalWrite(ledvert, LOW);tone(buzzer,3000);} 
if (SECU == 0 or (SECU > 0 & SECU < 150)){SECURITE = "SECU: COURT CIRCUIT";SATTENTE= 100;digitalWrite(ledrouge, LOW);tone(buzzer,3000);}
if (ALIM > 6000 or ALIM < 0) {ALIM = 9999;}
if (ATT > 6000 or ATT < 0) {ATT = 9999;}
if (SECU > 6000 or SECU < 0) {SECU = 9999;}
Serial.print("Z");
Serial.print(ALIM);
Serial.print("Y");
Serial.print(ATT);
Serial.print("X");
Serial.print(SECU);
Serial.print("W");
Serial.print(SATTENTE);
Serial.print("V");
Serial.print(SSECURITE);
Serial.print("!");
Serial.print(MENU);
Serial.println(">");}
}
}

/************************************************************************************************************************/
void MENUVOID(){
char key = keypad.getKey();
if (Serial.available()){
  int READ = Serial.read();
  if (READ == '1') key = 'B';
  else if (READ == '2') key = 'A';
  else if (READ == '3') MENU = 14;tone (12, 1000, 50);
  //Serial.print(READ);
  }
  
  
 if (key != NO_KEY){
 tone (12, 1000, 50); // allume le buzzer actif arduino
 if (key == '*' and MENU < 5 and MENU > 1){MENU--;}
 if (key == '1'){MENU=12;}
 if (key == '2'){MENU=13;}
 if (key == '3'){MENU=14;}
 if (key== '#' and MENU < 4 and MENU > 0){MENU++;}
 if (key== 'B' ) {MENU= 20;}
 if (key== 'D' and MENU == 2) {MENU = 12;}
 if (key== 'D' and MENU == 3) {MENU = 13;}
 if (key== 'D' and MENU == 4) {MENU = 14;}
 if (key== 'C' and MENU == 12) {MENU = 2;}
 if (key== 'C' and MENU == 13) {MENU = 3;}
 if (key== 'C' and MENU == 14) {MENU = 4;}
 if (key== 'C' and MENU == 20) {MENU= 1;}
 if (key== 'A' and MENU == 20) {MENU= 21;}
lcd.clear();
key == NO_KEY;
    }
}
void LCD(){
// attendre 1 seconde avant la prochaine mesure
while (MENU == 1) {
digitalWrite(ledvert, HIGH);
digitalWrite(ledrouge, HIGH);
noTone(buzzer);
if (MENU != MENUOLD){lcd.clear();}
MENUOLD=1;
MENUBACK=1;
MENUVOID();
lcd.setCursor(0,0);
lcd.print("CHOIX DU MODE");
lcd.setCursor(0,1);
lcd.print("* = ");
lcd.write(0);
lcd.setCursor(10,1);
lcd.print("# = ");
lcd.write(1);

}

while (MENU == 2) {
digitalWrite(ledvert, HIGH);
digitalWrite(ledrouge, HIGH);
noTone(buzzer);
if (MENU != MENUOLD){lcd.clear();}
MENUOLD=2;
MENUBACK=2;
MENUVOID();
lcd.setCursor(0,0);
lcd.print("TEST VOLET / CCF");
lcd.setCursor(0,1);
lcd.print("ALIM");
lcd.setCursor(6,1);
lcd.print("ATT");
lcd.setCursor(12,1);
lcd.print("SECU");

}
while (MENU == 3) {
digitalWrite(ledvert, HIGH);
digitalWrite(ledrouge, HIGH);
noTone(buzzer);
if (MENU != MENUOLD){lcd.clear();}
MENUOLD=3;
MENUBACK=3;
MENUVOID();
lcd.setCursor(0,0);
lcd.print("TEST PCF");
lcd.setCursor(0,1);
lcd.print("ALIM");
lcd.setCursor(6,1);
lcd.print("");
lcd.setCursor(12,1);
lcd.print("SECU");

}

while (MENU == 4) {
digitalWrite(ledvert, HIGH);
digitalWrite(ledrouge, HIGH);
noTone(buzzer);
if (MENU != MENUOLD){lcd.clear();}
MENUOLD=4;
MENUBACK=4;
MENUVOID();
lcd.setCursor(0,0);
lcd.print("TEST Ohmmetre");
lcd.setCursor(0,1);
lcd.print("ALIM");
lcd.setCursor(6,1);
lcd.print("ATT");
lcd.setCursor(12,1);
lcd.print("SECU");
}

while (MENU == 12){
if (MENU != MENUOLD){lcd.clear();}
if (SECURITE != SECURITEOLD or ATTENTE != ATTENTEOLD){lcd.clear();}
SECURITEOLD=SECURITE;
ATTENTEOLD=ATTENTE;
MENUOLD=12;
MENUBACK=12;
MENUVOID();
OHM();
lcd.setCursor(0,0);
lcd.print(ATTENTE);
lcd.setCursor(0,1);
lcd.print(SECURITE);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
}


while (MENU == 13){
if (MENU != MENUOLD){lcd.clear();}
if (SECURITE != SECURITEOLD){lcd.clear();}
SECURITEOLD=SECURITE;
MENUOLD=13;
MENUBACK=13;
MENUVOID();
OHM();
lcd.setCursor(0,0);
lcd.print(SECURITE);
}

while (MENU == 14){ 
if (MENU != MENUOLD){lcd.clear();}
MENUOLD=14;
MENUBACK=14;
MENUVOID();
OHM();
if (ALIM != ALIMOLD or ATT != ATTOLD or SECU != SECUOLD){lcd.clear();}
lcd.setCursor(0,0);
lcd.print("ALIM");
lcd.setCursor(0,1);
lcd.print(ALIM);
ALIMOLD = ALIM ;
lcd.setCursor(6,0);
lcd.print("ATT");
lcd.setCursor(6,1);
lcd.print(ATT);
ATTOLD = ATT;
lcd.setCursor(12,0);
lcd.print("SECU");
lcd.setCursor(12,1);
lcd.print(SECU);
SECUOLD = SECU;
}

while (MENU == 20){ 
noTone(12);
if (MENU != MENUOLD){lcd.clear();}
if (ALIM != ALIMOLD){lcd.clear();}
ALIMOLD = ALIM ;
MENUOLD=20;
MENUVOID();
OHM();
lcd.setCursor(0,0);
lcd.print("DECLANCHEMENT  X");
lcd.setCursor(0,1);
lcd.print(ALIM);
lcd.setCursor(4,1);
lcd.write(3);
lcd.setCursor(8,1);
lcd.print("PRESS: A");


}
while (MENU == 21){
if (MENU != MENUOLD){lcd.clear(); unsigned long currentMillis = millis(); stoprelay=currentMillis+1500; digitalWrite(ledvert, HIGH);digitalWrite(ledrouge, HIGH);}
MENUOLD=21;
MENUVOID();
lcd.setCursor(0,0);
lcd.print("DECLANCHEMENT ");
lcd.setCursor(15,0);
lcd.write(2);
lcd.setCursor(0,1);
lcd.print("ON          WAIT");
digitalWrite(ledrouge, LOW); digitalWrite(ledvert, LOW);delay(10);digitalWrite(ledrouge, HIGH); digitalWrite(ledvert, HIGH);delay(10);digitalWrite(relay, HIGH);
for(int x=0; x<180; x++){                   // crée une variable "x" de type "int" dont la valeur de départ est de 0 et qui lorsqu'elle est inférieur 
                                                 // à 180 augmente de +1. Ce qui permet de s'arrurer que la valeur sinusoïdale ne va pas dans le négatif.
     
        sinVal = (sin(x*(3.1412/180)));         // permet de convertir la valeur de x en radians pour la suite du programme.
        toneVal = 2000+(int(sinVal*1000));      // formule qui permet de convertir la variable en une nouvelle varianble "toneVal" qui défénira la fréquence du Buzzer.
        tone(buzzer, toneVal);                      // utilise la fonction tone, constituer du numéro de la Broche (13) et de la fréquence (toneVal)
        delay(2); 
        }
unsigned long currentMillis = millis();
if (currentMillis >= stoprelay) {
previousMillis = currentMillis;
MENU=MENUBACK;
digitalWrite(relay, LOW);
noTone(12);
}

}}
