#include <cy_serdebug.h>
#include <cy_serial.h>

#include "clock_tool.h"
#include "time_tool.h"
//#include "dht_tool.h"

void do_sensor() {

  //  get_dht22();

}

void setup() {
  cy_serial::start(__FILE__);

  init_time();

  init_clock();

  //init_dht22();
  
  //do_sensor();

  Alarm.timerRepeat(1,  tick);
  Alarm.timerRepeat(60, do_status);
  // Alarm.timerRepeat(600, do_sensor);

}

void loop() {

  check_time(0);

  Alarm.delay(10);
  
}
