#include <ClockAnalog.h>
#include "memtool.h"

ClockAnalog ClockA(12, 13, 25);

void do_status() {
  time_t sy_time = now();
  time_t clock_time = ClockA.time();

  DebugPrintln(" ");
  DebugPrint("Sys   : " + String(day(sy_time)) + "." + String(month(sy_time)) + "." + String(year(sy_time)) );
  DebugPrintln(" " + String(hour(sy_time)) + ":" + String(minute(sy_time)) + ":" + String(second(sy_time)) );
  DebugPrint("Clock : " + String(day(clock_time)) + "." + String(month(clock_time)) + "." + String(year(clock_time)) );
  DebugPrintln(" " + String(hour(clock_time)) + ":" + String(minute(clock_time)) + ":" + String(second(clock_time)) );
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
    } // else { --> differences > 1 day will be handled in ClogAnalog class
    
    diff_sec = ClockA.adjust_time( now() );
    if (diff_sec != 0) {
      DebugPrintln("Diff Adjust" + String(diff_sec) + " seconds" );
    }
    
    //}

  }

  ClockA.step_sec();

  time_to_mem( ClockA.time() );
}

void init_clock() {
  ClockA.init();

  if ( is_mem_valid() ) {
    ClockA.set_time(rtcData.mem_time);
  } else {
    ClockA.init_time(now());
  }

  do_status();
}

