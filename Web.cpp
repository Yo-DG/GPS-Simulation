#include "Web.h"
#include "Gps.h"
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

WebServer server(80);
bool simu=0; //Variable pour lancer ou arreter la simulation
bool destOrCog=0;
bool ssidState=0, passwordState=0, ipState=0, dnsState=0, gatewayState=0, subnetState=0;
String ssidValue, passwordValue;
int ip1, ip2, ip3, ip4;
int dns1, dns2, dns3, dns4;
int gateway1, gateway2, gateway3, gateway4;
int subnet1, subnet2, subnet3, subnet4;

extern double latDep, lonDep, lati, longi, latDest, lonDest, d; //Récupération 
extern double sog, cog; //des valeurs GPS
extern bool paramSimu, arriveDest;

//---------------------------------------------------------------------------------------
//Fonction qui met en place le serveur Web
void setupWeb()
{
    SPIFFS.begin();

    server.on("/getLat", handleLat);
    server.on("/getLon", handleLon);
    server.on("/getDistance", handleDistance);
    server.on("/simu", handleSim);
    server.on("/ssid", handleSsid);
    server.on("/password", handlePassword);
    server.on("/ip", handleIp);
    server.on("/dns", handleDns);
    server.on("/gateway", handleGateway);
    server.on("/subnet", handleSubnet);
    server.on("/sog", handleSog);
    server.on("/cog", handleCog);
    server.on("/latDep", handleLatDep);
    server.on("/lonDep", handleLonDep);
    server.on("/latDest", handleLatDest);
    server.on("/lonDest", handleLonDest);
    server.on("/paramSimu", handleParamSimu);
    server.on("/destOrCog", handleDestOrCog);
    server.on("/getCog", handleGetCog);
    server.on("/arriveDest", handleArriveDest);
    server.onNotFound([]() {                                  // If the client requests any URI
        if (!handleFileRead(server.uri()))                    // send it if it exists
            server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
    });

    server.begin();
    Serial.println("HTTP server started");
    Serial.println();
}

//---------------------------------------------------------------------------------------
void loopWeb(){
    server.handleClient();
}

//---------------------------------------------------------------------------------------
//Fonction qui permet de lire differents type de fichier
String getContentType(String filename)
{ // convert the file extension to the MIME type
    if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".jpg"))
        return "image/jpg";
    else if (filename.endsWith(".jpeg"))
        return "image/jpeg";
    else if (filename.endsWith(".png"))
        return "image/png";
    return "text/plain";
}

//---------------------------------------------------------------------------------------
//Fonction qui ouvre les fichiers pour afficher la page Web
bool handleFileRead(String path)
{ // send the right file to the client (if it exists)
    String contentType = "";
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/")&&(WiFi.softAPgetStationNum()>0))
        path += "indexAp.html";
    if (path.endsWith("/"))
        path += "index.html";           // If a folder is requested, send the index file
    contentType = getContentType(path); // Get the MIME type
    if (SPIFFS.exists(path))
    {                                                       // If the file exists
        File file = SPIFFS.open(path, "r");                 // Open it
        size_t sent = server.streamFile(file, contentType); // And send it to the client
        file.close();                                       // Then close the file again
        return true;
    }
    Serial.println("\tFile Not Found");
    return false; // If the file doesn't exist, return false
}

//---------------------------------------------------------------------------------------
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne la latitude
void handleLat(){
    String latString=String(lati,7);
    server.send(200, "text/plain", latString);
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne la longitude
void handleLon(){
    String lonString=String(longi,7);
    server.send(200, "text/plain", lonString);
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne la distance entre le point de départ et la position actuelle
void handleDistance(){
    String distanceString=String(d);
    server.send(200, "text/plain", distanceString);   
}

//---------------------------------------------------------------------------------------
//Fonction qui va permettre de controler l'état de la simulation via 2 boutons
void handleSim(){
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i)=="simState"){
     String response;
     if( server.arg(i) =="1") {
      simu=1;
      response="ON";
     }
     else  {
      simu=0;
      response="OFF";
     }
         
     server.send(200, "text/plain", response);
    }
  }
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne SOG
void handleSog(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            sog=(server.arg(i)).toDouble();
            String sogString=String(sog);
            server.send(200, "text/plain", sogString);    
        }
    }
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne COG
void handleCog(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            cog=(server.arg(i)).toDouble();
            String cogString=String(cog);
            server.send(200, "text/plain", cogString);    
        }
    }
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne latDep
void handleLatDep(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            latDep=(server.arg(i)).toDouble();
            String latDepString=String(latDep,7);
            server.send(200, "text/plain", latDepString);    
        }
    }
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne lonDep
void handleLonDep(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            lonDep=(server.arg(i)).toDouble();
            String lonDepString=String(lonDep,7);
            server.send(200, "text/plain", lonDepString);    
        }
    }
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne latDest
void handleLatDest(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            latDest=(server.arg(i)).toDouble();
            String latDestString=String(latDest,7);
            server.send(200, "text/plain", latDestString);    
        }
    }
}

//---------------------------------------------------------------------------------------
//Fonction qui retourne lonDest
void handleLonDest(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            lonDest=(server.arg(i)).toDouble();
            String lonDestString=String(lonDest,7);
            server.send(200, "text/plain", lonDestString);    
        }
    }
}

//---------------------------------------------------------------------------------------
void handleSsid(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            ssidValue=server.arg(i);
            // Serial.println(ssidValue);
            server.send(200, "text/plain", ssidValue);
            ssidState=1;    
        }
    }
}

