#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include "memtool.h"
//#include "tools_wifiman.h"
#include <ClockAnalog.h>
#include "time_tool.h"
//#include "ota_tool.h"
//#include "dht_tool.h"

ClockAnalog ClockA(12, 13, 45);

void do_status() {
  time_t sy_time = now();
  time_t clock_time = ClockA.time();

  DebugPrintln(" ");
  DebugPrint("Sys   : " + String(day(sy_time)) + "." + String(month(sy_time)) + "." + String(year(sy_time)) );
  DebugPrintln(" " + String(hour(sy_time)) + ":" + String(minute(sy_time)) + ":" + String(second(sy_time)) );
  DebugPrint("Clock : " + String(day(clock_time)) + "." + String(month(clock_time)) + "." + String(year(clock_time)) );
  DebugPrintln(" " + String(hour(clock_time)) + ":" + String(minute(clock_time)) + ":" + String(second(clock_time)) );
  DebugPrintln(" ");

  printDateTime(gps.date, gps.time);
}

void tick( ) {
  int diff_sec;
  // time set?
  if ( timeStatus() != timeNotSet ) {
    // if set, but not synced, we adjust to system time anyway

    // how big is the differenc?
    diff_sec = abs(now() - ClockA.time());
    // more than one day?
    if ( diff_sec > 86400 ) {
      DebugPrintln("Diff " + String(diff_sec) + " seconds" );
      do_status();
    } else {
      diff_sec = ClockA.adjust_time( now() );
      if (diff_sec != 0) {
        DebugPrintln("Diff Adjust" + String(diff_sec) + " seconds" );
      }
    }


    //    diff_sec = ClockA.adjust_time( now() );
    //    if (diff_sec != 0) {
    //      DebugPrintln("Diff " + String(diff_sec) + " seconds" );
    //      // more than one day?
    //      if ( diff_sec > 86400 ) {
    //        do_status();
    //      }
    //    }
  }

  ClockA.step_sec();

  time_to_mem( ClockA.time() );
}

void do_sensor() {

//  get_dht22();

}

void setup() {
#ifdef serdebug
  Serial.begin(115200);
#endif

  DebugPrintln("\n" + String(__DATE__) + ", " + String(__TIME__) + " " + String(__FILE__));

  ClockA.init();

  //wifi_init("ESPClockA");

  //init_ota("ESPClockA");

  //init_dht22();

  init_time();

  if ( is_mem_valid() ) {
    ClockA.set_time(rtcData.mem_time);
  } else {
    ClockA.init_time(now());
  }

  do_status();
  //do_sensor();

  Alarm.timerRepeat(1,  tick);
  Alarm.timerRepeat(60, do_status);
  Alarm.timerRepeat(600, do_sensor);

}

void loop() {

  check_time();

  //check_wifi_conn();

  //check_ota();

  Alarm.delay(10);
}
