#include <SoftwareSerial.h>
//SoftwareSerial mySerial(0, 1);
#include <SPI.h>
#include <RF24.h>
#define pinCE   7             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  8             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define tunnel1 "PIPE1"
#define tunnel2 "PIPE2"
int v4;
int v5;
int v6;
RF24 radio(pinCE, pinCSN);    // Instanciation du NRF24L01
const byte adresses[][6] = {tunnel1, tunnel2};
struct DonneesAenvoyer {
  int ALIM;
  int ATT;
  int SECU;
  int ATTENTE;
  int SECURITE;
  int MENU;
};
struct DonneesArecevoir {
  int bt1;

};
DonneesAenvoyer donnees;
DonneesArecevoir recevoir;
char buf[100];
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long interval = 1000;
void setup() {
  //mySerial.begin(9600);
  Serial.begin(9600) ;
  radio.begin();                      // Initialisation du module NRF24
  radio.openWritingPipe(adresses[0]);      // Ouverture du "tunnel1" en ÉCRITURE
  radio.openReadingPipe(1, adresses[1]);    // Ouverture du tunnel en ÉCRITURE, avec le "nom" qu'on lui a donné
  radio.setPALevel(RF24_PA_MAX);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
 // radio.powerUp();
  //radio.startListening();

}
void loop() {
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

 if (Serial.available())
 {
    radio.stopListening(); 
    int n=Serial.readBytesUntil('>',buf,sizeof buf-1);
    buf[n]=0;           // fin de la chaine
    char *p=strchr(buf,'Z');
    float v1=atof(p+1);
    p=strchr(buf,'Y');
    float v2=atof(p+1);
    p=strchr(buf,'X');
    float v3=atof(p+1);
    p=strchr(buf,'W');
    v4=atof(p+1);
    p=strchr(buf,'V');
    v5=atof(p+1);
    p=strchr(buf,'!');
    v6=atof(p+1);
  donnees.ALIM =v1;
  donnees.ATT =v2;
  donnees.SECU =v3;
  donnees.ATTENTE =v4;
  donnees.SECURITE =v5;
  donnees.MENU =v6;
  radio.write(&donnees, sizeof(donnees));
  Serial.flush();
  }}
  delay(150);
  radio.startListening();
if(radio.available()) {
   radio.read(&recevoir, sizeof(recevoir));
   Serial.print(recevoir.bt1);
}
}