//---------------------------------------------------------------------------------------
void handlePassword(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            passwordValue=server.arg(i);
            passwordState=1;
            // Serial.println(passwordValue);
            // server.send(200, "text/plain", passwordValue);    
        }
    }
}

//---------------------------------------------------------------------------------------
void handleIp(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            String ipValue=server.arg(i);
            // Serial.println(ipValue);
            server.send(200, "text/plain", ipValue); 
            ip1=(ipValue.substring(0,ipValue.indexOf("."))).toInt();
            ipValue=ipValue.substring(ipValue.indexOf(".")+1, ipValue.lastIndexOf("\0"));
            // Serial.println(ip1);

            ip2=(ipValue.substring(0,ipValue.indexOf("."))).toInt();
            ipValue=ipValue.substring(ipValue.indexOf(".")+1, ipValue.lastIndexOf("\0"));
            // Serial.println(ip2);

            ip3=(ipValue.substring(0,ipValue.indexOf("."))).toInt();
            ipValue=ipValue.substring(ipValue.indexOf(".")+1, ipValue.lastIndexOf("\0"));
            // Serial.println(ip3);

            ip4=(ipValue.substring(0,ipValue.lastIndexOf("\0"))).toInt();
            // Serial.println(ip4);

            ipState=1;
        }
    }
}

//---------------------------------------------------------------------------------------
void handleDns(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            String dnsValue=server.arg(i);
            // Serial.println(dnsValue);
            server.send(200, "text/plain", dnsValue); 
            dns1=(dnsValue.substring(0,dnsValue.indexOf("."))).toInt();
            dnsValue=dnsValue.substring(dnsValue.indexOf(".")+1, dnsValue.lastIndexOf("\0"));
            // Serial.println(dns1);

            dns2=(dnsValue.substring(0,dnsValue.indexOf("."))).toInt();
            dnsValue=dnsValue.substring(dnsValue.indexOf(".")+1, dnsValue.lastIndexOf("\0"));
            // Serial.println(dns2);

            dns3=(dnsValue.substring(0,dnsValue.indexOf("."))).toInt();
            dnsValue=dnsValue.substring(dnsValue.indexOf(".")+1, dnsValue.lastIndexOf("\0"));
            // Serial.println(dns3);

            dns4=(dnsValue.substring(0,dnsValue.lastIndexOf("\0"))).toInt();
            // Serial.println(dns4);

            dnsState=1;
        }
    }
}

//---------------------------------------------------------------------------------------
void handleGateway(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            String gatewayValue=server.arg(i);
            // Serial.println(gatewayValue);
            server.send(200, "text/plain", gatewayValue); 
            gateway1=(gatewayValue.substring(0,gatewayValue.indexOf("."))).toInt();
            gatewayValue=gatewayValue.substring(gatewayValue.indexOf(".")+1, gatewayValue.lastIndexOf("\0"));
            // Serial.println(gateway1);

            gateway2=(gatewayValue.substring(0,gatewayValue.indexOf("."))).toInt();
            gatewayValue=gatewayValue.substring(gatewayValue.indexOf(".")+1, gatewayValue.lastIndexOf("\0"));
            // Serial.println(gateway2);

            gateway3=(gatewayValue.substring(0,gatewayValue.indexOf("."))).toInt();
            gatewayValue=gatewayValue.substring(gatewayValue.indexOf(".")+1, gatewayValue.lastIndexOf("\0"));
            // Serial.println(gateway3);

            gateway4=(gatewayValue.substring(0,gatewayValue.lastIndexOf("\0"))).toInt();
            // Serial.println(gateway4);

            gatewayState=1;
        }
    }
}

//---------------------------------------------------------------------------------------
void handleSubnet(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            String subnetValue=server.arg(i);
            // Serial.println(subnetValue);
            server.send(200, "text/plain", subnetValue); 
            subnet1=(subnetValue.substring(0,subnetValue.indexOf("."))).toInt();
            subnetValue=subnetValue.substring(subnetValue.indexOf(".")+1, subnetValue.lastIndexOf("\0"));
            // Serial.println(subnet1);

            subnet2=(subnetValue.substring(0,subnetValue.indexOf("."))).toInt();
            subnetValue=subnetValue.substring(subnetValue.indexOf(".")+1, subnetValue.lastIndexOf("\0"));
            // Serial.println(subnet2);

            subnet3=(subnetValue.substring(0,subnetValue.indexOf("."))).toInt();
            subnetValue=subnetValue.substring(subnetValue.indexOf(".")+1, subnetValue.lastIndexOf("\0"));
            // Serial.println(subnet3);

            subnet4=(subnetValue.substring(0,subnetValue.lastIndexOf("\0"))).toInt();
            // Serial.println(subnet4);

            subnetState=1;
        }
    }
}

//---------------------------------------------------------------------------------------
void handleParamSimu(){
    for (uint8_t i = 0; i < server.args(); i++) {
        if (server.argName(i)=="value"){
            paramSimu=1;
            //Serial.println(paramSimu);
        }
    }
}

//---------------------------------------------------------------------------------------
void handleDestOrCog(){
    for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i)=="value"){
     if( server.arg(i) =="1") {
      destOrCog=1;
     }
     else {
      destOrCog=0;
     }
    }
  }
}

//---------------------------------------------------------------------------------------
void handleGetCog(){
    String cogString=String(cog);
    server.send(200, "text/plain", cogString);   
}

//---------------------------------------------------------------------------------------
void handleArriveDest(){
    String arriveDestString=String(arriveDest);
    server.send(200, "text/plain", arriveDestString);  
    if(arriveDest==1){arriveDest=0;} 
}