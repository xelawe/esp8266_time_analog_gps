#include <cy_serdebug.h>
#include <cy_serial.h>

#include "memtool.h"
#include "clock_tool.h"
#include "time_tool.h"
//#include "dht_tool.h"

void do_sensor() {

  //  get_dht22();

}

void setup() {
  cy_serial::start(__FILE__);

  //init_dht22();

  init_time();

  ClockA.init();
  if ( is_mem_valid() ) {
    ClockA.set_time(rtcData.mem_time);
  } else {
    ClockA.init_time(now());
  }

  do_status();
  //do_sensor();

  Alarm.timerRepeat(1,  tick);
  Alarm.timerRepeat(60, do_status);
  // Alarm.timerRepeat(600, do_sensor);

}

void loop() {

  check_time(0);

  Alarm.delay(10);
}
