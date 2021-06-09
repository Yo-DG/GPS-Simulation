/*
http://www.movable-type.co.uk/scripts/latlong.html

Formula:
   φ2 = asin( sin φ1 ⋅ cos δ + cos φ1 ⋅ sin δ ⋅ cos θ )
  λ2 = λ1 + atan2( sin θ ⋅ sin δ ⋅ cos φ1, cos δ − sin φ1 ⋅ sin φ2 )
where:
  φ is latitude
  λ is longitude
  θ is the bearing (clockwise from north)
  δ is the angular distance d/R
  d being the distance travelled
  R the earth’s radius= 6 371 000
*/
#include "Gps.h"
#include <string.h>
#include <NMEA2000_CAN.h>  
#include <N2kMessages.h>
#include <math.h>

#define radius 6371.000
#define rad(x) x*M_PI/180
#define deg(x) x*180/M_PI

using namespace std;

typedef struct  {

  //'A' = OK, 'V' = Void (warning)
  char status;
  double GPSTime; // Secs since midnight
  double latitude;
  double longitude;
  double trueCOG;
  double SOG;
  unsigned long daysSince1970;
  double variation;
} _GPRMC;

 _GPRMC GPRMC;

String gprmcSentence="";
double dist, bearing;
coord p0 = {43.1016667, 5.926667};

double latDep, lonDep, lati, longi, latDest, lonDest, d, dterre=6378137, dDest;
double sog,cog;
bool paramSimu=1, arriveDest=0;

extern bool destOrCog, simu;

//---------------------------------------------------------------------------------------
void setupGps(){
  Serial2.begin(4800, SERIAL_8N1, 16, 17);

  // Reserve enough buffer for sending all messages. This does not work on small memory devices like Uno or Mega
  NMEA2000.SetN2kCANSendFrameBufSize(250);
  // Set Product information
  NMEA2000.SetProductInformation("00000001", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "Simulateur GPS",  // Manufacturer's Model ID
                                 "1.0.2.25 (2019-07-07)",  // Manufacturer's Software version code
                                 "1.0.2.0 (2019-07-07)" // Manufacturer's Model version
                                );
  // Set device information
  NMEA2000.SetDeviceInformation(1, // Unique number. Use e.g. Serial number.
                                132, // Device function=Analog to NMEA 2000 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                25, // Device class=Inter/Intranetwork Device. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );
  NMEA2000.SetForwardStream(&Serial);  // PC output on due programming port
  // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,22);
  //NMEA2000.SetDebugMode(tNMEA2000::dm_ClearText); // Uncomment this, so you can test code without CAN bus chips on Arduino Mega
  NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  NMEA2000.Open();
  delay(500);
}

unsigned char chk;
String lat, lon;
coord nextPoint, previousPoint;
double distPerSec;
int hour, minu, sec, day, mon, year;
unsigned long currentTime, previousTime, previousTime2;
const long interval=1000, interval2=100;

bool test=0;

//---------------------------------------------------------------------------------------
void loopGps(){

  tN2kMsg N2kMsg;

  if(paramSimu==1){
    GPRMC.GPSTime=0;
    chk=0;
    lat="";
    lon="";
    p0.lat=latDep;
    p0.lon=lonDep;
    GPRMC.latitude=p0.lat;
    GPRMC.longitude=p0.lon;
    lati=latDep;
    longi=lonDep;
    previousPoint=p0;
    GPRMC.SOG=sog;
    distPerSec=(GPRMC.SOG * 1.852) / 3600;
    GPRMC.trueCOG=cog;
    d=0;
    hour=8;
    minu=0;
    sec=0;
    day=16;
    mon=2;
    year=21;
    previousTime=0;

    paramSimu=0;
  }
  
  currentTime = millis();
  if (currentTime - previousTime >= interval) {
    if(destOrCog==1 && test==0){
      double dLon = (lonDest - lonDep);
      double y = sin(dLon*(PI/180)) * cos(latDest*(PI/180));
      double x = cos(latDep*(PI/180)) * sin(latDest*(PI/180)) - sin(latDep*(PI/180)) * cos(latDest*(PI/180)) * cos(dLon*(PI/180));
      cog=atan2(y, x);
      cog=cog*180/M_PI;
      GPRMC.trueCOG=cog;
      test=1;
    }
    
    nextPoint = CalculateDest(p0, distPerSec * GPRMC.GPSTime, GPRMC.trueCOG);
    GPRMC.latitude = nextPoint.lat;
    GPRMC.longitude = nextPoint.lon;
    GPRMC.GPSTime++;
    previousPoint = nextPoint;

    lati=GPRMC.latitude;
    longi=GPRMC.longitude;

    d=acos(sin(lati*(PI/180))*sin(latDep*(PI/180))+cos(lati*(PI/180))*cos(latDep*(PI/180))*cos((lonDep-longi)*(PI/180)))*dterre;
  
    lat = parseLat(GPRMC.latitude);
    lon = parseLon(GPRMC.longitude);

    gprmcSentence = "";
    gprmcSentence += "$GPRMC,";
    gprmcSentence += (hour<10)?String("0" + String(hour)):String(hour);
    gprmcSentence += (minu<10)?String("0" + String(minu)):String(minu);
    gprmcSentence += (sec<10)?String("0" + String(sec)):String(sec);
    gprmcSentence += ",A,";
    if(lati<0){
      gprmcSentence += lat + ",S,";
    }else{
      gprmcSentence += lat + ",N,";
    } 
    if(longi<0){
      gprmcSentence += lon + ",W,";
    }else{
      gprmcSentence += lon + ",E,";
    }
    gprmcSentence += String(GPRMC.SOG,1) + "," + String(GPRMC.trueCOG,1) + ",";
    gprmcSentence += (day<10)?String("0" + String(day)):String(day);
    gprmcSentence += (mon<10)?String("0" + String(mon)):String(mon);
    gprmcSentence += (year<10)?String("0" + String(year)):String(year);
    gprmcSentence += ",,";
    
    chk = 0;
    const char *calcCheck = gprmcSentence.c_str();
    for(int i = 1; calcCheck[i] !='\0'; i++)
      chk ^= calcCheck[i];
      
    gprmcSentence += "*";
    gprmcSentence += (chk < 0x10)?String("0" + String(chk,HEX)):String(chk,HEX);
    gprmcSentence += "\r\n";
    
    Serial.println(gprmcSentence);
    Serial2.println(gprmcSentence);

    SetN2kGNSS(N2kMsg,1,17555,62000,lati,longi,10.5,N2kGNSSt_GPS,N2kGNSSm_GNSSfix,12,0.8,0.5,15,1,N2kGNSSt_GPS,15,2);
    NMEA2000.SendMsg(N2kMsg);

    if(++sec == 60) {
      sec = 0;
      if(++minu == 60) {
        minu = 0;
        hour ++;
      }
    }

    if(destOrCog==1){
      dDest=acos(sin(lati*(PI/180))*sin(latDest*(PI/180))+cos(lati*(PI/180))*cos(latDest*(PI/180))*cos((lonDest-longi)*(PI/180)))*dterre;
      if(dDest<100){
        simu=0;
        Serial.println();
        Serial.println("Arrivé à destination !");
        arriveDest=1;
        destOrCog=0;
        test=0;
      }
    }
        
    previousTime = currentTime;
  }

  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;  
    SetN2kLatLonRapid(N2kMsg,lati,longi);
    NMEA2000.SendMsg(N2kMsg);
    SetN2kCOGSOGRapid(N2kMsg,1,N2khr_true,cog,sog);
    NMEA2000.SendMsg(N2kMsg);
  }
}

