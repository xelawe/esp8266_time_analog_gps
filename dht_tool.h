#include <DHT.h>

#define DHTPIN 4      // what digital pin we're connected to
#define DHTPOWERPIN 14     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
float dhttemp;
float dhthum;

void init_dht22() {
  pinMode(DHTPOWERPIN, OUTPUT);
  digitalWrite(DHTPOWERPIN, LOW);
}

void get_dht22() {

  digitalWrite(DHTPOWERPIN, HIGH);
  delay(500);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  digitalWrite(DHTPOWERPIN, LOW);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    DebugPrintln("Failed to read from DHT sensor!");
    return;
  }

  dhthum = h;
  dhttemp = t;

  DebugPrint("Humidity: ");
  DebugPrint(h);
  DebugPrint(" %\t");
  DebugPrint("Temperature: ");
  DebugPrint(t);
  DebugPrint(" *C ");
  DebugPrintln("");

  //  delay(1000);
  //send_val(gv_sens_nbr_t, t);
  //send_val(gv_sens_nbr_h, h);


}
