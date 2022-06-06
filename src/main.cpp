#include <math.h>

///// Sensor /////
#include <NewPing.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 14
#define MAX_DISTANCE 200
#define MIN_DISTANCE 16

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

///// First order filter /////
///// and ignore first zeroes /////
const double alpha = 0.05;
const int zeroes_threshold = 15;
int zeroes_count = 0;

double measurament;
double filtered = 0;

///// Higher output on closer distance /////
///// multiplier * ln(MAX_DISTANCE/x) ////
///// 40 * ln(200/cm) ////
const double multiplier = 40; // good results for 2m = 100 at 16cm
double level = 0;


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  delay(60);
  measurament = sonar.ping_cm();
  
  if (measurament == 0) {
    zeroes_count++;
  } else {
    zeroes_count = 0;
  }

  if (measurament < MIN_DISTANCE && measurament > 0) {
    measurament = MIN_DISTANCE;
  }
  if (zeroes_count == 0) {
    // use the measurement
    filtered = filtered * (1  - alpha) + measurament * alpha;
  } else if ( zeroes_count > zeroes_threshold) {
    // multiple zeros. reading should be MAX_DISTANCE
    filtered = filtered * (1  - alpha) + MAX_DISTANCE * alpha;
  }

  level = multiplier * log(MAX_DISTANCE / filtered);
  if (level > 100) {
    level = 100;
  }

  Serial.print("measurament: ");
  Serial.print(measurament);
  Serial.print(" smooth: ");
  Serial.print(filtered);
  Serial.print(" level: ");
  Serial.println(level);

}