//---------------------------------------------------------------------------------------
coord CalculateDest(coord pointInit, double dist, double bearing) {
  coord pointDest;
  double y, x;
  double tau = dist / radius;
  y = asin(sin(rad(pointInit.lat)) * cos(tau) + cos(rad(pointInit.lat)) * sin(tau) * cos(rad(bearing)));
  
  x = rad(pointInit.lon) + atan2( sin(rad(bearing)) * sin(tau) * cos(rad(pointInit.lat)) , cos(tau) - sin(rad(pointInit.lat)) * sin(y));
  
  pointDest.lat = deg(y);
  pointDest.lon = deg(x);

  return pointDest;
}

//---------------------------------------------------------------------------------------
void affDegMinSec(coord p) {
  
  int latDeg = p.lat;
  int latMin = (p.lat - latDeg) * 60;
  float latSec = (((p.lat - latDeg) * 60) - latMin) *60;
  printf("%02d°%02d'%.2f\", ", latDeg,latMin,latSec);
  int lonDeg = p.lon;
  int lonMin = (p.lon - lonDeg) * 60;
  float lonSec = (((p.lon - lonDeg) * 60) - lonMin) *60;
  printf("%02d°%02d'%.2f\" ", lonDeg,lonMin,lonSec);
}

//---------------------------------------------------------------------------------------
void affHMS( double t) {
  int h = t / 3600;
  int m = (t-h*3600) /60;
  int s = t - h*3600 - m*60;
  printf("\t%d:%d:%d\n",h,m,s);
}

//---------------------------------------------------------------------------------------
int checkSum(char * sent)
{int check=0;
 for(unsigned int i=0; i<strlen(sent); i++) 
  check ^= sent[i];
  Serial.println(check);
 return check;
}

//---------------------------------------------------------------------------------------
String parseLat(float l) {
  if(l<0){l=-l;}
  String lat="";
  if((int)l < 10) lat += String("0" + String((int)l));
  else lat += String((int)l);
  if(((l - (int)l)*60)<10.0)  lat += String( "0" + String (((l - (int)l)*60),3));
  else lat += String (((l - (int)l)*60),3); 
  return lat;
 
}

//---------------------------------------------------------------------------------------
String parseLon(float l) {
  if(l<0){l=-l;}
  String lon="";
  if((int)l < 10) lon += String("00" + String((int)l));
   else if((int)l < 100) lon += String("0" + String((int)l));
   else lon += String((int)l);
  if(((l - (int)l)*60)<10.0)  lon += String( "0" + String (((l - (int)l)*60),3)); 
  else lon += String (((l - (int)l)*60),3); 
  return lon;
 }
