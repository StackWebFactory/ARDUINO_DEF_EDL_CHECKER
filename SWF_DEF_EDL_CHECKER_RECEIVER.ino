#include <SPI.h>
#include <RF24.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/Roboto_10.h>
#define OLED_RESET -1
const int bouton1 = 3;
const int bouton2 = 4;
const int bouton3 = 5;
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
struct DonneesArecevoir{int ALIM;int ATT;int SECU;int ATTENTE;int SECURITE;int MENU;};
DonneesArecevoir donnees;
struct DonneesAenvoier{int bt1;};
DonneesAenvoier envoier;
String ATTENTE ;
String SECURITE ;
String MENU;
int ATTENTE1 ;
int SECURITE2 ;
int MENU1;
#define pinCE   7
#define pinCSN  8
#define tunnel1 "PIPE2"     // On définit un premier "nom de tunnel" (5 caractères), pour pouvoir envoyer des données à l'autre NRF24
#define tunnel2 "PIPE1"
RF24 radio(pinCE, pinCSN);
const byte adresses[][6] = {tunnel1, tunnel2};
char message[100];
void setup() {
oled.setTextColor(WHITE);
oled.setFont(&Roboto_10);
pinMode(bouton1, INPUT);
pinMode(bouton2, INPUT);
pinMode(bouton3, INPUT);
// Partie OLED
//if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {Serial.println(F("SSD1306 allocation failed"));while (true);}
oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
oled.clearDisplay();
// Partie SERIAL
Serial.begin(9600);
// Partie NRF24
  radio.begin();
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
}

void loop() {
 screen();
 TX();
 RX();
 WORK();
}

void RX(){
radio.startListening();
if (radio.available()) {
   // clear display
    radio.read(&donnees, sizeof(donnees));
    //Serial.print(donnees.ATTENTE);   
  ATTENTE1 = donnees.ATTENTE;
  SECURITE2 = donnees.SECURITE;
  MENU1 = donnees.MENU;
  }}
  
void WORK(){
if (ATTENTE1 == 999){ATTENTE = "OUVERT" ;}
else if (ATTENTE1 == 100){ATTENTE = "C.C" ;}
else if (ATTENTE1 == 5){ATTENTE = "0 EN ATT FL OK" ;}
else if (ATTENTE1 == 1){ATTENTE = "1 EN ATT" ;}
else if (ATTENTE1 == 2){ATTENTE = "2 EN ATT" ;}
else if (ATTENTE1 == 3){ATTENTE = "3 EN ATT" ;}
else if (ATTENTE1 == 4){ATTENTE = "4 EN ATT" ;}
else {ATTENTE = "NO DATA";}

if (SECURITE2 == 999){SECURITE = "OUVERT" ;}
else if (SECURITE2 == 100){SECURITE = "C.C" ;}
else if (SECURITE2 == 5){SECURITE = "0 EN SECU FL OK !" ;}
else if (SECURITE2 == 1){SECURITE = "1 EN SECU" ;}
else if (SECURITE2 == 2){SECURITE = "2 EN SECU" ;}
else if (SECURITE2 == 3){SECURITE = "3 EN SECU" ;}
else if (SECURITE2 == 4){SECURITE = "4 EN SECU" ;}
else {SECURITE = "NO DATA";}

if (MENU1 == 14){MENU = "STANDBY";}
else if (MENU1 == 20){MENU = "DECLANCHEMENT ??";}
else if (MENU1 != 14 && MENU1 != 20){MENU="OFFLINE";}
}
 
void TX(){
if (digitalRead(bouton1) == 1)envoier.bt1=1;
else if (digitalRead(bouton2) == 1)envoier.bt1=2;
else if (digitalRead(bouton3) == 1)envoier.bt1=3;
else envoier.bt1=0;
if (envoier.bt1 > 0){
radio.stopListening();
delay(50);
radio.write(&envoier, sizeof(envoier));
//radio.startListening();
//delay(300);
}}

void screen(){
oled.clearDisplay();
oled.setCursor(0,10);
oled.print(F("ATT: "));
oled.print (donnees.ATT);
oled.print(F(" "));
oled.print(F("SECU: "));
oled.println(donnees.SECU);
oled.print(F("          ALIM: "));
oled.println(donnees.ALIM);
oled.println(MENU);
oled.print(F("ATT: "));
oled.println(SECURITE);
oled.print(F("SECU: "));
oled.println(ATTENTE);
oled.display();
}
    
