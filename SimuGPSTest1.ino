#include <Arduino.h>
#include "Wifi.h"
#include "Gps.h"
#include "Web.h"

extern bool simu; //Variable pour lancer ou arreter la simulation
extern bool ssidState, passwordState, ipState, dnsState, gatewayState, subnetState;
extern bool autoConnect;
extern bool test;

unsigned long previousMillis=0;
const long interval=20000;

//---------------------------------------------------------------------------------------
void setup(){
  Serial.begin(4800);
  //Démarrage de l'access-Point
  setupAp();
  //Démarrage du serveur Web
  setupWeb();
  //Setup pour la simulation GPS
  setupGps();
}

//---------------------------------------------------------------------------------------
void loop(){
  unsigned long currentMillis=millis();
  if(ssidState==1&&passwordState==1&&ipState==1&&dnsState==1&&gatewayState==1&&subnetState==1){
    setupWifi();
    setupAp();
    autoConnect=1;

    ssidState=0;
    passwordState=0;
    ipState=0;
    dnsState=0;
    gatewayState=0;
    subnetState=0;
  }

  if(simu==1){
    loopGps();
  }
  else{
    test=0;
  }

  loopWeb();
  
  if(currentMillis-previousMillis>=interval){
    previousMillis=currentMillis;
    if(autoConnect==1){
      testConnection(); 
    }
  }
}  
  
