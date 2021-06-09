#include "Wifi.h"
#include "Web.h"
#include <WiFi.h>

// const char* ssid = "Redmi Note 9 Pro";
// const char* password = "raclette";

const char* ssidAp = "ESP32-GPS";
const char* passwordAp = "esp32nmea";

bool apDisconnected=0; //Variable pour l'état de l'AP
extern bool simu; //Variable pour lancer ou arreter la simulation
extern String ssidValue, passwordValue;
extern int ip1, ip2, ip3, ip4;
extern int dns1, dns2, dns3, dns4;
extern int gateway1, gateway2, gateway3, gateway4;
extern int subnet1, subnet2, subnet3, subnet4;
bool autoConnect=0;

//---------------------------------------------------------------------------------------
//Mise en place de la connexion Wi-Fi
void setupWifi()
{

  delay(10);
  IPAddress ip(ip1,ip2,ip3,ip4); //On paramètre une adresse IP fixe
  IPAddress dns(dns1,dns2,dns3,dns4);
  IPAddress gateway(gateway1,gateway2,gateway3,gateway4);
  IPAddress subnet(subnet1,subnet2,subnet3,subnet4);

  WiFi.config(ip, dns, gateway, subnet); //On rentre les paramètres de connexion
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssidValue);

  WiFi.mode(WIFI_STA); //On met l'ESP en mode de connexion Wi-Fi
  WiFi.begin(ssidValue.c_str(), passwordValue.c_str()); //On démarre la connexion

  for(int i=0;i<20;i++){ //On attend que l'ESP se connecte
    delay(500);
    Serial.print(".");

    if(WiFi.status() == WL_CONNECTED){ //Connexion réussie
      randomSeed(micros());

      Serial.println();
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      delay(500);
      autoConnect=1;

      break;
    }
  }
  if(WiFi.status() != WL_CONNECTED){ //Connexion ratée
    Serial.println();
    Serial.println("Connection failed");
    delay(500);
  }
}

//---------------------------------------------------------------------------------------
//Fonction de reconnexion au Wi-Fi
void reconnect(){
  Serial.println();
  Serial.println("Trying to reconnect");
  WiFi.reconnect();

  for(int i;i<20;i++){
    delay(500);
    Serial.print(".");

    if(WiFi.status() == WL_CONNECTED){
      Serial.println();
      Serial.println("WiFi reconnected");
      delay(500);

      break;
    }
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println();
    Serial.println("Reconnection failed");
    delay(500);
  }
}

//---------------------------------------------------------------------------------------
//Fonction qui vérifie l'état de la connexion Wi-Fi, tente de se reconnecter si besoin et gère l'état de l'AP
void testConnection(){ 
  if(WiFi.isConnected()==0){ //Si la connexion au Wi-Fi est perdue
    if(WiFi.softAPgetStationNum()>0&&simu==1){ //Si quelqu'un est connecté sur l'AP et qu'une simulation est en cours
      //Ne rien faire
    }
    else{ //Sinon
      reconnect(); //On tente de se reconnecter

      if(WiFi.status() == WL_CONNECTED){ //Si réussi

        if(WiFi.softAPgetStationNum()==0&&apDisconnected==0){ //Si personne n'est connecté sur l'AP et qu'il est actif
          WiFi.softAPdisconnect(); //On désactive l'AP
          Serial.println();
          Serial.println("AP is disconnected");
          Serial.println();
          apDisconnected=1;
        }
      }
      else{ //Sinon

        if(apDisconnected==1){ //Si l'AP est désactiver
          apDisconnected=0;
          setupAp(); //On réactive l'AP
        }
        WiFi.disconnect(); //On désactive la connexion Wi-Fi 
      }
    }
  }
  else{ //Sinon
    if(WiFi.softAPgetStationNum()==0&&apDisconnected==0){ //Si personne n'est connecté sur l'AP et qu'il est actif
      WiFi.softAPdisconnect(); //On désactive l'AP
      Serial.println();
      Serial.println("AP is disconnected");
      Serial.println();
      apDisconnected=1;
    }
  }
}

//---------------------------------------------------------------------------------------
//Fonction qui démarre l'AP
void setupAp(){
  Serial.println();
  Serial.print("Setting AP ");
  Serial.println(ssidAp); 

  Serial.println(WiFi.softAP(ssidAp, passwordAp) ? "Ready" : "Failed"); //Démarrage de l'AP
  delay(100);

  IPAddress ipAP(192,168,4,1);  //On paramètre une adresse IP fixe
  IPAddress gatewayAP(192,168,4,1);
  IPAddress subnetAP(255,255,255,0);

  WiFi.softAPConfig(ipAP, gatewayAP, subnetAP); //On rentre les paramètres de connexion dans l'AP

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}
