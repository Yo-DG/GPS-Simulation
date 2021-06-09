#include "Arduino.h"

#ifndef WEB_H
#define WEB_H

void setupWeb();
void loopWeb();
String getContentType(String filename);
bool handleFileRead(String path);
void handleNotFound();
// void handleLatDep();
// void handleLonDep();
void handleLat();
void handleLon();
void handleDistance();
void handleSim();
void handleSog();
void handleCog();
void handleLatDep();
void handleLonDep();
void handleLatDest();
void handleLonDest();
void handleSsid();
void handlePassword();
void handleIp();
void handleDns();
void handleGateway();
void handleSubnet();
void handleParamSimu();
void handleDestOrCog();
void handleGetCog();
void handleArriveDest();

#endif