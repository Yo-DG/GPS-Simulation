#include "Arduino.h"

#ifndef GPS_H
#define GPS_H
typedef struct _coord {
  double lat;
  double lon;
} coord;

void setupGps();
void loopGps();
coord CalculateDest(coord pointInit, double dist, double bearing);
void affDegMinSec(coord p);
void affHMS( double t);
int checkSum(char * sent);
String parseLat(float l);
String parseLon(float l);
#endif