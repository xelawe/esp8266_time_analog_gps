#include <ClockAnalog.h>

ClockAnalog ClockA(12, 13, 45);

void do_status() {
  time_t sy_time = now();
  time_t clock_time = ClockA.time();

  DebugPrintln(" ");
  DebugPrint("Sys   : " + String(day(sy_time)) + "." + String(month(sy_time)) + "." + String(year(sy_time)) );
  DebugPrintln(" " + String(hour(sy_time)) + ":" + String(minute(sy_time)) + ":" + String(second(sy_time)) );
  DebugPrint("Clock : " + String(day(clock_time)) + "." + String(month(clock_time)) + "." + String(year(clock_time)) );
  DebugPrintln(" " + String(hour(clock_time)) + ":" + String(minute(clock_time)) + ":" + String(second(clock_time)) );
  //  DebugPrintln(" ");

  //  printDateTime(gps.date, gps.time);
}


//void tick_tst() {
//  DebugPrintln("Tick!" );
//}

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

