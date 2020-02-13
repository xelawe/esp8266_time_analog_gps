
#include <Time.h> //http://www.arduino.cc/playground/Code/Time
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time
#include <TimeAlarms.h>
#include <Timezone.h>    //https://github.com/JChristensen/Timezone

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

int SyncInt = 300;
const int timeZone = 0;     // UTC

// Central European Time Zone (Vienna)
TimeChangeRule myDST = {"CEDT", Last, Sun, Mar, 2, 120};    //Daylight time = UTC - 4 hours
TimeChangeRule mySTD = {"CET", Last, Sun, Oct, 3,  60};     //Standard time = UTC - 5 hours
Timezone myTZ(myDST, mySTD);

TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do {
    while (ss.available()) {
      gps.encode(ss.read());
      //       char c = ss.read();
      //       gps.encode(c);
      //       Serial.print(c);
    }
  }  while (millis() - start < ms);

}


static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  DebugPrint("GPS   : ");
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d.%02d.%02d ", d.day(), d.month(), d.year());
    Serial.print(sz);
  }

  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  printInt(t.age(), t.isValid(), 5);
  DebugPrintln();

  smartDelay(0);
}

time_t conv_to_timet( int hr, int min, int sec, int dy, int mnth, int yr ) {
  tmElements_t tm;

  // year can be given as full four digit year or two digts (2010 or 10 for 2010);
  //it is converted to years since 1970
  if ( yr > 99)
    yr = yr - 1970;
  else
    yr += 30;

  tm.Year = yr;
  tm.Month = mnth;
  tm.Day = dy;
  tm.Hour = hr;
  tm.Minute = min;
  tm.Second = sec;
  return makeTime(tm);
}

time_t getGPSTime() {
  if ( gps.date.isValid() && gps.time.isValid() && gps.date.isUpdated() && gps.time.isUpdated() && gps.time.age() < 4000 ) {

    //    Serial.println();
    printDateTime(gps.date, gps.time);

    // set the Time to the latest GPS reading
    //setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
    time_t lv_time_utc = conv_to_timet(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
    //    adjustTime(offset * SECS_PER_HOUR);
    return  myTZ.toLocal(lv_time_utc, &tcr);
  }
}


void check_time(unsigned long ms) {
  smartDelay(ms);

  if ( timeStatus() != timeSet ) {
    if (SyncInt != 5) {
      SyncInt = 5;
      setSyncInterval(SyncInt);
    }
  } else {
    if (SyncInt != 600) {
      SyncInt = 600;
      setSyncInterval(SyncInt);
    }
  }
}

void init_time() {

  DebugPrintln("Starting GPSTime");
  ss.begin(GPSBaud);

  setSyncProvider(getGPSTime);

  while (timeStatus() == timeNotSet) { // wait until the time is set by the sync provider
    DebugPrintln("Waiting for Time-Sync");
    check_time(1000);
  }

  //reset_drift();

  DebugPrintln("done GPSTime");
}

